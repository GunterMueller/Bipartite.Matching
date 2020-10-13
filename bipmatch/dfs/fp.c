#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "graph.h"
#include "timer.h"
#include "fp.h"
#include <prof.h>

int eq, eex; /* operation counts */

struct {
  int u;
  Edge *current;
} stack[MAX_N];

Boolean visited[MAX_N];
int sp; /* stack pointer */
int base; /* temp and perm cut pointers */
int tcp, pcp; 

void Inits(U,V)
U_partition *U;
V_partition *V;
{

  int u;

  eq = eex = 0;
  sp = -1;
  for (u = 1; u <= U->size; u++)
    {
      U->A[u].nextEdge = U->A[u].firstEdge;
      visited[u] = FALSE;
      /* we put all unmatched vertices from U in the stack */
      if (!U->A[u].mate)
	{
	  stack[++sp].u = u;
/*
	  printf("%d is unmatched\n", u);
*/
	}
    }

  base = sp - 1; /* base points to the next unmatched vertex in U */
  tcp = pcp = MAX_N;
}

void UpdatePath(U, V, first, last)
U_partition *U;
V_partition *V;
int first, last;
{
  int i;
  int u, v;

  for (i = first; i <= last; i++)
    {
      u = stack[i].u;
      v = stack[i].current->h;
      V->A[v].mate = u;
      U->A[u].mate = v;
      visited[u] = FALSE;
      eex++;
    }
}

void FindPaths(U, V)
U_partition *U;
V_partition *V;
{
  /* finds augmenting paths by depth-first search. Array `current' records the
     edge being investigated, and the stack holds the vertices in the path.
     */

  int u, i;
  Edge *e;

  if (sp == -1) return; /* initial matching is maximum */
  u = stack[sp].u;

  /* now we process the stack. Note that the stack can decrease because of
     2 different reasons: 1) because a search from one of the original
     vertices in the stack was completed; in this case we must update base.
     2) a branch was searched unsuccessfully, and we retreat to the previous
     vertex; in this case there's no need to update base.
     */

  while (1)
    {
      visited[u] = TRUE;
      e = U->A[u].firstEdge;
      eq++;
    poppedVertex:
      /* entry point for u vertices popped from the stack */
      MARK (visit);
      while (e != (Edge *) 0 && visited[V->A[e->h].mate])
	{
	  e = e->next;
	  eq++;
	}
      MARK (noext);
      if (e == (Edge *) 0)
	{
	  /* no; cannot extend path from u */
	  
	  /* stack empty? */
	  if (sp == 0) break;
	  /* vertex copied to tc */
	  stack[--tcp].u = u;
	  u = stack[--sp].u;
	  /* are we finished with the current search? */
	  if (sp == base) 
	    {
	      /* yes; start new search */
	      base--; 
	      /* increase permanent cut */
	      pcp = tcp;
	    }
	  else
	    {
	      e = stack[sp].current->next;
	      eq++;
	      goto poppedVertex;
	    }
	}
      else
	{
	  MARK(ext);
	  /* yes; extend path */
	  stack[sp].current = e;
	  stack[++sp].u = u = V->A[e->h].mate;
	  /* look-ahead heuristic */
	  MARK(lahead);
	  e = U->A[u].nextEdge;
          eq++;
	  while (e != (Edge *) 0 && V->A[e->h].mate) 
	    {
	      e = e->next;
	      eq++;
	    }
	  U->A[u].nextEdge = e;
	  MARK(aupath);
	  if (e != (Edge *) 0)
	    {
	      /* augmenting path found */
	      /* u in the stack but not the edge to v */
	      V->A[e->h].mate = u;
	      U->A[u].mate = e->h;
	      visited[u] = FALSE;
	      /* now for the rest of the path */
	      UpdatePath(U, V, base+1, sp-1);
	      MARK(pcptcp);
	      /* temporary cut is emptied */
	      for (i = pcp-1; i >= tcp; i--) 
		{
		  visited[stack[i].u] = FALSE;
		}
	      tcp = pcp;
	      if (base < 0) break; 
	      /* start new search */
	      sp = base--;
	      u = stack[sp].u;
	    }
	}
    }
}

/*
PrintStatus(U,V)
U_partition *U;
V_partition *V;

{
  int u, v;

  for (u = 1; u <= U->size; u++)
    {
      if (current[u] == (Edge *) 0) printf("u%d nil\n", u);
      else printf("u%d %d\n", u, current[u]->h);
    }

  for (v = 1; v <= V->size; v++)
    {
      printf("v%d %d %d\n", v, V->A[v].visited, V->A[v].mate);
    }
  exit(1);
}
*/








