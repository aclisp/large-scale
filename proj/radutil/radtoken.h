/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/10/08 12:51:13
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_TOKEN_H__
#define __RAD_TOKEN_H__

#include <radutil/radcommon.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum lrad_token_t {
  T_INVALID = 0,                /* invalid token */
  T_EOL,                        /* end of line */
  T_LCBRACE,                    /* { */ /* open curly bracket */
  T_RCBRACE,                    /* } */ /* close curly bracket */
  T_LBRACE,                     /* ( */ /* open parenthesis */
  T_RBRACE,                     /* ) */ /* close parenthesis */
  T_LABRACE,                    /* < */ /* open angle bracket */
  T_RABRACE,                    /* > */ /* close angle bracket */
  T_LSBRACE,                    /* [ */ /* open square bracket */
  T_RSBRACE,                    /* ] */ /* close square bracket */
  T_COMMA,                      /* , */
  T_SEMICOLON,                  /* ; */
  T_COLON,                      /* : */

  T_OP_ADD,                     /* += */
  T_OP_SUB,                     /* -= */
  T_OP_SET,                     /* := */
  T_OP_EQ,                      /* = */
  T_OP_NE,                      /* != */
  T_OP_GE,                      /* >= */
  T_OP_GT,                      /* > */
  T_OP_LE,                      /* <= */
  T_OP_LT,                      /* < */
  T_OP_REG_EQ,                  /* =~ */
  T_OP_REG_NE,                  /* !~ */
  T_OP_CMP_TRUE,                /* =* */
  T_OP_CMP_FALSE,               /* !* */
  T_OP_CMP_EQ,                  /* == */
  T_OP_AND,                     /* && */
  T_OP_OR,                      /* || */
  T_HASH,                       /* # */
  T_BRVBAR,                     /* | */ /* broken vertical bar */
  T_AMP,                        /* & */ /* ampersand */
  T_BARE_WORD,                  /* bare word */
  T_DOUBLE_QUOTED_STRING,       /* "foo" */
  T_SINGLE_QUOTED_STRING,       /* 'foo' */
  T_BACK_QUOTED_STRING,         /* `foo` */
  T_TOKEN_LAST
} LRAD_TOKEN;

#define T_EQSTART   T_OP_ADD
#define T_EQEND     (T_OP_CMP_EQ + 1)
#define T_WORD(t)   (t == T_BARE_WORD ||            \
                     t == T_DOUBLE_QUOTED_STRING || \
                     t == T_SINGLE_QUOTED_STRING || \
                     t == T_BACK_QUOTED_STRING)

int        getword     (const char **ptr, char *buf, size_t buflen);
int        getbareword (const char **ptr, char *buf, size_t buflen);
LRAD_TOKEN gettoken    (const char **ptr, char *buf, size_t buflen);

LRAD_TOKEN gettok(const char **ptr, char *buf, size_t buflen,
                  const LRAD_NAME_NUMBER *tokenlist);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_TOKEN_H__ */
