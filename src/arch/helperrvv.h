#ifndef HELPERRVV_H
#define HELPERRVV_H

#if !defined(SLEEF_GENHEADER)
#include <riscv_vector.h>
#include "misc.h"

#if defined(VECTLENDP) || defined(VECTLENSP)
#error VECTLENDP or VECTLENSP already defined
#endif
#endif // #if !defined(SLEEF_GENHEADER)

#if CONFIG == 1
#define ISANAME "RISC-V Vector Extension with Min. VLEN"
#define SLEEF_RVV_VLEN __riscv_v_min_vlen
#else
#define ISANAME "RISC-V Vector Extension VLEN=2^"#CONFIG
#define SLEEF_RVV_VLEN (1 << CONFIG)
#endif

#ifndef CONFIG
#error CONFIG macro not defined
#endif

#define ENABLE_SP
#define ENABLE_FMA_DP
#define ENABLE_DP

static INLINE int vavailability_i(int name) { return -1; }

////////////////////////////////////////////////////////////////////////////////
// RISC-V Vector Types
////////////////////////////////////////////////////////////////////////////////

// About the RISC-V Vector type translations:
//
// Because the single- and double-precision versions of the RVV port have
// conflicting definitions of the vmask and vopmask types, they can only
// be defined for at most one precision level in a single translation unit.
// Any functions that use vmask or vopmask types are thus enabled only by the
// corresponding ENABLE_RVV_SP or ENABLE_RVV_DP macro guards.
#if defined(ENABLE_RVV_SP) && defined(ENABLE_RVV_DP)
#error Cannot simultaneously define ENABLE_RVV_SP and ENABLE_RVV_DP
#endif

#ifdef ENABLE_RVV_SP
// Types that conflict with ENABLE_RVV_DP
#ifdef ENABLE_RVVM1
typedef vuint64m2_t vmask;
typedef vbool32_t vopmask;
#else
typedef vuint64m4_t vmask;
typedef vbool16_t vopmask;
#endif
#endif

#ifdef ENABLE_RVV_DP
// Types that conflict with ENABLE_RVV_SP
#ifdef ENABLE_RVVM1
typedef vuint64m1_t vmask;
typedef vbool64_t vopmask;
#else
typedef vuint64m2_t vmask;
typedef vbool32_t vopmask;
#endif
#endif

// LMUL-Dependent Type & Macro Definitions:
//
// Some SLEEF types are multi-value structs. RVV vectors have unknown length at
// compile time, so they cannote appear in a struct in Clang. They are instead
// represented as single vectors with "members" packed into the registers of a
// wide-LMUL register group. In the largest cases (ddi_t and ddf_t), this
// requires LMUL=8 if the base type (vfloat or vdouble) has LMUL=2, meaning
// LMUL=2 is currently the widest option for SLEEF function argument types.
#ifdef ENABLE_RVVM1

typedef vint32mf2_t vint;
typedef vfloat64m1_t vdouble;
typedef vfloat64m2_t vdouble2;
typedef vfloat64m4_t vdouble3;
typedef vfloat64m4_t dd2;
typedef vuint64m2_t vquad;
typedef vint32m2_t di_t;
typedef vint32m4_t ddi_t;
typedef vfloat32m1_t vfloat;
typedef vfloat32m2_t vfloat2;
typedef vfloat32m4_t df2;
typedef vint32m1_t vint2;
typedef vint32m2_t fi_t;
typedef vint32m4_t dfi_t;
#define SLEEF_RVV_SP_LMUL 1
#define SLEEF_RVV_DP_LMUL 1
#define VECTLENSP (SLEEF_RVV_SP_LMUL * SLEEF_RVV_VLEN / 32)
#define VECTLENDP (SLEEF_RVV_DP_LMUL * SLEEF_RVV_VLEN / 64)
#define SLEEF_RVV_SP_VCAST_VF_F vfmv_v_f_f32m1
#define SLEEF_RVV_SP_VCAST_VI2_I vmv_v_x_i32m1
#define SLEEF_RVV_SP_VCAST_VU2_U vmv_v_x_u32m1
#define SLEEF_RVV_SP_VREINTERPRET_VF vreinterpret_f32m1
#define SLEEF_RVV_SP_VREINTERPRET_VF2 vreinterpret_f32m2
#define SLEEF_RVV_SP_VREINTERPRET_VM vreinterpret_u64m2
#define SLEEF_RVV_SP_VREINTERPRET_VI2 vreinterpret_i32m1
#define SLEEF_RVV_SP_VREINTERPRET_2VI vreinterpret_i32m2
#define SLEEF_RVV_SP_VREINTERPRET_4VI vreinterpret_i32m4
#define SLEEF_RVV_SP_VREINTERPRET_VU vreinterpret_u32m1
#define SLEEF_RVV_SP_VREINTERPRET_VU2 vreinterpret_u32m1
#define SLEEF_RVV_SP_VGET_VI2 vget_i32m1
#define SLEEF_RVV_SP_VGET_2VI vget_i32m2
#define SLEEF_RVV_SP_VGET_VF vget_f32m1
#define SLEEF_RVV_SP_VGET_VF2 vget_f32m2
#define SLEEF_RVV_SP_VGET_4VF vget_f32m4
#define SLEEF_RVV_SP_VGET_VU2 vget_u32m2
#define SLEEF_RVV_SP_LOAD_VF vle32_v_f32m1
#define SLEEF_RVV_SP_LOAD_VI2 vle32_v_i32m1
#define SLEEF_RVV_SP_VCAST_VM_U vmv_v_x_u64m2
#define SLEEF_RVV_SP_VREINTERPRET_VM vreinterpret_u64m2
#define SLEEF_RVV_SP_VREINTERPRET_VI64 vreinterpret_i64m2
#define SLEEF_RVV_SP_VREINTERPRET_VU vreinterpret_u32m1
#define SLEEF_RVV_SP_LOAD_VI vle32_v_i32m1
#define SLEEF_RVV_DP_VCAST_VD_D vfmv_v_f_f64m1
#define SLEEF_RVV_DP_VCAST_VD_VI(x) vfwcvt_f(x, VECTLENDP)
#define SLEEF_RVV_DP_VCAST_VI_I vmv_v_x_i32mf2
#define SLEEF_RVV_DP_VCAST_VM_U vmv_v_x_u64m1
#define SLEEF_RVV_DP_VREINTERPRET_VD vreinterpret_f64m1
#define SLEEF_RVV_DP_VREINTERPRET_VD2 vreinterpret_f64m2
#define SLEEF_RVV_DP_VREINTERPRET_4VI_VD2(x) \
  vreinterpret_v_i64m2_i32m2(vreinterpret_i64m2(x))
#define SLEEF_RVV_DP_VREINTERPRET_VD2_4VI(x) \
  vreinterpret_f64m2(vreinterpret_v_i32m2_i64m2(x))
#define SLEEF_RVV_DP_VREINTERPRET_4VD vreinterpret_f64m4
#define SLEEF_RVV_DP_VREINTERPRET_4VD_8VI(x) \
  vreinterpret_f64m4(vreinterpret_v_i32m4_i64m4(x))
#define SLEEF_RVV_DP_VREINTERPRET_8VI_4VD(x) \
  vreinterpret_v_i64m4_i32m4(vreinterpret_i64m4(x))
#define SLEEF_RVV_DP_VREINTERPRET_VM vreinterpret_u64m1
#define SLEEF_RVV_DP_VREINTERPRET_VI64 vreinterpret_i64m1
#define SLEEF_RVV_DP_VREINTERPRET_VU64 vreinterpret_u64m1
#define SLEEF_RVV_DP_VREINTERPRET_VI vreinterpret_i32mf2
#define SLEEF_RVV_DP_VREINTERPRET_VI2 vreinterpret_i32m1
#define SLEEF_RVV_DP_VREINTERPRET_2VI vreinterpret_i32m2
#define SLEEF_RVV_DP_VREINTERPRET_4VI vreinterpret_i32m4
#define SLEEF_RVV_DP_VREINTERPRET_8VI vreinterpret_i32m8
#define SLEEF_RVV_DP_VREINTERPRET_VU vreinterpret_u32mf2
#define SLEEF_RVV_DP_VREINTERPRET_2VU vreinterpret_u32m2
#define SLEEF_RVV_DP_VREINTERPRET_4VU vreinterpret_u32m4
#define SLEEF_RVV_DP_VGET_VM vget_u64m1
#define SLEEF_RVV_DP_VGET_VD vget_f64m1
#define SLEEF_RVV_DP_VGET_VD2 vget_f64m2
#define SLEEF_RVV_DP_VGET_4VD vget_f64m2
#define SLEEF_RVV_DP_VGET_VI vget_i32m1
#define SLEEF_RVV_DP_VGET_VI2 vget_i32m1
#define SLEEF_RVV_DP_VGET_2VI vget_i32m1
#define SLEEF_RVV_DP_VGET_4VI vget_i32m2
#define SLEEF_RVV_DP_VGET_8VI vget_i32m4
#define SLEEF_RVV_DP_VGET_VU vget_u32m1
#define SLEEF_RVV_DP_LOAD_VD vle64_v_f64m1
#define SLEEF_RVV_DP_LOAD_VI vle32_v_i32mf2

#else

typedef vint32m1_t vint;
typedef vfloat64m2_t vdouble;
typedef vfloat64m4_t vdouble2;
typedef vfloat64m8_t vdouble3;
typedef vfloat64m8_t dd2;
typedef vuint64m4_t vquad;
typedef vint32m4_t di_t;
typedef vint32m8_t ddi_t;
typedef vfloat32m2_t vfloat;
typedef vfloat32m4_t vfloat2;
typedef vfloat32m8_t df2;
typedef vint32m2_t vint2;
typedef vint32m4_t fi_t;
typedef vint32m8_t dfi_t;
#define SLEEF_RVV_SP_LMUL 2
#define SLEEF_RVV_DP_LMUL 2
#define VECTLENSP (SLEEF_RVV_SP_LMUL * SLEEF_RVV_VLEN / 32)
#define VECTLENDP (SLEEF_RVV_DP_LMUL * SLEEF_RVV_VLEN / 64)
#define SLEEF_RVV_SP_VCAST_VF_F vfmv_v_f_f32m2
#define SLEEF_RVV_SP_VCAST_VI2_I vmv_v_x_i32m2
#define SLEEF_RVV_SP_VCAST_VU2_U vmv_v_x_u32m2
#define SLEEF_RVV_SP_VREINTERPRET_VF vreinterpret_f32m2
#define SLEEF_RVV_SP_VREINTERPRET_VF2 vreinterpret_f32m4
#define SLEEF_RVV_SP_VREINTERPRET_VM vreinterpret_u64m4
#define SLEEF_RVV_SP_VREINTERPRET_VI2 vreinterpret_i32m2
#define SLEEF_RVV_SP_VREINTERPRET_2VI vreinterpret_i32m4
#define SLEEF_RVV_SP_VREINTERPRET_4VI vreinterpret_i32m8
#define SLEEF_RVV_SP_VREINTERPRET_VU vreinterpret_u32m2
#define SLEEF_RVV_SP_VREINTERPRET_VU2 vreinterpret_u32m2
#define SLEEF_RVV_SP_VGET_VI2 vget_i32m2
#define SLEEF_RVV_SP_VGET_2VI vget_i32m4
#define SLEEF_RVV_SP_VGET_VF vget_f32m2
#define SLEEF_RVV_SP_VGET_VF2 vget_f32m4
#define SLEEF_RVV_SP_VGET_4VF vget_f32m8
#define SLEEF_RVV_SP_VGET_VU2 vget_u32m4
#define SLEEF_RVV_SP_LOAD_VF vle32_v_f32m2
#define SLEEF_RVV_SP_LOAD_VI2 vle32_v_i32m2
#define SLEEF_RVV_SP_VCAST_VM_U vmv_v_x_u64m4
#define SLEEF_RVV_SP_VREINTERPRET_VM vreinterpret_u64m4
#define SLEEF_RVV_SP_VREINTERPRET_VI64 vreinterpret_i64m4
#define SLEEF_RVV_SP_VREINTERPRET_VU vreinterpret_u32m2
#define SLEEF_RVV_SP_LOAD_VI vle32_v_i32m2
#define SLEEF_RVV_DP_VCAST_VD_D vfmv_v_f_f64m2
#define SLEEF_RVV_DP_VCAST_VD_VI(x) vfwcvt_f(x, VECTLENDP)
#define SLEEF_RVV_DP_VCAST_VI_I vmv_v_x_i32m1
#define SLEEF_RVV_DP_VCAST_VM_U vmv_v_x_u64m2
#define SLEEF_RVV_DP_VREINTERPRET_VD vreinterpret_f64m2
#define SLEEF_RVV_DP_VREINTERPRET_VD2 vreinterpret_f64m4
#define SLEEF_RVV_DP_VREINTERPRET_4VI_VD2(x) \
  vreinterpret_v_i64m4_i32m4(vreinterpret_i64m4(x))
#define SLEEF_RVV_DP_VREINTERPRET_VD2_4VI(x) \
  vreinterpret_f64m4(vreinterpret_v_i32m4_i64m4(x))
#define SLEEF_RVV_DP_VREINTERPRET_4VD vreinterpret_f64m8
#define SLEEF_RVV_DP_VREINTERPRET_4VD_8VI(x) \
  vreinterpret_f64m8(vreinterpret_v_i32m8_i64m8(x))
#define SLEEF_RVV_DP_VREINTERPRET_8VI_4VD(x) \
  vreinterpret_v_i64m8_i32m8(vreinterpret_i64m8(x))
#define SLEEF_RVV_DP_VREINTERPRET_VM vreinterpret_u64m2
#define SLEEF_RVV_DP_VREINTERPRET_VI64 vreinterpret_i64m2
#define SLEEF_RVV_DP_VREINTERPRET_VU64 vreinterpret_u64m2
#define SLEEF_RVV_DP_VREINTERPRET_VI vreinterpret_i32m1
#define SLEEF_RVV_DP_VREINTERPRET_VI2 vreinterpret_i32m1
#define SLEEF_RVV_DP_VREINTERPRET_2VI vreinterpret_i32m2
#define SLEEF_RVV_DP_VREINTERPRET_4VI vreinterpret_i32m4
#define SLEEF_RVV_DP_VREINTERPRET_8VI vreinterpret_i32m8
#define SLEEF_RVV_DP_VREINTERPRET_VU vreinterpret_u32m1
#define SLEEF_RVV_DP_VREINTERPRET_2VU vreinterpret_u32m2
#define SLEEF_RVV_DP_VREINTERPRET_4VU vreinterpret_u32m4
#define SLEEF_RVV_DP_VGET_VM vget_u64m2
#define SLEEF_RVV_DP_VGET_VD vget_f64m2
#define SLEEF_RVV_DP_VGET_VD2 vget_f64m4
#define SLEEF_RVV_DP_VGET_4VD vget_f64m4
#define SLEEF_RVV_DP_VGET_VI vget_i32m1
#define SLEEF_RVV_DP_VGET_VI2 vget_i32m1
#define SLEEF_RVV_DP_VGET_2VI vget_i32m2
#define SLEEF_RVV_DP_VGET_4VI vget_i32m4
#define SLEEF_RVV_DP_VGET_8VI vget_i32m8
#define SLEEF_RVV_DP_VGET_VU vget_u32m1
#define SLEEF_RVV_DP_LOAD_VD vle64_v_f64m2
#define SLEEF_RVV_DP_LOAD_VI vle32_v_i32m1

#endif // ENABLE_RVVM1

////////////////////////////////////////////////////////////////////////////////
// Single-Precision Functions
////////////////////////////////////////////////////////////////////////////////

/****************************************/
/* Multi-value and multi-word types     */
/****************************************/
// fi type
static INLINE vfloat figetd_vf_di(fi_t d) {
  return SLEEF_RVV_SP_VREINTERPRET_VF(SLEEF_RVV_SP_VGET_VI2(d, 0));
}
static INLINE vint2 figeti_vi2_di(fi_t d) {
  return SLEEF_RVV_SP_VGET_VI2(d, 1);
}
static INLINE fi_t fisetdi_fi_vf_vi2(vfloat d, vint2 i) {
  fi_t res;
  res = vset(res, 0, SLEEF_RVV_SP_VREINTERPRET_VI2(d));
  res = vset(res, 1, i);
  return res;
}
static INLINE vfloat2 dfigetdf_vf2_dfi(dfi_t d) {
  return SLEEF_RVV_SP_VREINTERPRET_VF2(SLEEF_RVV_SP_VGET_2VI(d, 0));
}
static INLINE vint2 dfigeti_vi2_dfi(dfi_t d) {
  return SLEEF_RVV_SP_VGET_VI2(d, 2);
}
static INLINE dfi_t dfisetdfi_dfi_vf2_vi2(vfloat2 v, vint2 i) {
  dfi_t res;
  res = vset(res, 0, SLEEF_RVV_SP_VREINTERPRET_2VI(v));
  res = vset(res, 2, i);
  return res;
}
static INLINE dfi_t dfisetdf_dfi_dfi_vf2(dfi_t dfi, vfloat2 v) {
  return vset(dfi, 0, SLEEF_RVV_SP_VREINTERPRET_2VI(v));
}
// vfloat2 type
static INLINE vfloat vf2getx_vf_vf2(vfloat2 v) {
  return SLEEF_RVV_SP_VGET_VF(v, 0);
}
static INLINE vfloat vf2gety_vf_vf2(vfloat2 v) {
  return SLEEF_RVV_SP_VGET_VF(v, 1);
}
static INLINE vfloat2 vf2setxy_vf2_vf_vf(vfloat x, vfloat y) {
  vfloat2 res;
  res = vset(res, 0, x);
  res = vset(res, 1, y);
  return res;
}
static INLINE vfloat2 vf2setx_vf2_vf2_vf(vfloat2 v, vfloat d) {
  return vset(v, 0, d);
}
static INLINE vfloat2 vf2sety_vf2_vf2_vf(vfloat2 v, vfloat d) {
  return vset(v, 1, d);
}
// df2 type
static df2 df2setab_df2_vf2_vf2(vfloat2 a, vfloat2 b) {
  df2 res;
  res = vset(res, 0, a);
  res = vset(res, 1, b);
  return res;
}
static vfloat2 df2geta_vf2_df2(df2 d) { return SLEEF_RVV_SP_VGET_VF2(d, 0); }
static vfloat2 df2getb_vf2_df2(df2 d) { return SLEEF_RVV_SP_VGET_VF2(d, 1); }
static INLINE vint2 vreinterpret_vi2_vf(vfloat vf) {
  return SLEEF_RVV_SP_VREINTERPRET_VI2(vf);
}
static INLINE vfloat vreinterpret_vf_vi2(vint2 vi) {
  return SLEEF_RVV_SP_VREINTERPRET_VF(vi);
}


/****************************************/
/* Type Conversions and Broadcasts      */
/****************************************/
static INLINE vfloat vcast_vf_f(float f) {
  return SLEEF_RVV_SP_VCAST_VF_F(f, VECTLENSP);
}
static INLINE vfloat vrint_vf_vf(vfloat vd) {
  // It is not currently possible to safely set frm for intrinsics,
  // so emulate round-to-nearest behavior
  vfloat half = SLEEF_RVV_SP_VCAST_VF_F(0.5, VECTLENSP);
  half = vfsgnj(half, vd, VECTLENSP);
  vfloat res = vfadd(vd, half, VECTLENSP);
  vint2 i = vfcvt_rtz_x(res, VECTLENSP);
  return vfcvt_f(i, VECTLENSP);
}
static INLINE vfloat vcast_vf_vi2(vint2 vi) {
  return vfcvt_f(vi, VECTLENSP);
}
static INLINE vint2 vcast_vi2_i(int i) {
  return SLEEF_RVV_SP_VCAST_VI2_I(i, VECTLENSP);
}
static INLINE vint2 vrint_vi2_vf(vfloat vf) {
  // It is not currently possible to safely set frm for intrinsics,
  // so emulate round-to-nearest behavior
  vfloat half = SLEEF_RVV_SP_VCAST_VF_F(0.5, VECTLENSP);
  half = vfsgnj(half, vf, VECTLENSP);
  vfloat res = vfadd(vf, half, VECTLENSP);
  return vfcvt_rtz_x(res, VECTLENSP);
}
static INLINE vint2 vtruncate_vi2_vf(vfloat vf) {
  return vfcvt_rtz_x(vf, VECTLENSP);
}
static INLINE vfloat vtruncate_vf_vf(vfloat vf) {
  return vcast_vf_vi2(vtruncate_vi2_vf(vf));
}


/****************************************/
/* Memory Operations                    */
/****************************************/
static INLINE vfloat vload_vf_p(const float *ptr) {
  return SLEEF_RVV_SP_LOAD_VF(ptr, VECTLENSP);
}
static INLINE vfloat vloadu_vf_p(const float *ptr) {
  return SLEEF_RVV_SP_LOAD_VF(ptr, VECTLENSP);
}
static INLINE void vstore_v_p_vf(float *ptr, vfloat v) {
  vse32(ptr, v, VECTLENSP);
}
static INLINE void vstoreu_v_p_vf(float *ptr, vfloat v) {
  vse32(ptr, v, VECTLENSP);
}
static INLINE void vstoreu_v_p_vi2(int32_t *ptr, vint2 v) {
  vse32(ptr, v, VECTLENSP);
}
static INLINE vfloat vgather_vf_p_vi2(const float *ptr, vint2 vi2) {
  return vluxei32(ptr, vmul(SLEEF_RVV_SP_VREINTERPRET_VU(vi2), sizeof(float), VECTLENSP), VECTLENSP);
}


/****************************************/
/* Floating-Point Arithmetic            */
/****************************************/
static INLINE vfloat vadd_vf_vf_vf(vfloat x, vfloat y) {
  return vfadd(x, y, VECTLENSP);
}
static INLINE vfloat vsub_vf_vf_vf(vfloat x, vfloat y) {
  return vfsub(x, y, VECTLENSP);
}
static INLINE vfloat vmul_vf_vf_vf(vfloat x, vfloat y) {
  return vfmul(x, y, VECTLENSP);
}
static INLINE vfloat vdiv_vf_vf_vf(vfloat x, vfloat y) {
  return vfdiv(x, y, VECTLENSP);
}
static INLINE vfloat vmax_vf_vf_vf(vfloat x, vfloat y) {
  return vfmax(x, y, VECTLENSP);
}
static INLINE vfloat vmin_vf_vf_vf(vfloat x, vfloat y) {
  return vfmin(x, y, VECTLENSP);
}
static INLINE vfloat vrec_vf_vf(vfloat d) {
  return vfdiv(vcast_vf_f(1.0f), d, VECTLENSP);
}
static INLINE vfloat vsqrt_vf_vf(vfloat d) {
  return vfsqrt(d, VECTLENSP);
}
// fused multiply-add/subtract
static INLINE vfloat vmla_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) {
  return vfmadd(x, y, z, VECTLENSP);
}
static INLINE vfloat vmlanp_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) {
  return vfnmsub(x, y, z, VECTLENSP);
}
// sign manipulation
static INLINE vfloat vmulsign_vf_vf_vf(vfloat x, vfloat y) {
  return vfsgnjx(x, y, VECTLENSP);
}
static INLINE vfloat vcopysign_vf_vf_vf(vfloat x, vfloat y) {
  return vfsgnj(x, y, VECTLENSP);
}
static INLINE vfloat vsign_vf_vf(vfloat f) {
  return vfsgnj(SLEEF_RVV_SP_VCAST_VF_F(1.0f, VECTLENSP), f, VECTLENSP);
}
static INLINE vfloat vorsign_vf_vf_vf(vfloat x, vfloat y) {
  vint2 xi = SLEEF_RVV_SP_VREINTERPRET_VI2(x);
  vint2 yi = SLEEF_RVV_SP_VREINTERPRET_VI2(y);
  vint2 xioryi = vor(xi, yi, VECTLENSP);
  vfloat xory = SLEEF_RVV_SP_VREINTERPRET_VF(xioryi);
  return vfsgnj(x, xory, VECTLENSP);
}
static INLINE vfloat vabs_vf_vf(vfloat f) {
  return vfabs(f, VECTLENSP);
}
static INLINE vfloat vneg_vf_vf(vfloat f) {
  return vfneg(f, VECTLENSP);
}


/****************************************/
/* Integer Arithmetic and Logic         */
/****************************************/
static INLINE vint2 vadd_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vadd(x, y, VECTLENSP);
}
static INLINE vint2 vsub_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vsub(x, y, VECTLENSP);
}
static INLINE vint2 vneg_vi2_vi2(vint2 x) {
  return vneg(x, VECTLENSP);
}
static INLINE vint2 vand_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vand(x, y, VECTLENSP);
}
static INLINE vint2 vandnot_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vand(vnot(x, VECTLENSP), y, VECTLENSP);
}
static INLINE vint2 vor_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vor(x, y, VECTLENSP);
}
static INLINE vint2 vxor_vi2_vi2_vi2(vint2 x, vint2 y) {
  return vxor(x, y, VECTLENSP);
}
static INLINE vint2 vsll_vi2_vi2_i(vint2 x, int c) {
  return vsll(x, c, VECTLENSP);
}
static INLINE vint2 vsra_vi2_vi2_i(vint2 x, int c) {
  return vsra(x, c, VECTLENSP);
}
static INLINE vint2 vsrl_vi2_vi2_i(vint2 x, int c) {
  return SLEEF_RVV_SP_VREINTERPRET_VI2(vsrl(SLEEF_RVV_SP_VREINTERPRET_VU2(x), c, VECTLENSP));
}

#ifdef ENABLE_RVV_SP
/****************************************/
/* Bitmask Operations                   */
/****************************************/
static INLINE vfloat vreinterpret_vf_vm(vmask vm) {
  return SLEEF_RVV_SP_VREINTERPRET_VF(vncvt_x(vm, VECTLENSP));
}
static INLINE vmask vreinterpret_vm_vf(vfloat vf) {
  return vwcvtu_x(SLEEF_RVV_SP_VREINTERPRET_VU(vf), VECTLENSP);
}
static INLINE int vtestallones_i_vo32(vopmask g) {
  return vcpop(g, VECTLENSP) == VECTLENSP;
}
static INLINE vmask vcast_vm_i_i(int64_t h, int64_t l) {
  return SLEEF_RVV_SP_VCAST_VM_U((((uint64_t)h) << 32) | (uint32_t) l, VECTLENSP);
}
static INLINE vmask vand_vm_vm_vm(vmask x, vmask y) {
  return vand(x, y, VECTLENSP);
}
static INLINE vmask vor_vm_vm_vm(vmask x, vmask y) {
  return vor(x, y, VECTLENSP);
}
static INLINE vmask vxor_vm_vm_vm(vmask x, vmask y) {
  return vxor(x, y, VECTLENSP);
}
static INLINE vmask vandnot_vm_vm_vm(vmask x, vmask y) {
  return vand(SLEEF_RVV_SP_VREINTERPRET_VM(vnot(SLEEF_RVV_SP_VREINTERPRET_VI64(x), VECTLENSP)), y, VECTLENSP);
}
static INLINE vmask vor_vm_vo32_vm(vopmask x, vmask y) {
  return vmerge(x, y, -1, VECTLENSP);
}
static INLINE vmask vand_vm_vo32_vm(vopmask x, vmask y) {
  return vmerge(vmnot(x, VECTLENSP), y, 0, VECTLENSP);
}
static INLINE vmask vandnot_vm_vo32_vm(vopmask x, vmask y) {
  return vmerge(x, y, 0, VECTLENSP);
}


/****************************************/
/* Logical Mask Operations              */
/****************************************/
static INLINE vopmask vand_vo_vo_vo(vopmask x, vopmask y) {
  return vmand(x, y, VECTLENSP);
}
static INLINE vopmask vandnot_vo_vo_vo(vopmask x, vopmask y) {
  return vmandn(y, x, VECTLENSP);
}
static INLINE vopmask vor_vo_vo_vo(vopmask x, vopmask y) {
  return vmor(x, y, VECTLENSP);
}
static INLINE vopmask vxor_vo_vo_vo(vopmask x, vopmask y) {
  return vmxor(x, y, VECTLENSP);
}
// single precision FP comparison
static INLINE vopmask veq_vo_vf_vf(vfloat x, vfloat y) {
  return vmfeq(x, y, VECTLENSP);
}
static INLINE vopmask vneq_vo_vf_vf(vfloat x, vfloat y) {
  return vmfne(x, y, VECTLENSP);
}
static INLINE vopmask vgt_vo_vf_vf(vfloat x, vfloat y) {
  return vmfgt(x, y, VECTLENSP);
}
static INLINE vopmask vge_vo_vf_vf(vfloat x, vfloat y) {
  return vmfge(x, y, VECTLENSP);
}
static INLINE vopmask vlt_vo_vf_vf(vfloat x, vfloat y) {
  return vmflt(x, y, VECTLENSP);
}
static INLINE vopmask vle_vo_vf_vf(vfloat x, vfloat y) {
  return vmfle(x, y, VECTLENSP);
}
static INLINE vopmask visnan_vo_vf(vfloat d) {
  return vmfne(d, d, VECTLENSP);
}
static INLINE vopmask visinf_vo_vf(vfloat d) {
  return vmfeq(vfabs(d, VECTLENSP), SLEEF_INFINITYf, VECTLENSP);
}
static INLINE vopmask vispinf_vo_vf(vfloat d) {
  return vmfeq(d, SLEEF_INFINITYf, VECTLENSP);
}
// conditional select
static INLINE vfloat vsel_vf_vo_vf_vf(vopmask mask, vfloat x, vfloat y) {
  return vmerge(mask, y, x, VECTLENSP);
}
static INLINE vfloat vsel_vf_vo_f_f(vopmask mask, float v1, float v0) {
  return vfmerge(mask, vcast_vf_f(v0), v1, VECTLENSP);
}
static INLINE vfloat vsel_vf_vo_vo_f_f_f(vopmask o0, vopmask o1, float d0, float d1, float d2) {
  return vfmerge(o0, vfmerge(o1, vcast_vf_f(d2), d1, VECTLENSP), d0, VECTLENSP);
}
static INLINE vfloat vsel_vf_vo_vo_vo_f_f_f_f(vopmask o0, vopmask o1, vopmask o2, float d0, float d1, float d2, float d3) {
  return vfmerge(o0, vfmerge(o1, vfmerge(o2, vcast_vf_f(d3), d2, VECTLENSP), d1, VECTLENSP), d0, VECTLENSP);
}
// integer comparison
static INLINE vopmask veq_vo_vi2_vi2(vint2 x, vint2 y) {
  return vmseq(x, y, VECTLENSP);
}
static INLINE vopmask vgt_vo_vi2_vi2(vint2 x, vint2 y) {
  return vmsgt(x, y, VECTLENSP);
}
static INLINE vint2 vgt_vi2_vi2_vi2(vint2 x, vint2 y) {
  vint2 zero = vcast_vi2_i(0);
  return vmerge(vmsgt(x, y, VECTLENSP), zero, -1, VECTLENSP);
}
// integer conditional select
static INLINE vint2 vsel_vi2_vo_vi2_vi2(vopmask m, vint2 x, vint2 y) {
  return vmerge(m, y, x, VECTLENSP);
}
static INLINE vint2 vand_vi2_vo_vi2(vopmask x, vint2 y) {
  return vmerge(vmnot(x, VECTLENSP), y, 0, VECTLENSP);
}
#endif // ENABLE_RVV_SP


////////////////////////////////////////////////////////////////////////////////
// Double-Precision Functions
////////////////////////////////////////////////////////////////////////////////

/****************************************/
/* Multi-value and multi-word types     */
/****************************************/
// vdouble2 type
static INLINE const vdouble vd2getx_vd_vd2(vdouble2 v) {
  return SLEEF_RVV_DP_VGET_VD(v, 0);
}
static INLINE const vdouble vd2gety_vd_vd2(vdouble2 v) {
  return SLEEF_RVV_DP_VGET_VD(v, 1);
}
static INLINE const vdouble2 vd2setxy_vd2_vd_vd(vdouble x, vdouble y) {
  vdouble2 res;
  res = vset(res, 0, x);
  res = vset(res, 1, y);
  return res;
}
static INLINE const vdouble2 vd2setx_vd2_vd2_vd(vdouble2 v, vdouble d) {
  return vset(v, 0, d);
}
static INLINE const vdouble2 vd2sety_vd2_vd2_vd(vdouble2 v, vdouble d) {
  return vset(v, 1, d);
}
// dd2 type
static dd2 dd2setab_dd2_vd2_vd2(vdouble2 a, vdouble2 b) {
  dd2 res;
  res = vset(res, 0, a);
  res = vset(res, 1, b);
  return res;
}
static vdouble2 dd2geta_vd2_dd2(dd2 d) { return SLEEF_RVV_DP_VGET_4VD(d, 0); }
static vdouble2 dd2getb_vd2_dd2(dd2 d) { return SLEEF_RVV_DP_VGET_4VD(d, 1); }
// vdouble3 type
static INLINE vdouble vd3getx_vd_vd3(vdouble3 v) { return SLEEF_RVV_DP_VGET_VD(v, 0); }
static INLINE vdouble vd3gety_vd_vd3(vdouble3 v) { return SLEEF_RVV_DP_VGET_VD(v, 1); }
static INLINE vdouble vd3getz_vd_vd3(vdouble3 v) { return SLEEF_RVV_DP_VGET_VD(v, 2); }
static INLINE vdouble3 vd3setxyz_vd3_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  vdouble3 res;
  res = vset(res, 0, x);
  res = vset(res, 1, y);
  res = vset(res, 2, z);
  return res;
}
static INLINE vdouble3 vd3setx_vd3_vd3_vd(vdouble3 v, vdouble d) { return vset(v, 0, d); }
static INLINE vdouble3 vd3sety_vd3_vd3_vd(vdouble3 v, vdouble d) { return vset(v, 1, d); }
static INLINE vdouble3 vd3setz_vd3_vd3_vd(vdouble3 v, vdouble d) { return vset(v, 2, d); }
// di type
static INLINE vdouble digetd_vd_di(di_t d) {
  return SLEEF_RVV_DP_VGET_VD(SLEEF_RVV_DP_VREINTERPRET_VD2_4VI(d), 0);
}
static INLINE vint digeti_vi_di(di_t d) {
#ifdef ENABLE_RVVM1
  return vlmul_trunc_i32mf2(SLEEF_RVV_DP_VGET_VI(d, 1));
#else
  return SLEEF_RVV_DP_VGET_VI(d, 2);
#endif
}
static INLINE di_t disetdi_di_vd_vi(vdouble d, vint i) {
  di_t res;
  res = SLEEF_RVV_DP_VREINTERPRET_4VI_VD2(vset(SLEEF_RVV_DP_VREINTERPRET_VD2_4VI(res), 0, d));
#ifdef ENABLE_RVVM1
  res = vset(res, 1, vlmul_ext_i32m1(i));
#else
  res = vset(res, 2, i);
#endif
  return res;
}
// ddi type
static INLINE vdouble2 ddigetdd_vd2_ddi(ddi_t d) {
  return SLEEF_RVV_DP_VGET_VD2(SLEEF_RVV_DP_VREINTERPRET_4VD_8VI(d), 0);
}
static INLINE vint ddigeti_vi_ddi(ddi_t d) {
#ifdef ENABLE_RVVM1
  return vlmul_trunc_i32mf2(SLEEF_RVV_DP_VGET_VI(d, 2));
#else
  return SLEEF_RVV_DP_VGET_VI(d, 4);
#endif
}
static INLINE ddi_t ddisetddi_ddi_vd2_vi(vdouble2 v, vint i) {
  ddi_t res;
  res = SLEEF_RVV_DP_VREINTERPRET_8VI_4VD(vset(SLEEF_RVV_DP_VREINTERPRET_4VD_8VI(res), 0, v));
#ifdef ENABLE_RVVM1
  res = vset(res, 2, vlmul_ext_i32m1(i));
#else
  res = vset(res, 4, i);
#endif
  return res;
}
static INLINE ddi_t ddisetdd_ddi_ddi_vd2(ddi_t ddi, vdouble2 v) {
  return SLEEF_RVV_DP_VREINTERPRET_8VI_4VD(vset(SLEEF_RVV_DP_VREINTERPRET_4VD_8VI(ddi), 0, v));
}

/****************************************/
/* Type Conversions and Broadcasts      */
/****************************************/
static INLINE vdouble vcast_vd_d(double d) {
  return SLEEF_RVV_DP_VCAST_VD_D(d, VECTLENDP);
}
static INLINE vdouble vcast_vd_vi(vint i) {
  return SLEEF_RVV_DP_VCAST_VD_VI(i);
}
static INLINE vint vcast_vi_i(int32_t i) {
  return SLEEF_RVV_DP_VCAST_VI_I(i, VECTLENDP);
}
static INLINE vint vrint_vi_vd(vdouble vd) {
  // It is not currently possible to safely set frm for intrinsics,
  // so emulate round-to-nearest behavior
  vdouble half = SLEEF_RVV_DP_VCAST_VD_D(0.5, VECTLENDP);
  half = vfsgnj(half, vd, VECTLENDP);
  vdouble res = vfadd(vd, half, VECTLENDP);
  return vfncvt_rtz_x(res, VECTLENDP);
}
static INLINE vdouble vrint_vd_vd(vdouble vd) {
  // It is not currently possible to safely set frm for intrinsics,
  // so emulate round-to-nearest behavior
  vdouble half = SLEEF_RVV_DP_VCAST_VD_D(0.5, VECTLENDP);
  half = vfsgnj(half, vd, VECTLENDP);
  vdouble res = vfadd(vd, half, VECTLENDP);
  return vfwcvt_f(vfncvt_rtz_x(res, VECTLENDP), VECTLENDP);
}
static INLINE vint vtruncate_vi_vd(vdouble vd) {
  return vfncvt_rtz_x(vd, VECTLENDP);
}
static INLINE vdouble vtruncate_vd_vd(vdouble vd) {
  return vcast_vd_vi(vtruncate_vi_vd(vd));
}


/****************************************/
/* Memory Operations                    */
/****************************************/
static INLINE vdouble vload_vd_p(const double *ptr) {
  return SLEEF_RVV_DP_LOAD_VD(ptr, VECTLENDP);
}
static INLINE vdouble vloadu_vd_p(const double *ptr) {
  return SLEEF_RVV_DP_LOAD_VD(ptr, VECTLENDP);
}
static INLINE vint vloadu_vi_p(int32_t *p) {
  return SLEEF_RVV_DP_LOAD_VI(p, VECTLENDP);
}
static INLINE void vstore_v_p_vd(double *ptr, vdouble v) {
  vse64(ptr, v, VECTLENDP);
}
static INLINE void vstoreu_v_p_vd(double *ptr, vdouble v) {
  vse64(ptr, v, VECTLENDP);
}
static INLINE void vstoreu_v_p_vi(int32_t *ptr, vint v) {
  vse32(ptr, v, VECTLENDP);
}
static INLINE vdouble vgather_vd_p_vi(const double *ptr, vint vi) {
  return vluxei64(ptr, vwmulu(SLEEF_RVV_DP_VREINTERPRET_VU(vi), sizeof(double), VECTLENDP), VECTLENDP);
}


/****************************************/
/* Floating-Point Arithmetic            */
/****************************************/
static INLINE vdouble vadd_vd_vd_vd(vdouble x, vdouble y) {
  return vfadd(x, y, VECTLENDP);
}
static INLINE vdouble vsub_vd_vd_vd(vdouble x, vdouble y) {
  return vfsub(x, y, VECTLENDP);
}
static INLINE vdouble vrec_vd_vd(vdouble d) {
  return vfdiv(vcast_vd_d(1.0), d, VECTLENDP);
}
static INLINE vdouble vabs_vd_vd(vdouble d) {
  return vfabs(d, VECTLENDP);
}
static INLINE vdouble vsqrt_vd_vd(vdouble d) {
  return vfsqrt(d, VECTLENDP);
}
static INLINE vdouble vmul_vd_vd_vd(vdouble x, vdouble y) {
  return vfmul(x, y, VECTLENDP);
}
static INLINE vdouble vdiv_vd_vd_vd(vdouble x, vdouble y) {
  return vfdiv(x, y, VECTLENDP);
}
static INLINE vdouble vmax_vd_vd_vd(vdouble x, vdouble y) {
  return vfmax(x, y, VECTLENDP);
}
static INLINE vdouble vmin_vd_vd_vd(vdouble x, vdouble y) {
  return vfmin(x, y, VECTLENDP);
}
// fused multiply add / sub
static INLINE vdouble vmla_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vfmadd(x, y, z, VECTLENDP);
}
static INLINE vdouble vmlapn_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vfmsub(x, y, z, VECTLENDP);
}
static INLINE vdouble vfma_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vfmadd(x, y, z, VECTLENDP);
}
static INLINE vdouble vfmanp_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vfnmsub(x, y, z, VECTLENDP);
}
static INLINE vdouble vfmapn_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) {
  return vfmsub(x, y, z, VECTLENDP);
}
// sign manipulation
static INLINE vdouble vmulsign_vd_vd_vd(vdouble x, vdouble y) {
  return vfsgnjx(x, y, VECTLENDP);
}
static INLINE vdouble vcopysign_vd_vd_vd(vdouble x, vdouble y) {
  return vfsgnj(x, y, VECTLENDP);
}
static INLINE vdouble vorsign_vd_vd_vd(vdouble x, vdouble y) {
  return vfsgnj(x, SLEEF_RVV_DP_VREINTERPRET_VD(vor(SLEEF_RVV_DP_VREINTERPRET_VM(x), SLEEF_RVV_DP_VREINTERPRET_VM(y), VECTLENDP)), VECTLENDP);
}
static INLINE vdouble vneg_vd_vd(vdouble d) {
  return vfneg(d, VECTLENDP);
}


/****************************************/
/* Integer Arithmetic and Logic         */
/****************************************/
static INLINE vint vadd_vi_vi_vi(vint x, vint y) {
  return vadd(x, y, VECTLENDP);
}
static INLINE vint vsub_vi_vi_vi(vint x, vint y) {
  return vsub(x, y, VECTLENDP);
}
static INLINE vint vneg_vi_vi(vint x) {
  return vneg(x, VECTLENDP);
}
static INLINE vint vand_vi_vi_vi(vint x, vint y) {
  return vand(x, y, VECTLENDP);
}
static INLINE vint vandnot_vi_vi_vi(vint x, vint y) {
  return vand(vnot(x, VECTLENDP), y, VECTLENDP);
}
static INLINE vint vor_vi_vi_vi(vint x, vint y) {
  return vor(x, y, VECTLENDP);
}
static INLINE vint vxor_vi_vi_vi(vint x, vint y) {
  return vxor(x, y, VECTLENDP);
}
static INLINE vint vsll_vi_vi_i(vint x, int c) {
  return vsll(x, c, VECTLENDP);
}
static INLINE vint vsra_vi_vi_i(vint x, int c) {
  return vsra(x, c, VECTLENDP);
}
static INLINE vint vsrl_vi_vi_i(vint x, int c) {
  return SLEEF_RVV_DP_VREINTERPRET_VI(vsrl(SLEEF_RVV_DP_VREINTERPRET_VU(x), c, VECTLENDP));
}


#ifdef ENABLE_RVV_DP
/****************************************/
/* Bitmask Operations                   */
/****************************************/
static INLINE vmask vcast_vm_i64(int64_t c) {
  return SLEEF_RVV_DP_VCAST_VM_U(c, VECTLENDP);
}
static INLINE vmask vcast_vm_u64(uint64_t c) {
  return SLEEF_RVV_DP_VCAST_VM_U(c, VECTLENDP);
}
static INLINE vmask vcast_vm_i_i(int64_t h, int64_t l) {
  return SLEEF_RVV_DP_VCAST_VM_U((((uint64_t)h) << 32) | (uint32_t) l, VECTLENDP);
}
static INLINE vmask vcast_vm_vi(vint vi) {
  return SLEEF_RVV_DP_VREINTERPRET_VM(vwcvt_x(vi, VECTLENDP));
}
static INLINE vmask vcastu_vm_vi(vint vi) {
  return vsll(SLEEF_RVV_DP_VREINTERPRET_VM(vwcvt_x(vi, VECTLENDP)), 32, VECTLENDP);
}
static INLINE vint vcastu_vi_vm(vmask vm) {
  return SLEEF_RVV_DP_VREINTERPRET_VI(vnsrl(vm, 32, VECTLENDP));
}
static INLINE vint vcast_vi_vm(vmask vm) {
  return SLEEF_RVV_DP_VREINTERPRET_VI(vncvt_x(vm, VECTLENDP));
}
static INLINE vmask vand_vm_vo64_vm(vopmask x, vmask y) {
  return vmerge(vmnot(x, VECTLENDP), y, 0, VECTLENDP);
}
static INLINE vmask vand_vm_vm_vm(vmask x, vmask y) {
  return vand(x, y, VECTLENDP);
}
static INLINE vmask vor_vm_vm_vm(vmask x, vmask y) {
  return vor(x, y, VECTLENDP);
}
static INLINE vmask vxor_vm_vm_vm(vmask x, vmask y) {
  return vxor(x, y, VECTLENDP);
}
static INLINE vmask vandnot_vm_vm_vm(vmask x, vmask y) {
  return vand(SLEEF_RVV_DP_VREINTERPRET_VM(vnot(SLEEF_RVV_DP_VREINTERPRET_VI64(x), VECTLENDP)), y, VECTLENDP);
}
static INLINE vmask vandnot_vm_vo64_vm(vopmask x, vmask y) {
  return vmerge(x, y, 0, VECTLENDP);
}
static INLINE vmask vsll64_vm_vm_i(vmask mask, int64_t c) {
  return vsll(mask, c, VECTLENDP);
}
static INLINE vmask vsub64_vm_vm_vm(vmask x, vmask y) {
  return SLEEF_RVV_DP_VREINTERPRET_VM(vsub(SLEEF_RVV_DP_VREINTERPRET_VI64(x), SLEEF_RVV_DP_VREINTERPRET_VI64(y), VECTLENDP));
}
static INLINE vmask vsrl64_vm_vm_i(vmask mask, int64_t c) {
  return vsrl(mask, c, VECTLENDP);
}
static INLINE vmask vadd64_vm_vm_vm(vmask x, vmask y) {
  return vadd(x, y, VECTLENDP);
}
static INLINE vmask vor_vm_vo64_vm(vopmask x, vmask y) {
  return vmerge(x, y, -1, VECTLENDP);
}
static INLINE vmask vsel_vm_vo64_vm_vm(vopmask mask, vmask x, vmask y) {
  return vmerge(mask, y, x, VECTLENDP);
}
static INLINE vmask vneg64_vm_vm(vmask mask) {
  return SLEEF_RVV_DP_VREINTERPRET_VM(vneg(SLEEF_RVV_DP_VREINTERPRET_VI64(mask), VECTLENDP));
}
static INLINE vdouble vreinterpret_vd_vm(vmask vm) {
  return SLEEF_RVV_DP_VREINTERPRET_VD(vm);
}
static INLINE vmask vreinterpret_vm_vd(vdouble vd) {
  return SLEEF_RVV_DP_VREINTERPRET_VM(vd);
}

// vquad type
static INLINE const vmask vqgetx_vm_vq(vquad v) { return SLEEF_RVV_DP_VGET_VM(v, 0); }
static INLINE const vmask vqgety_vm_vq(vquad v) { return SLEEF_RVV_DP_VGET_VM(v, 1); }
static INLINE vquad vqsetxy_vq_vm_vm(vmask x, vmask y) {
  vquad res;
  res = vset(res, 0, x);
  res = vset(res, 1, y);
  return res;
}
static INLINE vquad vqsetx_vq_vq_vm(vquad v, vmask x) { return vset(v, 0, x); }
static INLINE vquad vqsety_vq_vq_vm(vquad v, vmask y) { return vset(v, 1, y); }



/****************************************/
/* Logical Mask Operations              */
/****************************************/
static INLINE vopmask vcast_vo64_vo32(vopmask vo) {
  return vo;
}
static INLINE vopmask vcast_vo32_vo64(vopmask vo) {
  return vo;
}
static INLINE vopmask vand_vo_vo_vo(vopmask x, vopmask y) {
  return vmand(x, y, VECTLENDP);
}
static INLINE vopmask vandnot_vo_vo_vo(vopmask x, vopmask y) {
  return vmandn(y, x, VECTLENDP);
}
static INLINE vopmask vor_vo_vo_vo(vopmask x, vopmask y) {
  return vmor(x, y, VECTLENDP);
}
static INLINE vopmask vxor_vo_vo_vo(vopmask x, vopmask y) {
  return vmxor(x, y, VECTLENDP);
}
static INLINE vopmask veq64_vo_vm_vm(vmask x, vmask y) {
  return vmseq(x, y, VECTLENDP);
}
static INLINE vopmask vgt64_vo_vm_vm(vmask x, vmask y) {
  return vmsgt(SLEEF_RVV_DP_VREINTERPRET_VI64(x), SLEEF_RVV_DP_VREINTERPRET_VI64(y), VECTLENDP);
}
// double-precision comparison
static INLINE vopmask visinf_vo_vd(vdouble d) {
  return vmfeq(vfabs(d, VECTLENDP), SLEEF_INFINITY, VECTLENDP);
}
static INLINE vopmask vispinf_vo_vd(vdouble d) {
  return vmfeq(d, SLEEF_INFINITY, VECTLENDP);
}
static INLINE vopmask veq_vo_vd_vd(vdouble x, vdouble y) {
  return vmfeq(x, y, VECTLENDP);
}
static INLINE vopmask vneq_vo_vd_vd(vdouble x, vdouble y) {
  return vmfne(x, y, VECTLENDP);
}
static INLINE vopmask vlt_vo_vd_vd(vdouble x, vdouble y) {
  return vmflt(x, y, VECTLENDP);
}
static INLINE vopmask vle_vo_vd_vd(vdouble x, vdouble y) {
  return vmfle(x, y, VECTLENDP);
}
static INLINE vopmask vgt_vo_vd_vd(vdouble x, vdouble y) {
  return vmfgt(x, y, VECTLENDP);
}
static INLINE vopmask vge_vo_vd_vd(vdouble x, vdouble y) {
  return vmfge(x, y, VECTLENDP);
}
static INLINE vopmask visnan_vo_vd(vdouble d) {
  return vmfne(d, d, VECTLENDP);
}
// double-precision conditional select
static INLINE vdouble vsel_vd_vo_vd_vd(vopmask mask, vdouble x, vdouble y) {
  return vmerge(mask, y, x, VECTLENDP);
}
static INLINE vdouble vsel_vd_vo_d_d(vopmask mask, double v0, double v1) {
  return vfmerge(mask, vcast_vd_d(v1), v0, VECTLENDP);
}
static INLINE vdouble vsel_vd_vo_vo_d_d_d(vopmask o0, vopmask o1, double d0, double d1, double d2) {
  return vfmerge(o0, vfmerge(o1, vcast_vd_d(d2), d1, VECTLENDP), d0, VECTLENDP);
}
static INLINE vdouble vsel_vd_vo_vo_vo_d_d_d_d(vopmask o0, vopmask o1, vopmask o2, double d0, double d1, double d2, double d3) {
  return vfmerge(o0, vfmerge(o1, vfmerge(o2, vcast_vd_d(d3), d2, VECTLENDP), d1, VECTLENDP), d0, VECTLENDP);
}
static INLINE int vtestallones_i_vo64(vopmask g) {
  return vcpop(g, VECTLENDP) == VECTLENDP;
}
// integer comparison
static INLINE vopmask veq_vo_vi_vi(vint x, vint y) {
  return vmseq(x, y, VECTLENDP);
}
static INLINE vopmask vgt_vo_vi_vi(vint x, vint y) {
  return vmsgt(x, y, VECTLENDP);
}
static INLINE vint vgt_vi_vi_vi(vint x, vint y) {
  vint zero = vcast_vi_i(0);
  return vmerge(vmsgt(x, y, VECTLENDP), zero, -1, VECTLENDP);
}
// integer conditional select
static INLINE vint vsel_vi_vo_vi_vi(vopmask m, vint x, vint y) {
  return vmerge(m, y, x, VECTLENDP);
}
static INLINE vint vandnot_vi_vo_vi(vopmask mask, vint vi) {
  return vmerge(mask, vi, 0, VECTLENDP);
}
static INLINE vint vand_vi_vo_vi(vopmask x, vint y) {
  return vmerge(vmnot(x, VECTLENDP), y, 0, VECTLENDP);
}
#endif // ENABLE_RVV_DP

#endif // HELPERRVV_H
