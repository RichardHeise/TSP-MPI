#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

int min_distance;
int nb_towns;
int rank, procs;
int global_min_distance;

typedef struct {
    int to_town;
    int dist;
} d_info;

d_info **d_matrix;
int *dist_to_origin;

int *task_queue;
int task_queue_size;
MPI_Request steal_request;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1e6;
}

void tsp(int depth, int current_length, int *path, int *paths);

void greedy_shortest_first_heuristic(int *x, int *y);

void init_tsp();

int run_tsp();

void request_steal();

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    int num_instances, st;

    if (rank == 0) {
        st = scanf("%u", &num_instances);
        if (st != 1) exit(1);
    }

    MPI_Bcast(&num_instances, 1, MPI_INT, 0, MPI_COMM_WORLD);

    while (num_instances-- > 0) {
        double start_time, end_time;
        if (rank == 0) {
            start_time = get_time();
        }

        int local_min_distance = run_tsp();
        MPI_Reduce(&local_min_distance, &global_min_distance, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            end_time = get_time();
            printf("%d\n", global_min_distance);
            printf("Tempo total: %.2f seconds\n", end_time - start_time);
        }
    }

    MPI_Finalize();
    return 0;
}

void tsp(int depth, int current_length, int *path, int *paths) {
    int i;
    if (current_length >= min_distance) return;
    if (depth == nb_towns) {
        current_length += dist_to_origin[path[nb_towns - 1]];
        if (current_length < min_distance) {
            min_distance = current_length;
        }
        return;
    }

    int town, me, dist;
    me = path[depth - 1];

    for (i = 0; i < nb_towns; i++) {
        town = d_matrix[me][i].to_town;
        if (!paths[town]) {
            path[depth] = town;
            paths[town] = 1;
            dist = d_matrix[me][i].dist;
            tsp(depth + 1, current_length + dist, path, paths);
            paths[town] = 0;
        }
    }
}

void greedy_shortest_first_heuristic(int *x, int *y) {
    int i, j, k, dist;
    int *tempdist;

    tempdist = (int *)malloc(sizeof(int) * nb_towns);

    for (i = 0; i < nb_towns; i++) {
        for (j = 0; j < nb_towns; j++) {
            int dx = x[i] - x[j];
            int dy = y[i] - y[j];
            tempdist[j] = dx * dx + dy * dy;
        }
        for (j = 0; j < nb_towns; j++) {
            int tmp = INT_MAX;
            int town = 0;
            for (k = 0; k < nb_towns; k++) {
                if (tempdist[k] < tmp) {
                    tmp = tempdist[k];
                    town = k;
                }
            }
            tempdist[town] = INT_MAX;
            d_matrix[i][j].to_town = town;
            dist = (int)sqrt(tmp);
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

    if (rank == 0) {
        st = scanf("%d", &nb_towns);
        if (st != 1) exit(1);
    }

    MPI_Bcast(&nb_towns, 1, MPI_INT, 0, MPI_COMM_WORLD);

    d_matrix = (d_info **)malloc(sizeof(d_info *) * nb_towns);
    for (i = 0; i < nb_towns; i++)
        d_matrix[i] = (d_info *)malloc(sizeof(d_info) * nb_towns);
    dist_to_origin = (int *)malloc(sizeof(int) * nb_towns);

    x = (int *)malloc(sizeof(int) * nb_towns);
    y = (int *)malloc(sizeof(int) * nb_towns);

    if (rank == 0) {
        for (i = 0; i < nb_towns; i++) {
            st = scanf("%d %d", x + i, y + i);
            if (st != 2) exit(1);
        }
    }

    // Broadcast x and y to all processes
    MPI_Bcast(x, nb_towns, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(y, nb_towns, MPI_INT, 0, MPI_COMM_WORLD);

    greedy_shortest_first_heuristic(x, y);

    free(x);
    free(y);
}

void request_steal() {
    int dest = (rank + 1) % procs;
    MPI_Isend(&rank, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &steal_request);
}

int run_tsp() {
    int *path, *paths;
    int *taskQ;
    int taskQ_size = 0;

    init_tsp();

    int chunk_size = (nb_towns - 1) / procs + 1;
    int start = rank * chunk_size + 1;
    int end = (rank + 1) * chunk_size;

    if (end > nb_towns)
        end = nb_towns;

    path = malloc(sizeof(int) * nb_towns);
    paths = calloc(sizeof(int), nb_towns);
    path[0] = 0;
    paths[0] = 1;

    taskQ = malloc(sizeof(int) * chunk_size);

    for (int i = start; i < end; i++) {
        taskQ[taskQ_size++] = i;
    }

    while (taskQ_size > 0) {
        int current_task = taskQ[--taskQ_size];
        path[1] = current_task;
        paths[current_task] = 1;
        tsp(2, dist_to_origin[current_task], path, paths);
        paths[current_task] = 0;

        if (taskQ_size == 0) {
            request_steal();
        }
    }

    free(taskQ);
    free(path);
    free(paths);

    return min_distance;
}
