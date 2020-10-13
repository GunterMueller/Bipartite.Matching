/* main.c */
#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "bgraph.h"
#include "io.h"

/* forward declarations */
extern int CheckMatching();
extern void InitialMatch();
extern void Solve();

main(argc, argv)
int argc;
char *argv[];
{
  FILE *f1, *f2;
  Upartition *U;
  Vpartition *V;

  Boolean output = FALSE;
  int matchingSize;

  if (argc < 1 || argc > 2)
    Barf("Usage: match <input [<output file>]");

  f1 = stdin;

  if (argc == 2){
    output = TRUE;
    if ((f2 = fopen(argv[1], "w")) == NULL)
      Barf("Could not open output file");
  }
  else
    output = FALSE;

  U = (Upartition *) Alloc(sizeof(Upartition));
  V = (Vpartition *) Alloc(sizeof(Vpartition));
  InputBipartiteGraph(f1, U, V);
/*
  OutputBipartiteGraph(stdout, U, V);
*/

  Solve(U, V);
  if (output){
    OutputMatching(f2, U, V);
    fclose(f2);
  }

  matchingSize = CheckMatching(U, V);

  printf("nu=%d, nv=%d, m=%d, size=%d (%.1f%%)\n",
	 U->card, V->card, U->nEdges, matchingSize, 
	 (float) 100*matchingSize/U->card);
}

void Solve(U, V)
Upartition *U;
Vpartition *V;
{
  int card;

  InitialMatch(U, V);
  card = Match(U, V);
}

int CheckMatching(U, V)
Upartition *U;
Vpartition *V;
{

  Vertex u, u2, v;
  Edge *e;
  Vertex count, rightCount;
  Vertex q[MAX_CARD];
  char visited[MAX_CARD];
  int matchingSize = 0;
  int h, t;

  /* test for consistency on U partition */
  for (u = 1; u <= U->card; u++)
    {
      v = U->A[u].matchedTo;
      if (v)
	{
	  matchingSize++;
	  if (V->A[v].current->other != u)
	    {
	      printf("ERROR: matching inconsistent: u%d matched to v%d but v%d matched to u%d\n",
		     u, v, v, V->A[v].current->other);
	      exit(1);
	      /* continue; */
	    }
	}
    }

  /* test for consistency on V partition */
  for (v = 1; v <= V->card; v++)
    {
      if (V->A[v].current != (Edge *) 0)
	{
	  u = V->A[v].current->other;
	  if (U->A[u].matchedTo != v)
	    {
	      printf("ERROR: matching inconsistent: v%d matched to u%d but u%d matched to v%d\n",
		     v, u, u, U->A[u].matchedTo);
	      exit(1);
	      /* continue; */
	    }
	}
    }

  /* augmenting path search */

  /* put all unmatched u vertices in queue and try to reach an unmatched
    v vertex by fwd bfs. */

  h = 0;
  t = -1;
  for (u = 1; u <= U->card; u++)
    {
      if (!U->A[u].matchedTo) 
	{
	  q[++t] = u;
	  visited[u] = 1;
	}
    }

  while (h <= t)
    {
      u = q[h++];
      e = U->A[u].e;
      while (e != (Edge *) 0)
	{
	  if (!e->matched && V->A[e->other].current == (Edge *) 0)
	    {
	      printf("ERROR: matching not maximum.\n");
	      exit(1);
	    }
	  if (U->A[u].matchedTo != e->other &&
	      !visited[V->A[e->other].current->other])
	    {
	      u2 = V->A[e->other].current->other;
	      visited[u2] = TRUE;
	      q[++t] = u2;
	    }
	  e = e->next;
	}
    }
  return matchingSize;
}

void InitialMatch(U, V)
Upartition *U;
Vpartition *V;
{
  Vertex u, v;
  Edge *e;
  int count = 0;
  Boolean matched;

  /* we initialize structure fields and do a greedy match */

  /* init */

  for (v = 1; v <= V->card; v++)
    {
      V->A[v].current = (Edge *) 0;
    }

  for (u = 1; u <= U->card; u++)
    {
      e = U->A[u].e;
      matched = FALSE;
      while (e != (Edge *) 0)
	{
	  if (V->A[e->other].current == (Edge *) 0)
	    {
	      e->matched = e->mate->matched = TRUE;
	      V->A[e->other].current = e->mate;
	      U->A[u].matchedTo = e->other;
	      count++;
	      matched = TRUE;
	      break;
	    }
	  e = e->next;
	}
    }
  printf("%d vertices in U were initially matched (%.1f%%).\n",
	 count, (float) 100*count/U->card);

}
