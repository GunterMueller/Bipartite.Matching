/* bgraph.h */

#define MAX_CARD 65600 /* max number of vertices in one partition */

typedef int Vertex;

typedef struct enode {
  struct enode *next;
  struct enode *mate;
  Vertex other;
  Boolean matched;
} Edge;

typedef struct {
  Edge *e, *current;
  int label;
} Vvertex;

typedef struct {
  Edge *e, *current;
  Vertex matchedTo;
  int label;
} Uvertex;

typedef struct {
  Uvertex A[MAX_CARD];
  Vertex card;
  int nEdges;
} Upartition;

typedef struct {
  Vvertex A[MAX_CARD];
  Vertex card;
} Vpartition;



