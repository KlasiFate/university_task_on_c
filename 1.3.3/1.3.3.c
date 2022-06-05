#include <stdio.h>

char name_in[] = "input.txt";
char name_out[] = "output.txt";

int simple_digits[5000];
int simple_digits_len = 0;

void gen_simple_digits(){
    int digits[5000];
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
    simple_digits_len = 1;
    simple_digits[0] = 0;
    for(int i = 1; i < 5000; i++){
        if(digits[i] != 0){
            simple_digits[simple_digits_len] = simple_digits[i];
            simple_digits_len++;
        }
    }
}

long long int Ls[1000];
long long int Ls_len = 1000;

void gen_L(){
    Ls[0] = 2;
    Ls[1] = 1;
    for(int i = 2; i < Ls_len; i++){
        Ls[i] = (long long int)(Ls[i - 1] + Ls[i - 2]);
    }
}

int check_if_digit_is_simple(int digit){
    for(int i = 2; i < simple_digits_len; i++){
        if (digit % simple_digits[i] == 0){
            return 0;
        }
    }
    return 1;
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

    gen_simple_digits();
    gen_L();

    if (check_if_digit_is_simple(5) == 0){
        printf("yes");
    }
}