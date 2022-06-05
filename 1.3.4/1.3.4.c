#include <stdio.h>

char name_in[] = "input.txt";
char name_out[] = "output.txt";

int digits[5000];
int digits2[5000];

int gen_simple_digits(){
    for(int i = 0; i < 5000; i++){
        digits[i] = i;
    }

    for(int i = 2; i < 5000; i++){
        if (digits[i] == 0){
            continue;
        }
        for(int i2 = i + 1; i2 < 5000; i2++){
            if (i2 % i == 0){
                digits[i2] = 0;
            }
        }
    }
    int len = 1;
    digits2[0] = 0;
    for(int i = 1; i < 5000; i++){
        if(digits[i] != 0){
            digits2[len] = digits[i];
            len++;
        }
    }
    return len;
}




int main(){
    FILE* fp_in;
    FILE* fp_out;
    if((fp_in = fopen(name_in, "r")) == NULL){
        printf("Attemtp to open file, %s is fail", name_in);
        return 1;
    }
    if((fp_out = fopen(name_out, "w")) == NULL){
        printf("Attemtp to open file, %s is fail", name_out);
        return 1;
    }

    int N;
    fscanf(fp_in, "%d", &N);


    int len = gen_simple_digits();
    int sum = 0;
    for(int row_i = 0; row_i < N; row_i++){
        for(int column_i = 0; column_i < N; column_i++){
            if (column_i + 1 == N){
                fprintf(fp_out, "%d\n", digits2[column_i + row_i]);
            }else{
                fprintf(fp_out, "%d ", digits2[column_i + row_i]);
            }
            sum += digits2[column_i + row_i];
        }
    }
    fprintf(fp_out, "%d", sum);

    
}