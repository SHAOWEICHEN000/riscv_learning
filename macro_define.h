#define macro_fft_cycle_count fft_cycle_count=add_CPI*add_cnt+sub_CPI*sub_cnt+mul_CPI*mul_cnt+div_CPI*div_cnt+lw_CPI*lw_cnt+sw_CPI*sw_cnt+others_CPI*others_cnt+fadd_CPI*fadd_cnt+fsub_CPI*fsub_cnt+fmul_CPI*fmul_cnt+fdiv_CPI*fdiv_cnt;
#define macro_fft_cpu_time fft_cpu_time = (float)(fft_cycle_count*384.0);
#define macro_arraymul_vector_cycle_count arraymul_vector_cycle_count =0;
#define macro_arraymul_vector_cpu_time arraymul_vector_cpu_time =0; 
#define macro_arraymul_baseline_cycle_count_2_1 arraymul_baseline_cycle_count_2_1 = fadd_CPI *fadd_cnt+fsub_CPI*fsub_cnt+fmul_CPI*fmul_cnt+fdiv_CPI*fdiv_cnt+flw_CPI*flw_cnt+fsw_CPI*fsw_cnt+add_CPI*add_cnt+others_CPI*others_cnt+mul_CPI*mul_cnt+lw_cnt*lw_CPI+sw_cnt*sw_CPI;

#define macro_arraymul_baseline_cycle_count arraymul_baseline_cycle_count = fadd_CPI *fadd_cnt+fsub_CPI*fsub_cnt+fmul_CPI*fmul_cnt+fdiv_CPI*fdiv_cnt+flw_CPI*flw_cnt+fsw_CPI*fsw_cnt+add_CPI*add_cnt+others_CPI*others_cnt;

#define macro_arraymul_baseline_cpu_time_2_1 arraymul_baseline_cpu_time_2_1 =(float)( arraymul_baseline_cycle_count_2_1*384.0);

#define macro_arraymul_baseline_cpu_time arraymul_baseline_cpu_time =(float)( arraymul_baseline_cycle_count*384.0);

#define macro_calc_arraymul_baseline_ratio arraymul_baseline_ratio= (float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_cnt + fmul_cnt + fdiv_cnt) / \
                (float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_cnt + fmul_cnt + fdiv_cnt + lw_cnt + sw_cnt + others_cnt);

#define macro_calc_arraymul_double_ratio arraymul_baseline_ratio =(float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_cnt + fmul_cnt + fdiv_cnt) / \
                (float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_\
cnt + fmul_cnt + fdiv_cnt + lw_cnt + sw_cnt + others_cnt);

#define macro_arraymul_double_cycle_count arraymul_double_cycle_count = dadd_CPI*dadd_cnt+dsub_CPI*dsub_cnt+dmul_CPI*dmul_cnt+ddiv_CPI*ddiv_cnt+dlw_CPI*dlw_cnt+dsw_CPI*dsw_cnt+add_CPI*add_cnt+others_CPI*others_cnt;

#define macro_arraymul_double_cpu_time arraymul_double_cpu_time = ( float)(arraymul_double_cycle_count*384.0);

#define macro_calc_fft_ratio \
    fft_ratio = (float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_cnt + fmul_cnt + fdiv_cnt) / \
                (float)(add_cnt + sub_cnt + mul_cnt + div_cnt + fadd_cnt + fsub_cnt + fmul_cnt + fdiv_cnt + lw_cnt + sw_cnt + others_cnt);
