#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "timer.h"
#include "bgraph.h"
#include "io.h"
#include "match.h"

#define DBG

double  Runtime;

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

  BeginTiming();
  Solve(U, V);
  Runtime = EndTiming()/100.0;

  if (output){
    OutputMatching(f2, U, V);
    fclose(f2);
  }

  matchingSize = CheckMatching(U, V);

  printf("nu=%d, nv=%d, m=%d, size=%d (%.1f%%), run time %.2f secs\n",
	 U->card, V->card, U->nEdges, matchingSize, 
	 (float) 100*matchingSize/U->card ,Runtime);
}

void Solve(U, V)
Upartition *U;
Vpartition *V;
{

  InitialMatch(U, V);
  Match(U, V);
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
	  if (!V->A[v].matched)
	    {
	      printf("ERROR: %d(u) is matched to %d(v), but %d is umatched\n",
					u, v, v);
	      exit(1);
	      /* continue; */
	    }
	  e = V->A[v].e;
	  count = 0;
	  while (e != (Edge *) 0)
	    {
	      if (e->matched) count++;
	      e = e->next;
	    }
	  if (count != 1)
	    {
	      printf("ERROR: %d(u) and %d(v) are matched, but edge count = %d\n",
		     u, v, count);
	      printf("u.label=%d, v.label=%d v.excess=%d\n",
		     U->A[u].label, V->A[v].label, V->A[v].excess);
	      exit(1);
	    }
	  V->A[v].matched = u; /* important for bfs checking below */
	}
    }

  /* test for consistency on V partition */
  for (v = 1; v <= V->card; v++)
    {
      if (V->A[v].matched) rightCount = 1;
      else rightCount = 0;
      e = V->A[v].e;
      count = 0;
      while (e != (Edge *) 0)
	{
	  if (e->matched) count++;
	  e = e->next;
	}
      if (count != rightCount)
	{
	  printf("ERROR: %d(v): rightCount = %d, count = %d\n",
		 v, rightCount, count);
	  exit(1);
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
	  if (!e->matched && !V->A[e->other].matched)
	    {
	      printf("ERROR: matching not maximum.\n");
	      printf("msize=%d\n", matchingSize);
	      exit(1);
	    }
	  if (!e->matched && !visited[V->A[e->other].matched]) /* here we use
				       the assgmt. to v.matched made above */
	    {
	      u2 = V->A[e->other].matched;
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

#ifdef CHECK
  printf("Initializing structure fields...\n");
#endif

  /* init */
  for (v = 1; v <= V->card; v++)
    {
      V->A[v].current = V->A[v].e;
      V->A[v].matched = FALSE;
      V->A[v].excess = 0;
    }

#ifdef CHECK
  printf("greedy matching...\n");
#endif

  for (u = 1; u <= U->card; u++)
    {
      U->A[u].current = U->A[u].e;
      /* LabelU[u] = 2; */
      e = U->A[u].e;
      matched = FALSE;
      while (e != (Edge *) 0)
	{
	  if (!V->A[e->other].matched)
	    {
	      V->A[e->other].matched = TRUE;
	      U->A[u].matchedTo = e->other;
	      e->mate->matched = TRUE;
	      count++;
/*
	      printf("%d matched to %d\n", u, e->other);
*/
	      matched = TRUE;
	      break;
	    }
	  e = e->next;
	}
      if (!matched && U->A[u].e != (Edge *) 0)
	{
	  e = U->A[u].e;
	  V->A[e->other].matched = TRUE;
	  U->A[u].matchedTo = e->other;
	  e->mate->matched = TRUE;
	  V->A[e->other].excess++;
	}
    }
  printf("%d vertices in V were initially matched (%.1f%%).\n",
	 count, (float) 100*count/V->card);

}
