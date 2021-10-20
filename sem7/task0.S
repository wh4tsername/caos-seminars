    .intel_syntax noprefix
    .text
    .global very_important_function

    // (N=rdi, *A=rsi, *B=rdx, *R=rcx)
very_important_function:

    vxorps  ymm2, ymm2, ymm2 // clean using XOR method
    mov     r8, rdi // rdi will change, but N will in use too
    mov     r9, rdx // rdx will change on mul operation

.LoopBegin:
    sub     rdi, 8          // 8 elements at once
    mov     rax, 4
    mul     rdi

    vmovaps ymm0, [rsi + rax]
    vmovaps ymm1, [r9 + rax]
    vaddps  ymm0, ymm0, ymm1
    vmovaps [rcx + rax], ymm0

    vhaddps ymm2, ymm2, ymm0  // ymm2 = ∑ymm2ᵢ + ∑ymm0ᵢ

    cmp     rdi, 0
    jg      .LoopBegin

    mov     rcx, 8
    vxorps  ymm0, ymm0, ymm0 // clean using XOR method
.Loop8Times:
    // ymm2 = [x7,x6,x5,x4,x3,x2,x1,x0]
    vextractf128 xmm1, ymm2, 0  // xmm0 = [x3,x2,x1,x0]
    vaddss      xmm0, xmm0, xmm1
    vpermilps   ymm3, ymm2, 0b00111001 // ymm3 = [x4,x7,x6,x5,x0,x3,x2,x1]
    vperm2f128  ymm4, ymm3, ymm3, 0b10000001 // ymm4 = [0,0,0,0,x4,x7,x6,x5]
    vblendps    ymm2, ymm3, ymm4, 0b10001000 // ymm2 = [0,x7,x6,x5,x4,x3,x2,x1]
    loop .Loop8Times

    vcvtsi2sd xmm1, xmm1, r8    // xmm1 = (double) (int64_t r8)
    vcvtss2sd xmm0, xmm0, xmm0  // xmm0 = (double) (float r8)
    vdivsd  xmm0, xmm0, xmm1    // xmm0 = xmm0 / xmm1
    ret

