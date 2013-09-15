#include "radutil.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

static const LRAD_NAME_NUMBER tokens[] = {
    { "=~", T_OP_REG_EQ,    },  /* order is important! */
    { "!~", T_OP_REG_NE,    },
    { "{",  T_LCBRACE,      },
    { "}",  T_RCBRACE,      },
    { "(",  T_LBRACE,       },
    { ")",  T_RBRACE,       },
    { ",",  T_COMMA,        },
    { "+=", T_OP_ADD,       },
    { "-=", T_OP_SUB,       },
    { ":=", T_OP_SET,       },
    { "=*", T_OP_CMP_TRUE,  },
    { "!*", T_OP_CMP_FALSE, },
    { "==", T_OP_CMP_EQ,    },
    { "=",  T_OP_EQ,        },
    { "!=", T_OP_NE,        },
    { ">=", T_OP_GE,        },
    { ">",  T_OP_GT,        },
    { "<=", T_OP_LE,        },
    { "<",  T_OP_LT,        },
    { "#",  T_HASH,         },
    { ";",  T_SEMICOLON,    },
    { NULL, 0,              },
};

/*
 *  This works only as long as special tokens
 *  are max. 2 characters, but it's fast.
 */
#define TOKEN_MATCH(bptr, tptr)                     \
    ( (tptr)[0] == (bptr)[0] &&                     \
      ((tptr)[1] == (bptr)[1] || (tptr)[1] == 0) )

/*
 *  Read a word from a buffer and advance pointer.
 *  This function knows about escapes and quotes.
 *
 *  At end-of-line, buf[0] is set to '\0'.
 *  Returns 0 or special token value.
 */
LRAD_TOKEN gettok(const char **ptr, char *buf, size_t buflen,
                  const LRAD_NAME_NUMBER *tokenlist)
{
    char *s;
    const char *p;
    int quote;
    int escape;
    int x;
    const LRAD_NAME_NUMBER*t;
    LRAD_TOKEN rcode;

    buf[0] = 0;

    /* Skip whitespace */
    p = *ptr;
    while (*p && isspace((int) *p))
        p++;

    if (*p == 0) {
        *ptr = p;
        return T_EOL;
    }

    /*
     *  Might be a 1 or 2 character token.
     */
    if (tokenlist) for (t = tokenlist; t->name; t++) {
        if (TOKEN_MATCH(p, t->name)) {
            strncpy(buf, t->name, buflen);
            buf[buflen-1] = '\0';
            p += strlen(t->name);
            while (isspace((int) *p))
                p++;
            *ptr = p;
            return (LRAD_TOKEN) t->number;
        }
    }

    /* Read word. */
    quote = 0;
    if ((*p == '"') ||
        (*p == '\'') ||
        (*p == '`')) {
        quote = *p;
        p++;
    }
    s = buf;
    escape = 0;

    while (*p && buflen-- > 1) { /* FIXED by Huanghao, 2005/10/11 */
                                 /* To make buf null terminated */
        if (escape) {
            escape = 0;
            switch(*p) {
            case 'r':
                *s++ = '\r';
                break;
            case 'n':
                *s++ = '\n';
                break;
            case 't':
                *s++ = '\t';
                break;
            case '"':
                *s++ = '"';
                break;
            case '\'':
                *s++ = '\'';
                break;
            case '`':
                *s++ = '`';
                break;
            default:
                if (*p >= '0' && *p <= '9' &&
                    sscanf(p, "%3o", &x) == 1) {
                    *s++ = x;
                    p += 2;
                } else
                    *s++ = *p;
                break;
            }
            p++;
            continue;
        }
        if (*p == '\\') {
            p++;
            escape = 1;
            buflen++; /* FIXED by Huanghao, 2005/10/11 */
                      /* Here we continue without inc s, so buflen
                       * should be decreased by 1 */
            continue;
        }
        if (quote && (*p == quote)) {
            p++;
            break;
        }
        if (!quote) {
            if (isspace((int) *p))
                break;
            if (tokenlist) {
                for (t = tokenlist; t->name; t++)
                    if (TOKEN_MATCH(p, t->name))
                        break;
                if (t->name != NULL)
                    break;
            }
        }
        *s++ = *p++;
    }
    *s++ = 0;

    /* Skip whitespace again. */
    while (*p && isspace((int) *p))
        p++;
    *ptr = p;

    /* we got SOME form of output string, even if it is empty */
    switch (quote) {
    default:
        rcode = T_BARE_WORD;
        break;

    case '\'':
        rcode = T_SINGLE_QUOTED_STRING;
        break;

    case '"':
        rcode = T_DOUBLE_QUOTED_STRING;
        break;

    case '`':
        rcode = T_BACK_QUOTED_STRING;
        break;
    }

    return rcode;
}

/*
 *  Read a "word" - this means we don't honor
 *  tokens as delimiters.
 */
int getword(const char **ptr, char *buf, size_t buflen)
{
    return gettok(ptr, buf, buflen, NULL) == T_EOL ? 0 : 1;
}

/*
 *  Read a bare "word" - this means we don't honor
 *  tokens as delimiters.
 */
int getbareword(const char **ptr, char *buf, size_t buflen)
{
    LRAD_TOKEN token;

    token = gettok(ptr, buf, buflen, NULL);
    if (token != T_BARE_WORD) {
        return 0;
    }

    return 1;
}

/*
 *  Read the next word, use tokens as delimiters.
 */
LRAD_TOKEN gettoken(const char **ptr, char *buf, size_t buflen)
{
    return gettok(ptr, buf, buflen, tokens);
}
