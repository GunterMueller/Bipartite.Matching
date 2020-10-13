void globalUpdate(simpleQueue &Q1, simpleQueue &Q2);

void Augment(simpleQueue &S)  // check!
{
  Xnode *u;
  Ynode *v;

  while (!S.isEmpty())
    {
      u = S.pop();
      v = u->getCurrent()->head();
      u->match(v);
      v->match(u);
#ifdef STATS
      stats.flowArcCnt++;
      stats.searchArcCnt++;
#endif
    }
#ifdef STATS
  stats.mVal++;
#endif
}   

void greedyMatch()
{
  Xnode *u;
  Ynode *v;
  arc *e;

  forallXNodes(u,G) {
#ifdef STATS
    stats.searchArcCnt++;
#endif
    assert(!u->isMatched());
    forallOutArcs(e,u) {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      v = e->head();
      if (!v->isMatched()) {
#ifdef STATS
	stats.mVal++;
	stats.greedCnt++;
	stats.flowArcCnt++;
#endif
	v->match(u);
	u->match(v);
	break;
      }
    }
  }
}

void arBIM(simpleQueue &Q, simpleQueue &S, simpleQueue &Qbfs)
  // Q is a queue of unmatched X nodes
  // S is a stack that contains the augmenting path
  // Qbfs is a queue used in global updates
{
  Xnode *u, *u2, *u_init;
  Ynode *v;
  arc *e, *stopA;
  bool foundPath;
  long relsSinceUpdate = 0;
  long MaxDistance = (G.numNodes() - 2)/2;

  forallYNodes(v,G) 
    {
      v->initialize();
#ifdef STATS
      stats.searchArcCnt++;
#endif
    }
  forallXNodes(u,G) 
    {
      u->initialize();
#ifdef STATS
      stats.searchArcCnt++;
#endif
    }

  if (param.greedy) greedyMatch();

  globalUpdate(Q, Qbfs);
  
  while (!Q.isEmpty()) // queue loop
    {
      u = u_init = Q.dequeue(); // u_init is first node in path
      S.reinitialize();
      S.enqueue(u);
      foundPath = false;
      do //  using node u as base, we try to move forward (search loop)
	{
	  e = u->getCurrent();
	  stopA = u->lastOutArc();
	  while (e <= stopA) // scan loop
	    {
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	      v = e->head();
	      if (v == G.getSource())
		{
		  e = G.succArc(e); continue;
		}
	      if (v->isMatched())
		{
		  u2 = v->getMatchNode();
		  if (u->XgetDistance() != u2->XgetDistance()+1)
		    {
		      e = G.succArc(e); continue;
		    }
		  // advance
		  u->setCurrent(e);
		  S.enqueue(u2);
		  u = u2;
		  // we update the loop control variables
		  e = u->getCurrent();
		  stopA = u->lastOutArc();
		}
	      else
		{
		  // augmenting path found
		  u->setCurrent(e);
		  Augment(S);
		  foundPath = true;
		  break; // from scan loop
		}
	    } // end of scan loop
	  if (!foundPath)
	    {
	      // relabel
	      u->XsetDistance(u->XgetDistance()+1);
	      relsSinceUpdate++;
	      u->setCurrent(u->firstOutArc());
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	      //if (S.isEmpty())  // back to first node in path
	      if (u == u_init)  // back to first node in path
		{
		  if (u->XgetDistance() < MaxDistance) {
		    Q.enqueue(u);
		  }
		  break;
		}
	      else
		{
		  if (relsSinceUpdate > param.globalUpdateFreq *
		      (double) G.numNodes()) /// must check what's best!!!
		    {
		      globalUpdate(Q, Qbfs);
		      relsSinceUpdate = 0;
		      break;
		    }
		  // retreat
		  S.pop();
		  u = S.top();
#ifdef STATS
		  stats.searchArcCnt++;
#endif
		  u->setCurrent(G.succArc(u->getCurrent()));
		}
	    }
	}  while (!foundPath); // we may break from this loop also
                               // because stack is empty or after
                               // a global update
    } // queue loop
}

void globalUpdate(simpleQueue &Q, simpleQueue &Qbfs)
  // Q is the queue used by the main routine;
  // Qbfs is the queue used for the BFS done here
{
  Xnode *u;     
  Ynode *v, *v2; 
  arc  *e;
  DistType  currentDistance; // current depth in the breadth-first search

  Q.reinitialize();
  Qbfs.reinitialize();

  forallXNodes(u,G) {
    u->unlabel();  // mark all X-nodes as unlabeled
    u->setCurrent(u->firstOutArc());
#ifdef STATS
    stats.searchArcCnt++;
#endif
  }

  forallYNodes(v,G) {
    if (!v->isMatched()) {  // add all unmatched Y-nodes to the queue
      Qbfs.enqueue(v);
#ifdef STATS
      stats.searchArcCnt++;
#endif
    }
  }

  G.getSink()->setDistance(-1); // we need this for the Ydistance
                                // trick to work

  while (!Qbfs.isEmpty()) {
    v = Qbfs.dequeue();
    currentDistance = v->YgetDistance();  // distance of node being scanned
    forallOutArcs(e,v) {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      u = e->head(); 
      // skip node if u has already been found or if e is a matching edge
      if ( u->isUnlabeled() && u != v->getMatchNode()) {
	u->XsetDistance(currentDistance);
	// reverse arcs not available
	// u->setCurrent(e->reverseArc());
	if (!u->isMatched()) {  // if u is unmatched put it in main queue
	  Q.enqueue(u);
	}
	else {
	  v2 = u->getMatchNode();
	  Qbfs.enqueue(v2);
	}
      }
    }
  }
}

void CheckMatching(simpleQueue &S)
{
  // we check whether matching is maximum by simple DFS (taken
  // straight from dfs.cc)
  Xnode *u, *v;
  Ynode *w;
  arc *e, *stopA;
  bool foundPath, firstEdge;

  // should add some consistency checking

  forallXNodes(u,G) 
    {
      u->unlabel();
      u->setCurrent(u->firstOutArc());
    }      

  forallXNodes(u,G) 
    {
      if (u->isMatched()) continue;
      // start DFS from u
      foundPath = false;
      firstEdge = true;
      u->label();
      S.enqueue(u);
      do
	{
	  v = S.pop();
	  e = v->getCurrent();
	  if (firstEdge)
	    firstEdge = false;
	  else
	    e = G.succArc(e); // we examined e already
	  stopA = v->lastOutArc();
	  while (e <= stopA) // try to move forward
	    {
	      w = e->head();
	      if (w == G.getSource())
		{
		  e = G.succArc(e);; continue;
		}
	      if (w->isMatched())
		{
		  u = w->getMatchNode();
		  if (!u->isUnlabeled())  // includes w == u
		    {
		      e = G.succArc(e); continue;
		    }

		  // u reached for the first time
		  v->setCurrent(e);
		  S.enqueue(v);
		  u->label();
		  v = u;
		  e = v->firstOutArc();
		  stopA = v->lastOutArc();
		}
	      else
		{
		  // found augmenting path
		  cout << "Error: Matching is not maximum!\n";
		  return;
		}
	    }
	} while (!foundPath && !S.isEmpty());
    }
}
