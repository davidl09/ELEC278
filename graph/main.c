#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

#include "fifo.h"

typedef struct graph {
    bool **edges;
    int *vertices;
    size_t numVertices;
} graph;

graph* makeGraph(int numVertices) {
    graph* g = (graph*)malloc(sizeof(graph));

    g->numVertices = numVertices;
    g->edges = (bool **)calloc(numVertices, sizeof (bool *));
    g->vertices = calloc (numVertices, sizeof (int));

    for (int i = 0; i < numVertices; ++i) {
        g->edges[i] = calloc(numVertices, sizeof (bool));
    }

    return g;
}

void freeGraph(graph* g) {
    for (int i = 0; i < g->numVertices; ++i) {
        free(g->edges[i]);
        g->edges[i] = NULL;
    }
    free((void *)g->edges);
    g->edges = NULL;
    free(g->vertices);
    g->vertices = NULL;
    free(g);
}

void connect(graph* g, size_t from, size_t to) {
    if (from > g->numVertices || to > g->numVertices) return;
    g->edges[from][to] = true;
}

void disconnect(graph* g, size_t from, size_t to) {
    if (from > g->numVertices || to > g->numVertices) return;
    g->edges[from][to] = false;
}

void setVal(graph* g, size_t vertex, int value) {
    if (vertex > g->numVertices) return;
    g->vertices[vertex] = value;
}

void DFT_R(graph *g, int curr, bool visited[]) {
    printf("%c -> ", g->vertices[curr]);
    visited[curr] = true;
    for (int i = 0; i < g->numVertices; ++i) {
        if (g->edges[curr][i] && !visited[i]) {
            DFT_R(g, i, visited);
        }
    }
}

void DFT(graph *g, int start) {
    bool *visited = calloc(g->numVertices, sizeof (bool));
    DFT_R(g, start, visited);
    free(visited);
}

DEFINE_FIFO(int)

int BFT(graph *g, int target, int start) {
    fifo_int q = fifo_int_make(2 * g->numVertices);
    bool *visited = calloc(g->numVertices, sizeof (bool));
    fifo_int_push_back(&q, g->vertices[start]);
    while (fifo_int_size(&q)) {
        int v = fifo_int_pop_front(&q);
        if (v == target) {
            free(visited);
            fifo_int_free(&q);
            return v;
        }
    }


    free (visited);
}

void printGraph(const graph* g) {
    printf("   ");
    for (int i = 0; i < g->numVertices; ++i) {
        printf("%c ", i + 'A');
        g->vertices[i] = i + 'A';
    }
    putchar('\n');
    for (int i = 0; i < g->numVertices; ++i) {
        printf("%c: ", i + 'A');
        for (int j = 0; j < g->numVertices; ++j) {
            printf("%d ", g->edges[i][j]);
        }
        putchar('\n');
    }
}



int main() {
    graph* g = makeGraph(6);
    for (int i = 'A'; i <= 'F'; ++i) {
        setVal(g, i - 'A', i);
    }
    for (int i = 0; i < 2; ++i) {
        connect(g, i, i + 1);
    }
    connect(g, 'C' - 'A', 'E' - 'A');
    connect(g, 'E' - 'A', 'F' - 'A');
    connect(g, 'E' - 'A', 'D' - 'A');
    connect(g, 'D' - 'A', 'B' - 'A');
    BFT(g, 0);
    freeGraph(g);
    return 0;
}
