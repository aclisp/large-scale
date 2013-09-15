/*
 * md5.h        Structures and prototypes for md5.
 *
 * Version:     $Id: md5.h,v 1.13 2004/05/28 06:27:53 phampson Exp $
 * License:		LGPL, but largely derived from a public domain source.
 *
 */

#ifndef _LRAD_MD5_H
#define _LRAD_MD5_H

/*
 *  FreeRADIUS defines to ensure globally unique MD5 function names,
 *  so that we don't pick up vendor-specific broken MD5 libraries.
 */
#define MD5_CTX		librad_MD5_CTX
#define MD5Init		librad_MD5Init
#define MD5Update	librad_MD5Update
#define MD5Final	librad_MD5Final
#define MD5Transform	librad_MD5Transform
#define MD5         librad_md5_calc

/*  The below was retrieved from
 *  http://www.openbsd.org/cgi-bin/cvsweb/~checkout~/src/sys/crypto/md5.h?rev=1.1
 *  With the following changes: uint64_t => uint32_t[2]
 *  Commented out #include <sys/cdefs.h>
 *  Commented out the __BEGIN and __END _DECLS, and the __attributes.
 */

/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.  This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 */

#define	MD5_BLOCK_LENGTH		64
#define	MD5_DIGEST_LENGTH		16

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

typedef struct MD5Context {
	uint32_t state[4];			/* state */
	uint32_t count[2];			/* number of bits, mod 2^64 */
	uint8_t buffer[MD5_BLOCK_LENGTH];	/* input buffer */
} MD5_CTX;

/* include <sys/cdefs.h> */

#ifdef __cplusplus
extern "C" {
#endif

/* __BEGIN_DECLS */
void	 MD5Init(MD5_CTX *);
void	 MD5Update(MD5_CTX *, const uint8_t *, size_t)
/*		__attribute__((__bounded__(__string__,2,3)))*/;
void	 MD5Final(uint8_t [MD5_DIGEST_LENGTH], MD5_CTX *)
/*		__attribute__((__bounded__(__minbytes__,1,MD5_DIGEST_LENGTH)))*/;
void	 MD5Transform(uint32_t [4], const uint8_t [MD5_BLOCK_LENGTH])
/*		__attribute__((__bounded__(__minbytes__,1,4)))*/
/*		__attribute__((__bounded__(__minbytes__,2,MD5_BLOCK_LENGTH)))*/;
/* __END_DECLS */

void librad_md5_calc(unsigned char *output, unsigned char *input,
		     unsigned int inputlen);

#ifdef __cplusplus
}
#endif

#endif /* _LRAD_MD5_H */
