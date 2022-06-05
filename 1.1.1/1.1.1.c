#include <stdio.h>
#include <stdlib.h>

int* pointer_size;
int int_size;


int* find_middles_in_colums(int** matrix, int width, int height){
    int* middles_in_colums = (int*)malloc(width * (int)sizeof(int_size));
    int sum, row_i, column_i;
    for(int column_i = 0; column_i < width; column_i++){
        sum = 0;
        for(int row_i = 0; row_i < height; row_i++){
            sum += matrix[row_i][column_i];
        }
        middles_in_colums[column_i] = sum / height;
    }
    return middles_in_colums;
}

int* find_maxs_in_columns(int** matrix, int width, int height){
    int is_setted, max, row_i, column_i;
    int* maxs_in_colums = (int*)malloc(width * (int)sizeof(int_size));
    for(int column_i = 0; column_i < width; column_i++){
        is_setted = 0;
        for(int row_i = 0; row_i < height; row_i++){
            if(is_setted == 0 || max < matrix[row_i][column_i]){
                is_setted = 1;
                max = matrix[row_i][column_i];
            }
        }
        maxs_in_colums[column_i] = max;
    }
    return maxs_in_colums;
}

int* find_mins_in_rows(int** matrix, int width, int height){
    int is_setted, min, row_i, column_i;
    int* mins_in_rows = (int*)malloc(height * (int)sizeof(int_size));

    for(row_i = 0; row_i < height; row_i++){
        is_setted = 0;
        for(column_i = 0; column_i < width; column_i++){
            if(is_setted == 0 || min > matrix[row_i][column_i]){
                is_setted = 1;
                min = matrix[row_i][column_i];
            }
        }
        mins_in_rows[row_i] = min;
    }
    return mins_in_rows;
}
void print_result(int** matrix, int width, int height, int point_counts, FILE* fp_out){
    fprintf(fp_out, "%d\n", point_counts);
    int row_i, column_i;
    for(row_i = 0; row_i < height; row_i++){
        for(column_i = 0; column_i < width; column_i++){
            if(column_i + 1 == width){
                fprintf(fp_out, "%d\n", matrix[row_i][column_i]);
            }else{
                fprintf(fp_out, "%d ", matrix[row_i][column_i]);
            }
        }
    }
}


void process_matrix(int** matrix, int width, int height, FILE* fp_out){
    int* maxs_in_colums = find_maxs_in_columns(matrix, width, height);
    int* mins_in_rows = find_mins_in_rows(matrix, width, height);
    int* middles_in_colums = find_middles_in_colums(matrix, width, height);
    int row_i, column_i, point_counts;
    point_counts = 0;
    for(row_i = 0; row_i < height; row_i++){
        for(column_i = 0; column_i < width; column_i++){
            if(matrix[row_i][column_i] == mins_in_rows[row_i] && matrix[row_i][column_i] == maxs_in_colums[column_i]){
                point_counts++;
                matrix[row_i][column_i] = middles_in_colums[column_i];
            }
        }
    }

    print_result(matrix, width, height, point_counts, fp_out);
    free(maxs_in_colums);
    free(mins_in_rows);
    free(middles_in_colums);
}




int** get_memory_for_matrix(int width, int height){
    int** column = (int**)malloc(height * (int)sizeof(pointer_size));
    for(int row_i = 0; row_i < height; row_i++){
        column[row_i] = (int*)malloc(width * (int)sizeof(int_size));
    }
    return column;
}

char name_in[] = "input.txt";
char name_out[] = "output.txt";

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

    int width, height;
    fscanf(fp_in, "%d %d", &width, &height);
    
    int** matrix = get_memory_for_matrix(width, height);

    int row_i, column_i;
    for(row_i = 0; row_i < height; row_i++){
        for(column_i = 0; column_i < width; column_i++){
            fscanf(fp_in, "%d", &matrix[row_i][column_i]);
        }
    }

    process_matrix(matrix, width, height, fp_out);

    return 0;
}