; RUN: llc < %s -march=nvptx -mcpu=sm_20 | FileCheck %s
; RUN: llc < %s -march=nvptx64 -mcpu=sm_20 | FileCheck %s


define i16 @cvt_i16_f32(float %x) {
; CHECK: cvt.rzi.u16.f32 %rs{{[0-9]+}}, %f{{[0-9]+}};
; CHECK: ret;
  %a = fptoui float %x to i16
  ret i16 %a
}

define i16 @cvt_i16_f64(double %x) {
; CHECK: cvt.rzi.u16.f64 %rs{{[0-9]+}}, %fl{{[0-9]+}};
; CHECK: ret;
  %a = fptoui double %x to i16
  ret i16 %a
}

define i32 @cvt_i32_f32(float %x) {
; CHECK: cvt.rzi.u32.f32 %r{{[0-9]+}}, %f{{[0-9]+}};
; CHECK: ret;
  %a = fptoui float %x to i32
  ret i32 %a
}

define i32 @cvt_i32_f64(double %x) {
; CHECK: cvt.rzi.u32.f64 %r{{[0-9]+}}, %fl{{[0-9]+}};
; CHECK: ret;
  %a = fptoui double %x to i32
  ret i32 %a
}


define i64 @cvt_i64_f32(float %x) {
; CHECK: cvt.rzi.u64.f32 %rl{{[0-9]+}}, %f{{[0-9]+}};
; CHECK: ret;
  %a = fptoui float %x to i64
  ret i64 %a
}

define i64 @cvt_i64_f64(double %x) {
; CHECK: cvt.rzi.u64.f64 %rl{{[0-9]+}}, %fl{{[0-9]+}};
; CHECK: ret;
  %a = fptoui double %x to i64
  ret i64 %a
}

define float @cvt_f32_i16(i16 %x) {
; CHECK: cvt.rn.f32.u16 %f{{[0-9]+}}, %rs{{[0-9]+}};
; CHECK: ret;
  %a = uitofp i16 %x to float
  ret float %a
}

define float @cvt_f32_i32(i32 %x) {
; CHECK: cvt.rn.f32.u32 %f{{[0-9]+}}, %r{{[0-9]+}};
; CHECK: ret;
  %a = uitofp i32 %x to float
  ret float %a
}

define float @cvt_f32_i64(i64 %x) {
; CHECK: cvt.rn.f32.u64 %f{{[0-9]+}}, %rl{{[0-9]+}};
; CHECK: ret;
  %a = uitofp i64 %x to float
  ret float %a
}

define float @cvt_f32_f64(double %x) {
; CHECK: cvt.rn.f32.f64 %f{{[0-9]+}}, %fl{{[0-9]+}};
; CHECK: ret;
  %a = fptrunc double %x to float
  ret float %a
}

define float @cvt_f32_s16(i16 %x) {
; CHECK: cvt.rn.f32.s16 %f{{[0-9]+}}, %rs{{[0-9]+}}
; CHECK: ret
  %a = sitofp i16 %x to float
  ret float %a
}

define float @cvt_f32_s32(i32 %x) {
; CHECK: cvt.rn.f32.s32 %f{{[0-9]+}}, %r{{[0-9]+}}
; CHECK: ret
  %a = sitofp i32 %x to float
  ret float %a
}

define float @cvt_f32_s64(i64 %x) {
; CHECK: cvt.rn.f32.s64 %f{{[0-9]+}}, %rl{{[0-9]+}}
; CHECK: ret
  %a = sitofp i64 %x to float
  ret float %a
}

define double @cvt_f64_i16(i16 %x) {
; CHECK: cvt.rn.f64.u16 %fl{{[0-9]+}}, %rs{{[0-9]+}};
; CHECK: ret;
  %a = uitofp i16 %x to double
  ret double %a
}

define double @cvt_f64_i32(i32 %x) {
; CHECK: cvt.rn.f64.u32 %fl{{[0-9]+}}, %r{{[0-9]+}};
; CHECK: ret;
  %a = uitofp i32 %x to double
  ret double %a
}

define double @cvt_f64_i64(i64 %x) {
; CHECK: cvt.rn.f64.u64 %fl{{[0-9]+}}, %rl{{[0-9]+}};
; CHECK: ret;
  %a = uitofp i64 %x to double
  ret double %a
}

define double @cvt_f64_f32(float %x) {
; CHECK: cvt.f64.f32 %fl{{[0-9]+}}, %f{{[0-9]+}};
; CHECK: ret;
  %a = fpext float %x to double
  ret double %a
}

define double @cvt_f64_s16(i16 %x) {
; CHECK: cvt.rn.f64.s16 %fl{{[0-9]+}}, %rs{{[0-9]+}}
; CHECK: ret
  %a = sitofp i16 %x to double
  ret double %a
}

define double @cvt_f64_s32(i32 %x) {
; CHECK: cvt.rn.f64.s32 %fl{{[0-9]+}}, %r{{[0-9]+}}
; CHECK: ret
  %a = sitofp i32 %x to double
  ret double %a
}

define double @cvt_f64_s64(i64 %x) {
; CHECK: cvt.rn.f64.s64 %fl{{[0-9]+}}, %rl{{[0-9]+}}
; CHECK: ret
  %a = sitofp i64 %x to double
  ret double %a
}
