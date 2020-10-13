# -*- makefile -*-
# $Id: Makefile,v 1.7 1997/06/19 19:09:58 pmartin Exp $

AR_FLAGS = -DNO_ERROR_HANDLING -DSTATS -DCONCISE
CXXFLAGS += $(AR_FLAGS)

BINARIES = bim_lds

MAIN = lds_run.cc

PKGTYPE = cc

# pick options for the binaries
bim_lds_opts= -DLDS 

include ../../make.config

