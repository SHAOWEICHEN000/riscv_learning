#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "macro_define.h"
#include "arraymul.h"
int main(int argc, char *argv[]){
    init();
    input();
    printf("===== Question 2-2 =====\n");
    printf("array size = %d\n", arr_size);
    printf("student id = %d\n", (int)(student_id));
    printf("output: ");
    improved_version();
    return 0;
}

void improved_version(){
    float *p_h = h;
    float *p_x = x;
    float *p_y = y;
    float id = student_id;// id = your_student_id % 100;
    //:w
    //original C code
    for (int i = 0; i < arr_size; i++){
        p_y[i] = p_h[i] * p_x[i] + id;
    }
int offset = 0, vl, left;
int    N = arr_size;
    while (offset < arr_size) {
        left = arr_size - offset;
   int    left = N - offset;
   int    vl;
 
    int arr_length = arr_size;
    
          // #include "arraymul_improved.c"
	   asm volatile(
            "vsetvli %0, %1, e32, m1    \n\t"  // t0 = VL
            "mv      %0, t0            \n\t"  // C: vl = t0
            :"+r"(vl)
            :"r"(left)
            : "t0"
            );
	   offset += vl;
    

	   asm volatile(
            "vle32.v v0, (%[hp])       \n\t"
            :
            : [hp]"r"(p_h+offset)
            : "v0"
            );
	    flw_cnt++; // load floating vector
	   asm volatile(
            "vle32.v    v1, (%[xp])       \n\t"
            :
            : [xp]"r"(p_x+offset)
            : "v1"
            );
	    flw_cnt++; // load floating vector
	   asm volatile(
            "vfmul.vv v2, v0, v1       \n\t"
            :
            :
            : "v2"
            );
	    fmul_cnt++; // load floating vector
	   asm volatile(
            "vfadd.vf v3, v2, %[id]    \n\t"
            :
            : [id]"f"(id)
            : "v3"
            );
	    fadd_cnt++; // load floating vector
	   asm volatile(
            "vse32.v    v3, (%[yp])       \n\t"
            :
            : [yp]"r"(p_y+offset)
            :"v3"
            );
	     fsw_cnt++; // load floating vector
    for (int i = 0; i < arr_size; i++){
        printf(" %.6f ", y[i]);
    }

    printf("\n\n");
    printf("add counter used: %d\n", add_cnt);
    printf("sub counter used: %d\n", sub_cnt);
    printf("mul counter used: %d\n", mul_cnt);
    printf("div counter used: %d\n", div_cnt);
    printf("lw counter used: %d\n", lw_cnt);
    printf("sw counter used: %d\n", sw_cnt);
    printf("fadd counter used: %d\n", fadd_cnt);
    printf("fsub counter used: %d\n", fsub_cnt);
    printf("fmul counter used: %d\n", fmul_cnt);
    printf("fdiv counter used: %d\n", fdiv_cnt);
    printf("flw counter used: %d\n", flw_cnt);
    printf("fsw counter used: %d\n", fsw_cnt);
    printf("others counter used: %d\n", others_cnt);
    macro_arraymul_vector_cycle_count
    printf("The total cycle count in this program: %.0f\n", arraymul_vector_cycle_count);
macro_arraymul_vector_cpu_time
    printf("CPU time = %f us\n", arraymul_vector_cpu_time);
    FILE *fp_1;
    fp_1 = fopen("arraymul_vector_cpu_time.txt", "w");
    fprintf(fp_1, "%f", arraymul_vector_cpu_time);
    fclose(fp_1);

    float speedup = 0.0;

    FILE *fp_2;
    fp_2 = fopen("arraymul_baseline_cpu_time.txt", "r");
    fscanf(fp_2, "%f", &speedup);
    fclose(fp_2);
    speedup = speedup / arraymul_vector_cpu_time;
    printf("V extension ISA faster %f times than baseline ISA\n", speedup);
}
}
void init(){
    add_cnt = 0;
    sub_cnt = 0;
    mul_cnt = 0;
    div_cnt = 0;
    lw_cnt = 0;
    sw_cnt = 0;
    others_cnt = 0;

    float temp = arr_size;
    while(temp != 1 && temp > 1 )
    {
        temp = temp / 2;
    }
    if (temp != 1)
    {
        printf("log2(arr_size) must be an integer !!!\n");
        exit(0);
    }

    h = calloc(arr_size, sizeof(float));
    x = calloc(arr_size, sizeof(float));
    y = calloc(arr_size, sizeof(float));
}
void input(){

        int fd;
        char *buf = (char *)calloc(3000, sizeof(char));

        fd = open("./arraymul_input.txt", O_RDONLY);
        if(fd < 0)
                exit(EXIT_FAILURE);
        if(read(fd, buf, 3000) < 0)
                exit(EXIT_FAILURE);

        char *token;
    token = strtok(buf, " ");
        int count = 0;

        for(int i = 0; i < arr_size*2; ++i){
                if(i < arr_size)
                    h[i] = atof(token);
                else
                    x[i-arr_size] = atof(token);

                token = strtok(NULL, " ");
	}
}
