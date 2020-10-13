/* io.c */

#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "bgraph.h"
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
Upartition *U;
Vpartition *V;
{
  char c, c1, ReadChar(), buff[100];
  char finalLine = FALSE;

  int i = 0, verticesInU, verticesInV, edges, u, v;

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
#ifdef CHECK
      if ((++i) % 10000 == 0) printf("%d\n", i);
#endif
      break;

    case 'c':
      SkipLine(f);
      break;

    case 'p':
      GetString(f, buff);
      verticesInU = GetInt(f);
      verticesInV = GetInt(f);
      edges = GetInt(f);
      finalLine = TRUE;
      break;

    default:
      Barf("Unexpected case in InputBipartiteGraph\n");
      break;
    }


  }
  if (!finalLine) Barf("Final line in input file is missing.\n");
  U->card = verticesInU;
  V->card = verticesInV;
  U->nEdges = edges;
}

void OutputBipartiteGraph(f, U, V)
FILE *f;
Upartition *U;
Vpartition *V;
{
  int u;
  Edge *e;

  fprintf(f, "p bipartite-matching %d %d %d\n",
	  U->card, V->card, U->nEdges);
  for (u = 1; u <= U->card; u++)
    {
      e = U->A[u].e;
      while (e != (Edge *) 0)
	{
	  fprintf(f, "a %d %d\n", u, e->other);
	  e = e->next;
	}
    }
}

void OutputMatching(f, U, V)
FILE *f;
Upartition *U;
Vpartition *V;
{
  int u;

  fprintf(f, "Bipartite Matching; Results for |U|=%d, |V|=%d, |E|=%d\n\n",
	  U->card, V->card, U->nEdges);
  for (u = 1; u <= U->card; u++)
    if (U->A[u].matchedTo) fprintf(f, "%d matched to %d\n", u, U->A[u].matchedTo);
}


