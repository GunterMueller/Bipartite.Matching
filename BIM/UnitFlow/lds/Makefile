# -*- makefile -*-
# $Id: Makefile,v 1.7 1997/08/15 17:51:08 pmartin Exp $

UF_FLAGS =  -DSTATS -DNO_ERROR_HANDLING -DCONCISE -DCHECK
CXXFLAGS += $(UF_FLAGS)

BINARIES = uf_lds

MAIN = lds_run.cc
PKGTYPE = cc

# pick options for the binaries
uf_lds_opts= -DNO_BUCKET -DQUEUE -DLDS

include ../../make.config

