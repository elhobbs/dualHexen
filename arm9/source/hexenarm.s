#include <calico/asm.inc>
@hexenarm.s
@rww - custom arm routines for hexends

@==========================================================================
@Math Functions
@==========================================================================

@=====================================
@HexenDivInt/HexenDivIntU
@regular int divide
@=====================================
FUNC_START32 HexenDivInt
	@set r12 to address of DIV_NUMERATOR32
	ldr		r12, =67109520
	
	@set DIV_CR DIV_32_32 bit
	mov		r2, #0
	strh	r2, [r12, #-16]

	@move a to DIV_NUMERATOR32
	str		r0, [r12]
	@move b to DIV_DENOMINATOR32
	str		r1, [r12, #8]
	
	@hang around until busy bit is clear
.regWaitA:
	ldrh	r0, [r12, #-16]
	ands	r0, r0, #32768
	bne		.regWaitA
			
	@put result from DIV_RESULT32 into r0
	ldr		r0, [r12, #16]
		
	bx		lr
FUNC_END

@=====================================
@FixedMul
@fast fixed multiply
@=====================================
FUNC_START32 FixedMul
	smull	r2, r3, r0, r1
	
	@shift by FRACBITS
	mov		r1, r2, lsr #16
	mov		r2, r3, lsl #16
	orr		r0, r1, r2

	bx		lr
FUNC_END

@=====================================
@FixedDiv
@bounds checking prefrace to FixedDiv2
@=====================================
FUNC_START32 FixedDiv
	mov		r2, r0
	mov		r3, r1
	
	@abs a, b	
	cmp		r2, #0
	rsblt	r2, r2, #0
	cmp		r3, #0
	rsblt	r3, r3, #0

	mov		r2, r2, lsr #14

	@if abs(a)>>14 >= abs(b)
	cmp		r2, r3

	blt		FixedDiv2

	@then return (a^b)<0 ? MININT : MAXINT
	eors	r2, r0, r1
	movpl	r0, #2147483647
	movmi	r0, #2147483648
	
	bx		lr
FUNC_END

@=====================================
@FixedDiv2
@fast fixed divide
@=====================================
FUNC_START32 FixedDiv2
	@store low 32 bits in r2
	mov		r2, r0
	
	@extend to r3
	ands	r3, r0, #2147483648
	movmi	r3, #-1
	
	@shift over 2 registers	
	mov		r3, r3, lsl #16
	mov		r0, r2, lsr #16
	orr		r3, r3, r0
	mov		r2, r2, asl #16
	
	@set r12 to address of DIV_NUMERATOR64
	ldr		r12, =67109520
	
	@set DIV_CR DIV_64_32 bit
	mov		r0, #1
	strh	r0, [r12, #-16]

	@move expanded a into DIV_NUMERATOR64
	stmia	r12, {r2-r3}

	@move b into DIV_DENOMINATOR32
	str		r1, [r12, #8]

	@hang around until busy bit is clear
.regWaitB:
	ldrh	r0, [r12, #-16]
	ands	r0, r0, #32768
	bne		.regWaitB
	
	@put result from DIV_RESULT32 into r0
	ldr		r0, [r12, #16]

	bx		lr
FUNC_END

@==========================================================================
@Render Functions
@==========================================================================

	.extern	ylookup
	.extern	columnofs

	.extern	dc_drawdata

@=====================================
@R_DrawColumn
@Source is the top of the column to scale
@=====================================
FUNC_START32 R_DrawColumn
	ldr		r0, =dc_drawdata
	@set r2 to count, r1 to dc_yl (reused), check for negative range
	ldr		r2, [r0, #8]
	ldr		r1, [r0, #4]
	sub		r2, r2, r1
	cmp		r2, #0
	bxlt	lr

	@put necessary registers on stack
	stmfd	sp!, {r4, r5, r6}
	
	@use r1 (dc_yl) as index into ylookup, r12 becomes ylookup[dc_yl]
	ldr		r12, =ylookup
	ldr		r12, [r12, r1, asl #2]
	
	@load dc_x into r4, r3 becomes columnofs[dc_x]
	ldr		r4, [r0]
	ldr		r3, =columnofs
	ldr		r3, [r3, r4, asl #2]
	
	@add r12 (ylookup[dc_yl]) and r3 (columnofs[dc_x]) into r3 (ptr to dest)
	add		r3, r12, r3
	
	@load centery to r4, subtract from r1 (dc_yl) to r1, multiply result r1 by dc_iscale (loaded to r4) to r12
	ldr		r4, [r0, #32]
	sub		r1, r1, r4
	ldr		r4, [r0, #16]
	mul		r12, r1, r4
	
	@load dc_texturemid to r1, add r12
	ldr		r1, [r0, #20]
	add		r1, r1, r12
	
	@load location of dc_source into r12
	ldr		r12, [r0, #24]

	@load location of dc_colormap into r5
	ldr		r5, [r0, #12]

	@r0 == dc_drawdata (can now be scratched)
	@r1 == frac
	@r2 == count	
	@r3 == dest
	@r4 == fracstep
	@r5 == dc_colormap
	@r12 == dc_source

	@loop through and fill out dest
.colCount:
	mov		r6, r1, lsr #16
	and		r6, r6, #127
	@index into dc_source with r12
	ldrb	r0, [r12, r6]
	@index into dc_colormap with r0
	ldrb	r6, [r5, r0]
	@store result from dc_colormap in dest
	strb	r6, [r3]
	@increment r3 (dest) by SCREENWIDTH, r1 (frac) by r4 (fracstep)
	add		r3, r3, #256
	add		r1, r1, r4
	
	subs	r2, r2, #1
	bpl		.colCount
	
	@pop registers back off stack
	ldmfd	sp!, {r4, r5, r6}

	bx		lr
FUNC_END

@=====================================
@R_DrawSpan
@floors, ceilings, etc
@=====================================
FUNC_START32 R_DrawSpan
	@put necessary registers on stack
	stmfd	sp!, {r4, r5, r6, r7, r8}
	
	ldr		r0, =dc_drawdata
	
	@r2 == ylookup[ds_y]
	ldr		r1, [r0, #36]
	ldr		r2, =ylookup
	ldr		r2, [r2, r1, asl #2]
	
	@r3 == columnofs[ds_x1], r1 == ds_x1
	ldr		r1, [r0, #40]
	ldr		r3, =columnofs
	ldr		r3, [r3, r1, asl #2]
	
	@add together for count
	add		r2, r2, r3

	@load other necessary values into registers
	ldr		r3, [r0, #44]
	sub		r1, r3, r1
	ldr		r3, [r0, #52]
	ldr		r4, [r0, #56]
	ldr		r5, [r0, #60]
	ldr		r6, [r0, #64]
	ldr		r7, [r0, #48]
	ldr		r8, [r0, #68]
	
	@r1 == count
	@r2 == dest
	@r3 == xfrac
	@r4 == yfrac
	@r5 == ds_xtep
	@r6 == ds_ystep
	@r7 == ds_colormap
	@r8 == ds_source

	@go through count and fill in dest
.spanCount:
	mov		r0, r4, lsr #10
	and		r0, r0, #4032
	mov		r12, r3, lsr #16
	and		r12, r12, #63
	add		r0, r0, r12

	ldrb	r0, [r8, r0]
	ldrb	r0, [r7, r0]
	strb	r0, [r2]
	add		r2, r2, #1
	add		r3, r3, r5
	add		r4, r4, r6

	subs	r1, r1, #1
	bpl		.spanCount

	@pop registers back off stack
	ldmfd	sp!, {r4, r5, r6, r7, r8}
	
	bx		lr
FUNC_END

@=====================================
@R_DrawColumnLow
@Source is the top of the column to scale
@=====================================
FUNC_START32 R_DrawColumnLow
	ldr		r0, =dc_drawdata
	@set r2 to count, r1 to dc_yl (reused), check for negative range
	ldr		r2, [r0, #8]
	ldr		r1, [r0, #4]
	sub		r2, r2, r1
	cmp		r2, #0
	bxlt	lr

	@put necessary registers on stack
	stmfd	sp!, {r4, r5, r6}
	
	@use r1 (dc_yl) as index into ylookup, r12 becomes ylookup[dc_yl]
	ldr		r12, =ylookup
	ldr		r12, [r12, r1, asl #2]
	
	@load dc_x into r4, r3 becomes columnofs[dc_x]
	ldr		r4, [r0]
	@mul dc_x*2 first in r4 for low detail version
	mov		r6, #2
	mul		r4, r6, r4
	ldr		r3, =columnofs
	ldr		r3, [r3, r4, asl #2]
	
	@add r12 (ylookup[dc_yl]) and r3 (columnofs[dc_x]) into r3 (ptr to dest)
	add		r3, r12, r3
	
	@load centery to r4, subtract from r1 (dc_yl) to r1, multiply result r1 by dc_iscale (loaded to r4) to r12
	ldr		r4, [r0, #32]
	sub		r1, r1, r4
	ldr		r4, [r0, #16]
	mul		r12, r1, r4
	
	@load dc_texturemid to r1, add r12
	ldr		r1, [r0, #20]
	add		r1, r1, r12
	
	@load location of dc_source into r12
	ldr		r12, [r0, #24]

	@load location of dc_colormap into r5
	ldr		r5, [r0, #12]

	@r0 == dc_drawdata (can now be scratched)
	@r1 == frac
	@r2 == count	
	@r3 == dest
	@r4 == fracstep
	@r5 == dc_colormap
	@r12 == dc_source

	@loop through and fill out dest
.colCountLow:
	mov		r6, r1, lsr #16
	and		r6, r6, #127
	@index into dc_source with r12
	ldrb	r0, [r12, r6]
	@index into dc_colormap with r0
	ldrb	r6, [r5, r0]

	@store result from dc_colormap in dest
	@low detail version stuffs the same byte into two pixels
	@to avoid two strbs, shift the 8 bits of r6 into the next 8 bits and strh
	mov		r0, r6, lsl #8
	orr		r6, r6, r0
	strh	r6, [r3]
	
	@increment r3 (dest) by SCREENWIDTH, r1 (frac) by r4 (fracstep)
	add		r3, r3, #256
	add		r1, r1, r4
	
	subs	r2, r2, #1
	bpl		.colCountLow
	
	@pop registers back off stack
	ldmfd	sp!, {r4, r5, r6}

	bx		lr
FUNC_END

@=====================================
@R_DrawSpanLow
@floors, ceilings, etc
@=====================================
FUNC_START32 R_DrawSpanLow
	@put necessary registers on stack
	stmfd	sp!, {r4, r5, r6, r7, r8}
	
	ldr		r0, =dc_drawdata
	
	@r2 == ylookup[ds_y]
	ldr		r1, [r0, #36]
	ldr		r2, =ylookup
	ldr		r2, [r2, r1, asl #2]
	
	@r3 == columnofs[ds_x1], r1 == ds_x1
	ldr		r1, [r0, #40]
	ldr		r3, =columnofs
	@mul ds_x1*2 into r4 and use as offset (for low detail version)
	mov		r4, #2
	mul		r4, r1, r4
	ldr		r3, [r3, r4, asl #2]
	
	@add together for count
	add		r2, r2, r3

	@load other necessary values into registers
	ldr		r3, [r0, #44]
	sub		r1, r3, r1
	ldr		r3, [r0, #52]
	ldr		r4, [r0, #56]
	ldr		r5, [r0, #60]
	ldr		r6, [r0, #64]
	ldr		r7, [r0, #48]
	ldr		r8, [r0, #68]
	
	@r1 == count
	@r2 == dest
	@r3 == xfrac
	@r4 == yfrac
	@r5 == ds_xtep
	@r6 == ds_ystep
	@r7 == ds_colormap
	@r8 == ds_source

	@go through count and fill in dest
.spanCountLow:
	mov		r0, r4, lsr #10
	and		r0, r0, #4032
	mov		r12, r3, lsr #16
	and		r12, r12, #63
	add		r0, r0, r12

	ldrb	r0, [r8, r0]
	ldrb	r0, [r7, r0]
	
	@store the same byte and increment by 2 for low detail version
	@to avoid two strbs, shift the 8 bits of r0 into the next 8 bits and strh
	mov		r12, r0, lsl #8
	orr		r0, r0, r12
	strh	r0, [r2]
	
	add		r2, r2, #2
	
	add		r3, r3, r5
	add		r4, r4, r6

	subs	r1, r1, #1
	bpl		.spanCountLow

	@pop registers back off stack
	ldmfd	sp!, {r4, r5, r6, r7, r8}
	
	bx		lr

FUNC_END