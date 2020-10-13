#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "graph.h"
#include "timer.h"
#include "fp.h"
#include <prof.h>

int eq, eex;

int iUnmatched;

struct {
  int u;       /* vertex */
  int i_parent; /* index in queue of parent vertex in tree of u */
  int v;      /* current mate of u */
} queue[MAX_N];

Boolean visited[MAX_N];

void Inits(U,V)
U_partition *U;
V_partition *V;
{

  int u;

  eq = eex = 0;

  iUnmatched = MAX_N;
  for (u = 1; u <= U->size; u++)
    {
      U->A[u].nextEdge = U->A[u].firstEdge;
      visited[u] = FALSE;
      /* we put all unmatched vertices from U at the other end of the queue */
      if (!U->A[u].mate)
	{
	  queue[--iUnmatched].u = u;
#ifdef DBG
	  printf("%d is unmatched\n", u);
#endif
	}
    }
}

void UpdatePath(U, V, i1, i2)
U_partition *U;
V_partition *V;
int i1, i2;
{
  int u, v;

  while (i2 >= 0)
    {
      u = queue[i2].u;
      v = queue[i1].v;
      V->A[v].mate = u;
      U->A[u].mate = v;
      i1 = i2;
      i2 = queue[i2].i_parent;
      eex++;
    }
}

void FindPaths(U, V)
U_partition *U;
V_partition *V;
{
  /* finds augmenting paths by breadth-first search. */

  int tail;
  int head;
  int u, i;
  Edge *e, *e2;

  /* external loop gets unmatched u vertices from far end of array queue */
  while (iUnmatched < MAX_N)
    {
      head = tail = 0;
      queue[head].u = queue[iUnmatched++].u;
      queue[head].i_parent = -1; /* root of BFS tree */
#ifdef DBG
      printf("unmatched u%d will be processed\n", queue[head].u);
#endif
      /* queue processing */
      while (tail >= head)
	{
	  u = queue[head++].u;
	  e = U->A[u].firstEdge;
	  eq++;
	  /* adjacency list scanning */
	  while (e != (Edge *) 0)
	    {
	      if (!visited[V->A[e->h].mate])
		{
		  /* extend tree */
		  queue[++tail].u = u = V->A[e->h].mate;
		  queue[tail].i_parent = head-1;
		  queue[tail].v = e->h;
		  visited[u] = TRUE;
		  /* look-ahead heuristic */
		  e2 = U->A[u].nextEdge;
		  eq++;
		  while (e2 != (Edge *) 0 && V->A[e2->h].mate)
		    {
		      e2 = e2->next;
		      eq++;
		    }
		  U->A[u].nextEdge = e2;
		  if (e2 != (Edge *) 0)
		    {
#ifdef DBG
		      printf("augmenting path found\n");
#endif
		      /* u in the queue but not the edge to v */
		      V->A[e2->h].mate = u;
		      U->A[u].mate = e2->h;
		      /* now for the rest of the path */
		      UpdatePath(U, V, tail, head-1);
		      /* temporary cut is emptied */
		      for (i = 0; i <= tail; i++) 
			{
			  visited[queue[i].u] = FALSE;
			}
		      tail = head-1; /* to get off from queue loop */
		      break; /* to get off from adj list scan loop */
		    }
		}
	      e = e->next;
	      eq++;
	    }
	}
    }
}


PrintStatus(U,V)
U_partition *U;
V_partition *V;

{
  int u, v;

  printf("printing status...\n");
/*
  for (u = 1; u <= U->size; u++)
    {
      if (current[u] == (Edge *) 0) printf("u%d nil\n", u);
      else printf("u%d %d\n", u, current[u]->h);
    }
*/

  for (v = 1; v <= V->size; v++)
    {
      printf("v%d %d\n", v, V->A[v].mate);
    }
/*  exit(1); */
}





























