#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "timer.h"
#include "bgraph.h"
#include "io.h"
#include "match.h"

#define QUEUE_SIZE 2*MAX_CARD-1

int DCount, PCount, RCount, WCount;
int eq; /* edge queries */
int BFSthreshold;
int BFSlag;
int Nvertices;

void Match(U, V)
Upartition *U;
Vpartition *V;
{
  Vertex u, v, w;
  Edge *e;
  Boolean RelabelU();

  int Q[2*MAX_CARD];
  int head, tail;
  int rightLabel;

  DCount = PCount = RCount = WCount = eq = 0;
  Nvertices = U->card+V->card+2;
  BFSthreshold = 0; /* we start with global relabeling */
  BFSlag = 64*U->card; /* U->nEdges/2.5; */
  head = 0; 
  tail = 0;

  /* enqueue first active vertices (overmatched v vertices) */
  for (v = 1; v <= V->card; v++)
    {
      if (V->A[v].matched)
	{
	  if (V->A[v].excess > 0) Q[tail++] = v;
	}	  
    }

  /* process queue */
  while (tail != head)
    {
      DCount++;
      if (DCount >= BFSthreshold)
	{
	  /* global relabeling activated */

	  GlobalRelabel(U, V);
	  BFSthreshold += BFSlag;
	  WCount++;
	}
      w = Q[head];
      head = (head == QUEUE_SIZE) ? 0 : head+1;

      if (w < 0 ) /* w in U */
	{
	  u = -w;
	  /* if this u vertex has a label above n we know it is on the
	     source side of the mincut, so we can ignore it in further
	     processing */
	  if (U->A[u].label >= Nvertices) continue;
	  while (1)
	    {
	      eq++;
	      e = U->A[u].current;
	      /* ok to push? */
	      if (V->A[e->other].label+1 == U->A[u].label)
		{
		  v = e->other;
		  e->mate->matched = TRUE;
		  U->A[u].matchedTo = v;
		  if (!V->A[v].matched) V->A[v].matched = TRUE;
		  else
		    {
		      V->A[v].excess++;
		      Q[tail] = v;
		      tail = (tail == QUEUE_SIZE) ? 0 : tail+1;
		    }
		  PCount++;
		  break;
		}
	      else if (e->next == (Edge *) 0)
		{
		  if (!RelabelU(U, V, u)) break;
		}
	      else U->A[u].current = e->next;
	    }
	}
      else  /* w in V */
	{
	  v = w;
	  while (V->A[v].excess > 0)
	    {
	      eq++;
	      e = V->A[v].current;
	      if (e->matched && U->A[e->other].label+1 == V->A[v].label)
		{
		  e->matched = FALSE;
		  U->A[e->other].matchedTo = 0;
		  /* hack: u vertices are placed in queue as negative,
		     to distinguish them from v vertices */
		  Q[tail] =  -(e->other);
		  tail = (tail == QUEUE_SIZE) ? 0 : tail+1;
		  V->A[v].excess--;
		  PCount++;
		}
	      else if (e->next == (Edge *) 0)
		{
		  RelabelV(U, V, v);
		}
	      else
		{
		  V->A[v].current = e->next;
		}
	    }
	}
    }
  printf("D %d P %d R %d W %d eq(th) %.0f eq/m %.2f opcount(th) %.2f\n",
         DCount, PCount, RCount, WCount, eq/1000.0, (double) eq/U->nEdges,
	 (PCount+eq)/1000.0);

}

Boolean RelabelU(U, V, u)
Upartition *U;
Vpartition *V;
Vertex u;
{
  int l;
  Edge *e, *current;

  current = (Edge *) 0;
  RCount++;
  l = INFINITY;
  e = U->A[u].e;
  eq++;
  while (e != (Edge *) 0)
    {
      if (V->A[e->other].label < l) 
	{
	  l = V->A[e->other].label;
	  current = e;
	}
      e = e->next;
      eq++;
    }
  U->A[u].label = l+1;
  U->A[u].current = current;
  if (l+1 >= Nvertices) return FALSE; /* vertex on source side of the cut */
  return TRUE;
}

RelabelV(U, V, v)
Upartition *U;
Vpartition *V;
Vertex v;
{
  int l;
  Edge *e, *current;

  current = (Edge *) 0;
  RCount++;
  l = INFINITY;
  e = V->A[v].e;
  eq++;
  while (e != (Edge *) 0)
    {
      if (e->matched && U->A[e->other].label < l) 
	{
	  l = U->A[e->other].label;
	  current = e;
	}
      e = e->next;
      eq++;
    }
  V->A[v].current = current;
  V->A[v].label = l+1;
}

GlobalRelabel(U, V)
Upartition *U;
Vpartition *V;
{

  Vertex Q2[2*MAX_CARD];
  Boolean vVisited[MAX_CARD];
  Boolean uVisited[MAX_CARD];
  int h, t;
  Vertex v, v2, u;
  Edge *e;

  h = 0;
  t = -1;
  vVisited[0] = TRUE; /* for the hack below */
  for (v = 1; v <= V->card; v++)
    {
      if (!V->A[v].matched)
	{
	  Q2[++t] = v;
	  vVisited[v] = TRUE;
	  V->A[v].label = 1;
	}
      else 
	{
	  vVisited[v] = FALSE;
	  V->A[v].label = Nvertices+2;
	}
    }
  for (u = 1; u <= U->card; u++)
    {
      uVisited[u] = FALSE;
      U->A[u].label = Nvertices+1;
    }

  while (h <= t)
    {
      v = Q2[h++];
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
	    }
	  e = e->next;
	  eq++;
	}
    }
}
