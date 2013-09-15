#include "radutil.h"
#include "radtest.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

RAD_TEST(gettok3)
{
    char input[256];
    char str[] = "Hello world!";
    for(size_t i=0;i<sizeof(str)-2;++i)
        snprintf(input+i*4, 5, "\\%03o", *(str+i));

    const char* p = input;
    char output[64];
    gettok(&p, output, sizeof(output), NULL);
    rad_assert(strcmp(output, "Hello world") == 0);
}

RAD_TEST(gettok2)
{
    typedef struct _expr_t {
        char op[8];
        char lhs[64];
        char rhs[64];
    } expr_t;

    const char* p = "add:5,4";
    char buf[256];
    const LRAD_NAME_NUMBER tokens[] = {
        { ":", T_COLON },
        { ",", T_COMMA },
        { NULL, 0,     },
    };
    LRAD_TOKEN t;
    expr_t expr;

    t = gettok(&p, buf, sizeof(buf), tokens);
    rad_assert(T_WORD(t));
    strscpy(expr.op, buf);

    t = gettok(&p, buf, sizeof(buf), tokens);
    rad_assert(t == T_COLON);

    t = gettok(&p, buf, sizeof(buf), tokens);
    rad_assert(T_WORD(t));
    strscpy(expr.lhs, buf);

    t = gettok(&p, buf, sizeof(buf), tokens);
    rad_assert(t == T_COMMA);

    t = gettok(&p, buf, sizeof(buf), tokens);
    rad_assert(T_WORD(t));
    strscpy(expr.rhs, buf);
}

RAD_TEST(gettok)
{
    char buf[256];

    {
        const LRAD_NAME_NUMBER tokens[] = {
            { ":=", T_OP_SET, },  /* order is important! */
            { ":",  T_COLON,  },
            { "=",  T_OP_EQ,  },
            { NULL, 0,        },
        };
        const char* p = " a:='b' ";
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_BARE_WORD);
        rad_assert(strcmp("a", buf) == 0);
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_OP_SET);
        rad_assert(strcmp(":=", buf) == 0);
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_SINGLE_QUOTED_STRING);
        rad_assert(strcmp("b", buf) == 0);
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_EOL);
    }
    {
        const LRAD_NAME_NUMBER tokens[] = {
            { ":",  T_COLON,  },  /* order is important! */
            { ":=", T_OP_SET, },
            { "=",  T_OP_EQ,  },
            { NULL, 0,        },
        };
        const char* p = "a := 'b'";
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_BARE_WORD);
        rad_assert(strcmp("a", buf) == 0);
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_COLON);
        rad_assert(strcmp(":", buf) == 0);
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_OP_EQ);
        rad_assert(strcmp("=", buf) == 0);
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_SINGLE_QUOTED_STRING);
        rad_assert(strcmp("b", buf) == 0);
        rad_verify(gettok(&p, buf, sizeof(buf), tokens) == T_EOL);
    }
}

RAD_TEST(getbareword)
{
    /* By implement, getword and getbareword are only different at
     * return condition. getword returns 1 until meets T_EOL, while
     * getbareword returns 1 until meets meets tokens other than
     * T_BARE_WORD.
     * The word *bareword* means *true* word, which is a word without
     * quotation mark.
     */

    const char* p = " abcd ";
    char buf[256];
    int retval;

    retval = getbareword(&p, buf, sizeof(buf));
    rad_assert(retval == 1);
    rad_assert(strcmp("abcd", buf) == 0);

    retval = getbareword(&p, buf, sizeof(buf));
    rad_assert(retval == 0);

    p = "  'abcd' ";

    retval = getbareword(&p, buf, sizeof(buf));
    rad_assert(retval == 0);
}

RAD_TEST(getword)
{
#define GETWORD_INPUT                           \
    " \t\r\nfirst \tsecond'abc'def 'third'"     \
    "fourth,\tfifth \", sixth\""                \
    "\n seventh:\r\t`\teighth \n` ninth\'\r"    \
    "'hel\\l\\'''ja\\tck\\\"''over'  \r\t\n"

    /* The input string to be parsed */
    char str[] = GETWORD_INPUT;

    /* Parsing rules:
     * 1. Take whitespace as delimiter except quoted word;
     * 2. Whitespaces is skipped;
     * 3. Escapes and quotes are taken care of.
     * Note:
     * Escape mark in getword is *DOUBLE* backslash
     */
    const char* p = str;
    char buf[256];

    for(int i=1; getword(&p, buf, sizeof(buf)); ++i) {
        switch(i) {
        case 1: rad_assert(strcmp("first", buf) == 0); break;
        case 2: rad_assert(strcmp("second'abc'def", buf) == 0); break;
        case 3: rad_assert(strcmp("third", buf) == 0); break;
        case 4: rad_assert(strcmp("fourth,", buf) == 0); break;
        case 5: rad_assert(strcmp("fifth", buf) == 0); break;
        case 6: rad_assert(strcmp(", sixth", buf) == 0); break;
        case 7: rad_assert(strcmp("seventh:", buf) == 0); break;
        case 8: rad_assert(strcmp("\teighth \n", buf) == 0); break;
        case 9: rad_assert(strcmp("ninth'", buf) == 0); break;
        case 10: rad_assert(strcmp("hell'", buf) == 0); break;
        case 11: rad_assert(strcmp("ja\tck\"", buf) == 0); break;
        case 12: rad_assert(strcmp("over", buf) == 0); break;
        default: rad_assert(false); return;
        }
    }

    /* Original input string is not changed */
    rad_assert(strcmp(str, GETWORD_INPUT) == 0);

    /* How about a limited buffer is provided? */
    p = str;
    char sbuf[10];

    for(int i=1; getword(&p, sbuf, sizeof(sbuf)); ++i) {
        switch(i) {
        case 1: rad_assert(strcmp("first", sbuf) == 0); break;
        case 2: rad_assert(strcmp("second'ab", sbuf) == 0); break;
        case 3: rad_assert(strcmp("c'def", sbuf) == 0); break;
        case 4: rad_assert(strcmp("third", sbuf) == 0); break;
        case 5: rad_assert(strcmp("fourth,", sbuf) == 0); break;
        case 6: rad_assert(strcmp("fifth", sbuf) == 0); break;
        case 7: rad_assert(strcmp(", sixth", sbuf) == 0); break;
        case 8: rad_assert(strcmp("seventh:", sbuf) == 0); break;
        case 9: rad_assert(strcmp("\teighth \n", sbuf) == 0); break;
        case 10: rad_assert(strcmp(" ninth'\r'", sbuf) == 0); break;
        case 11: rad_assert(strcmp("hell'''ja", sbuf) == 0); break;
        case 12: rad_assert(strcmp("\tck\"''ove", sbuf) == 0); break;
        case 13: rad_assert(strcmp("r'", sbuf) == 0); break;
        default: rad_assert(false); return;
        }
    }
    /* So, limited buffer is very dangerous! It will break the
     * quotation rule */

#undef GETWORD_INPUT
}
