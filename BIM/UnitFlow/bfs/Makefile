# -*- makefile -*-
# $Id: Makefile,v 1.7 1997/08/15 17:51:08 pmartin Exp $

UF_FLAGS = -DSTATS -DNO_ERROR_HANDLING -DCONCISE
CXXFLAGS += $(UF_FLAGS)

BINARIES = uf_bfs

MAIN = bfs_run.cc

PKGTYPE = cc

# pick options for the binaries
uf_bfs_opts= -DNO_BUCKET -DQUEUE -DBDFS

include ../../make.config

