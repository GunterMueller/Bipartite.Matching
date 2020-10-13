/* manip.c */

#include <stdio.h>
#include "misc.h"
#include "bgraph.h"
#include "manip.h"

void Init_U(U)
Upartition *U;
{
  int u;

  for (u = 0; u < MAX_CARD; u++)
    {
      U->A[u].e = (Edge *) 0;
      U->A[u].matchedTo = 0;
    }
  U->card = 0;  
}

void Init_V(V)
Vpartition *V;
{
  int v;

  for (v = 0; v < MAX_CARD; v++)
    {
      V->A[v].e = (Edge *) 0;
    }
  V->card = 0;  
}

void AddEdge(u, v, U, V)
int u, v;
Upartition *U;
Vpartition *V;
{
  Edge *e1, *e2;

  e1 = (Edge *) Alloc(sizeof(Edge));
  e2 = (Edge *) Alloc(sizeof(Edge));

  e1->mate = e2;
  e2->mate = e1;

  e1->next = U->A[u].e;
  U->A[u].e = e1;
  e1->other = v;
  e1->matched = FALSE;

  e2->next = V->A[v].e;
  V->A[v].e = e2;
  e2->other = u;
  e2->matched = FALSE;
}

Edge *EdgeLookup(U, u, v)
Upartition *U;
Vertex u, v;
{
  Edge *e;

  e = U->A[u].e;
  while (e != (Edge *) 0)
    {
      if (e->other == v) return e;
      e = e->next;
    }
  return (Edge *) 0;
}


