#define MAIN
#define MATCHING

#include <iostream.h>
#include <stdlib.h>

#include "param.h"
#include "statistics.h"
#include "dfs.h"
#include "inline.h"

#include "timer.c"
#include "dfs.cc"
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

  simpleQueue Q(G.numNodes()); // a queue containing X-nodes to be scanned
  simpleQueue S(G.numNodes()); // a stack containing visited X-nodes

  float t = timer();
  dfsBIM(Q,S);
  t = timer() - t;

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
  cout << "c Augmenting Paths: "<<stats.augCnt<< '\n';
#endif
  cout << "c Edge scans: " << stats.searchArcCnt; << '\n';
  cout << "c Flow updates: " << stats.flowArcCnt << '\n';
  cout << "\n";
#endif  
#endif
//node *u;
// cout << "  Matches: \n";
// forallXNodes(u,G) if (u->isMatched()) cout << "M " << G.index(u) << ' ' << 
//                                              G.index(u->getMatchNode()) << '\n';
}



