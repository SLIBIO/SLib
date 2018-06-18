@ This file was created from a .asm file
@  using the ads2gas_apple.pl script.

	.set WIDE_REFERENCE, 0
	.set ARCHITECTURE, 5
	.set DO1STROUNDING, 0
 @
 @  Copyright (c) 2016 The WebM project authors. All Rights Reserved.
 @
 @  Use of this source code is governed by a BSD-style license
 @  that can be found in the LICENSE file in the root of the source
 @  tree. An additional intellectual property rights grant can be found
 @  in the file PATENTS.  All contributing project authors may
 @  be found in the AUTHORS file in the root of the source tree.
 @

    .include  "./vpx_config.asm"

     @ Helper functions used to load tran_low_t into int16, narrowing if
     @ necessary.

     @ $dst0..3 are d registers with the pairs assumed to be contiguous in
     @ non-high-bitdepth builds. q0-q3 are used as temporaries in high-bitdepth.
.macro LOAD_TRAN_LOW_TO_S16
    .if CONFIG_VP9_HIGHBITDEPTH
    vld1.s32        {q0,q1}, [$4]!
    vld1.s32        {q2,q3}, [$4]!
    vmovn.i32       $0, q0
    vmovn.i32       $1, q1
    vmovn.i32       $2, q2
    vmovn.i32       $3, q3
    .else
    vld1.s16        {$0-$1,$2-$3}, [$4]!
    .endif
    .endm

     @ $0..3 are d registers. q0-q3 are used as temporaries in high-bitdepth.
.macro LOAD_TRAN_LOW_TO_S16X2
    .if CONFIG_VP9_HIGHBITDEPTH
    vld2.s32        {q0,q1}, [$4]!
    vld2.s32        {q2,q3}, [$4]!
    vmovn.i32       $0, q0
    vmovn.i32       $1, q2
    vmovn.i32       $2, q1
    vmovn.i32       $3, q3
    .else
    vld2.s16        {$0,$1,$2,$3}, [$4]!
    .endif
    .endm
