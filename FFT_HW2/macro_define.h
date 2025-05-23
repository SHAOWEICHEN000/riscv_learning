#define macro_fft_cycle_count fft_cycle_count=1;
#define macro_fft_cpu_time fft_cpu_time = 0;
#define macro_arraymul_vector_cycle_count arraymul_vector_cycle_count = 0;

#define macro_arraymul_vector_cpu_time arraymul_vector_cpu_time = 0;

#define macro_arraymul_baseline_cycle_count arraymul_baseline_cycle_count = 0;

#define macro_arraymul_baseline_cpu_time arraymul_baseline_cpu_time = 0;

#define macro_calc_arraymul_baseline_ratio arraymul_baseline_ratio = 0;

#define macro_calc_arraymul_double_ratio arraymul_baseline_ratio = 0;

#define macro_arraymul_double_cycle_count arraymul_double_cycle_count = 0;

#define macro_arraymul_double_cpu_time arraymul_double_cpu_time = 0;

#define macro_calc_fft_ratio \
    fft_ratio = (float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_cnt + fmul_cnt + fdiv_cnt) / \
                (float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_cnt + fmul_cnt + fdiv_cnt + lw_cnt + sw_cnt + others_cnt);
