/*
utils.c

Implementations for helper functions to do with reading and writing.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "utils.h"

/* Default cost for edges. */
#define DEFAULTCOST 1

struct graphProblem {
  int numRooms;
  int numConnections;
  int startRoom;
  int bossRoom;
  int numShortcuts;
  int *shortcutStart;
  int *shortcutEnd;
  struct graph *graph;
  int numHeartRooms;
  int *heartRooms;
};

struct graphProblem *readProblem(FILE *file, enum problemPart part){
  int i;
  int startRoom;
  int endRoom;
  /* Allocate space for problem specification */
  struct graphProblem *problem = (struct graphProblem *)
    malloc(sizeof(struct graphProblem));
  assert(problem);

  /* First line of input is the number of rooms. */
  assert(fscanf(file, "%d", &(problem->numRooms)) == 1);
  /* Next line comprises number of connections between rooms. */
  assert(fscanf(file, "%d", &(problem->numConnections)) == 1);
  /* Next line comprises the start room. */
  assert(fscanf(file, "%d", &(problem->startRoom)) == 1);
  /* Next line comprises the boss room. */
  assert(fscanf(file, "%d", &(problem->bossRoom)) == 1);

  /* Build graph number of rooms. */
  problem->graph = newGraph(problem->numRooms);
  /* Add all edges to graph. */
  for(i = 0; i < problem->numConnections; i++){
    assert(fscanf(file, "%d %d", &startRoom, &endRoom) == 2);
    addEdge(problem->graph, startRoom, endRoom, DEFAULTCOST);
  }

  /* Handle PART_B */
  if(part == PART_B){
    /* Read number of shortcuts */
    assert(fscanf(file, "%d", &(problem->numShortcuts)) == 1);
    problem->shortcutStart = (int *) malloc(sizeof(int)* problem->numShortcuts);
    assert(problem->shortcutStart || problem->numShortcuts == 0);
    problem->shortcutEnd = (int *) malloc(sizeof(int)* problem->numShortcuts);
    assert(problem->shortcutEnd || problem->numShortcuts == 0);
    for(i = 0; i < problem->numShortcuts; i++){
      /* Read each shortcut connection. */
      assert(fscanf(file, "%d %d", &startRoom, &endRoom) == 2);
      (problem->shortcutStart)[i] = startRoom;
      (problem->shortcutEnd)[i] = endRoom;
    }
  } else {
    problem->shortcutStart = NULL;
    problem->shortcutEnd = NULL;
    problem->numShortcuts = 0;
  }

  /* Handle PART_C */
  if(part == PART_C){
    /* Read number of heart rooms */
    assert(fscanf(file, "%d", &(problem->numHeartRooms)) == 1);
    problem->heartRooms = (int *) malloc(sizeof(int)* problem->numHeartRooms);
    assert(problem->heartRooms || problem->numHeartRooms == 0);
    for(i = 0; i < problem->numHeartRooms; i++){
      /* Read each heart room. */
      assert(fscanf(file, "%d", &(problem->heartRooms[i])) == 1);
    }
  } else {
    problem->numHeartRooms = 0;
    problem->heartRooms = NULL;
  }

  return problem;
}

struct solution *findSolution(struct graphProblem *problem,
  enum problemPart part){
  return graphSolve(problem->graph, part, problem->numRooms,
    problem->startRoom, problem->bossRoom, problem->numShortcuts,
    problem->shortcutStart, problem->shortcutEnd, problem->numHeartRooms,
    problem->heartRooms);
}

void freeProblem(struct graphProblem *problem){
  /* No need to free if no data allocated. */
  if(! problem){
    return;
  }
  freeGraph(problem->graph);
  if(problem->shortcutStart){
    free(problem->shortcutStart);
  }
  if(problem->shortcutEnd){
    free(problem->shortcutEnd);
  }
  if(problem->heartRooms){
    free(problem->heartRooms);
  }

  free(problem);
}

void freeSolution(struct solution *solution){
  /* No need to free if no data allocated. */
  if(! solution){
    return;
  }
  free(solution);
}
