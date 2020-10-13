/* conversor de formato dimacs para bcm */

#include <stdio.h>
#include "misc.h"
#include "utility.h"
#include "bgraph.h"
#include "io.h"

extern void OutputMatrix();

main(argc, argv)
int argc;
char *argv[];
{
  FILE *f1, *f2;
  Upartition *U;
  Vpartition *V;
  Boolean output = FALSE;

  if (argc < 1 || argc > 2)
    Barf("Usage: d2b <input [<output file>]");

  f1 = stdin;
  f2 = stdout;

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
  OutputMatrix(f2, U, V);
}

void OutputMatrix(f, U, V)
FILE *f;
Upartition *U;
Vpartition *V;
{
  int i, j, u;
  int rp[MAX_CARD];
  Edge *e;

  i = 1; j = 0;
  printf("%d %d %d\n", U->card, V->card, U->nEdges);
  for (u = 1; u <= U->card; u++)
    {
      e = U->A[u].e;
      rp[++j] = i;
      while (e != (Edge *) 0)
	{
	  i++;
	  printf("%d ", e->other);
	  e = e->next;
	}
    }
  printf("\n");
  rp[++j] = i;
  for (i = 1; i <= j; i++) printf("%d ",rp[i]);
  printf("\n");
}

