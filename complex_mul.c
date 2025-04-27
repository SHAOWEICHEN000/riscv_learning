"addi %[others_cnt], %[others_cnt], 1\n\t"
 asm volatile(
"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
        "fmul.s %[out], %[in1], %[in2]\n\t"

        : [in1] "f" (a.Re), [in2] "f" (b.Re)
"addi %[others_cnt], %[others_cnt], 1\n\t"
    );

"addi %[others_cnt], %[others_cnt], 1\n\t"
    asm volatile(
"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
        "fmul.s %[out], %[in1], %[in2]\n\t"
        : [in1] "f" (a.Im), [in2] "f" (b.Im)
"addi %[others_cnt], %[others_cnt], 1\n\t"
    );

"addi %[others_cnt], %[others_cnt], 1\n\t"
    asm volatile(
"addi %[fsub_cnt], %[fsub_cnt], 1\n\t"
        "fsub.s %[out], %[in1], %[in2]\n\t"
        : [in1] "f" (temp1), [in2] "f" (temp2)
"addi %[others_cnt], %[others_cnt], 1\n\t"
    );

"addi %[others_cnt], %[others_cnt], 1\n\t"
    asm volatile(
"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
        "fmul.s %[out], %[in1], %[in2]\n\t"
        : [in1] "f" (a.Re), [in2] "f" (b.Im)
"addi %[others_cnt], %[others_cnt], 1\n\t"
    );

"addi %[others_cnt], %[others_cnt], 1\n\t"
    asm volatile(
"addi %[fmul_cnt], %[fmul_cnt], 1\n\t"
        "fmul.s %[out], %[in1], %[in2]\n\t"
        : [in1] "f" (a.Im), [in2] "f" (b.Re)
"addi %[others_cnt], %[others_cnt], 1\n\t"
    );

"addi %[others_cnt], %[others_cnt], 1\n\t"
    asm volatile(
"addi %[fadd_cnt], %[fadd_cnt], 1\n\t"
        "fadd.s %[out], %[in1], %[in2]\n\t"
        : [in1] "f" (temp3), [in2] "f" (temp4)
"addi %[others_cnt], %[others_cnt], 1\n\t"
    );
