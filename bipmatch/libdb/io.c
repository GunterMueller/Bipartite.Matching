/* io.c */

#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "graph.h"
#include "manip.h"
#include "io.h"

/* File Format

modified DIMACS format (adapted to bipartite graphs)

c <Comment lines>
p bipartite-matching <vertices in U> <vertices in V> <edges>
a <src> <dst>  -- edge

* * * nodes in BOTH partitions in range 1 . . n * * *

Solution graphs
c <Comment lines>
s <matching size>
f <src> <dst> (matched edges)

*/

void InputBipartiteGraph(f, U, V)
FILE *f;
U_partition *U;
V_partition *V;
{
  char c, c1, ReadChar(), buff[100];

  int i, verticesInU, verticesInV, edges, u, v;

  Init_U(U);
  Init_V(V);
  
  while (1) {
    if (EOF_Test(f))
      break;
    c = ReadChar(f);
    switch (c) {

    case 'a':
      u = GetInt(f);
      v = GetInt(f);
      AddEdge(u, v, U, V);
      break;

    case 'c':
      SkipLine(f);
      break;

    case 'p':
      GetString(f, buff);
      verticesInU = GetInt(f);
      verticesInV = GetInt(f);
      edges = GetInt(f);
      break;

    default:
      printf("first char in a line = %c\n",c);
      Barf("Unexpected case in InputBipartiteGraph\n");
      break;
    }
  }
  U->size = verticesInU;
  V->size = verticesInV;
  U->edges = edges;
}

void OutputBipartiteGraph(f, U, V)
FILE *f;
U_partition *U;
V_partition *V;
{
  int u;
  Edge *e;

  fprintf(f, "p bipartite-matching %d %d %d\n",
	  U->size, V->size, U->edges);
  for (u = 1; u <= U->size; u++)
    {
      e = U->A[u].firstEdge;
      while (e != (Edge *) 0)
	{
	  fprintf(f, "a %d %d\n", u, e->h);
	  e = e->next;
	}
    }
}

void OutputMatching(f, U, V)
FILE *f;
U_partition *U;
V_partition *V;
{
  int u;

  fprintf(f, "Bipartite Matching; Results for |U|=%d, |V|=%d, |E|=%d\n\n",
	  U->size, V->size, U->edges);
  for (u = 1; u <= U->size; u++)
    if (U->A[u].mate) fprintf(f, "%d matched to %d\n", u, U->A[u].mate);
}


