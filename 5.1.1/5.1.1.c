#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct WayTableStruct
{
    int* costs;
    int room_count;
} WayTable;

typedef struct ArrayStruct
{   
    int* array;
    int lenght;
    int space;
} Array;

const int DEFAULT_ARRAY_SIZE = 64;

Array* Array_new(){
    Array* array = (Array*) malloc(sizeof(Array));
    array->array = (int*) malloc(sizeof(int) * DEFAULT_ARRAY_SIZE);
    array->lenght = 0;
    array->space = DEFAULT_ARRAY_SIZE;
    return array;
}

void Array_destruct(Array* array){
    free(array->array);
    free(array);
}

void Array_append(Array* array, int element){
    if(array->space - array->lenght < 1){
        array->array = (int*) realloc(array->array, array->space + DEFAULT_ARRAY_SIZE);
        array->space += DEFAULT_ARRAY_SIZE;
    }
    array->array[array->lenght] = element;
    array->lenght++;
}

int Array_pop(Array* array){
    array->lenght--;
    return array->array[array->lenght];
}







WayTable* WayTable_new(int room_count){
    WayTable* way_table = (WayTable*) malloc(sizeof(WayTable));

    int lenght = room_count * (room_count - 1) / 2;
    way_table->costs = (int*) malloc(sizeof(int) * lenght);
    way_table->room_count = room_count;

    for(int i = 0; i < lenght; i++){
        way_table->costs[i] = -1;
    }

    return way_table;
}

void WayTable_destruct(WayTable* way_table){
    free(way_table->costs);
    free(way_table);
}

int WayTable_get_way_cost(WayTable* way_table, int from, int to){
    if(from == to){
        return 0;
    }
    if(from > to){
        int temp;
        temp = from;
        from = to;
        to = temp;
    }

    int room_count = way_table->room_count;
    int position = (2 * room_count - 1 - from) * from / 2 + to - 1 - from;

    return way_table->costs[position];
}

void WayTable_set_way_cost(WayTable* way_table, int from, int to, int cost){
    if(from == to){
        return;
    }

    if(from > to){
        int temp;
        temp = from;
        from = to;
        to = temp;
    }

    int room_count = way_table->room_count;
    int position = (2 * room_count - 1 - from) * from / 2 + to - 1 - from;

    way_table->costs[position] = cost;
}





void dfs(WayTable* way_table, int* riches, int from, int power, Array* visited, int cost_sum, int* max_rich){
    int room_count = way_table->room_count;
    int cost, to, i, skip;

    if(*max_rich < riches[from]){
        *max_rich = riches[from];
    }

    for(to = 0; to < room_count; to++){
        if(to == from){
            continue;
        }

        cost = WayTable_get_way_cost(way_table, from, to);
        if(cost < 0 || cost + cost_sum > power){
            continue;
        }

        skip = 0;
        for(i = 0; i < visited->lenght; i++){
            if(visited->array[i] == to){
                skip = 1;
                break;
            }   
        }
        if(skip == 1){
            continue;
        }

        Array_append(visited, to);
        dfs(way_table, riches, to, power, visited, cost_sum + cost, max_rich);
        Array_pop(visited);

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

    int power, room_count, way_count;
    fscanf(file_in, "%d %d %d", &power, &room_count, &way_count);

    WayTable* way_table = WayTable_new(room_count);

    int i, from, to, cost;
    for(i = 0; i < way_count; i++){
        fscanf(file_in, "%d %d %d", &from, &to, &cost);
        WayTable_set_way_cost(way_table, from, to, cost);
    }

    int* riches = (int*) malloc(sizeof(int) * room_count);

    for(int i = 0; i < room_count; i++){
        fscanf(file_in, "%d", riches + i);
    }

    fclose(file_in);


    Array* array = Array_new();
    int max_rich = -1;

    dfs(way_table, riches, 0, power, array, 0, &max_rich);

    Array_destruct(array);
    free(riches);
    WayTable_destruct(way_table);

    fprintf(file_out, "%d", max_rich);

    fclose(file_out);
}