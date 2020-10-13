/* manip.c */

#include <stdio.h>
#include "misc.h"
#include "graph.h"
#include "manip.h"

void Init_U(U)
U_partition *U;
{
  int u;

  for (u = 0; u < MAX_N; u++)
    {
      U->A[u].firstEdge = (Edge *) 0;
      U->A[u].mate = 0;
    }
  U->size = 0;  
}

void Init_V(V)
V_partition *V;
{
  int v;

  for (v = 0; v < MAX_N; v++)
    {
      /* V->A[v].firstEdge = (Edge *) 0; */
      V->A[v].mate = 0;
    }
  V->size = 0;  
}

void AddEdge(u, v, U, V)
int u, v;
U_partition *U;
V_partition *V;
{
  Edge *e1, *e2;

  e1 = (Edge *) Alloc(sizeof(Edge));
  e2 = (Edge *) Alloc(sizeof(Edge));

  e1->next = U->A[u].firstEdge;
  U->A[u].firstEdge = e1;
  e1->h = v;
/*  e2->next = V->A[v].firstEdge;
  V->A[v].firstEdge = e2; */
  e2->h = u;
}
