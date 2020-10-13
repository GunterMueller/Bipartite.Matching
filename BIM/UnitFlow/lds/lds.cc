void printArc(arc *a)
{
  cout << "   (" << a->reverseArc()->head()->index()+1 << "," <<
    a->head()->index()+1 << ")";
}

// forw declaration
bool globalUpdate(simpleQueue &Q);

bool Relabel(node *u) 
{
  arc  *a;
  arc  *minA;  // the arc out of u with lowest head
  node *v;
  long minD;   // the height of the head 
  long d;

  minD = LARGE;
  minA = u->lastOutArc();
  forallOutArcs(a,u) {
#ifdef STATS
    stats.searchArcCnt++;
#endif
    if (a->isAvailable() && !a->head()->isReached()) {
      d = a->head()->getDistance();
      if (d < minD) {
	minD = d;
	minA = a;
      }
    }
  }
  // note that this relabel is DIFFERENT from the usual relabel, in
  // that we don't change the current label if we don't find a
  // suitable neighbor.
  if (minD == LARGE) return false;
  else
    {
      u->setDistance(minD+1);
      u->setCurrent(minA);
      return true;
    }
}

void augment(simpleQueue &S)
{
  node *u;
  arc *a;

  while (!S.isEmpty()) {
    u = S.pop();
    a = u->getCurrent();     
    a->use();
#ifdef STATS
    stats.searchArcCnt++;
    stats.flowArcCnt++;
#endif
  }
}   

void LDSUnitFlow(simpleQueue &S1, simpleQueue &S2, simpleQueue &Q)
{
  node *u, *v, *init_v;
  arc  *a, *curr, *a2, *aa;
  long relsSinceUpdate = 0; // used to determine when to perform global update

  // We use two stacks: one (S1) for holding the path and another (S2)
  // for holding all vertices explored in a search. 
  // We place node u that is being searched on S1.
  // S2 is used to unmark vertices after a successful search.

  forallNodes(u,G) u->initialize();
  if (!globalUpdate(Q))
    {
      cout << "ERROR: source does not reach sink\n";
      return;
    }
  G.getSource()->setReached(true);
  forallOutArcs(a, G.getSource()) // source neighbor loop
    {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      assert(!a->isUsed());
      init_v = a->head();
      if (init_v->isReached()) continue;
      // start search from init_v
      if (init_v == G.getSink()) 
	{
	  a->use();
#ifdef STATS
	  stats.flowArcCnt++;
#endif
	  continue;
	}
      G.getSource()->setCurrent(a);
      init_v->setReached(true);
      u = init_v;
      S1.enqueue(u); 
      S2.enqueue(u);
      while (1) // search loop
	{
	  curr = u->getCurrent();
	  forallOutArcsAfter(aa, u, curr) // scan loop
	    {
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	      if (aa->isUsed()) continue; // cap == flow
	      v = aa->head();
	      if (u->getDistance() <= v->getDistance()) continue; // wrong label
	      if (v->isReached()) continue; // already visited
	      // found admissible arc
	      if (v == G.getSink())
		{
		  // found augmenting path
		  u->setCurrent(aa);
		  augment(S1);
		  G.getSource()->getCurrent()->use(); // source's not
						      // on stack
#ifdef STATS
		  stats.flowArcCnt++;
#endif
		  // clean up
		  while (!S2.isEmpty())
		    {
		      u = S2.pop();
		      u->setReached(false);
		    }
		  goto ContinueSourceNeighborLoop;
		}
	      else
		{
		  // advance
		  S1.enqueue(v); 
		  S2.enqueue(v);
		  u->setCurrent(aa);
		  u = v;
		  u->setReached(true);
		  goto ContinueSearchLoop;
		}
	    }
	  // we can only be here if there's NO admissible arc 
	  // (end of scan loop was reached)
	  relsSinceUpdate++;
	  if (relsSinceUpdate > param.globalUpdateFreq *(double)G.numNodes()) {
	    if (!globalUpdate(Q)) return;
	    relsSinceUpdate = 0;
	  }
	  if (Relabel(u))
	    {
	      aa = u->getCurrent();
	      u = aa->head();
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	      u->setReached(true);
	      S1.enqueue(u);
	      S2.enqueue(u);
	    }
	  else 
	    {
	      if (u == init_v) {
		// back to first vertex
		// visited vertices remain marked
		S2.reinitialize();
		goto ContinueSourceNeighborLoop;
	      }
	      /* retreat */
	      S1.pop();
	      u = S1.top();
	      a2 = u->getCurrent();
	      u->setCurrent(++a2); // low level!!!
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	    }
	ContinueSearchLoop: ;
	}
    ContinueSourceNeighborLoop: ;
    }
}      
		
bool globalUpdate(simpleQueue &Q)
{
  node *u, *v;
  arc  *e;
  DistType  currentDistance; // the current depth in the breadth first search

#ifdef STATS
  stats.updateCnt++;
#endif
  Q.reinitialize();
  forallNodes(u,G) u->setDistance(VERY_FAR); // mark all nodes as unlabeled
  G.getSink()->setDistance(0);           // set sink to distance 0
  Q.enqueue(G.getSink());                // add the sink to the queue
  while (!Q.isEmpty()) {      
    v = Q.dequeue();
    currentDistance = v->getDistance();  // distance of node being scanned
    forallOutArcs(e,v) {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      arc &ee = *e;
      u = ee.head();
      if (u->getDistance() < VERY_FAR) continue;
      if (e->isAvailable()) continue;
      u->setDistance(currentDistance+1);
      Q.enqueue(u);
    }
  }
  return (G.getSource()->getDistance() < VERY_FAR);
}

void CheckFlow(simpleQueue &Q)
{
  node *u, *v;
  long f, fs = 0, ft = 0;
  arc *a;

  forallOutArcs(a,G.getSource()) if (a->isUsed()) fs++;
  forallOutArcs(a,G.getSink()) if (a->isAvailable()) ft++;
  if (fs != ft)
    {
      cout << "ERROR: flow out of s(" << fs << ") != flow into t(" << ft << ")\n";
    }
  // maximality check with BFS
  forallNodes(u,G)
    {
      u->setDistance(VERY_FAR);
    }
  u = G.getSource();
  u->setDistance(0);
  Q.reinitialize();
  Q.enqueue(u);
  do 
    {      
      v = Q.dequeue();
      forallOutArcs(a,v) {
	if (a->isUsed()) continue;
	u = a->head();
	if (u->getDistance() < VERY_FAR) continue;
	if (u == G.getSink())
	  {
	    cout << "ERROR: flow not maximum!\n";
	    return;
	  }
	else
	  {
	    u->setDistance(0);
	    Q.enqueue(u);
	  }
      }
    } while (!Q.isEmpty());
}
