/* match.c */
#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "bgraph.h"
#include "manip.h"

int eq, eex; /* op counts: eq = edge queries, eex = edge exchanges */
Queue *Q;
int Nvertices;
int rootDeq;

/* implementation of Alt, Blum, Mehlhorn, and Paul algorithm */

int Match(U, V)
Upartition *U;
Vpartition *V;
{
  Vertex u, v, u2;
  Edge *e, *last_e;
  int i, j;
  Vertex path[2*MAX_CARD];
  int card = 0;
  int BFSthreshold, BFSlag;
  int DCount = 0, WCount = 0, RCount = 0; /* op counts */

  eq = eex = 0;
  Nvertices = U->card+V->card;
  Q = MakeQueue(Nvertices);
  BFSthreshold = BFSlag = V->card;
  rootDeq = 0;
  GlobalRelabel(U, V);
  while (!QueueEmpty(Q))
    {
      DCount++;
      u = Dequeue(Q);
      i = 0;
      path[0] = u;
      while (1)
	{
	  e = U->A[u].current;
	  eq++;
	  if (e != (Edge *) 0 && e->matched) { e = e->next; eq++; }
	  U->A[u].current = e;
	  if (e != (Edge *) 0)
	    {
	      v = e->other;
	      if (U->A[u].label == V->A[v].label + 1)
		{
		  if (V->A[v].label == 0 && V->A[v].current == (Edge *) 0)
		    {
		      /* breakthrough */
		      i++; card++;
		      path[i] = v;
		      U->A[path[0]].matchedTo = path[1];
		      /* first edge */
		      e = U->A[path[0]].current;
		      e->matched = TRUE;
		      e->mate->matched = TRUE;
		      for (j = 1; j < i; j += 2)
			{
			  last_e = e;
			  e = V->A[path[j]].current;
		          V->A[path[j]].current = last_e->mate;
			  e->matched = FALSE;
			  e->mate->matched = FALSE;
			  U->A[path[j+1]].matchedTo = path[j+2];
			  e = U->A[path[j+1]].current;
			  e->matched = TRUE;
			  e->mate->matched = TRUE;
			  eex++;
			}
		      V->A[path[j]].current = e->mate;
		      i = 0;
		      goto NextInQueue;
		    }
		  else
		    {
		      /* advance */
		      u2 = V->A[v].current->other;
		      if (V->A[v].label == U->A[u2].label + 1)
			{
			  path[++i] = v;
			  path[++i] = u2;
			  u = u2;
			  goto NextInPath;
			}
		      else
			{
			  /* must relabel v */
			  V->A[v].label += 2;
			  RCount++;
			  if (RCount > BFSthreshold)
			    {
			      GlobalRelabel(U, V);
			      BFSthreshold += BFSlag;
			      WCount++;
			      goto NextInQueue;
			    }
			}
		    }
		}
	      U->A[u].current = U->A[u].current->next;
	    }
	  else /* retreat */
	    {
	      U->A[u].label += 2;
	      RCount++;
	      U->A[u].current = U->A[u].e;
	      if (i == 0) /* back to the first vertex in the path */
		{
		  if (U->A[u].label < Nvertices) Enqueue(Q, u);
		  else rootDeq++;
		  break;
		}
	      else
		{
		  V->A[path[i-1]].label += 2;
		  RCount++;
		  if (RCount > BFSthreshold)
		    {
		      GlobalRelabel(U, V);
		      BFSthreshold += BFSlag;
		      WCount++;
		      goto NextInQueue;
		    }
		  i -= 2;
		  u = path[i];
		  U->A[u].current = U->A[u].current->next;
		}
	    NextInPath:
	      ;
	    }
	}
    NextInQueue:
      ;
    }
  printf("D %d R %d W %d rootDeq %d eq %d eex %d opcount(th) %.2f\n", 
	 DCount, RCount, WCount, rootDeq, eq, eex, (eq+eex)/1000.0);
  return card;
}

GlobalRelabel(U, V)
Upartition *U;
Vpartition *V;
{

  Vertex Q2[2*MAX_CARD];
  Boolean vVisited[MAX_CARD];
  Boolean uVisited[MAX_CARD];
  int h, t;
  Vertex v, v2, v3, u, w;
  Edge *e;

  InitQueue(Q);

  h = 0;
  t = -1;
  vVisited[0] = TRUE; /* for the hack below */
  for (v = 1; v <= V->card; v++)
    {
      if (V->A[v].current == (Edge *) 0)
	{
	  Q2[++t] = v;
	  vVisited[v] = TRUE;
	  V->A[v].label = 0;
	}
      else 
	{
	  vVisited[v] = FALSE;
	  V->A[v].label = INFINITY;
	}
    }

  for (u = 1; u <= U->card; u++)
    {
      uVisited[u] = FALSE;
      U->A[u].label = INFINITY;
      U->A[u].current = U->A[u].e;
    }

  while (h <= t)
    {
      v = Q2[h++];
      /* printf("scanning from v%d ...\n", v); */
      e = V->A[v].e;
      eq++;
      while (e != (Edge *) 0)
	{
	  if (!e->matched && !uVisited[e->other])
	    {
	      u = e->other;
	      uVisited[u] = TRUE;
	      U->A[u].label = V->A[v].label + 1;
	      if (!vVisited[U->A[u].matchedTo])
		{
		  /* hack: u may not be matched, in which case matchedTo will
		     be zero, and we assume vVisited[0] == TRUE.
		     */
		  v2 = U->A[u].matchedTo;
		  vVisited[v2] = TRUE;
		  Q2[++t] = v2;
		  V->A[v2].label = V->A[v].label + 2;
		}
	      else if (U->A[u].matchedTo == 0)
		{
		  Enqueue(Q, u);
		}
	    }
	  e = e->next;
	  eq++;
	}
    }
}
