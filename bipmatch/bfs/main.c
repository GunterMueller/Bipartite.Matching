#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "timer.h"
#include "graph.h"
#include "io.h"
#include "fp.h"

/*
#define DBG
*/
double  Runtime;
int Npaths;

/* forward declarations */
extern void Match();
extern Boolean CheckMatching();
extern void GreedyMatch();

main(argc, argv)
int argc;
char *argv[];
{
  FILE *f1, *f2;
  U_partition *U;
  V_partition *V;

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

  U = (U_partition *) Alloc(sizeof(U_partition));
  V = (V_partition *) Alloc(sizeof(V_partition));
  InputBipartiteGraph(f1, U, V);

  BeginTiming();
  Match(U, V);
  Runtime = EndTiming()/100.0;

  matchingSize = MeasureMatching(U, V);

  if (output){
    OutputMatching(f2, U, V);
    fclose(f2);
  }

  CheckMatching(U, V);

  printf("eq=%d eex=%d opcount(th) %.2f\n", eq, eex, (eq+eex)/1000.0);
  printf("n=%d, m=%d, size=%d (%.1f%%), run time %.2f secs\n",
	 U->size, U->edges, matchingSize, 
	 (float) 100*matchingSize/U->size , Runtime);
}

void Match(U, V)
U_partition *U;
V_partition *V;
{

  GreedyMatch(U, V);
  Inits(U,V);
  FindPaths(U, V);
}

int MeasureMatching(U, V)
U_partition *U;
V_partition *V;
{
  int size = 0, u;

  for (u = 1; u <= U->size; u++) if (U->A[u].mate) size++;
  return size;
}

Boolean CheckMatching(U, V)
U_partition *U;
V_partition *V;
{

  int u, v;
  Edge *e, *current[MAX_N];
  int stack[MAX_N];
  int sp, base;
  Boolean ok = TRUE;
  Boolean visited[MAX_N];

  /* check for consistency */

  for (u = 1; u <= U->size; u++)
    {
      v = U->A[u].mate;
      if (v)
	{
	  if (V->A[v].mate != u)
	    {
	      printf("Inconsistent matching between %d(U) and %d(V)\n",
					u, v);
	      ok = FALSE;
	    }
	}
    }
  for (v = 1; v <= V->size; v++)
    {
      u = V->A[v].mate;
      if (u)
	{
	  if (U->A[u].mate != v)
	    {
	      printf("Inconsistent matching between %d(V) and %d(U)\n",
					v, u);
	      ok = FALSE;
	    }
	}
    }

  if (!ok) return ok;

  /* check for maximality */

  sp = -1;
  for (u = 1; u <= U->size; u++)
    {
      if (!U->A[u].mate) stack[++sp] = u;
      current[u] = U->A[u].firstEdge;
    }
  for (v = 1; v <= V->size; v++) visited[v] = FALSE;

  if (sp == -1) return TRUE;
  base = sp - 1;
  u = stack[sp];
  while (1)
    {
      e = current[u];
      while (e != (Edge *) 0 && visited[e->h]) e = e->next;
      current[u] = e;
      if (e == (Edge *) 0)
	{
	  /* stack empty? */
	  if (sp == 0) break;
	  u = stack[--sp];
	  /* are we finished with the current search? */
	  if (sp == base) base--; 
	  else current[u] = current[u]->next;
	}
      else
	{
	  /* is v in V matched? */
	  if (!V->A[e->h].mate)
	    {
	      /* no; augmenting path found */
	      printf("matching NOT maximum; augm. path:\n");
	      while(base < sp)
		{
		  printf("%d->",stack[++base]);
		}
	      printf("\n");
	      return FALSE;
	    }

	  else /* yes */
	    { 
	      stack[++sp] = u = V->A[e->h].mate;
	      visited[e->h] = TRUE;
	    }
	}
    }

  return TRUE;

}

void GreedyMatch(U, V)
U_partition *U;
V_partition *V;
{
  int u;
  Edge *e;
  int count = 0;

  for (u = 1; u <= U->size; u++)
    {
      e = U->A[u].firstEdge;
      while (e != (Edge *) 0)
	{
	  if (!V->A[e->h].mate)
	    {
	      V->A[e->h].mate = u;
	      U->A[u].mate = e->h;
#ifdef DBG
	      printf("%d matched to %d\n", u, e->h);
#endif
	      count++;
	      break;
	    }
	  e = e->next;
	}
    }
  printf("%d vertices in U were initially matched (%.1f%%).\n",
	 count, (float) 100*count/U->size);
}
