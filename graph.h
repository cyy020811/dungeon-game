/*
graph.h

Visible structs and functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/

/* Definition of a graph. */
struct graph;

enum problemPart;

struct solution;

/* A particular solution to a graph problem. */
#ifndef SOLUTION_STRUCT
#define SOLUTION_STRUCT
struct solution {
  int heartsLost;
};
#endif

/* Which part the program should find a solution for. */
#ifndef PART_ENUM
#define PART_ENUM
enum problemPart {
  PART_A = 0,
  PART_B = 1,
  PART_C = 2
};
#endif

/* Creates an undirected graph with the given numVertices and no edges and
returns a pointer to it. NumEdges is the number of expected edges. */
struct graph* newGraph(int numVertices);

/* Adds an edge to the given graph. */
void addEdge(struct graph* g, int start, int end, int cost);

/*  Uses Dijkstra's Algorithm to find the shortest path */
int dijkstra(struct graph* g, int bossRoom, int* heartRooms, int numHeartRooms);

/* Find the number of hearts which will be left if Lonk takes the optimal path.
  In all parts the graph will be the graph formed by regular rooms in the
  dungeon.
  numRooms is the number of rooms in the dungeon.
  Lonk starts in the startingRoom and the boss is in the bossRoom.
  For PART_B, the numShortcuts denotes the number of shortcuts,
    shortcutStarts represents the room on one side of each shortcut which
    Lonk's key could be used to open, and shortuctEnds represents the other side.
  For PART_A and PART_C, numShortcuts will be 0, shortcutStarts and shortcutEnds
    will be NULL.
  For PART_C, numHeartRooms denotes the number of heart rooms, heartRooms is the
    list of rooms which contain hearts.
  For PART_A and PART_B, numHeartRooms will be 0 and heartRooms will be NULL.
 */
struct solution* graphSolve(struct graph* g, enum problemPart part,
  int numRooms, int startingRoom, int bossRoom, int numShortcuts,
  int* shortcutStarts, int* shortcutEnds, int numHeartRooms, int* heartRooms);

/* Returns a new graph which is a deep copy of the given graph (which must be
  freed with freeGraph when no longer used). */
struct graph* duplicateGraph(struct graph* g);

/* Frees all memory used by graph. */
void freeGraph(struct graph* g);

/* Frees all data used by solution. */
void freeSolution(struct solution* solution);



