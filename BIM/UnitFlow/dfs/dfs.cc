long augp = 0;

void augment(simpleQueue &Q)
{
  node *v;

  // cout << "augmenting path " << ++augp << " found\n";
  while (!Q.isEmpty()) {
    v = Q.pop();
    v->getCurrent()->use(); 
#ifdef STATS
    stats.flowArcCnt++;
    stats.searchArcCnt++;
#endif
  }
  G.getSource()->getCurrent()->use();
#ifdef STATS
  stats.flowArcCnt++;
  stats.searchArcCnt++;
#endif
}   

void  DFSUnitFlow(simpleQueue &Q, simpleQueue &S)
  // Q is used as DFS stack
  // S contains all visited nodes for reinitialization
{
  node *u, *v;
  arc  *a, *e, *e2, *stopA;
  bool foundPath;

  forallNodes(u,G) 
    {
      u->setReached(false);          // mark all nodes as unreached
      u->resetCurrent();
      u->setLookAhead(u->firstOutArc());
    }
  G.getSource()->setReached(true);
  forallOutArcs(a,G.getSource())
    {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      u = a->head();
      if (u->isReached()) continue;
      G.getSource()->setCurrent(a);
      // checking for augmenting path of length 1
      if (u == G.getSink())
	{
	  augment(Q);
	  continue;
	}
      else 
	{
	  // checking for augmenting path of length 2
	  // must check this here because in main loop we can only find
	  // augpaths of length 3 or greater
	  forallOutArcsAfter(e2,u,u->getLookAhead())
	    {
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	      if (!e2->isUsed() && e2->head() == G.getSink())
		{
		  e2->use();
		  augment(Q);
		  u->setLookAhead(G.succArc(e2));
#ifdef STATS
		  stats.searchArcCnt++;
#endif
		  continue;
		}
	    }
	  u->setLookAhead(e2);
	}
      // DFS from u
      foundPath = false;
      Q.enqueue(u);
      S.enqueue(u);
      u->setReached(true);
      do
	{      
	  v = Q.pop();
	  e = v->getCurrent();
	  stopA = v->lastOutArc();
	  while (e <= stopA) // search forward
	    {
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	      if (e->isUsed()) 
		{
		  e = G.succArc(e); continue;
		}
	      u = e->head();
	      if (u->isReached())
		{
		  e = G.succArc(e); continue;
		}
	      v->setCurrent(e);
	      Q.enqueue(v);
	      u->setReached(true);
	      S.enqueue(u);
	      // look ahead
	      forallOutArcsAfter(e2,u,u->getLookAhead())
		{
#ifdef STATS
		  stats.searchArcCnt++;
#endif
		  if (!e2->isUsed() && e2->head() == G.getSink())
		    {
		      foundPath = true;
		      break;
		    }
		}
	      u->setLookAhead(e2);
	      if (foundPath)
		{
		  Q.enqueue(u);
		  u->setCurrent(e2);
		  augment(Q);
		  break;
		}
	      v = u;
	      e = v->getCurrent();
	      stopA = v->lastOutArc();
	    }
	} while (!Q.isEmpty() && !foundPath);

      if (foundPath)
	{
	  // clean up
	  Q.reinitialize();
	  while (!S.isEmpty())
	    {
	      u = S.pop();
	      u->setReached(false);
	      u->resetCurrent();
	    }
	}
      else
	S.reinitialize(); // visited nodes stay reached
    }
}
