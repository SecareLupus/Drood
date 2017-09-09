# -*- Mode: Makefile -*-

INCLUDED_AUTOCONF_MK = 1
USE_AUTOCONF	= 1

MOZILLA_CLIENT	= 1

prefix		= /usr/local
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin
includedir	= ${prefix}/include/nspr
libdir		= ${exec_prefix}/lib
datadir		= ${prefix}/share

dist_prefix	= /home/desmond/Programming/Drood/Drood/mozilla/dist
dist_bindir	= ${dist_prefix}/bin
dist_includedir = ${dist_prefix}/include/nspr
dist_libdir	= ${dist_prefix}/lib

DIST		= $(dist_prefix)

RELEASE_OBJDIR_NAME = Linux4.10_x86_64_PTH_OPT.OBJ
OBJDIR_NAME	= .
OBJDIR		= $(OBJDIR_NAME)
OBJ_SUFFIX	= o
LIB_SUFFIX	= a
DLL_SUFFIX	= so
ASM_SUFFIX	= s
MOD_NAME	= nspr20

MOD_MAJOR_VERSION = 4
MOD_MINOR_VERSION = 7
MOD_PATCH_VERSION = 1

LIBNSPR		= -L$(dist_libdir) -lnspr$(MOD_MAJOR_VERSION)
LIBPLC		= -L$(dist_libdir) -lplc$(MOD_MAJOR_VERSION)

CROSS_COMPILE	= 
BUILD_OPT	= 1

USE_CPLUS	= 
USE_IPV6	= 
USE_N32		= 
USE_64		= 1
GC_LEAK_DETECTOR = 
ENABLE_STRIP	= 

USE_PTHREADS	= 1
USE_BTHREADS	= 
PTHREADS_USER	= 
CLASSIC_NSPR	= 

AS		= $(CC)
ASFLAGS		= $(CFLAGS)
CC		= gcc
CCC		= 
NS_USE_GCC	= 1
GCC_USE_GNU_LD	= 
MSC_VER		= 
AR		= /usr/bin/ar
AR_FLAGS	= cr $@
LD		= /usr/bin/ld
RANLIB		= ranlib
PERL		= /usr/bin/perl
RC		= 
RCFLAGS		= 
STRIP		= /usr/bin/strip
NSINSTALL	= $(MOD_DEPTH)/config/$(OBJDIR_NAME)/nsinstall
FILTER		= 
IMPLIB		= 
CYGWIN_WRAPPER	= 
MT		= 

OS_CPPFLAGS	= 
OS_CFLAGS	= $(OS_CPPFLAGS)  -Wall -pthread -O2 $(DSO_CFLAGS)
OS_CXXFLAGS	= $(OS_CPPFLAGS)  -Wall -pthread -O2 $(DSO_CFLAGS)
OS_LIBS         = -lpthread -ldl 
OS_LDFLAGS	= 
OS_DLLFLAGS	= 
DLLFLAGS	= 
EXEFLAGS  = 
OPTIMIZER	= 

MKSHLIB		= $(CC) $(DSO_LDOPTS) -o $@
DSO_CFLAGS	= -fPIC
DSO_LDOPTS	= -shared -Wl,-soname -Wl,$(notdir $@)

RESOLVE_LINK_SYMBOLS = 

HOST_CC		= gcc
HOST_CFLAGS	=  -DXP_UNIX
HOST_LDFLAGS	= 

DEFINES		=  -UDEBUG  -DMOZILLA_CLIENT=1 -DNDEBUG=1 -DHAVE_VISIBILITY_HIDDEN_ATTRIBUTE=1 -DHAVE_VISIBILITY_PRAGMA=1 -DXP_UNIX=1 -D_GNU_SOURCE=1 -DHAVE_FCNTL_FILE_LOCKING=1 -DLINUX=1 -DHAVE_LCHOWN=1 -DHAVE_STRERROR=1 -D_REENTRANT=1 

MDCPUCFG_H	= _linux.cfg
PR_MD_CSRCS	= linux.c
PR_MD_ASFILES	= os_Linux_x86_64.s
PR_MD_ARCH_DIR	= unix
CPU_ARCH	= x86

OS_TARGET	= Linux
OS_ARCH		= Linux
OS_RELEASE	= 4.10
OS_TEST		= x86_64

NOSUCHFILE	= /no-such-file
AIX_LINK_OPTS	= 
MOZ_OBJFORMAT	= 
ULTRASPARC_LIBRARY = 

OBJECT_MODE	= 
ifdef OBJECT_MODE
export OBJECT_MODE
endif

VISIBILITY_FLAGS = -fvisibility=hidden
WRAP_SYSTEM_INCLUDES = 

MACOSX_DEPLOYMENT_TARGET = 
ifdef MACOSX_DEPLOYMENT_TARGET
export MACOSX_DEPLOYMENT_TARGET
endif

MACOS_SDK_DIR	= 

NEXT_ROOT	= 
ifdef NEXT_ROOT
export NEXT_ROOT
endif
