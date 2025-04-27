#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "fft.h"
#include "macro_define.h"


typedef struct {
    float Re;
    float Im;
} Complex;


Complex complex_add(Complex a, Complex b)
{
    Complex result;
    //result.Re = a.Re + b.Re;
    //result.Im = a.Im + b.Im;
    //fadd_cnt += 2;  // 分別減了實部與虛部
    asm volatile(
	"fadd.s  %[C_Re],     %[A_Re],     %[B_Re] \n\t"
        "addi    %[fadd_cnt], %[fadd_cnt], 1       \n\t"
        "fadd.s  %[C_Im],     %[A_Im],     %[B_Im] \n\t"
        "addi    %[fadd_cnt], %[fadd_cnt], 1       \n\t"
        :[C_Re] "=f"(result.Re), [C_Im] "=f"(result.Im), [fadd_cnt] "+r"(fadd_cnt)
        :[A_Re] "f"(a.Re), [B_Re] "f"(b.Re), [A_Im] "f"(a.Im), [B_Im] "f"(b.Im));
    return result;
}

Complex complex_sub(Complex a, Complex b)
{
    Complex result;
    //result.Re = a.Re - b.Re;
    //result.Im = a.Im - b.Im;
    //fsub_cnt += 2;
    asm volatile(
       	"fsub.s   %[C_Re],      %[A_Re],     %[B_Re] \n\t"
        "addi     %[fsub_cnt],  %[fsub_cnt], 1       \n\t"
        "fsub.s   %[C_Im],      %[A_Im],     %[B_Im] \n\t"
        "addi     %[fsub_cnt],  %[fsub_cnt], 1       \n\t"

        :[C_Re] "=f"(result.Re), [C_Im] "=f"(result.Im), [fsub_cnt] "+r"(fsub_cnt)
        :[A_Re] "f"(a.Re), [B_Re] "f"(b.Re), [A_Im] "f"(a.Im), [B_Im] "f"(b.Im));
    return result;
}

Complex complex_mul(Complex a, Complex b)
{
    Complex result;
  /*  float real_part = (a.Re * b.Re) - (a.Im * b.Im);
    float imag_part = (a.Re * b.Im) + (a.Im * b.Re);

    result.Re = real_part;
    result.Im = imag_part;

    fmul_cnt += 4;  // 4 次乘法
    fadd_cnt += 1;  // 一次加法（虛部）
    fsub_cnt += 1;  // 一次減法（實部）
*/
float temp1, temp2, temp3, temp4;

    asm volatile(
        "fmul.s %[out], %[in1], %[in2]\n\t"
	"addi     %[fmul_cnt],  %[fmul_cnt], 1       \n\t"

        : [out] "=f" (temp1),[fmul_cnt]"+r"(fmul_cnt)
        : [in1] "f" (a.Re), [in2] "f" (b.Re)
    );

    asm volatile(
        "fmul.s %[out], %[in1], %[in2]\n\t"
	"addi     %[fmul_cnt],  %[fmul_cnt], 1       \n\t"
        : [out] "=f" (temp2),[fmul_cnt]"+r"(fmul_cnt)
        : [in1] "f" (a.Im), [in2] "f" (b.Im)
    );

    asm volatile(
        "fsub.s %[out], %[in1], %[in2]\n\t"
	"addi     %[fsub_cnt],  %[fsub_cnt], 1       \n\t"
        : [out] "=f" (result.Re),[fsub_cnt]"+r"(fsub_cnt)
        : [in1] "f" (temp1), [in2] "f" (temp2)
    );

    asm volatile(
        "fmul.s %[out], %[in1], %[in2]\n\t"
	"addi     %[fmul_cnt],  %[fmul_cnt], 1       \n\t"
        : [out] "=f" (temp3),[fmul_cnt]"+r"(fmul_cnt)
        : [in1] "f" (a.Re), [in2] "f" (b.Im)
    );

    asm volatile(
        "fmul.s %[out], %[in1], %[in2]\n\t"
	"addi     %[fmul_cnt],  %[fmul_cnt], 1       \n\t"
        : [out] "=f" (temp4),[fmul_cnt]"+r"(fmul_cnt)
        : [in1] "f" (a.Im), [in2] "f" (b.Re)
    );

    asm volatile(
        "fadd.s %[out], %[in1], %[in2]\n\t"
	"addi     %[fadd_cnt],  %[fadd_cnt], 1       \n\t"
        : [out] "=f" (result.Im),[fadd_cnt]"+r"(fadd_cnt)
        : [in1] "f" (temp3), [in2] "f" (temp4)
    );
    return result;
}

uint32_t Log2(uint32_t N)
{
    uint32_t log = 0;
    /*    while (N >>= 1) {
        log++;
        add_cnt++;
        others_cnt++;  // for >>=
        }
*/

    asm volatile(
        "mv t0, %[N]                            \n\t"
        "mv t1, x0                              \n\t"
        "addi %[others_cnt],%[others_cnt],2     \n\t"
    "loop:                                      \n\t"
        "srli t0, t0, 1                         \n\t"
	"addi %[others_cnt],%[others_cnt],2     \n\t"
        "beqz t0, end_loop                      \n\t"
        "addi t1, t1, 1                         \n\t"
	"addi %[add_cnt],%[add_cnt],1           \n\t"
        "j loop                                 \n\t"
    "end_loop:                                  \n\t"
        "mv %[log], t1                          \n\t"
        "addi %[others_cnt],%[others_cnt],2     \n\t"

        : [log] "+r"(log), 
	  [add_cnt] "+r"(add_cnt), 
	  [others_cnt] "+r"(others_cnt) 
        :[N]"r"(N)
        : "t0", "t1"
    );
    
    return log;
}

float PI(void)
{
    /* description: Gregory-Leibniz series
    * 
    *    we use Leibniz formula to approximate Pi
    * 
    *    pi/4 = (1 - 1/3 + 1/5 - 1/7 + 1/9 - ...)
    *    pi   = 4(1 - 1/3 + 1/5 - 1/7 + 1/9 - ...)
    * 
    **/
    float pi = 0.0f;
/*    int sign = 1;
    int denominator = 1;

    for (int i = 0; i < iter; i++) {
        pi += sign * (1.0f / denominator);
        sign *= -1;
        denominator += 2;
    }*/
    asm volatile(
    "li t4, 0                       \n\t" // i = 0
    "li t1, 1                       \n\t" // sign = 1
    "li t2, 1                       \n\t" // denominator = 1
"loop_pi:                             \n\t"
    "bge t4, %[N], end_pi           \n\t"
    "fcvt.s.w ft0, t2               \n\t" // ft0 = float(denominator)
    "addi %[lw_cnt], %[lw_cnt],  1    \n\t"
    "li t5, 0x3f800000 		    \n\t"	
    "fmv.w.x ft1, t5                  \n\t" // ft1 = 1.
    "fdiv.s ft0, ft1, ft0            \n\t" // ft0 = 1.0 / denominator
    "addi %[fdiv_cnt], %[fdiv_cnt], 1 \n\t" // 1次 fdiv
    "fcvt.s.w ft1, t1               \n\t" // ft1 = float(sign)
    "addi %[lw_cnt], %[lw_cnt], 1     \n\t"
    "fmul.s ft0, ft0, ft1            \n\t" // ft0 = (1/denominator) * sign
    "addi %[mul_cnt], %[mul_cnt], 1 \n\t" // 1次 mul(actually is fmul)
    "fadd.s %[pi], %[pi], ft0        \n\t" // pi += ft0
    "addi %[fadd_cnt], %[fadd_cnt], 1\n\t" // 1次 fadd
    "neg t1, t1                     \n\t" // sign = -sign
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "addi t2, t2, 2                  \n\t" // denominator += 2
    "addi %[add_cnt], %[add_cnt], 2 \n\t"
    "addi t4, t4, 1                  \n\t" // i += 1
    "j loop_pi                      \n\t"
"end_pi:                              \n\t"
    "li t5, 0x40800000              \n\t"
    "fmv.w.x ft1, t5                  \n\t"
    "addi %[add_cnt], %[add_cnt], 4 \n\t"
    : [pi] "+f" (pi),
      [add_cnt]"+r"(add_cnt),
      [fadd_cnt] "+r"(fadd_cnt), 
      [fsub_cnt]"+r"(fsub_cnt), 
      [mul_cnt]"+r"(mul_cnt),
      [fdiv_cnt] "+r"(fdiv_cnt), 
      [lw_cnt] "+r"(lw_cnt), 
      [sw_cnt] "+r"(sw_cnt),
      [others_cnt] "+r"(others_cnt)
    : [N] "r" (iter)
    : "t0", "t1", "t2", "t4","t5", "ft0", "ft1"
);

    pi = 4 * pi;
    return pi;
}

uint32_t bit_reverse(uint32_t b, uint32_t m)
{
	uint32_t result = 0;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t bit = (b >> i) & 1;
        result |= bit << ((m - 1) - i);
    }
asm volatile(
	"li %[result], 0\n\t"//result=0
	"li t4,0\n\t"//i=0
	"loop_bit:\n\t"
	"bge t4,%[m],end_bit\n\t"
	"srl t0,%[b],t4\n\t"
	"addi %[others_cnt],%[others_cnt],1\n\t"
	"and t1,t0,1\n\t"
	"addi %[others_cnt],%[others_cnt],1\n\t"
	"addi t2,t1,0\n\t"
	
	"addi t3,%[m],-1\n\t"

	"sub  t5,t3,t4\n\t"
	"sll t6,t2,t5\n\t"
	"addi %[others_cnt],%[others_cnt],1\n\t"
	"or  %[result],%[result],t6\n\t"
	"addi %[others_cnt],%[others_cnt],1\n\t"
	"addi t4,t4,1\n\t"
	"j loop_bit\n\t"
	"end_bit:\n\t"
	:[result] "+r"(result),
	 [add_cnt]"+r"(add_cnt),
	 [sub_cnt]"+r"(sub_cnt),
	 [others_cnt]"+r"(others_cnt)
	:[m] "r"(m),[b] "r"(b)
	:"t4","t0","t1","t2","t3","t5","t6"
	    );
/*    asm volatile (
        #include "bit_reverse.c"
        : [b] "+r"(b), [lw_cnt] "+r"(lw_cnt), [add_cnt] "+r"(add_cnt), [sub_cnt] "+r"(sub_cnt), [sw_cnt] "+r"(sw_cnt), [others_cnt] "+r"(others_cnt)
        : [m] "r"(m)
        : "t0", "t1", "t2"
    );*/
    return result;
}

void fft(Complex *x, uint32_t N)
{
    uint32_t k = N, n;
    pi = PI();
    float thetaT = pi / N;
    Complex phiT = {cos(thetaT), -sin(thetaT)}; // Twiddle Factor
    Complex T, temp;
    
    while (k > 1) { // Butterfly Computation
        n = k;
        k >>= 1;
        phiT = complex_mul(phiT, phiT);
        T.Re = 1.0;
        T.Im = 0.0;
        
        for (uint32_t l = 0; l < k; l++)
        {
            for (uint32_t a = l; a < N; a += n)
            {
                uint32_t b = a + k;
                temp = complex_sub(x[a], x[b]);
                x[a] = complex_add(x[a], x[b]);
                x[b] = complex_mul(temp, T);
            }
            T = complex_mul(T, phiT);
        }
    }

    // Bit-reverse
    uint32_t m = Log2(N);
    for (uint32_t a = 0; a < N; a++)
    {
        uint32_t b = a;
        b = bit_reverse(b, m);
        if (b > a) {
            temp = x[a];
            x[a] = x[b];
            x[b] = temp;
        }
    }
}

void print_fft(Complex *x, int N)
{
    for (int i = 0; i < N; i++)
    {
        printf("X[%d] = %.6f + %.6fj\n", i, x[i].Re, x[i].Im);
    }
}

int main() {

    FILE *fp = fopen("fft_input.txt", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }

float pi;

    Complex *data = malloc(N * sizeof(Complex));
    if (data == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        fscanf(fp, "%f %f", &data[i].Re, &data[i].Im);
    }

    fft(data, N);

    printf("===== Question 1-1 =====\n");

    printf("FFT result:\n");
    print_fft(data, N);
    printf("\n");

    printf("PI = %.6f\n", pi);
    printf("\n");

    printf("add counter used: %d\n", add_cnt);
    printf("sub counter used: %d\n", sub_cnt);
    printf("mul counter used: %d\n", mul_cnt);
    printf("div counter used: %d\n", div_cnt);
    printf("fadd counter used: %d\n", fadd_cnt);
    printf("fsub counter used: %d\n", fsub_cnt);
    printf("fmul counter used: %d\n", fmul_cnt);
    printf("fdiv counter used: %d\n", fdiv_cnt);
    printf("lw counter used: %d\n", lw_cnt);
    printf("sw counter used: %d\n", sw_cnt);
    printf("others counter used: %d\n", others_cnt);
	
    macro_fft_cycle_count
    printf("The total cycle count in this program: %.0f\n", fft_cycle_count);

    macro_fft_cpu_time
    printf("CPU time = %f us\n", fft_cpu_time);
    
    macro_calc_fft_ratio 
    
    if(fft_ratio > 0.5){
        printf("This program is a CPU bound task.\n");
        }
    else{
        printf("This program is a Memory bound task.\n");
        }

    return 0;
}
