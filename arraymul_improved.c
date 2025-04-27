// arraymul_improved.c
#include "arraymul.h"

void improved_version() {
    float *h_ptr = h;
    float *x_ptr = x;
    float *y_ptr = y;
    float  id_scalar = student_id % 100;
    int    N         = arr_size;
    int    offset    = 0;
    int    vl;   // 实际的向量长度

    while (offset < N) {
        int left = N - offset;
        // 1) vsetvli: 设定 vl = min(left, VLMAX)
        asm volatile(
            "vsetvli t0, %1, e32, m1    \n\t"  // t0 = VL
            "mv      %0, t0            \n\t"  // C: vl = t0
            : "=r"(vl), "+r"(left)
            :
            : "t0"
        );
        vsetvli_cnt++;

        // 2) load vector h
        asm volatile(
            "vlw.v    v0, (%[hp])       \n\t"
            : 
            : [hp]"r"(h_ptr+offset)
            : "v0"
        );
        vlw_cnt++;

        // 3) load vector x
        asm volatile(
            "vlw.v    v1, (%[xp])       \n\t"
            : 
            : [xp]"r"(x_ptr+offset)
            : "v1"
        );
        vlw_cnt++;

        // 4) vfmul.vv v2 = v0 * v1
        asm volatile(
            "vfmul.vv v2, v0, v1       \n\t"
            :
            :
            : "v2"
        );
        vfmul_vv_cnt++;

        // 5) vfadd.vf v3 = v2 + id_scalar
        asm volatile(
            "vfadd.vf v3, v2, %[id]    \n\t"
            :
            : [id]"f"(id_scalar)
            : "v3"
        );
        vfadd_vf_cnt++;

        // 6) vsw.v v3 -> y
        asm volatile(
            "vsw.v    v3, (%[yp])       \n\t"
            :
            : [yp]"r"(y_ptr+offset)
            :
        );
        vsw_cnt++;

        // 7) 其他剩余指令
        others_cnt += 3; // while 判断、offset 更新等大概 3 条指令

        offset += vl;
    }
}

