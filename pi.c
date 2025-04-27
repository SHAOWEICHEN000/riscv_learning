"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"li t4, 0                       \n\t" // i = 0
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
    "li t1, 1                       \n\t" // sign = 1
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
    "li t2, 1                       \n\t" // denominator = 1
"loop_pi:                             \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
    "bge t4, %[N], end_pi           \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
    "fcvt.s.w ft0, t2               \n\t" // ft0 = float(denominator)
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
    "li t5, 0x3f800000              \n\t"       
"addi %[others_cnt], %[others_cnt], 1\n\t"
    "fmv.w.x ft1, t5                  \n\t" // ft1 = 1.
"addi %[fdiv_cnt], %[fdiv_cnt], 1\n\t"
    "fdiv.s ft0, ft1, ft0            \n\t" // ft0 = 1.0 / denominator
"addi %[others_cnt], %[others_cnt], 1\n\t"
    "fcvt.s.w ft1, t1               \n\t" // ft1 = float(sign)
"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
    "fmul.s ft0, ft0, ft1            \n\t" // ft0 = (1/denominator) * sign
"addi %[fadd_cnt], %[fadd_cnt], 1\n\t"
    "fadd.s %[pi], %[pi], ft0        \n\t" // pi += ft0
"addi %[others_cnt], %[others_cnt], 1\n\t"
    "neg t1, t1                     \n\t" // sign = -sign
"addi %[add_cnt], %[add_cnt], 1\n\t"
    "addi t2, t2, 2                  \n\t" // denominator += 2
"addi %[add_cnt], %[add_cnt], 1\n\t"
    "addi t4, t4, 1                  \n\t" // i += 1
"addi %[others_cnt], %[others_cnt], 1\n\t"
    "j loop_pi                      \n\t"
"end_pi:                              \n\t"
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
    "li t5, 0x40800000              \n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"
    "fmv.w.x ft1, t5                  \n\t"
