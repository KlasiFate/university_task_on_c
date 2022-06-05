#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct TupleStruct
{
    int first_number;
    int second_number;
    void* pointer;
} Touple;

typedef struct MatrixStruct
{
    int size;
    int** td_array;
    int determinant;
} Matrix;



Touple scan_matrixes(FILE* file_in){

    int matrix_count;
    fscanf(file_in, "%d", &matrix_count);
    Matrix* matrixes = (Matrix*) malloc(sizeof(Matrix) * matrix_count);

    int matrix_size, max_matrix_size = 0;
    for(int matrix_i = 0; matrix_i < matrix_count; matrix_i++){
        fscanf(file_in, "%d", &matrix_size);
        if(matrix_size > max_matrix_size){
            max_matrix_size = matrix_size;
        }
        int** column = (int**) malloc(sizeof(int) * matrix_size);
        for(int row_i = 0; row_i < matrix_size; row_i++){
            column[row_i] = (int*) malloc(sizeof(int) * matrix_size);
            for(int column_i = 0; column_i < matrix_size; column_i++){
                fscanf(file_in, "%d", column[row_i] + column_i);
            }
        }
        matrixes[matrix_i] = (Matrix) {
            matrix_size,
            column,
            0
        };
    }

    return (Touple) {max_matrix_size, matrix_count, matrixes};
}



int get_determinant(Matrix matrix, int row_i, int* excluded_columns, int excluded_columns_len){
    int** td_array = matrix.td_array;
    int matrix_size = matrix.size;

    int sign, skip_column, i;
    int submatrix_colum_i = 0, determinant = 0;
    for(int column_i = 0; column_i < matrix_size; column_i++){
        skip_column = 0;
        for(i = 0; i < excluded_columns_len; i++){
            if(excluded_columns[i] == column_i){
                skip_column = 1;
                break;
            }
        }
        if (skip_column == 1){
            continue;
        }

        if ((submatrix_colum_i + 2) % 2 == 0){
            sign = 1;
        }else{
            sign = -1;
        };
        if(matrix_size - excluded_columns_len != 1){
            excluded_columns[excluded_columns_len] = column_i;
            determinant += sign * td_array[row_i][column_i] * get_determinant(matrix, row_i + 1, excluded_columns, excluded_columns_len + 1);
        }else{
            determinant += td_array[row_i][column_i];
            break;
        }
        submatrix_colum_i++;
    }

    return determinant;
}

int compare_matrixes(Matrix* matrix1, Matrix* matrix2){
    if(matrix1->determinant < matrix2->determinant){
        return -1;
    }
    if(matrix1->determinant == matrix2->determinant){
        return 0;
    }
    return 1;
}




void quick_sort(Matrix* matrixes, int matrix_count){
    
    int i = 0, j = matrix_count - 1;
    int middle_determinant = matrixes[matrix_count >> 1].determinant;

    Matrix temp;

    do{
        while(matrixes[i].determinant < middle_determinant){
            i++;
        }
        while(matrixes[j].determinant > middle_determinant){
            j--;
        }

        if(i <= j){
            temp = matrixes[i];
            matrixes[i] = matrixes[j];
            matrixes[j] = temp;
            i++;
            j--;
        }
    }while(i <= j);

    if (j > 0){
        quick_sort(matrixes, j + 1);
    }
    if (matrix_count > i){
        quick_sort(matrixes + i, matrix_count - i);
    }

}



int main(){
    FILE* file_in;
    FILE* file_out;
    if((file_in = fopen("input.txt", "r")) == NULL){
        // printf("Attemtp to open file, %s is fail", name_in);
        return 1;
    }
    if((file_out = fopen("output.txt", "w")) == NULL){
        // printf("Attemtp to open file, %s is fail", name_out);
        return 1;
    }

    Touple result = scan_matrixes(file_in);
    int max_matrix_size = result.first_number;
    int matrix_count = result.second_number;
    Matrix* matrixes = (Matrix*) result.pointer;
    fclose(file_in);

    int* excluded_columns = (int*) malloc(sizeof(int) * (max_matrix_size - 1));
    for(int matrix_i = 0; matrix_i < matrix_count; matrix_i++){
        matrixes[matrix_i].determinant = get_determinant(matrixes[matrix_i], 0, excluded_columns, 0);
    }
    free(excluded_columns);

    // qsort(matrixes, matrix_count, sizeof(Matrix), (int(*) (const void* matrix1, const void* matrix2)) compare_matrixes);


    quick_sort(matrixes, matrix_count);


    Matrix matrix;
    int** td_array;
    int row_i, column_i;
    for(int matrix_i = 0; matrix_i < matrix_count; matrix_i++){
        matrix = matrixes[matrix_i];

        td_array = matrix.td_array;
        for(row_i = 0; row_i < matrix.size; row_i++){
            for(column_i = 0; column_i < matrix.size; column_i++){
                if(column_i != matrix.size - 1){
                    fprintf(file_out, "%d ", td_array[row_i][column_i]);
                }else{
                    fprintf(file_out, "%d\n", td_array[row_i][column_i]);
                }
            }
            free(td_array[row_i]);
        }
        free(td_array);
    }
    free(matrixes);
    fclose(file_out);
}















