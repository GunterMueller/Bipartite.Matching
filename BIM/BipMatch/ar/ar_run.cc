#define MAIN
#define MATCHING

#include <iostream.h>
#include <stdlib.h>

#include "param.h"
#include "statistics.h"
#include "ar.h"
#include "inline.h"

#include "timer.c"
#include "ar.cc"
#include "../pr/bim_parser.h"
#include "base_parser.cc"
#include "parse_arguments.cc"

void parse();

main(int argc, char *argv[])
{

  arc *e;
  parse_arguments(argc,argv,"gio:u:");

#ifndef CONCISE
  cout << "c Begin Parsing...\n";
#endif

  parse();
  param.greedy = true; // greedy initial solution

#ifndef CONCISE
  cout << "c Parsing has finished. Begin matching calculations...\n";
  cout << "c \n";
  cout << "c Nodes: " << G.numNodes() - 2 << "\n";  
  cout << "c Arcs:  " << (G.numArcs() - 2*(G.numNodes()-2))/2 << "\n";
  cout << "c \n";

#ifdef OUTPUT_GRAPH
  cout << "p bipartite-graph " << (G.numNodes() - 2)/2 << " ";
  cout << (G.numNodes() - 2)/2 << " " << (G.numArcs() - 2*(G.numNodes()-2))/2 <<"\n";
  node *u;
  forallXNodes(u,G) {
    forallOutArcs(e,u) {
      if (e->head() == G.getSource()) continue; 
      cout << "a " << u->index()+1 << " " << e->head()->index()+1-(G.numNodes() - 2)/2 << "\n";
    }
  }
  exit(0);
#endif
#endif

  simpleQueue Q1(G.numNodes()); // queue used to process unmatched X nodes
  simpleQueue S(G.numNodes()); // stack containing augmenting path
  simpleQueue Q2(G.numNodes()); // queue for global upadte BFS

  float t = timer();
  arBIM(Q1,S,Q2);
  t = timer() - t;
  CheckMatching(S);

#ifdef CONCISE
printf ("%8ld %8ld %8ld %8lld %8lld %11.2f\n",
         G.numNodes()-2,(G.numArcs() - 2*(G.numNodes()-2))/2,stats.mVal, 
	stats.flowArcCnt,stats.searchArcCnt,t);
#endif
#ifndef CONCISE
  cout << "c Running Time: " << t << '\n';
#ifdef STATS
  cout << "c Matching Size: " << stats.mVal << '\n';
if (param.greedy) cout << "c Greedy Init: " <<stats.greedCnt << '\n';
#ifdef AUGMENT
// cout << "c Augmenting Paths: "<<stats.augCnt<< '\n';
#endif
  cout << "c Edge scans: " << stats.searchArcCnt << '\n';
  cout << "c Flow updates: " << stats.flowArcCnt << '\n';
  cout << "\n";
#endif  
#endif
}
