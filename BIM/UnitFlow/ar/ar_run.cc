// Hey emacs! This is -*- c++ -*-
// $Id: ar_run.cc,v 1.8 1998/02/9 3:11pm setubal Exp $

#define MAIN
#define UNIT_FLOW

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "param.h"
#include "statistics.h"
#include "uf.h"

#include "inline.h"

#include "timer.c"
#include "ar.cc"
#include "../pr/uf_parser.h"
#include "base_parser.cc"
#include "parse_arguments.cc"

main(int argc, char *argv[])
{
  CapType flowValue;
  arc *a;

  parse_arguments(argc,argv,"go:u:");

#ifndef CONCISE
  cout << "Begin Parsing...\n";
#endif
  parse();

#ifndef CONCISE
  cout << "Parsing has finished. Begin max-flow calculations...\n";
  cout << "\n";
  cout << "c Nodes: " << G.numNodes() << "\n";  
  cout << "c Arcs:  " << G.numArcs()/2 << "\n";
  cout << "\n";
#endif
  simpleQueue S(G.numNodes()); // contains augmenting paths & used in GR

  float t = timer();
  ARUnitFlow(S);
  t = timer() - t;
#if CHECK
  // cout << "checking flow...\n";
  CheckFlow(S);
  // cout << "flow checked.\n";
#endif
  flowValue = 0;
  forallOutArcs(a,G.getSource())
    if (a->isUsed())
      flowValue++;

#ifndef CONCISE
  cout << "c Running Time: " << t << '\n';
  cout << "c Flow: " << flowValue << '\n';
#endif
#ifdef CONCISE
printf ("%8ld %8ld %8ld %21llu %21llu %11.2f\n",
         G.numNodes(),G.numArcs()/2,
	flowValue,
	stats.flowArcCnt,stats.searchArcCnt,t);
#endif
}
