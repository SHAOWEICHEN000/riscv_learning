#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    int arr_size;
    fscanf(input, "%d", &arr_size);
    int arr[arr_size];

    // Read integers from input file into the array
    for (int i = 0; i < arr_size; i++) {
        int data;
        fscanf(input, "%d", &data);
        arr[i] = data;
   }
    fclose(input);

    int *p_a = &arr[0];

    // array a bubble sort
    /* Original C code segment
    for (int i = 0; i < arr_size - 1; i++) {
       for (int j = 0; j < arr_size - i -1; j++) {
            if (*(p_a + j) > *(p_a + j + 1)) {
                int tmp = *(p_a + j);
                *(p_a + j) = *(p_a + j + 1);
                *(p_a + j + 1) = tmp;
            }
        }
    }
    */

    for (int i = 0; i < arr_size - 1; i++) {
        for (int j = 0; j < arr_size - i - 1; j++) {
            asm volatile(
           "slli t0, %[j], 2\n\t"           // t0 = j * 4
"add t0, %[base], t0\n\t"        // t0 = &arr[j]
    "lw t1, 0(t0)\n\t"               // t1 = arr[j]
    "lw t2, 4(t0)\n\t"               // t2 = arr[j+1]
    "ble t1, t2, 1f\n\t"             // if arr[j] <= arr[j+1] skip swap
    "sw t2, 0(t0)\n\t"               // arr[j] = arr[j+1]
    "sw t1, 4(t0)\n\t"               // arr[j+1] = arr[j]
    "1:\n\t"
    :
    : [base] "r"(p_a), [j] "r"(j)
   : "t0", "t1", "t2", "memory"
);


        }
    }
    p_a = &arr[0];
    for (int i = 0; i < arr_size; i++)
        printf("%d ", *p_a++);
    printf("\n");
    return 0;
}

