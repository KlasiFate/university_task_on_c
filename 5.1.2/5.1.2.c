#include <stdio.h>
#include <stdlib.h>


typedef struct WayTable
{
    int n;
    int* array;
} WayTable;


WayTable* WayTable_new(int n){
    WayTable* way_table = (WayTable*) malloc(sizeof(WayTable));
    way_table->n = n;
    way_table->array = (int*) malloc(sizeof(int) * n * (n - 1) / 2);
    for(int i = 0; i < n; ++i){
        way_table->array[i] = -1;
    }

    return way_table;
}

void WayTable_destruct(WayTable* way_table){
    free(way_table->array);
    free(way_table);
}

void WayTable_put(WayTable* way_table, int from, int to, int cost){
    if(from > to){
        int temp;
        temp = from;
        from = to;
        to = temp;
    }

    way_table->array[(2 * way_table->n - 1 - from) * from / 2 + to - 1 - from] = cost;
}

int WayTable_get(WayTable* way_table, int from, int to){
    if(from > to){
        int temp;
        temp = from;
        from = to;
        to = temp;
    }

    return way_table->array[(2 * way_table->n - 1 - from) * from / 2 + to - 1 - from];
}

int* find_way_using_Dijkstra(WayTable* way_table, int k){
    
    static int* visited = NULL;
    static int visited_len = 0;

    static int from = 0;

    static int* way = NULL;
    static int min_way_cost = -1;
    static int current_way_cost = 0;

    int should_all_be_reset = 0;
    if(visited == NULL){
        should_all_be_reset = 1;
        visited = (int*) malloc(sizeof(int) * way_table->n);
        way = (int*) malloc(sizeof(int) * (way_table->n + 2)) + 2;
    }
    visited[visited_len++] = from;

    if(from == k){
        if(min_way_cost == -1 || min_way_cost > current_way_cost){
            for(int i = 0; i < visited_len; ++i){
                way[i] = visited[i];
            }
            min_way_cost = current_way_cost;
            *(way - 2) = visited_len;
            *(way - 1) = current_way_cost;
        }
        --visited_len;
        return NULL;
    }

    for(int i = 0; i < way_table->n; ++i){
        int cost = WayTable_get(way_table, from, i);
        if(cost > -1){
            int cont = 0;
            for(int j = 0; j < visited_len; ++j){
                if(visited[j] == i){
                    cont = 1;
                    break;
                }
            }
            if(cont == 1){
                continue;
            }
            current_way_cost += cost;
            int temp = from;
            from = i;
            find_way_using_Dijkstra(way_table, k);
            from = temp;
            current_way_cost -= cost;
        }
    }
    --visited_len;

    if(should_all_be_reset){
        free(visited);
        return way - 2;
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

    int k, n, m;
    fscanf(file_in, "%d %d %d", &k, &n, &m);

    WayTable* way_table = WayTable_new(n);

    for(int i = 0; i < m; ++i){
        int from, to, time;
        fscanf(file_in, "%d %d %d", &from, &to, &time);
        WayTable_put(way_table, from, to, time);
    }

    int* res = find_way_using_Dijkstra(way_table, k);
    WayTable_destruct(way_table);
    int way_len = res[0];
    fprintf(file_out, "%d\n", res[1]);
    res += 2;
    for(int i = 0; i < way_len; ++i){
        fprintf(file_out, "%d ", res[i]);
    }

    free(res - 2);

    return 0;
}