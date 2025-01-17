/*******************************************************************************
 * Copyright IBM Corp. and others 1991
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef fltconst_h
#define fltconst_h

#include "omrcomp.h"

/* IEEE floats consist of: sign bit, exponent field, significand field
	single:  31 = sign bit, 30..23 = exponent (8 bits), 22..0 = significand (23 bits)
	double:  63 = sign bit, 62..52 = exponent (11 bits), 51..0 = significand (52 bits)

	inf				==	(all exponent bits set) and (all mantissa bits clear)
	nan				==	(all exponent bits set) and (at least one mantissa bit set)
	finite			==	(at least one exponent bit clear)
	zero      		==	(all exponent bits clear) and (all mantissa bits clear)
	denormal		==	(all exponent bits clear) and (at least one mantissa bit set)
	positive		==	sign bit clear
	negative		==	sign bit set
*/


#define	MAX_U32_DOUBLE	(ESDOUBLE) (4294967296.0)	/* 2^32 */
#define	MAX_U32_SINGLE		(ESSINGLE) (4294967296.0)	/* 2^32 */

#define J9_POS_PI      (ESDOUBLE)(3.141592653589793)

#ifdef OMR_ENV_LITTLE_ENDIAN
#define DOUBLE_LO_OFFSET		0
#define DOUBLE_HI_OFFSET			1
#define LONG_LO_OFFSET			0
#define LONG_HI_OFFSET			1
#else
#define DOUBLE_LO_OFFSET		1
#define DOUBLE_HI_OFFSET			0
#define LONG_LO_OFFSET			1
#define LONG_HI_OFFSET			0
#endif

#define RETURN_FINITE				0
#define RETURN_NAN					1
#define RETURN_POS_INF			2
#define RETURN_NEG_INF			3

#define DOUBLE_SIGN_MASK_HI				0x80000000
#define DOUBLE_EXPONENT_MASK_HI		0x7FF00000
#define DOUBLE_MANTISSA_MASK_LO		0xFFFFFFFF
#define DOUBLE_MANTISSA_MASK_HI		0x000FFFFF

#if defined(OMR_ENV_DATA64)
#define DOUBLE_SIGN_MASK				J9CONST64(0x8000000000000000)
#define DOUBLE_EXPONENT_MASK		J9CONST64(0x7FF0000000000000)
#define DOUBLE_MANTISSA_MASK		J9CONST64(0x000FFFFFFFFFFFFF)
#define DOUBLE_NAN_BITS					(DOUBLE_EXPONENT_MASK | J9CONST64(0x0008000000000000))
#endif /* defined(OMR_ENV_DATA64) */

#define SINGLE_SIGN_MASK				0x80000000
#define SINGLE_EXPONENT_MASK	0x7F800000
#define SINGLE_MANTISSA_MASK	0x007FFFFF

#define SINGLE_NAN_BITS				(SINGLE_EXPONENT_MASK | 0x00400000)

typedef union u64u32dbl_tag {
	uint64_t    u64val;
	uint32_t    u32val[2];
	int32_t    i32val[2];
	double  dval;
} U64U32DBL;

/* Replace P_FLOAT_HI and P_FLOAT_LOW */
/* These macros are used to access the high and low 32-bit parts of a double (64-bit) value. */
#define LOW_U32_FROM_DBL_PTR(dblptr) (((U64U32DBL *)(dblptr))->u32val[DOUBLE_LO_OFFSET])
#define HIGH_U32_FROM_DBL_PTR(dblptr) (((U64U32DBL *)(dblptr))->u32val[DOUBLE_HI_OFFSET])
#define LOW_I32_FROM_DBL_PTR(dblptr) (((U64U32DBL *)(dblptr))->i32val[DOUBLE_LO_OFFSET])
#define HIGH_I32_FROM_DBL_PTR(dblptr) (((U64U32DBL *)(dblptr))->i32val[DOUBLE_HI_OFFSET])
#define LOW_U32_FROM_DBL(dbl) LOW_U32_FROM_DBL_PTR(&(dbl))
#define HIGH_U32_FROM_DBL(dbl) HIGH_U32_FROM_DBL_PTR(&(dbl))
#define LOW_I32_FROM_DBL(dbl) LOW_I32_FROM_DBL_PTR(&(dbl))
#define HIGH_I32_FROM_DBL(dbl) HIGH_I32_FROM_DBL_PTR(&(dbl))

#define LOW_U32_FROM_LONG64_PTR(long64ptr) (((U64U32DBL *)(long64ptr))->u32val[LONG_LO_OFFSET])
#define HIGH_U32_FROM_LONG64_PTR(long64ptr) (((U64U32DBL *)(long64ptr))->u32val[LONG_HI_OFFSET])
#define LOW_I32_FROM_LONG64_PTR(long64ptr) (((U64U32DBL *)(long64ptr))->i32val[LONG_LO_OFFSET])
#define HIGH_I32_FROM_LONG64_PTR(long64ptr) (((U64U32DBL *)(long64ptr))->i32val[LONG_HI_OFFSET])
#define LOW_U32_FROM_LONG64(long64) LOW_U32_FROM_LONG64_PTR(&(long64))
#define HIGH_U32_FROM_LONG64(long64) HIGH_U32_FROM_LONG64_PTR(&(long64))
#define LOW_I32_FROM_LONG64(long64) LOW_I32_FROM_LONG64_PTR(&(long64))
#define HIGH_I32_FROM_LONG64(long64) HIGH_I32_FROM_LONG64_PTR(&(long64))

#define IS_ZERO_DBL_PTR(dblptr) ((LOW_U32_FROM_DBL_PTR(dblptr) == 0) && ((HIGH_U32_FROM_DBL_PTR(dblptr) == 0) || (HIGH_U32_FROM_DBL_PTR(dblptr) == DOUBLE_SIGN_MASK_HI)))
#define IS_ONE_DBL_PTR(dblptr) ((HIGH_U32_FROM_DBL_PTR(dblptr) == 0x3ff00000 || HIGH_U32_FROM_DBL_PTR(dblptr) == 0xbff00000) && (LOW_U32_FROM_DBL_PTR(dblptr) == 0))
#define IS_NAN_DBL_PTR(dblptr) (((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_EXPONENT_MASK_HI) == DOUBLE_EXPONENT_MASK_HI) && (LOW_U32_FROM_DBL_PTR(dblptr) | (HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_MANTISSA_MASK_HI)))
#define IS_INF_DBL_PTR(dblptr) (((HIGH_U32_FROM_DBL_PTR(dblptr) & (DOUBLE_EXPONENT_MASK_HI|DOUBLE_MANTISSA_MASK_HI)) == DOUBLE_EXPONENT_MASK_HI) && (LOW_U32_FROM_DBL_PTR(dblptr) == 0))
#define IS_DENORMAL_DBL_PTR(dblptr) (((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_EXPONENT_MASK_HI) == 0) && ((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_MANTISSA_MASK_HI) != 0 || (LOW_U32_FROM_DBL_PTR(dblptr) != 0)))
#define IS_FINITE_DBL_PTR(dblptr) ((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_EXPONENT_MASK_HI) < DOUBLE_EXPONENT_MASK_HI)
#define IS_POSITIVE_DBL_PTR(dblptr) ((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_SIGN_MASK_HI) == 0)
#define IS_NEGATIVE_DBL_PTR(dblptr) ((HIGH_U32_FROM_DBL_PTR(dblptr) & DOUBLE_SIGN_MASK_HI) != 0)
#define IS_NEGATIVE_MAX_DBL_PTR(dblptr) ((HIGH_U32_FROM_DBL_PTR(dblptr) == 0xFFEFFFFF) && (LOW_U32_FROM_DBL_PTR(dblptr) == 0xFFFFFFFF))

#define IS_ZERO_DBL(dbl) IS_ZERO_DBL_PTR(&(dbl))
#define IS_ONE_DBL(dbl) IS_ONE_DBL_PTR(&(dbl))
#define IS_NAN_DBL(dbl) IS_NAN_DBL_PTR(&(dbl))
#define IS_INF_DBL(dbl) IS_INF_DBL_PTR(&(dbl))
#define IS_DENORMAL_DBL(dbl) IS_DENORMAL_DBL_PTR(&(dbl))
#define IS_FINITE_DBL(dbl) IS_FINITE_DBL_PTR(&(dbl))
#define IS_POSITIVE_DBL(dbl) IS_POSITIVE_DBL_PTR(&(dbl))
#define IS_NEGATIVE_DBL(dbl) IS_NEGATIVE_DBL_PTR(&(dbl))
#define IS_NEGATIVE_MAX_DBL(dbl) IS_NEGATIVE_MAX_DBL_PTR(&(dbl))

#define IS_ZERO_SNGL_PTR(fltptr)  ((*U32P((fltptr)) & (uint32_t)~SINGLE_SIGN_MASK) == (uint32_t)0)
#define IS_ONE_SNGL_PTR(fltptr) ((*U32P((fltptr)) == 0x3f800000) || (*U32P((fltptr)) == 0xbf800000))
#define IS_NAN_SNGL_PTR(fltptr)  ((*U32P((fltptr)) & (uint32_t)~SINGLE_SIGN_MASK) > (uint32_t)SINGLE_EXPONENT_MASK)
#define IS_INF_SNGL_PTR(fltptr)  ((*U32P((fltptr)) & (uint32_t)~SINGLE_SIGN_MASK) == (uint32_t)SINGLE_EXPONENT_MASK)
#define IS_DENORMAL_SNGL_PTR(fltptr)  (((*U32P((fltptr)) & (uint32_t)~SINGLE_SIGN_MASK)-(uint32_t)1) < (uint32_t)SINGLE_MANTISSA_MASK)
#define IS_FINITE_SNGL_PTR(fltptr)  ((*U32P((fltptr)) & (uint32_t)~SINGLE_SIGN_MASK) < (uint32_t)SINGLE_EXPONENT_MASK)
#define IS_POSITIVE_SNGL_PTR(fltptr)  ((*U32P((fltptr)) & (uint32_t)SINGLE_SIGN_MASK) == (uint32_t)0)
#define IS_NEGATIVE_SNGL_PTR(fltptr)  ((*U32P((fltptr)) & (uint32_t)SINGLE_SIGN_MASK) != (uint32_t)0)

#define IS_ZERO_SNGL(flt) IS_ZERO_SNGL_PTR(&(flt))
#define IS_ONE_SNGL(flt) IS_ONE_SNGL_PTR(&(flt))
#define IS_NAN_SNGL(flt) IS_NAN_SNGL_PTR(&(flt))
#define IS_INF_SNGL(flt) IS_INF_SNGL_PTR(&(flt))
#define IS_DENORMAL_SNGL(flt) IS_DENORMAL_SNGL_PTR(&(flt))
#define IS_FINITE_SNGL(flt) IS_FINITE_SNGL_PTR(&(flt))
#define IS_POSITIVE_SNGL(flt) IS_POSITIVE_SNGL_PTR(&(flt))
#define IS_NEGATIVE_SNGL(flt) IS_NEGATIVE_SNGL_PTR(&(flt))

#define SET_NAN_DBL_PTR(dblptr) HIGH_U32_FROM_DBL_PTR(dblptr) = (DOUBLE_EXPONENT_MASK_HI | 0x00080000); LOW_U32_FROM_DBL_PTR(dblptr) = 0
#define SET_PZERO_DBL_PTR(dblptr) HIGH_U32_FROM_DBL_PTR(dblptr) = 0; LOW_U32_FROM_DBL_PTR(dblptr) = 0
#define SET_NZERO_DBL_PTR(dblptr) HIGH_U32_FROM_DBL_PTR(dblptr) = DOUBLE_SIGN_MASK_HI; LOW_U32_FROM_DBL_PTR(dblptr) = 0
#define SET_PINF_DBL_PTR(dblptr) HIGH_U32_FROM_DBL_PTR(dblptr) = DOUBLE_EXPONENT_MASK_HI; LOW_U32_FROM_DBL_PTR(dblptr) = 0
#define SET_NINF_DBL_PTR(dblptr) HIGH_U32_FROM_DBL_PTR(dblptr) = (DOUBLE_EXPONENT_MASK_HI | DOUBLE_SIGN_MASK_HI); LOW_U32_FROM_DBL_PTR(dblptr) = 0

#define SET_NAN_SNGL_PTR(fltptr)  *U32P((fltptr)) = ((uint32_t)SINGLE_NAN_BITS)
#define SET_PZERO_SNGL_PTR(fltptr) *U32P((fltptr)) = 0
#define SET_NZERO_SNGL_PTR(fltptr) *U32P((fltptr)) = SINGLE_SIGN_MASK
#define SET_PINF_SNGL_PTR(fltptr) *U32P((fltptr)) = SINGLE_EXPONENT_MASK
#define SET_NINF_SNGL_PTR(fltptr) *U32P((fltptr)) = (SINGLE_EXPONENT_MASK | SINGLE_SIGN_MASK)

#if defined(OMR_ENV_DATA64)
#define PTR_DOUBLE_VALUE(dstPtr, aDoublePtr) ((U64U32DBL *)(aDoublePtr))->u64val = ((U64U32DBL *)(dstPtr))->u64val
#define PTR_DOUBLE_STORE(dstPtr, aDoublePtr) ((U64U32DBL *)(dstPtr))->u64val = ((U64U32DBL *)(aDoublePtr))->u64val
#define STORE_LONG(dstPtr, hi, lo) ((U64U32DBL *)(dstPtr))->u64val = (((uint64_t)(hi)) << 32) | (lo)
#else
/* on some platforms (HP720) we cannot reference an unaligned float.  Build them by hand, one uint32_t at a time. */
#ifdef ATOMIC_FLOAT_ACCESS
#define PTR_DOUBLE_STORE(dstPtr, aDoublePtr) HIGH_U32_FROM_DBL_PTR(dstPtr) = HIGH_U32_FROM_DBL_PTR(aDoublePtr); LOW_U32_FROM_DBL_PTR(dstPtr) = LOW_U32_FROM_DBL_PTR(aDoublePtr)
#define PTR_DOUBLE_VALUE(dstPtr, aDoublePtr) HIGH_U32_FROM_DBL_PTR(aDoublePtr) = HIGH_U32_FROM_DBL_PTR(dstPtr); LOW_U32_FROM_DBL_PTR(aDoublePtr) = LOW_U32_FROM_DBL_PTR(dstPtr)
#else
#define PTR_DOUBLE_STORE(dstPtr, aDoublePtr) (*(dstPtr) = *(aDoublePtr))
#define PTR_DOUBLE_VALUE(dstPtr, aDoublePtr) (*(aDoublePtr) = *(dstPtr))
#endif
#define STORE_LONG(dstPtr, hi, lo) HIGH_U32_FROM_LONG64_PTR(dstPtr) = (hi); LOW_U32_FROM_LONG64_PTR(dstPtr) = (lo)
#endif

#define PTR_SINGLE_VALUE(dstPtr, aSinglePtr) (*U32P(aSinglePtr) = *U32P(dstPtr))
#define PTR_SINGLE_STORE(dstPtr, aSinglePtr) *((uint32_t *)(dstPtr)) = (*U32P(aSinglePtr))

#endif     /* fltconst_h */
