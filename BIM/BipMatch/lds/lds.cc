long MaxDistance;

bool globalUpdate(simpleQueue &Q);

void Augment(simpleQueue &S)
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

bool Relabel(Xnode *u)
{
  long minD;
  arc *minA = nil;
  arc *a;
  Ynode *v;
  Xnode *u2;

  minD = MaxDistance;
  forallOutArcs(a,u)
    {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      v = a->head();
      if (v == G.getSource()) continue;
      assert(v->isMatched());
      u2 = v->getMatchNode();
      if (!u2->isReached() && u2->XgetDistance() < minD)
	{
	  minD = u2->XgetDistance();
	  minA = a;
	}
    }
  if (minD == MaxDistance) return false;
  else
    {
      u->XsetDistance(minD+1);
      u->setCurrent(minA);
      return true;
    }
}

void ldsBIM(simpleQueue &Q, simpleQueue &S, simpleQueue &Qbfs)
  // Q is a queue for visited nodes, used in cleaning-up after a
  // successful search 
  // S is a stack that contains the augmenting path
  // Qbfs is a queue used in global updates
{
  Xnode *u, *u2;
  Ynode *v;
  arc *e, *stopA;
  bool foundPath;
  long relsSinceUpdate = 0;

  MaxDistance = (G.numNodes() - 2)/2;
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
  if (!globalUpdate(Qbfs)) goto Finish;
  
  forallXNodes(u,G) // outermost loop
    {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      if (u->isMatched()) continue;
      assert(!u->isReached());
      foundPath = false;
      u->setReached(true);
      S.enqueue(u);
      Q.enqueue(u);
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
		  if (u2->isReached() || u->XgetDistance() <=
		      u2->XgetDistance()+1)
		    {
		      e = G.succArc(e); continue;
		    }
		  // found admissible arc; advance
		  u->setCurrent(e);
		  S.enqueue(u2);
		  Q.enqueue(u2);
		  u = u2;
		  u->setReached(true);
		  // we update the loop control variables
		  e = u->getCurrent();
		  stopA = u->lastOutArc();
		}
	      else // v not matched
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
	      u->setCurrent(e); // do we need this?
	      // relabel
	      relsSinceUpdate++;
	      if (relsSinceUpdate > param.globalUpdateFreq *
		  (double) G.numNodes()) /// must check what's best!!!
		{
		  if (globalUpdate(Qbfs)) relsSinceUpdate = 0;
		  else goto Finish;
		}
	      if (Relabel(u))
		{
		  // advance
		  u = u->getCurrent()->head()->getMatchNode();
		  S.enqueue(u);
		  Q.enqueue(u);
		  u->setReached(true);
		}
	      else // relabel failed; we should retreat, if possible
		{
		  S.pop(); // because u is the stack top
		  if (S.isEmpty()) break; // from search loop
		  // retreat
		  u = S.top();
		  u->setCurrent(G.succArc(u->getCurrent()));
#ifdef STATS
		  stats.searchArcCnt++;
#endif
		}
	    }
	}  while (!foundPath); // we may break from this loop also
                               // because stack is empty
      if (foundPath)
	{
	  // clean up
	  while (!Q.isEmpty())
	    {
	      u = Q.pop();
	      u->setReached(false);
	    }
	}
      else Q.reinitialize(); // visited nodes stay reached
    } // outermost loop
 Finish:;
}

bool globalUpdate(simpleQueue &Qbfs)
  // Qbfs is the queue used for the BFS done here
  // returns true iff there are unmatched nodes in X partition
  // reachable from the sink
{
  Xnode *u;     
  Ynode *v, *v2; 
  arc  *e;
  DistType  currentDistance; // current depth in the breadth-first search
  bool status = false;

  Qbfs.reinitialize();

  forallXNodes(u,G) {
    u->unlabel();  // mark all X-nodes as unlabeled
#ifdef STATS
    stats.searchArcCnt++;
#endif
  }
  forallYNodes(v,G) {
    if (!v->isMatched()) {  // add all unmatched Y-nodes to the queue
      Qbfs.enqueue(v);
    }
#ifdef STATS
    stats.searchArcCnt++;
#endif
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
	if (!u->isMatched()) {  // if u is unmatched put it in main queue
	  status = true;
	}
	else {
	  v2 = u->getMatchNode();
	  Qbfs.enqueue(v2);
	}
      }
    }
  }
  return status;
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
