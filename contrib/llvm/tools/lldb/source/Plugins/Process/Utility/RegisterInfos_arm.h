//===-- RegisterInfos_arm.h -------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifdef DECLARE_REGISTER_INFOS_ARM_STRUCT

// C Includes
#include <stddef.h>

// C++ Includes
// Other libraries and framework includes
// Project includes
#include "lldb/lldb-private.h"
#include "lldb/lldb-defines.h"
#include "lldb/lldb-enumerations.h"

#include "Utility/ARM_ehframe_Registers.h"
#include "Utility/ARM_DWARF_Registers.h"

using namespace lldb;
using namespace lldb_private;

#ifndef GPR_OFFSET
#error GPR_OFFSET must be defined before including this header file
#endif

#ifndef FPU_OFFSET
#error FPU_OFFSET must be defined before including this header file
#endif

#ifndef FPSCR_OFFSET
#error FPSCR_OFFSET must be defined before including this header file
#endif

#ifndef EXC_OFFSET
#error EXC_OFFSET_NAME must be defined before including this header file
#endif

#ifndef DEFINE_DBG
#error DEFINE_DBG must be defined before including this header file
#endif

enum
{
    gpr_r0 = 0,
    gpr_r1,
    gpr_r2,
    gpr_r3,
    gpr_r4,
    gpr_r5,
    gpr_r6,
    gpr_r7,
    gpr_r8,
    gpr_r9,
    gpr_r10,
    gpr_r11,
    gpr_r12,
    gpr_r13, gpr_sp = gpr_r13,
    gpr_r14, gpr_lr = gpr_r14,
    gpr_r15, gpr_pc = gpr_r15,
    gpr_cpsr,

    fpu_s0,
    fpu_s1,
    fpu_s2,
    fpu_s3,
    fpu_s4,
    fpu_s5,
    fpu_s6,
    fpu_s7,
    fpu_s8,
    fpu_s9,
    fpu_s10,
    fpu_s11,
    fpu_s12,
    fpu_s13,
    fpu_s14,
    fpu_s15,
    fpu_s16,
    fpu_s17,
    fpu_s18,
    fpu_s19,
    fpu_s20,
    fpu_s21,
    fpu_s22,
    fpu_s23,
    fpu_s24,
    fpu_s25,
    fpu_s26,
    fpu_s27,
    fpu_s28,
    fpu_s29,
    fpu_s30,
    fpu_s31,
    fpu_fpscr,

    fpu_d0,
    fpu_d1,
    fpu_d2,
    fpu_d3,
    fpu_d4,
    fpu_d5,
    fpu_d6,
    fpu_d7,
    fpu_d8,
    fpu_d9,
    fpu_d10,
    fpu_d11,
    fpu_d12,
    fpu_d13,
    fpu_d14,
    fpu_d15,
    fpu_d16,
    fpu_d17,
    fpu_d18,
    fpu_d19,
    fpu_d20,
    fpu_d21,
    fpu_d22,
    fpu_d23,
    fpu_d24,
    fpu_d25,
    fpu_d26,
    fpu_d27,
    fpu_d28,
    fpu_d29,
    fpu_d30,
    fpu_d31,

    fpu_q0,
    fpu_q1,
    fpu_q2,
    fpu_q3,
    fpu_q4,
    fpu_q5,
    fpu_q6,
    fpu_q7,
    fpu_q8,
    fpu_q9,
    fpu_q10,
    fpu_q11,
    fpu_q12,
    fpu_q13,
    fpu_q14,
    fpu_q15,

    exc_exception,
    exc_fsr,
    exc_far,

    dbg_bvr0,
    dbg_bvr1,
    dbg_bvr2,
    dbg_bvr3,
    dbg_bvr4,
    dbg_bvr5,
    dbg_bvr6,
    dbg_bvr7,
    dbg_bvr8,
    dbg_bvr9,
    dbg_bvr10,
    dbg_bvr11,
    dbg_bvr12,
    dbg_bvr13,
    dbg_bvr14,
    dbg_bvr15,

    dbg_bcr0,
    dbg_bcr1,
    dbg_bcr2,
    dbg_bcr3,
    dbg_bcr4,
    dbg_bcr5,
    dbg_bcr6,
    dbg_bcr7,
    dbg_bcr8,
    dbg_bcr9,
    dbg_bcr10,
    dbg_bcr11,
    dbg_bcr12,
    dbg_bcr13,
    dbg_bcr14,
    dbg_bcr15,

    dbg_wvr0,
    dbg_wvr1,
    dbg_wvr2,
    dbg_wvr3,
    dbg_wvr4,
    dbg_wvr5,
    dbg_wvr6,
    dbg_wvr7,
    dbg_wvr8,
    dbg_wvr9,
    dbg_wvr10,
    dbg_wvr11,
    dbg_wvr12,
    dbg_wvr13,
    dbg_wvr14,
    dbg_wvr15,

    dbg_wcr0,
    dbg_wcr1,
    dbg_wcr2,
    dbg_wcr3,
    dbg_wcr4,
    dbg_wcr5,
    dbg_wcr6,
    dbg_wcr7,
    dbg_wcr8,
    dbg_wcr9,
    dbg_wcr10,
    dbg_wcr11,
    dbg_wcr12,
    dbg_wcr13,
    dbg_wcr14,
    dbg_wcr15,

    k_num_registers
};

static uint32_t g_s0_invalidates[]  = { fpu_d0,  fpu_q0, LLDB_INVALID_REGNUM };
static uint32_t g_s1_invalidates[]  = { fpu_d0,  fpu_q0, LLDB_INVALID_REGNUM };
static uint32_t g_s2_invalidates[]  = { fpu_d1,  fpu_q0, LLDB_INVALID_REGNUM };
static uint32_t g_s3_invalidates[]  = { fpu_d1,  fpu_q0, LLDB_INVALID_REGNUM };
static uint32_t g_s4_invalidates[]  = { fpu_d2,  fpu_q1, LLDB_INVALID_REGNUM };
static uint32_t g_s5_invalidates[]  = { fpu_d2,  fpu_q1, LLDB_INVALID_REGNUM };
static uint32_t g_s6_invalidates[]  = { fpu_d3,  fpu_q1, LLDB_INVALID_REGNUM };
static uint32_t g_s7_invalidates[]  = { fpu_d3,  fpu_q1, LLDB_INVALID_REGNUM };
static uint32_t g_s8_invalidates[]  = { fpu_d4,  fpu_q2, LLDB_INVALID_REGNUM };
static uint32_t g_s9_invalidates[]  = { fpu_d4,  fpu_q2, LLDB_INVALID_REGNUM };
static uint32_t g_s10_invalidates[] = { fpu_d5,  fpu_q2, LLDB_INVALID_REGNUM };
static uint32_t g_s11_invalidates[] = { fpu_d5,  fpu_q2, LLDB_INVALID_REGNUM };
static uint32_t g_s12_invalidates[] = { fpu_d6,  fpu_q3, LLDB_INVALID_REGNUM };
static uint32_t g_s13_invalidates[] = { fpu_d6,  fpu_q3, LLDB_INVALID_REGNUM };
static uint32_t g_s14_invalidates[] = { fpu_d7,  fpu_q3, LLDB_INVALID_REGNUM };
static uint32_t g_s15_invalidates[] = { fpu_d7,  fpu_q3, LLDB_INVALID_REGNUM };
static uint32_t g_s16_invalidates[] = { fpu_d8,  fpu_q4, LLDB_INVALID_REGNUM };
static uint32_t g_s17_invalidates[] = { fpu_d8,  fpu_q4, LLDB_INVALID_REGNUM };
static uint32_t g_s18_invalidates[] = { fpu_d9,  fpu_q4, LLDB_INVALID_REGNUM };
static uint32_t g_s19_invalidates[] = { fpu_d9,  fpu_q4, LLDB_INVALID_REGNUM };
static uint32_t g_s20_invalidates[] = { fpu_d10, fpu_q5, LLDB_INVALID_REGNUM };
static uint32_t g_s21_invalidates[] = { fpu_d10, fpu_q5, LLDB_INVALID_REGNUM };
static uint32_t g_s22_invalidates[] = { fpu_d11, fpu_q5, LLDB_INVALID_REGNUM };
static uint32_t g_s23_invalidates[] = { fpu_d11, fpu_q5, LLDB_INVALID_REGNUM };
static uint32_t g_s24_invalidates[] = { fpu_d12, fpu_q6, LLDB_INVALID_REGNUM };
static uint32_t g_s25_invalidates[] = { fpu_d12, fpu_q6, LLDB_INVALID_REGNUM };
static uint32_t g_s26_invalidates[] = { fpu_d13, fpu_q6, LLDB_INVALID_REGNUM };
static uint32_t g_s27_invalidates[] = { fpu_d13, fpu_q6, LLDB_INVALID_REGNUM };
static uint32_t g_s28_invalidates[] = { fpu_d14, fpu_q7, LLDB_INVALID_REGNUM };
static uint32_t g_s29_invalidates[] = { fpu_d14, fpu_q7, LLDB_INVALID_REGNUM };
static uint32_t g_s30_invalidates[] = { fpu_d15, fpu_q7, LLDB_INVALID_REGNUM };
static uint32_t g_s31_invalidates[] = { fpu_d15, fpu_q7, LLDB_INVALID_REGNUM };

static uint32_t g_d0_contains[] =  { fpu_s0,  fpu_s1,  LLDB_INVALID_REGNUM };
static uint32_t g_d1_contains[] =  { fpu_s2,  fpu_s3,  LLDB_INVALID_REGNUM };
static uint32_t g_d2_contains[] =  { fpu_s4,  fpu_s5,  LLDB_INVALID_REGNUM };
static uint32_t g_d3_contains[] =  { fpu_s6,  fpu_s7,  LLDB_INVALID_REGNUM };
static uint32_t g_d4_contains[] =  { fpu_s8,  fpu_s9,  LLDB_INVALID_REGNUM };
static uint32_t g_d5_contains[] =  { fpu_s10, fpu_s11, LLDB_INVALID_REGNUM };
static uint32_t g_d6_contains[] =  { fpu_s12, fpu_s13, LLDB_INVALID_REGNUM };
static uint32_t g_d7_contains[] =  { fpu_s14, fpu_s15, LLDB_INVALID_REGNUM };
static uint32_t g_d8_contains[] =  { fpu_s16, fpu_s17, LLDB_INVALID_REGNUM };
static uint32_t g_d9_contains[] =  { fpu_s18, fpu_s19, LLDB_INVALID_REGNUM };
static uint32_t g_d10_contains[] = { fpu_s20, fpu_s21, LLDB_INVALID_REGNUM };
static uint32_t g_d11_contains[] = { fpu_s22, fpu_s23, LLDB_INVALID_REGNUM };
static uint32_t g_d12_contains[] = { fpu_s24, fpu_s25, LLDB_INVALID_REGNUM };
static uint32_t g_d13_contains[] = { fpu_s26, fpu_s27, LLDB_INVALID_REGNUM };
static uint32_t g_d14_contains[] = { fpu_s28, fpu_s29, LLDB_INVALID_REGNUM };
static uint32_t g_d15_contains[] = { fpu_s30, fpu_s31, LLDB_INVALID_REGNUM };

static uint32_t g_d0_invalidates[]  = { fpu_q0, LLDB_INVALID_REGNUM };
static uint32_t g_d1_invalidates[]  = { fpu_q0, LLDB_INVALID_REGNUM };
static uint32_t g_d2_invalidates[]  = { fpu_q1, LLDB_INVALID_REGNUM };
static uint32_t g_d3_invalidates[]  = { fpu_q1, LLDB_INVALID_REGNUM };
static uint32_t g_d4_invalidates[]  = { fpu_q2, LLDB_INVALID_REGNUM };
static uint32_t g_d5_invalidates[]  = { fpu_q2, LLDB_INVALID_REGNUM };
static uint32_t g_d6_invalidates[]  = { fpu_q3, LLDB_INVALID_REGNUM };
static uint32_t g_d7_invalidates[]  = { fpu_q3, LLDB_INVALID_REGNUM };
static uint32_t g_d8_invalidates[]  = { fpu_q4, LLDB_INVALID_REGNUM };
static uint32_t g_d9_invalidates[]  = { fpu_q4, LLDB_INVALID_REGNUM };
static uint32_t g_d10_invalidates[] = { fpu_q5, LLDB_INVALID_REGNUM };
static uint32_t g_d11_invalidates[] = { fpu_q5, LLDB_INVALID_REGNUM };
static uint32_t g_d12_invalidates[] = { fpu_q6, LLDB_INVALID_REGNUM };
static uint32_t g_d13_invalidates[] = { fpu_q6, LLDB_INVALID_REGNUM };
static uint32_t g_d14_invalidates[] = { fpu_q7, LLDB_INVALID_REGNUM };
static uint32_t g_d15_invalidates[] = { fpu_q7, LLDB_INVALID_REGNUM };
static uint32_t g_d16_invalidates[] = { fpu_q8, LLDB_INVALID_REGNUM };
static uint32_t g_d17_invalidates[] = { fpu_q8, LLDB_INVALID_REGNUM };
static uint32_t g_d18_invalidates[] = { fpu_q9, LLDB_INVALID_REGNUM };
static uint32_t g_d19_invalidates[] = { fpu_q9, LLDB_INVALID_REGNUM };
static uint32_t g_d20_invalidates[] = { fpu_q10, LLDB_INVALID_REGNUM };
static uint32_t g_d21_invalidates[] = { fpu_q10, LLDB_INVALID_REGNUM };
static uint32_t g_d22_invalidates[] = { fpu_q11, LLDB_INVALID_REGNUM };
static uint32_t g_d23_invalidates[] = { fpu_q11, LLDB_INVALID_REGNUM };
static uint32_t g_d24_invalidates[] = { fpu_q12, LLDB_INVALID_REGNUM };
static uint32_t g_d25_invalidates[] = { fpu_q12, LLDB_INVALID_REGNUM };
static uint32_t g_d26_invalidates[] = { fpu_q13, LLDB_INVALID_REGNUM };
static uint32_t g_d27_invalidates[] = { fpu_q13, LLDB_INVALID_REGNUM };
static uint32_t g_d28_invalidates[] = { fpu_q14, LLDB_INVALID_REGNUM };
static uint32_t g_d29_invalidates[] = { fpu_q14, LLDB_INVALID_REGNUM };
static uint32_t g_d30_invalidates[] = { fpu_q15, LLDB_INVALID_REGNUM };
static uint32_t g_d31_invalidates[] = { fpu_q15, LLDB_INVALID_REGNUM };

static uint32_t g_q0_contains[] =  { fpu_d0,  fpu_d1,  fpu_s0,  fpu_s1,  fpu_s2,  fpu_s3,  LLDB_INVALID_REGNUM };
static uint32_t g_q1_contains[] =  { fpu_d2,  fpu_d3,  fpu_s4,  fpu_s5,  fpu_s6,  fpu_s7,  LLDB_INVALID_REGNUM };
static uint32_t g_q2_contains[] =  { fpu_d4,  fpu_d5,  fpu_s8,  fpu_s9,  fpu_s10, fpu_s11, LLDB_INVALID_REGNUM };
static uint32_t g_q3_contains[] =  { fpu_d6,  fpu_d7,  fpu_s12, fpu_s13, fpu_s14, fpu_s15, LLDB_INVALID_REGNUM };
static uint32_t g_q4_contains[] =  { fpu_d8,  fpu_d9,  fpu_s16, fpu_s17, fpu_s18, fpu_s19, LLDB_INVALID_REGNUM };
static uint32_t g_q5_contains[] =  { fpu_d10, fpu_d11, fpu_s20, fpu_s21, fpu_s22, fpu_s23, LLDB_INVALID_REGNUM };
static uint32_t g_q6_contains[] =  { fpu_d12, fpu_d13, fpu_s24, fpu_s25, fpu_s26, fpu_s27, LLDB_INVALID_REGNUM };
static uint32_t g_q7_contains[] =  { fpu_d14, fpu_d15, fpu_s28, fpu_s29, fpu_s30, fpu_s31, LLDB_INVALID_REGNUM };
static uint32_t g_q8_contains[] =  { fpu_d16, fpu_d17, LLDB_INVALID_REGNUM };
static uint32_t g_q9_contains[] =  { fpu_d18, fpu_d19, LLDB_INVALID_REGNUM };
static uint32_t g_q10_contains[] = { fpu_d20, fpu_d21, LLDB_INVALID_REGNUM };
static uint32_t g_q11_contains[] = { fpu_d22, fpu_d23, LLDB_INVALID_REGNUM };
static uint32_t g_q12_contains[] = { fpu_d24, fpu_d25, LLDB_INVALID_REGNUM };
static uint32_t g_q13_contains[] = { fpu_d26, fpu_d27, LLDB_INVALID_REGNUM };
static uint32_t g_q14_contains[] = { fpu_d28, fpu_d29, LLDB_INVALID_REGNUM };
static uint32_t g_q15_contains[] = { fpu_d30, fpu_d31, LLDB_INVALID_REGNUM };

static RegisterInfo g_register_infos_arm[] = {
//  NAME         ALT     SZ   OFFSET          ENCODING          FORMAT                  EH_FRAME             DWARF                GENERIC                     PROCESS PLUGIN       LLDB NATIVE      VALUE REGS      INVALIDATE REGS
//  ===========  ======= ==   ==============  ================  ====================    ===================  ===================  ==========================  ===================  =============    ==============  =================
{   "r0",        nullptr, 4,  GPR_OFFSET(0),  eEncodingUint,    eFormatHex,           { ehframe_r0,          dwarf_r0,            LLDB_REGNUM_GENERIC_ARG1,   LLDB_INVALID_REGNUM, gpr_r0        }, nullptr,        nullptr, nullptr, 0},
{   "r1",        nullptr, 4,  GPR_OFFSET(1),  eEncodingUint,    eFormatHex,           { ehframe_r1,          dwarf_r1,            LLDB_REGNUM_GENERIC_ARG2,   LLDB_INVALID_REGNUM, gpr_r1        }, nullptr,        nullptr, nullptr, 0},
{   "r2",        nullptr, 4,  GPR_OFFSET(2),  eEncodingUint,    eFormatHex,           { ehframe_r2,          dwarf_r2,            LLDB_REGNUM_GENERIC_ARG3,   LLDB_INVALID_REGNUM, gpr_r2        }, nullptr,        nullptr, nullptr, 0},
{   "r3",        nullptr, 4,  GPR_OFFSET(3),  eEncodingUint,    eFormatHex,           { ehframe_r3,          dwarf_r3,            LLDB_REGNUM_GENERIC_ARG4,   LLDB_INVALID_REGNUM, gpr_r3        }, nullptr,        nullptr, nullptr, 0},
{   "r4",        nullptr, 4,  GPR_OFFSET(4),  eEncodingUint,    eFormatHex,           { ehframe_r4,          dwarf_r4,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r4        }, nullptr,        nullptr, nullptr, 0},
{   "r5",        nullptr, 4,  GPR_OFFSET(5),  eEncodingUint,    eFormatHex,           { ehframe_r5,          dwarf_r5,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r5        }, nullptr,        nullptr, nullptr, 0},
{   "r6",        nullptr, 4,  GPR_OFFSET(6),  eEncodingUint,    eFormatHex,           { ehframe_r6,          dwarf_r6,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r6        }, nullptr,        nullptr, nullptr, 0},
{   "r7",        nullptr, 4,  GPR_OFFSET(7),  eEncodingUint,    eFormatHex,           { ehframe_r7,          dwarf_r7,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r7        }, nullptr,        nullptr, nullptr, 0},
{   "r8",        nullptr, 4,  GPR_OFFSET(8),  eEncodingUint,    eFormatHex,           { ehframe_r8,          dwarf_r8,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r8        }, nullptr,        nullptr, nullptr, 0},
{   "r9",        nullptr, 4,  GPR_OFFSET(9),  eEncodingUint,    eFormatHex,           { ehframe_r9,          dwarf_r9,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r9        }, nullptr,        nullptr, nullptr, 0},
{   "r10",       nullptr, 4,  GPR_OFFSET(10), eEncodingUint,    eFormatHex,           { ehframe_r10,         dwarf_r10,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r10       }, nullptr,        nullptr, nullptr, 0},
{   "r11",       nullptr, 4,  GPR_OFFSET(11), eEncodingUint,    eFormatHex,           { ehframe_r11,         dwarf_r11,           LLDB_REGNUM_GENERIC_FP,     LLDB_INVALID_REGNUM, gpr_r11       }, nullptr,        nullptr, nullptr, 0},
{   "r12",       nullptr, 4,  GPR_OFFSET(12), eEncodingUint,    eFormatHex,           { ehframe_r12,         dwarf_r12,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, gpr_r12       }, nullptr,        nullptr, nullptr, 0},
{   "sp",        "r13",   4,  GPR_OFFSET(13), eEncodingUint,    eFormatHex,           { ehframe_sp,          dwarf_sp,            LLDB_REGNUM_GENERIC_SP,     LLDB_INVALID_REGNUM, gpr_sp        }, nullptr,        nullptr, nullptr, 0},
{   "lr",        "r14",   4,  GPR_OFFSET(14), eEncodingUint,    eFormatHex,           { ehframe_lr,          dwarf_lr,            LLDB_REGNUM_GENERIC_RA,     LLDB_INVALID_REGNUM, gpr_lr        }, nullptr,        nullptr, nullptr, 0},
{   "pc",        "r15",   4,  GPR_OFFSET(15), eEncodingUint,    eFormatHex,           { ehframe_pc,          dwarf_pc,            LLDB_REGNUM_GENERIC_PC,     LLDB_INVALID_REGNUM, gpr_pc        }, nullptr,        nullptr, nullptr, 0},
{   "cpsr",      "psr",   4,  GPR_OFFSET(16), eEncodingUint,    eFormatHex,           { ehframe_cpsr,        dwarf_cpsr,          LLDB_REGNUM_GENERIC_FLAGS,  LLDB_INVALID_REGNUM, gpr_cpsr      }, nullptr,        nullptr, nullptr, 0},

{   "s0",        nullptr, 4,  FPU_OFFSET(0),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s0,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s0        }, nullptr, g_s0_invalidates, nullptr, 0},
{   "s1",        nullptr, 4,  FPU_OFFSET(1),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s1,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s1        }, nullptr, g_s1_invalidates, nullptr, 0},
{   "s2",        nullptr, 4,  FPU_OFFSET(2),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s2,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s2        }, nullptr, g_s2_invalidates, nullptr, 0},
{   "s3",        nullptr, 4,  FPU_OFFSET(3),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s3,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s3        }, nullptr, g_s3_invalidates, nullptr, 0},
{   "s4",        nullptr, 4,  FPU_OFFSET(4),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s4,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s4        }, nullptr, g_s4_invalidates, nullptr, 0},
{   "s5",        nullptr, 4,  FPU_OFFSET(5),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s5,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s5        }, nullptr, g_s5_invalidates, nullptr, 0},
{   "s6",        nullptr, 4,  FPU_OFFSET(6),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s6,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s6        }, nullptr, g_s6_invalidates, nullptr, 0},
{   "s7",        nullptr, 4,  FPU_OFFSET(7),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s7,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s7        }, nullptr, g_s7_invalidates, nullptr, 0},
{   "s8",        nullptr, 4,  FPU_OFFSET(8),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s8,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s8        }, nullptr, g_s8_invalidates, nullptr, 0},
{   "s9",        nullptr, 4,  FPU_OFFSET(9),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s9,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s9        }, nullptr, g_s9_invalidates, nullptr, 0},
{   "s10",       nullptr, 4,  FPU_OFFSET(10), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s10,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s10       }, nullptr, g_s10_invalidates, nullptr, 0},
{   "s11",       nullptr, 4,  FPU_OFFSET(11), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s11,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s11       }, nullptr, g_s11_invalidates, nullptr, 0},
{   "s12",       nullptr, 4,  FPU_OFFSET(12), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s12,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s12       }, nullptr, g_s12_invalidates, nullptr, 0},
{   "s13",       nullptr, 4,  FPU_OFFSET(13), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s13,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s13       }, nullptr, g_s13_invalidates, nullptr, 0},
{   "s14",       nullptr, 4,  FPU_OFFSET(14), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s14,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s14       }, nullptr, g_s14_invalidates, nullptr, 0},
{   "s15",       nullptr, 4,  FPU_OFFSET(15), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s15,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s15       }, nullptr, g_s15_invalidates, nullptr, 0},
{   "s16",       nullptr, 4,  FPU_OFFSET(16), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s16,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s16       }, nullptr, g_s16_invalidates, nullptr, 0},
{   "s17",       nullptr, 4,  FPU_OFFSET(17), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s17,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s17       }, nullptr, g_s17_invalidates, nullptr, 0},
{   "s18",       nullptr, 4,  FPU_OFFSET(18), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s18,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s18       }, nullptr, g_s18_invalidates, nullptr, 0},
{   "s19",       nullptr, 4,  FPU_OFFSET(19), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s19,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s19       }, nullptr, g_s19_invalidates, nullptr, 0},
{   "s20",       nullptr, 4,  FPU_OFFSET(20), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s20,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s20       }, nullptr, g_s20_invalidates, nullptr, 0},
{   "s21",       nullptr, 4,  FPU_OFFSET(21), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s21,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s21       }, nullptr, g_s21_invalidates, nullptr, 0},
{   "s22",       nullptr, 4,  FPU_OFFSET(22), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s22,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s22       }, nullptr, g_s22_invalidates, nullptr, 0},
{   "s23",       nullptr, 4,  FPU_OFFSET(23), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s23,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s23       }, nullptr, g_s23_invalidates, nullptr, 0},
{   "s24",       nullptr, 4,  FPU_OFFSET(24), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s24,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s24       }, nullptr, g_s24_invalidates, nullptr, 0},
{   "s25",       nullptr, 4,  FPU_OFFSET(25), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s25,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s25       }, nullptr, g_s25_invalidates, nullptr, 0},
{   "s26",       nullptr, 4,  FPU_OFFSET(26), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s26,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s26       }, nullptr, g_s26_invalidates, nullptr, 0},
{   "s27",       nullptr, 4,  FPU_OFFSET(27), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s27,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s27       }, nullptr, g_s27_invalidates, nullptr, 0},
{   "s28",       nullptr, 4,  FPU_OFFSET(28), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s28,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s28       }, nullptr, g_s28_invalidates, nullptr, 0},
{   "s29",       nullptr, 4,  FPU_OFFSET(29), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s29,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s29       }, nullptr, g_s29_invalidates, nullptr, 0},
{   "s30",       nullptr, 4,  FPU_OFFSET(30), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s30,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s30       }, nullptr, g_s30_invalidates, nullptr, 0},
{   "s31",       nullptr, 4,  FPU_OFFSET(31), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_s31,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_s31       }, nullptr, g_s31_invalidates, nullptr, 0},
{   "fpscr",     nullptr, 4,  FPSCR_OFFSET,   eEncodingUint,    eFormatHex,           { LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_fpscr     }, nullptr,        nullptr, nullptr,    0},

{   "d0",        nullptr, 8,  FPU_OFFSET(0),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d0,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d0        }, g_d0_contains, g_d0_invalidates, nullptr, 0},
{   "d1",        nullptr, 8,  FPU_OFFSET(2),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d1,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d1        }, g_d1_contains, g_d1_invalidates, nullptr, 0},
{   "d2",        nullptr, 8,  FPU_OFFSET(4),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d2,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d2        }, g_d2_contains, g_d2_invalidates, nullptr, 0},
{   "d3",        nullptr, 8,  FPU_OFFSET(6),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d3,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d3        }, g_d3_contains, g_d3_invalidates, nullptr, 0},
{   "d4",        nullptr, 8,  FPU_OFFSET(8),  eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d4,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d4        }, g_d4_contains, g_d4_invalidates, nullptr, 0},
{   "d5",        nullptr, 8,  FPU_OFFSET(10), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d5,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d5        }, g_d5_contains, g_d5_invalidates, nullptr, 0},
{   "d6",        nullptr, 8,  FPU_OFFSET(12), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d6,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d6        }, g_d6_contains, g_d6_invalidates, nullptr, 0},
{   "d7",        nullptr, 8,  FPU_OFFSET(14), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d7,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d7        }, g_d7_contains, g_d7_invalidates, nullptr, 0},
{   "d8",        nullptr, 8,  FPU_OFFSET(16), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d8,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d8        }, g_d8_contains, g_d8_invalidates, nullptr, 0},
{   "d9",        nullptr, 8,  FPU_OFFSET(18), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d9,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d9        }, g_d9_contains, g_d9_invalidates, nullptr, 0},
{   "d10",       nullptr, 8,  FPU_OFFSET(20), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d10,           LLDB_INVALID_REGNUM,       LLDB_INVALID_REGNUM, fpu_d10      }, g_d10_contains, g_d10_invalidates, nullptr, 0},
{   "d11",       nullptr, 8,  FPU_OFFSET(22), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d11,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d11     }, g_d11_contains, g_d11_invalidates, nullptr, 0},
{   "d12",       nullptr, 8,  FPU_OFFSET(24), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d12,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d12     }, g_d12_contains, g_d12_invalidates, nullptr, 0},
{   "d13",       nullptr, 8,  FPU_OFFSET(26), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d13,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d13     }, g_d13_contains, g_d13_invalidates, nullptr, 0},
{   "d14",       nullptr, 8,  FPU_OFFSET(28), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d14,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d14     }, g_d14_contains, g_d14_invalidates, nullptr, 0},
{   "d15",       nullptr, 8,  FPU_OFFSET(30), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d15,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d15     }, g_d15_contains, g_d15_invalidates, nullptr, 0},
{   "d16",       nullptr, 8,  FPU_OFFSET(32), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d16,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d16     }, nullptr,   g_d16_invalidates, nullptr, 0 },
{   "d17",       nullptr, 8,  FPU_OFFSET(34), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d17,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d17       }, nullptr, g_d17_invalidates, nullptr, 0},
{   "d18",       nullptr, 8,  FPU_OFFSET(36), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d18,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d18       }, nullptr, g_d18_invalidates, nullptr, 0},
{   "d19",       nullptr, 8,  FPU_OFFSET(38), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d19,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d19       }, nullptr, g_d19_invalidates, nullptr, 0},
{   "d20",       nullptr, 8,  FPU_OFFSET(40), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d20,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d20       }, nullptr, g_d20_invalidates, nullptr, 0},
{   "d21",       nullptr, 8,  FPU_OFFSET(42), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d21,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d21       }, nullptr, g_d21_invalidates, nullptr, 0},
{   "d22",       nullptr, 8,  FPU_OFFSET(44), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d22,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d22       }, nullptr, g_d22_invalidates, nullptr, 0},
{   "d23",       nullptr, 8,  FPU_OFFSET(46), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d23,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d23       }, nullptr, g_d23_invalidates, nullptr, 0},
{   "d24",       nullptr, 8,  FPU_OFFSET(48), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d24,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d24       }, nullptr, g_d24_invalidates, nullptr, 0},
{   "d25",       nullptr, 8,  FPU_OFFSET(50), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d25,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d25       }, nullptr, g_d25_invalidates, nullptr, 0},
{   "d26",       nullptr, 8,  FPU_OFFSET(52), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d26,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d26       }, nullptr, g_d26_invalidates, nullptr, 0},
{   "d27",       nullptr, 8,  FPU_OFFSET(54), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d27,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d27       }, nullptr, g_d27_invalidates, nullptr, 0},
{   "d28",       nullptr, 8,  FPU_OFFSET(56), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d28,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d28       }, nullptr, g_d28_invalidates, nullptr, 0},
{   "d29",       nullptr, 8,  FPU_OFFSET(58), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d29,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d29       }, nullptr, g_d29_invalidates, nullptr, 0},
{   "d30",       nullptr, 8,  FPU_OFFSET(60), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d30,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d30       }, nullptr, g_d30_invalidates, nullptr, 0},
{   "d31",       nullptr, 8,  FPU_OFFSET(62), eEncodingIEEE754, eFormatFloat,         { LLDB_INVALID_REGNUM, dwarf_d31,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_d31       }, nullptr, g_d31_invalidates, nullptr, 0},

{   "q0",        nullptr, 16, FPU_OFFSET(0),  eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q0,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q0        }, g_q0_contains,  nullptr, nullptr, 0},
{   "q1",        nullptr, 16, FPU_OFFSET(4),  eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q1,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q1        }, g_q1_contains,  nullptr, nullptr, 0},
{   "q2",        nullptr, 16, FPU_OFFSET(8),  eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q2,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q2        }, g_q2_contains,  nullptr, nullptr, 0},
{   "q3",        nullptr, 16, FPU_OFFSET(12), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q3,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q3        }, g_q3_contains,  nullptr, nullptr, 0},
{   "q4",        nullptr, 16, FPU_OFFSET(16), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q4,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q4        }, g_q4_contains,  nullptr, nullptr, 0},
{   "q5",        nullptr, 16, FPU_OFFSET(20), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q5,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q5        }, g_q5_contains,  nullptr, nullptr, 0},
{   "q6",        nullptr, 16, FPU_OFFSET(24), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q6,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q6        }, g_q6_contains,  nullptr, nullptr, 0},
{   "q7",        nullptr, 16, FPU_OFFSET(28), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q7,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q7        }, g_q7_contains,  nullptr, nullptr, 0},
{   "q8",        nullptr, 16, FPU_OFFSET(32), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q8,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q8        }, g_q8_contains,  nullptr, nullptr, 0},
{   "q9",        nullptr, 16, FPU_OFFSET(36), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q9,            LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q9        }, g_q9_contains,  nullptr, nullptr, 0},
{   "q10",       nullptr, 16, FPU_OFFSET(40), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q10,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q10       }, g_q10_contains, nullptr, nullptr, 0},
{   "q11",       nullptr, 16, FPU_OFFSET(44), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q11,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q11       }, g_q11_contains, nullptr, nullptr, 0},
{   "q12",       nullptr, 16, FPU_OFFSET(48), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q12,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q12       }, g_q12_contains, nullptr, nullptr, 0},
{   "q13",       nullptr, 16, FPU_OFFSET(52), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q13,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q13       }, g_q13_contains, nullptr, nullptr, 0},
{   "q14",       nullptr, 16, FPU_OFFSET(56), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q14,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q14       }, g_q14_contains, nullptr, nullptr, 0},
{   "q15",       nullptr, 16, FPU_OFFSET(60), eEncodingVector,  eFormatVectorOfUInt8, { LLDB_INVALID_REGNUM, dwarf_q15,           LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, fpu_q15       }, g_q15_contains, nullptr, nullptr, 0},

{   "exception", nullptr, 4,  EXC_OFFSET(0),  eEncodingUint,    eFormatHex,           { LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, exc_exception }, nullptr,           nullptr, nullptr, 0},
{   "fsr",       nullptr, 4,  EXC_OFFSET(1),  eEncodingUint,    eFormatHex,           { LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, exc_fsr       }, nullptr,           nullptr, nullptr, 0},
{   "far",       nullptr, 4,  EXC_OFFSET(2),  eEncodingUint,    eFormatHex,           { LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM, LLDB_INVALID_REGNUM,        LLDB_INVALID_REGNUM, exc_far       }, nullptr,           nullptr, nullptr, 0},

{   DEFINE_DBG (bvr, 0) },
{   DEFINE_DBG (bvr, 1) },
{   DEFINE_DBG (bvr, 2) },
{   DEFINE_DBG (bvr, 3) },
{   DEFINE_DBG (bvr, 4) },
{   DEFINE_DBG (bvr, 5) },
{   DEFINE_DBG (bvr, 6) },
{   DEFINE_DBG (bvr, 7) },
{   DEFINE_DBG (bvr, 8) },
{   DEFINE_DBG (bvr, 9) },
{   DEFINE_DBG (bvr, 10) },
{   DEFINE_DBG (bvr, 11) },
{   DEFINE_DBG (bvr, 12) },
{   DEFINE_DBG (bvr, 13) },
{   DEFINE_DBG (bvr, 14) },
{   DEFINE_DBG (bvr, 15) },

{   DEFINE_DBG (bcr, 0) },
{   DEFINE_DBG (bcr, 1) },
{   DEFINE_DBG (bcr, 2) },
{   DEFINE_DBG (bcr, 3) },
{   DEFINE_DBG (bcr, 4) },
{   DEFINE_DBG (bcr, 5) },
{   DEFINE_DBG (bcr, 6) },
{   DEFINE_DBG (bcr, 7) },
{   DEFINE_DBG (bcr, 8) },
{   DEFINE_DBG (bcr, 9) },
{   DEFINE_DBG (bcr, 10) },
{   DEFINE_DBG (bcr, 11) },
{   DEFINE_DBG (bcr, 12) },
{   DEFINE_DBG (bcr, 13) },
{   DEFINE_DBG (bcr, 14) },
{   DEFINE_DBG (bcr, 15) },

{   DEFINE_DBG (wvr, 0) },
{   DEFINE_DBG (wvr, 1) },
{   DEFINE_DBG (wvr, 2) },
{   DEFINE_DBG (wvr, 3) },
{   DEFINE_DBG (wvr, 4) },
{   DEFINE_DBG (wvr, 5) },
{   DEFINE_DBG (wvr, 6) },
{   DEFINE_DBG (wvr, 7) },
{   DEFINE_DBG (wvr, 8) },
{   DEFINE_DBG (wvr, 9) },
{   DEFINE_DBG (wvr, 10) },
{   DEFINE_DBG (wvr, 11) },
{   DEFINE_DBG (wvr, 12) },
{   DEFINE_DBG (wvr, 13) },
{   DEFINE_DBG (wvr, 14) },
{   DEFINE_DBG (wvr, 15) },

{   DEFINE_DBG (wcr, 0) },
{   DEFINE_DBG (wcr, 1) },
{   DEFINE_DBG (wcr, 2) },
{   DEFINE_DBG (wcr, 3) },
{   DEFINE_DBG (wcr, 4) },
{   DEFINE_DBG (wcr, 5) },
{   DEFINE_DBG (wcr, 6) },
{   DEFINE_DBG (wcr, 7) },
{   DEFINE_DBG (wcr, 8) },
{   DEFINE_DBG (wcr, 9) },
{   DEFINE_DBG (wcr, 10) },
{   DEFINE_DBG (wcr, 11) },
{   DEFINE_DBG (wcr, 12) },
{   DEFINE_DBG (wcr, 13) },
{   DEFINE_DBG (wcr, 14) },
{   DEFINE_DBG (wcr, 15) }
};

#endif // DECLARE_REGISTER_INFOS_ARM_STRUCT
