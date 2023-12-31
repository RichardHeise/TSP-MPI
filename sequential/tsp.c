/* WSCAD - 9th Marathon of Parallel Programming 
 * Simple Brute Force Algorithm for the 
 * Traveling-Salesman Problem
 * Author: Emilio Francesquini - francesquini@ic.unicamp.br
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

int min_distance;
int nb_towns;

typedef struct {
    int to_town;
    int dist;
} d_info;

d_info **d_matrix;
int *dist_to_origin;
int *paths;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1e6;
}

void tsp (int depth, int current_length, int *path) {
    int i;
    if (current_length >= min_distance) return;
    if (depth == nb_towns) {
        current_length += dist_to_origin[path[nb_towns - 1]];
        if (current_length < min_distance)
            min_distance = current_length;
    } else {
        int town, me, dist;
        me = path[depth - 1];
        for (i = 0; i < nb_towns; i++) {
            town = d_matrix[me][i].to_town;
            if (!paths[town]) {
                path[depth] = town;
                paths[town] = 1;
                dist = d_matrix[me][i].dist;
                tsp (depth + 1, current_length + dist, path);
                paths[town] = 0;
            }
        }
    }
}

void greedy_shortest_first_heuristic(int *x, int *y) {
    int i, j, k, dist;
    int *tempdist;

    tempdist = (int*) malloc(sizeof(int) * nb_towns);
    //Could be faster, albeit not as didactic.
    //Anyway, for tractable sizes of the problem it
    //runs almost instantaneously.
    for (i = 0; i < nb_towns; i++) {
        for (j = 0; j < nb_towns; j++) {
            int dx = x[i] - x[j];
            int dy = y[i] - y[j];
            tempdist [j] = dx * dx + dy * dy;
        }
        for (j = 0; j < nb_towns; j++) {
            int tmp = INT_MAX;
            int town = 0;
            for (k = 0; k < nb_towns; k++) {
                if (tempdist [k] < tmp) {
                    tmp = tempdist [k];
                    town = k;
                }
            }
            tempdist [town] = INT_MAX;
            d_matrix[i][j].to_town = town;
            dist = (int) sqrt (tmp);
            d_matrix[i][j].dist = dist;
            if (i == 0)
                dist_to_origin[town] = dist;
        }
    }

    free(tempdist);
}

void init_tsp() {
    int i, st;
    int *x, *y;

    min_distance = INT_MAX;

    st = scanf("%u", &nb_towns);
    if (st != 1) exit(1);
 
    d_matrix = (d_info**) malloc (sizeof(d_info*) * nb_towns);
    for (i = 0; i < nb_towns; i++)
        d_matrix[i] = (d_info*) malloc (sizeof(d_info) * nb_towns);
    dist_to_origin = (int*) malloc(sizeof(int) * nb_towns);
   
    x = (int*) malloc(sizeof(int) * nb_towns);
    y = (int*) malloc(sizeof(int) * nb_towns);
    

    for (i = 0; i < nb_towns; i++) {
        st = scanf("%u %u", x + i, y + i);
        if (st != 2) exit(1);
    }
    
    greedy_shortest_first_heuristic(x, y);
    
    free(x);
    free(y);
}

int run_tsp() {
    int i, *path;

    init_tsp();
    
    path = (int*) malloc(sizeof(int) * nb_towns);
    paths = calloc(sizeof(int), nb_towns);
    path[0] = 0;
    paths[0] = 1;
    
    double start_time, end_time;
    start_time = get_time(); // Início da medição de tempo
    tsp (1, 0, path);
    end_time = get_time(); // Término da medição de tempo
    printf("paralelizavel: %f segundos\n", end_time - start_time);

    free(path);
    free(paths);
    for (i = 0; i < nb_towns; i++)
        free(d_matrix[i]);
    free(d_matrix);

    return min_distance;
}

int main (int argc, char **argv) {
    int num_instances, st;
    st = scanf("%u", &num_instances);
    if (st != 1) exit(1);
        while(num_instances-- > 0) {
            double start_time, end_time;
            start_time = get_time(); // Início da medição de tempo
            printf("%d\n", run_tsp());
            end_time = get_time(); // Término da medição de tempo
            printf("Tempo total: %f segundos\n", end_time - start_time);
        }
    return 0;
}
