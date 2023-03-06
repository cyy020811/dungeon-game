/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32

struct edge;

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge** edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
  int cost;
};

struct graph* newGraph(int numVertices) {
  struct graph* g = (struct graph*)malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph* g, int start, int end, int cost) {
  assert(g);
  struct edge* newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if ((g->numEdges + 1) > g->allocedEdges) {
    if (g->allocedEdges == 0) {
      g->allocedEdges = INITIALEDGES;
    }
    else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge**)realloc(g->edgeList,
      sizeof(struct edge*) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge*)malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;
  newEdge->cost = cost;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Returns a new graph which is a deep copy of the given graph (which must be
  freed with freeGraph when no longer used). */
struct graph* duplicateGraph(struct graph* g) {
  struct graph* copyGraph = (struct graph*)malloc(sizeof(struct graph));
  assert(copyGraph);
  copyGraph->numVertices = g->numVertices;
  copyGraph->numEdges = g->numEdges;
  copyGraph->allocedEdges = g->allocedEdges;
  copyGraph->edgeList = (struct edge**)malloc(sizeof(struct edge*) * g->allocedEdges);
  assert(copyGraph->edgeList || copyGraph->numEdges == 0);
  int i;
  /* Copy edge list. */
  for (i = 0; i < g->numEdges; i++) {
    struct edge* newEdge = (struct edge*)malloc(sizeof(struct edge));
    assert(newEdge);
    newEdge->start = (g->edgeList)[i]->start;
    newEdge->end = (g->edgeList)[i]->end;
    newEdge->cost = (g->edgeList)[i]->cost;
    (copyGraph->edgeList)[i] = newEdge;
  }
  return copyGraph;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph* g) {
  int i;
  for (i = 0; i < g->numEdges; i++) {
    free((g->edgeList)[i]);
  }
  if (g->edgeList) {
    free(g->edgeList);
  }
  free(g);
}

/*  Uses Dijkstra's Algorithm to find the shortest path*/
int dijkstra(struct graph* g, int bossRoom, int* heartRooms, int numHeartRooms) {
  int dist[g->numVertices];
  int prev[g->numVertices];
  int currVert;
  struct pq* pqueue = newPQ();
  /* Change the edge cost to zero if the room is a heartroom */
  for (int i = 0; i < numHeartRooms; i++) {
    for (int j = 0; j < g->numEdges; j++) {
      if ((g->edgeList)[j]->start == heartRooms[i]) (g->edgeList)[j]->cost = 0;
    }
  }
  /* Dijkstra's Algorithm implementation */
  for (int i = 0; i < g->numVertices; i++) {
    if (i == 0) dist[i] = 0;
    else dist[i] = INT_MAX;
    enqueue(pqueue, (void*)(size_t)i, dist[i]);
  }
  while (!empty(pqueue)) {
    currVert = (int)(size_t)deletemin(pqueue);
    for (int i = 0; i < g->numEdges; i++) {
      if ((g->edgeList)[i]->start == currVert) {
        if (elementOf(pqueue, (void*)(size_t)(g->edgeList)[i]->end)
          && dist[currVert] + (g->edgeList)[i]->cost < dist[(g->edgeList)[i]->end]) {

          dist[(g->edgeList)[i]->end] = dist[currVert] + (g->edgeList)[i]->cost;
          prev[(g->edgeList)[i]->end] = currVert;
          update(pqueue, (void*)(size_t)(g->edgeList)[i]->end, dist[(g->edgeList)[i]->end]);
        }
      }
      else if ((g->edgeList)[i]->end == currVert) {
        if (elementOf(pqueue, (void*)(size_t)(g->edgeList)[i]->start)
          && dist[currVert] + (g->edgeList)[i]->cost < dist[(g->edgeList)[i]->start]) {

          dist[(g->edgeList)[i]->start] = dist[currVert] + (g->edgeList)[i]->cost;
          prev[(g->edgeList)[i]->start] = currVert;
          update(pqueue, (void*)(size_t)(g->edgeList)[i]->start, dist[(g->edgeList)[i]->start]);
        }
      }
    }
  }
  free(pqueue);
  return dist[bossRoom];
}

struct solution* graphSolve(struct graph* g, enum problemPart part,
  int numRooms, int startingRoom, int bossRoom, int numShortcuts,
  int* shortcutStarts, int* shortcutEnds, int numHeartRooms, int* heartRooms) {
  struct solution* solution = (struct solution*)
    malloc(sizeof(struct solution));
  assert(solution);
  if (part == PART_A) {
    /* IMPLEMENT 2A SOLUTION HERE */
    solution->heartsLost = dijkstra(g, bossRoom, heartRooms, numHeartRooms);
  }
  else if (part == PART_B) {
    /* IMPLEMENT 2B SOLUTION HERE */
    int dist = INT_MAX;
    /* Finds the shortest path w/o different shortcuts */
    for (int i = 0; i < numShortcuts + 1; i++) {
      int currDist;
      if (i == numShortcuts) currDist = dijkstra(g, bossRoom, heartRooms, numHeartRooms);
      else {
        /* Adds one shortcut to the graph since Lonk only has one key*/
        struct graph* shortcutGraph = duplicateGraph(g);
        addEdge(shortcutGraph, shortcutStarts[i], shortcutEnds[i], 1);
        currDist = dijkstra(shortcutGraph, bossRoom, heartRooms, numHeartRooms);
        freeGraph(shortcutGraph);
      }
      if (currDist < dist) dist = currDist;
    }
    solution->heartsLost = dist;
  }
  else {
    /* IMPLEMENT 2C SOLUTION HERE */
    solution->heartsLost = dijkstra(g, bossRoom, heartRooms, numHeartRooms);
  }
  return solution;
}