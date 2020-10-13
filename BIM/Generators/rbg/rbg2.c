/* bipartite graph generator.
   Neighborhood by group. Each vertex u in group i may have neighbors in groups
   i-1, i, i+1 in V. Wraps around.

   Output file format specialized for bipartite graphs.

   U vertices that are part of group i are randomly numbered.

   J. C. Setubal, 23-dec-97 
   (supersedes previous undated versions. 
    This versions fixes 1 bug:
    - the actual number of neighbors of a u vertex was incorrectly set
    when N close to ene.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "misc.h"
#include "random.h"
#include <sys/time.h>

/* globals */
int Nvertices, Nedges;  /* Nvertices refers to # vertices in ONE partition */
int N;                  /* number of vertices in a group */
int Ngroups;
double ene; 		/* expected number of edges for each vertex */
int seed;
int  A[MAXK];	/* stores edges to be chosen by each vertex */
int B[MAX_CARD]; /* stores random permutation of u vertices */
double Limit; /* used in poisson variable simulation */

/* forw declarations to avoid messages from gcc */
extern void ParseCommandLine();
extern void GenerateGraph();
extern int GetK();
extern void GenerateEdges();
extern void PrintEdges();

void main(argc, argv)
int argc;
char **argv;
{
  ParseCommandLine(argc, argv);
  GenerateGraph();
}

void ParseCommandLine(argc,argv)
int argc;
char **argv;
{
  struct timeval tp;

  if ((argc < 4) || (argc > 5))
    {
      printf("Program usage: rbg2 <N> <G> <ene> [seed]\n");
      exit(1);
    }

  N = atoi(argv[1]);
  Ngroups = atoi(argv[2]);
  ene = atof(argv[3]);
  if (argc == 5)
    seed = atoi(argv[4]);
  else
    {
      gettimeofday(&tp, 0);
      seed = tp.tv_sec + tp.tv_usec;
    }
}

void GenerateGraph()
{

  int i, u;

  Nvertices = N*Ngroups;
  InitRandom(seed);

  /* simulation of a Poisson random variable */
  Limit = 1/exp(ene); /* used in GetK */

  /* initialize A */
  for (i = 0; i < MAXK; i++) A[i] = i;

  /* generate array B, with vertex numbers randomly distributed */
  for (i = 0; i < Nvertices; i++) B[i] = i;
  RandomPermutation(B, Nvertices);

  printf("c special random bipartite graph; N=%d, Ngroups=%d, ene=%.1f, seed=%d\n",
	 N, Ngroups, ene, seed);
  printf("c ene: expected number of edges for each vertex\n");
 /* other descriptive lines are filled out at the end */

  Nedges = 0;
  for (i = 0; i < Ngroups; i++)
    {
      for (u = i*N; u < (i+1)*N; u++)
	{
	  GenerateEdges(B[u], (i-1)*N, (i+2)*N - 1);
	}
    }
  printf("p bipartite-matching %d %d %d\n", Nvertices, Nvertices, Nedges);

}

void GenerateEdges(w, low, high)
int w;
int low, high;
{ 
  int k; /* actual number of neighbors */

  k = GetK(high-low+1);
  RandomSubset(low, high, k, A);
  PrintEdges(w, k);
}

int GetK(actual_maxk)
int actual_maxk; /* number of available vertices to choose from */
{
  int k = 0;
  double u = 1;

  while(1)
    {
      u *= ((double) random()/ (double) MAXRAND);
      if ((u < Limit) || k == actual_maxk || k == MAXK) break;
      k++;
    }
  return k;
}

void PrintEdges(w, k)
int w, k;
{

  int i;

  for (i = 0; i < k; i++)
    {
      /* statements below handle the wrap-around */
      if (A[i] < 0) A[i] += Nvertices;
      else if (A[i] >= Nvertices) A[i] -= Nvertices;
      printf("a %d %d\n", w+1, A[i] + 1);
    }
  Nedges += k;
}
