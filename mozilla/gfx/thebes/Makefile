
DEPTH		= ../..
topsrcdir	= ../..
srcdir		= .
VPATH		= .

include $(DEPTH)/config/autoconf.mk

MODULE	= thebes

DIRS	= public src

ifdef ENABLE_TESTS
ifndef MOZ_ENABLE_LIBXUL
TOOL_DIRS  += test
endif
endif

include $(topsrcdir)/config/rules.mk
