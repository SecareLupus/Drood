#
# ***** BEGIN LICENSE BLOCK *****
# Version: MPL 1.1/GPL 2.0/LGPL 2.1
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
#
# The Original Code is this file as it was released upon August 6, 1998.
#
# The Initial Developer of the Original Code is
# Christopher Seawood.
# Portions created by the Initial Developer are Copyright (C) 1998
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#   Benjamin Smedberg <benjamin@smedbergs.us>
#
# Alternatively, the contents of this file may be used under the terms of
# either of the GNU General Public License Version 2 or later (the "GPL"),
# or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
# in which case the provisions of the GPL or the LGPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of either the GPL or the LGPL, and not to allow others to
# use your version of this file under the terms of the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice
# and other provisions required by the GPL or the LGPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the MPL, the GPL or the LGPL.
#
# ***** END LICENSE BLOCK *****

# A netscape style .mk file for autoconf builds

INCLUDED_AUTOCONF_MK = 1
USE_AUTOCONF 	= 1
MOZILLA_CLIENT	= 1
BUILD_MODULES	= @BUILD_MODULES@
DROOD_VERSION = 2.9.7
MOZILLA_VERSION = 
FIREFOX_VERSION	= 3.0.5
THUNDERBIRD_VERSION	= 3.0a2pre
SUNBIRD_VERSION = 0.9
SEAMONKEY_VERSION	= 2.0a1pre
INSTANTBIRD_VERSION = 
PURPLE_MAJOR_VERSION = 
PURPLE_MINOR_VERSION = 
PURPLE_MICRO_VERSION = 

MOZ_BUILD_APP = drood
MOZ_APP_NAME	= drood
MOZ_APP_DISPLAYNAME = Drood
MOZ_APP_VERSION = 2.9.7

MOZ_PKG_SPECIAL = 

prefix		= /usr/local
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin
includedir	= ${prefix}/include/$(MOZ_APP_NAME)-$(MOZ_APP_VERSION)
libdir		= ${exec_prefix}/lib
datadir		= ${prefix}/share
mandir		= ${prefix}/man
idldir		= $(datadir)/idl/$(MOZ_APP_NAME)-$(MOZ_APP_VERSION)

installdir	= $(libdir)/$(MOZ_APP_NAME)-$(MOZ_APP_VERSION)
sdkdir		= $(libdir)/$(MOZ_APP_NAME)-devel-$(MOZ_APP_VERSION)

DIST		= $(DEPTH)/dist
LIBXUL_SDK      = 

L10NBASEDIR     = $(topsrcdir)/../l10n

ifdef LIBXUL_SDK
LIBXUL_DIST = $(LIBXUL_SDK)
else
LIBXUL_DIST = $(DIST)
endif

XULRUNNER_STUB_NAME = xulrunner-stub

MOZ_CHROME_FILE_FORMAT	= jar

MOZ_WIDGET_TOOLKIT	= gtk2
MOZ_GFX_TOOLKIT		= cairo
MOZ_X11			= 1

MOZ_JS_LIBS		   = -L$(LIBXUL_DIST)/bin -lmozjs

MOZ_DEBUG	= 
MOZ_DEBUG_MODULES = 
MOZ_PROFILE_MODULES = 
MOZ_DEBUG_ENABLE_DEFS		= -DDEBUG -D_DEBUG -DDEBUG_desmond -DTRACING
MOZ_DEBUG_DISABLE_DEFS	= -DNDEBUG -DTRIMMED
MOZ_DEBUG_FLAGS	= -g -fno-inline
MOZ_DEBUG_LDFLAGS=
MOZ_DBGRINFO_MODULES	= 
MOZ_EXTENSIONS  = inspector reporter
MOZ_IMG_DECODERS= png gif jpeg bmp xbm icon
MOZ_IMG_ENCODERS= png jpeg
MOZ_JSDEBUGGER  = 1
MOZ_PERF_METRICS = 
MOZ_LEAKY	= 
MOZ_MEMORY      = 1
MOZ_JPROF       = 
MOZ_SHARK       = 
MOZ_XPCTOOLS    = 
ENABLE_EAZEL_PROFILER=
EAZEL_PROFILER_CFLAGS=
EAZEL_PROFILER_LIBS=
GC_LEAK_DETECTOR = 
NS_TRACE_MALLOC = 
USE_ELF_DYNSTR_GC = 
USE_PREBINDING = 
INCREMENTAL_LINKER = 
MACOSX_DEPLOYMENT_TARGET = 
MOZ_MAIL_NEWS	= 
MOZ_CALENDAR	= 
MOZ_PLAINTEXT_EDITOR_ONLY = 
MOZ_COMPOSER = 
BUILD_STATIC_LIBS = 1
MOZ_ENABLE_LIBXUL = 
ENABLE_TESTS	= 
IBMBIDI = 1
MOZ_UNIVERSALCHARDET = 1
SUNCTL = 
ACCESSIBILITY = 1
MOZ_VIEW_SOURCE = 1
MOZ_XPINSTALL = 1
MOZ_JSLOADER  = 1
MOZ_USE_NATIVE_UCONV = 
MOZ_LDAP_XPCOM = 
MOZ_LDAP_XPCOM_EXPERIMENTAL = 
MOZ_BRANDING_DIRECTORY = other-licenses/branding/drood
XPCOM_USE_LEA = 
JS_ULTRASPARC_OPTS = 
JS_STATIC_BUILD = 
MOZ_ENABLE_POSTSCRIPT = 1
MOZ_INSTALLER	= 1
MOZ_UPDATER	= 
MOZ_UPDATE_PACKAGING	= 
MOZ_NO_ACTIVEX_SUPPORT = 1
MOZ_ACTIVEX_SCRIPTING_SUPPORT = 
MOZ_DISABLE_VISTA_SDK_REQUIREMENTS = 
MOZ_DISABLE_PARENTAL_CONTROLS = 
XPC_IDISPATCH_SUPPORT = 
MOZ_SPELLCHECK = 1
MOZ_XPFE_COMPONENTS = 1
MOZ_IPCD = 
MOZ_PROFILESHARING = 
MOZ_PROFILELOCKING = 1
MOZ_FEEDS = 1
MOZ_PLACES = 
MOZ_PLACES_BOOKMARKS = 
MOZ_STORAGE = 1
MOZ_SAFE_BROWSING = 
MOZ_URL_CLASSIFIER = 
MOZ_ZIPWRITER = 1
MOZ_MORK = 1
MOZ_MORKREADER = 
MOZ_NO_XPCOM_OBSOLETE = 1
MOZ_NO_FAST_LOAD = 
NS_PRINTING = 1
MOZ_CRASHREPORTER = 
MOZ_MOCHITEST = 1
MOZ_HELP_VIEWER = 

MOZ_JAVAXPCOM = 
JAVA_INCLUDE_PATH=""
JAVA=""
JAVAC=""
JAR=""

TAR=tar

# The MOZ_UI_LOCALE var is used to build a particular locale. Do *not*
# use the var to change any binary files. Do *not* use this var unless you
# write rules for the "clean-locale" and "locale" targets.
MOZ_UI_LOCALE = en-US

MOZ_COMPONENTS_VERSION_SCRIPT_LDFLAGS = -Wl,--version-script -Wl,$(BUILD_TOOLS)/gnu-ld-scripts/components-version-script
MOZ_COMPONENT_NSPR_LIBS=-L$(LIBXUL_DIST)/bin $(NSPR_LIBS)

MOZ_FIX_LINK_PATHS=-Wl,-rpath-link,$(LIBXUL_DIST)/bin

XPCOM_FROZEN_LDOPTS=-L$(LIBXUL_DIST)/bin -lxpcom
XPCOM_LIBS=-L$(LIBXUL_DIST)/bin -lxpcom -lxpcom_core 
MOZ_TIMELINE=

ENABLE_STRIP	= 
PKG_SKIP_STRIP	= 

ClientWallet=1
CookieManagement=1
SingleSignon=1

MOZ_OJI		= 1
MOZ_PLUGINS	= 1

MOZ_POST_DSO_LIB_COMMAND = 
MOZ_POST_PROGRAM_COMMAND = 

MOZ_BUILD_ROOT             = /home/desmond/Programming/Drood/Drood/mozilla

MOZ_XUL                    = 1
MOZ_RDF                    = 1

NECKO_PROTOCOLS = about data file ftp gopher http res viewsource
NECKO_DISK_CACHE = 1
NECKO_SMALL_BUFFERS = 
NECKO_COOKIES = 1
MOZ_AUTH_EXTENSION = 1

MOZ_NATIVE_HUNSPELL = 
MOZ_HUNSPELL_LIBS = 
MOZ_HUNSPELL_CFLAGS = 

MOZ_NATIVE_ZLIB	= 
MOZ_NATIVE_BZ2	= 
MOZ_NATIVE_JPEG	= 
MOZ_NATIVE_PNG	= 
MOZ_TREE_CAIRO = 1

MOZ_UPDATE_XTERM = 
MOZ_MATHML = 1
MOZ_PERMISSIONS = 1
MOZ_XTF = 1
MOZ_XMLEXTRAS = 1
MOZ_NO_INSPECTOR_APIS = 
MOZ_WEBSERVICES = 
MOZ_SVG = 1
MOZ_SVG_FOREIGNOBJECT = 1
MOZ_LIBART_CFLAGS = 
MOZ_ENABLE_CANVAS = 1
MOZ_CAIRO_CFLAGS = -I$(LIBXUL_DIST)/include/cairo
MOZ_XSLT_STANDALONE = 

MOZ_PREF_EXTENSIONS = 1

MOZ_CAIRO_LIBS = $(DEPTH)/gfx/cairo/cairo/src/$(LIB_PREFIX)mozcairo.$(LIB_SUFFIX) $(DEPTH)/gfx/cairo/libpixman/src/$(LIB_PREFIX)mozlibpixman.$(LIB_SUFFIX)   -lXrender -lfreetype -lfontconfig

MOZ_ENABLE_GNOMEUI = 
MOZ_GNOMEUI_CFLAGS = 
MOZ_GNOMEUI_LIBS = 

MOZ_ENABLE_STARTUP_NOTIFICATION = 
MOZ_STARTUP_NOTIFICATION_CFLAGS = 
MOZ_STARTUP_NOTIFICATION_LIBS = 

MOZ_GNOMEVFS_CFLAGS = 
MOZ_GNOMEVFS_LIBS = 

MOZ_ENABLE_GCONF = 1
MOZ_GCONF_CFLAGS = -pthread -I/usr/include/gconf/2 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
MOZ_GCONF_LIBS = -lgconf-2 -lglib-2.0

MOZ_LIBGNOME_CFLAGS = 
MOZ_LIBGNOME_LIBS = 

MOZ_ENABLE_GNOME_COMPONENT = 

MOZ_INSURE = 
MOZ_INSURIFYING = 
MOZ_INSURE_DIRS = 
MOZ_INSURE_EXCLUDE_DIRS = 

MOZ_NATIVE_NSPR = 
MOZ_NATIVE_NSS = 

CROSS_COMPILE   = 

OS_CPPFLAGS	=  
OS_CFLAGS	= $(OS_CPPFLAGS) -Wall -W -Wno-unused -Wpointer-arith -Wcast-align -W -Wno-long-long -pedantic -fno-strict-aliasing -pthread -pipe
OS_CXXFLAGS	= $(OS_CPPFLAGS) -fno-rtti -fno-exceptions -Wall -Wconversion -Wpointer-arith -Woverloaded-virtual -Wsynth -Wno-ctor-dtor-privacy -Wno-non-virtual-dtor -Wcast-align -Wno-long-long -pedantic -fno-strict-aliasing -fshort-wchar -pthread -pipe
OS_LDFLAGS	= -lpthread 

OS_COMPILE_CFLAGS = $(OS_CPPFLAGS) -include $(DEPTH)/mozilla-config.h -DMOZILLA_CLIENT $(filter-out %/.pp,-Wp,-MD,$(MDDEPDIR)/$(*F).pp)
OS_COMPILE_CXXFLAGS = $(OS_CPPFLAGS) -DMOZILLA_CLIENT -include $(DEPTH)/mozilla-config.h $(filter-out %/.pp,-Wp,-MD,$(MDDEPDIR)/$(*F).pp)

OS_INCLUDES	= $(NSPR_CFLAGS) $(JPEG_CFLAGS) $(PNG_CFLAGS) $(ZLIB_CFLAGS) $(LCMS_CFLAGS)
OS_LIBS		= -ldl -lm 
ACDEFINES	=  -DHAVE_64BIT_OS=1 -DMOZILLA_VERSION=\"\" -DMOZILLA_VERSION_U= -DD_INO=d_ino -DSTDC_HEADERS=1 -DHAVE_ST_BLKSIZE=1 -DHAVE_SIGINFO_T=1 -DHAVE_INT16_T=1 -DHAVE_INT32_T=1 -DHAVE_INT64_T=1 -DHAVE_UINT=1 -DHAVE_UNAME_DOMAINNAME_FIELD=1 -DHAVE_VISIBILITY_HIDDEN_ATTRIBUTE=1 -DHAVE_VISIBILITY_ATTRIBUTE=1 -DHAVE_DIRENT_H=1 -DHAVE_GETOPT_H=1 -DHAVE_SYS_BITYPES_H=1 -DHAVE_MEMORY_H=1 -DHAVE_UNISTD_H=1 -DHAVE_GNU_LIBC_VERSION_H=1 -DHAVE_NL_TYPES_H=1 -DHAVE_MALLOC_H=1 -DHAVE_X11_XKBLIB_H=1 -DHAVE_SYS_STATVFS_H=1 -DHAVE_SYS_STATFS_H=1 -DHAVE_MMINTRIN_H=1 -DHAVE_SYS_CDEFS_H=1 -DHAVE_LIBM=1 -DHAVE_LIBDL=1 -DHAVE_DLADDR=1 -DFUNCPROTO=15 -DHAVE_XSHM=1 -DHAVE_FT_BITMAP_SIZE_Y_PPEM=1 -DHAVE_FT_GLYPHSLOT_EMBOLDEN=1 -DHAVE_FT_LOAD_SFNT_TABLE=1 -DHAVE_FT_SELECT_SIZE=1 -D_REENTRANT=1 -DHAVE_RANDOM=1 -DHAVE_STRERROR=1 -DHAVE_LCHOWN=1 -DHAVE_FCHMOD=1 -DHAVE_SNPRINTF=1 -DHAVE_MEMMOVE=1 -DHAVE_RINT=1 -DHAVE_STAT64=1 -DHAVE_LSTAT64=1 -DHAVE_TRUNCATE64=1 -DHAVE_FLOCKFILE=1 -DHAVE_LOCALTIME_R=1 -DHAVE_STRTOK_R=1 -DHAVE_RES_NINIT=1 -DHAVE_GNU_GET_LIBC_VERSION=1 -DHAVE_LANGINFO_CODESET=1 -DVA_COPY=va_copy -DHAVE_VA_COPY=1 -DHAVE_VA_LIST_AS_ARRAY=1 -DHAVE_I18N_LC_MESSAGES=1 -DMOZ_EMBEDDING_LEVEL_DEFAULT=1 -DMOZ_EMBEDDING_LEVEL_BASIC=1 -DMOZ_EMBEDDING_LEVEL_MINIMAL=1 -DMOZ_BUILD_APP=drood -DMOZ_XUL_APP=1 -DMOZ_DEFAULT_TOOLKIT=\"cairo-gtk2\" -DMOZ_WIDGET_GTK2=1 -DMOZ_ENABLE_XREMOTE=1 -DMOZ_X11=1 -DMOZ_DISTRIBUTION_ID=\"org.mozilla\" -DMOZ_ENABLE_GCONF=1 -DMOZ_ENABLE_DBUS=1 -DOJI=1 -DIBMBIDI=1 -DMOZ_VIEW_SOURCE=1 -DACCESSIBILITY=1 -DMOZ_XPINSTALL=1 -DMOZ_JSLOADER=1 -DNS_PRINTING=1 -DNS_PRINT_PREVIEW=1 -DMOZ_NO_XPCOM_OBSOLETE=1 -DMOZ_XTF=1 -DMOZ_CRASHREPORTER_ENABLE_PERCENT=100 -DMOZ_MATHML=1 -DMOZ_ENABLE_CANVAS=1 -DMOZ_SVG=1 -DMOZ_SVG_FOREIGNOBJECT=1 -DMOZ_UPDATE_CHANNEL=default -DMOZ_FEEDS=1 -DMOZ_STORAGE=1 -DMOZ_LOGGING=1 -DSIZEOF_INT_P=8 -DMOZ_MEMORY_SIZEOF_PTR_2POW=3 -DMOZ_MEMORY=1 -DMOZ_MEMORY_LINUX=1 -DHAVE___CXA_DEMANGLE=1 -DHAVE__UNWIND_BACKTRACE=1 -DMOZ_USER_DIR=\".drood\" -DMOZ_STATIC_BUILD=1 -DHAVE_ARPA_NAMESER_COMPAT_H=1 -DHAVE_INET_NTOP=1 -DSIZEOF_TIME_T=8 -DHAVE_TIMEZONE=1 -DHAVE_TM_GMTOFF=1 -DHAVE_STDINT_H=1 -DHAVE_INTTYPES_H=1 -DMOZ_TREE_CAIRO=1 -DHAVE_UINT64_T=1 -DMOZ_XUL=1 -DMOZ_PROFILELOCKING=1 -DMOZ_RDF=1 -DMOZ_MORK=1 -DMOZ_DLL_SUFFIX=\".so\" -DXP_UNIX=1 -DUNIX_ASYNC_DNS=1 -DJS_THREADSAFE=1 -DMOZ_ACCESSIBILITY_ATK=1 -DATK_MAJOR_VERSION=2 -DATK_MINOR_VERSION=22 -DATK_REV_VERSION=0 -DMOZILLA_LOCALE_VERSION=\"1.9a1\" -DMOZILLA_REGION_VERSION=\"1.9a1\" -DMOZILLA_SKIN_VERSION=\"1.8\" 

WARNINGS_AS_ERRORS = -Werror

MOZ_OPTIMIZE	= 1
MOZ_OPTIMIZE_FLAGS = -Os -freorder-blocks -fno-reorder-functions 
MOZ_OPTIMIZE_LDFLAGS = 
MOZ_OPTIMIZE_SIZE_TWEAK = 

MOZ_RTTI_FLAGS_ON = -frtti

MOZ_PROFILE_GUIDED_OPTIMIZE_DISABLE = 
PROFILE_GEN_CFLAGS = -fprofile-generate
PROFILE_GEN_LDFLAGS = -fprofile-generate
PROFILE_USE_CFLAGS = -fprofile-use
PROFILE_USE_LDFLAGS = -fprofile-use

XCFLAGS		= 
XLDFLAGS	= 
XLIBS		= -lX11 

WIN_TOP_SRC	= 
CYGWIN_WRAPPER	= 
AS_PERL         = 
CYGDRIVE_MOUNT	= 
AR		= ar
AR_FLAGS	= cr $@
AR_EXTRACT	= $(AR) x
AR_LIST		= $(AR) t
AR_DELETE	= $(AR) d
AS		= $(CC)
ASFLAGS		= 
AS_DASH_C_FLAG	= -c
LD		= ld
RC		= 
RCFLAGS		= 
WINDRES		= :
USE_SHORT_LIBNAME = 
IMPLIB		= 
FILTER		= 
BIN_FLAGS	= 
MIDL		= 
MIDL_FLAGS	= 
_MSC_VER	= 

DLL_PREFIX	= lib
LIB_PREFIX	= lib
OBJ_SUFFIX	= o
LIB_SUFFIX	= a
DLL_SUFFIX	= .so
BIN_SUFFIX	= 
ASM_SUFFIX	= s
IMPORT_LIB_SUFFIX = 
USE_N32		= 
HAVE_64BIT_OS	= 1

# Temp hack.  It is not my intention to leave this crap in here for ever.
# Im talking to fur right now to solve the problem without introducing 
# NS_USE_NATIVE to the build system -ramiro.
NS_USE_NATIVE = 

CC		    = gcc
CXX		    = c++

GNU_AS		= 1
GNU_LD		= 1
GNU_CC		= 1
GNU_CXX		= 1
HAVE_GCC3_ABI	= 1
INTEL_CC	= 
INTEL_CXX	= 

HOST_CC		= gcc
HOST_CXX	= c++
HOST_CFLAGS	=  -DXP_UNIX
HOST_CXXFLAGS	= 
HOST_OPTIMIZE_FLAGS = -O3
HOST_NSPR_MDCPUCFG = \"md/_linux.cfg\"
HOST_AR		= $(AR)
HOST_AR_FLAGS	= $(AR_FLAGS)
HOST_LD		= 
HOST_RANLIB	= ranlib
HOST_BIN_SUFFIX	= 

HOST_LIBIDL_CONFIG = 
HOST_LIBIDL_CFLAGS = 
HOST_LIBIDL_LIBS   = 

HOST_OS_ARCH	= Linux
host_cpu	= x86_64
host_vendor	= unknown
host_os		= linux-gnu

TARGET_NSPR_MDCPUCFG = \"md/_linux.cfg\"
TARGET_CPU	= x86_64
TARGET_VENDOR	= unknown
TARGET_OS	= linux-gnu
TARGET_MD_ARCH	= unix
TARGET_XPCOM_ABI = x86_64-gcc3

AUTOCONF	= /usr/bin/autoconf
PERL		= /usr/bin/perl
PYTHON		= /usr/bin/python
RANLIB		= ranlib
WHOAMI		= /usr/bin/whoami
UNZIP		= /usr/bin/unzip
ZIP		= /usr/bin/zip
XARGS		= /usr/bin/xargs
STRIP		= strip
DOXYGEN		= :
MAKE		= /usr/bin/make
PBBUILD_BIN	= 
SDP		= 
NSINSTALL_BIN	= 
WGET		= wget

ifdef MOZ_NATIVE_JPEG
JPEG_CFLAGS	= 
JPEG_LIBS	= 
JPEG_REQUIRES	=
else
JPEG_CFLAGS	= 
JPEG_LIBS	= $(call EXPAND_LIBNAME_PATH,mozjpeg,$(DEPTH)/jpeg)
JPEG_REQUIRES	= jpeg
endif

ifdef MOZ_NATIVE_ZLIB
ZLIB_CFLAGS	= 
ZLIB_LIBS	= 
ZLIB_REQUIRES	=
else
ZLIB_CFLAGS	= 
ifdef MOZ_ENABLE_LIBXUL
MOZ_ZLIB_LIBS = $(call EXPAND_LIBNAME_PATH,mozz,$(DEPTH)/modules/zlib/src)
else
ZLIB_LIBS = $(call EXPAND_LIBNAME_PATH,mozz,$(DEPTH)/modules/zlib/src)
endif
ZLIB_REQUIRES	= zlib
endif

ifdef MOZ_NATIVE_BZ2
BZ2_CFLAGS	= 
BZ2_LIBS	= 
BZ2_REQUIRES	=
else
BZ2_CFLAGS	= 
BZ2_LIBS	= $(call EXPAND_LIBNAME_PATH,bz2,$(DEPTH)/modules/libbz2/src)
BZ2_REQUIRES	= libbz2
endif

ifdef MOZ_NATIVE_PNG
PNG_CFLAGS	= 
PNG_LIBS	= 
PNG_REQUIRES	=
else
PNG_CFLAGS	= 
PNG_LIBS	= $(call EXPAND_LIBNAME_PATH,mozpng,$(DEPTH)/modules/libimg/png)
PNG_REQUIRES	= png
endif

MOZ_NATIVE_LCMS	= 
LCMS_CFLAGS	= 
LCMS_LIBS	= -L$(LIBXUL_DIST)/bin -lmozlcms
ifdef MOZ_NATIVE_LCMS
LCMS_REQUIRES	=
else
LCMS_REQUIRES	= lcms
endif

MOZ_NATIVE_SQLITE = 
SQLITE_CFLAGS     = 
SQLITE_LIBS       = $(call EXPAND_LIBNAME_PATH,sqlite3,$(DIST)/lib)

NSPR_CONFIG	= 
NSPR_CFLAGS = -I$(LIBXUL_DIST)/include/nspr
NSPR_LIBS = -L$(LIBXUL_DIST)/lib -lplds4 -lplc4 -lnspr4 -lpthread -ldl

NSS_CONFIG	= 
NSS_CFLAGS	= -I$(LIBXUL_DIST)/include/nss
NSS_LIBS	= $(LIBS_DIR) -lcrmf -lsmime3 -lssl3 -lnss3 -lnssutil3 -lsoftokn3
NSS_DEP_LIBS	=         $(LIBXUL_DIST)/lib/$(LIB_PREFIX)crmf.$(LIB_SUFFIX)         $(LIBXUL_DIST)/lib/$(DLL_PREFIX)smime3$(DLL_SUFFIX)         $(LIBXUL_DIST)/lib/$(DLL_PREFIX)ssl3$(DLL_SUFFIX)         $(LIBXUL_DIST)/lib/$(DLL_PREFIX)nss3$(DLL_SUFFIX)         $(LIBXUL_DIST)/lib/$(DLL_PREFIX)nssutil3$(DLL_SUFFIX)         $(LIBXUL_DIST)/lib/$(DLL_PREFIX)softokn3$(DLL_SUFFIX)

LDAP_CFLAGS	= 
LDAP_LIBS	= 
XPCOM_GLUE_LDOPTS = $(LIBXUL_DIST)/lib/$(LIB_PREFIX)xpcomglue_s.$(LIB_SUFFIX) $(XPCOM_FROZEN_LDOPTS)
XPCOM_STANDALONE_GLUE_LDOPTS = $(LIBXUL_DIST)/lib/$(LIB_PREFIX)xpcomglue.$(LIB_SUFFIX)
MOZ_XPCOM_OBSOLETE_LIBS = -L$(LIBXUL_DIST)/lib -lxpcom_compat

USE_DEPENDENT_LIBS = 1

# UNIX98 iconv support
LIBICONV = 

# MKSHLIB_FORCE_ALL is used to force the linker to include all object
# files present in an archive. MKSHLIB_UNFORCE_ALL reverts the linker
# to normal behavior. Makefile's that create shared libraries out of
# archives use these flags to force in all of the .o files in the
# archives into the shared library.
WRAP_MALLOC_LIB         = 
WRAP_MALLOC_CFLAGS      = 
DSO_CFLAGS              = 
DSO_PIC_CFLAGS          = -fPIC
MKSHLIB                 = $(CXX) $(CXXFLAGS) $(DSO_PIC_CFLAGS) $(DSO_LDOPTS) -Wl,-h,$@ -o $@
MKCSHLIB                = $(CC) $(CFLAGS) $(DSO_PIC_CFLAGS) $(DSO_LDOPTS) -Wl,-h,$@ -o $@
MKSHLIB_FORCE_ALL       = -Wl,--whole-archive
MKSHLIB_UNFORCE_ALL     = -Wl,--no-whole-archive
DSO_LDOPTS              = -shared -Wl,-z,defs
DLL_SUFFIX              = .so

NO_LD_ARCHIVE_FLAGS     = 

GTK_CONFIG	= 
TK_CFLAGS	= $(MOZ_GTK2_CFLAGS)
TK_LIBS		= $(MOZ_GTK2_LIBS) 

MOZ_TOOLKIT_REGISTRY_CFLAGS = \
	$(TK_CFLAGS)

CAIRO_FT_CFLAGS		= -I/usr/include/freetype2

MOZ_ENABLE_CAIRO_FT	= 1
MOZ_ENABLE_GLITZ        = 
MOZ_ENABLE_GTK2		= 1
MOZ_ENABLE_PHOTON	= 
MOZ_ENABLE_COCOA	= 
MOZ_ENABLE_XREMOTE	= 1

MOZ_GTK2_CFLAGS		= -pthread -I/usr/include/gtk-2.0 -I/usr/include/gtk-unix-print-2.0 -I/usr/include/gtk-2.0 -I/usr/include/atk-1.0 -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
MOZ_GTK2_LIBS		= -lgtk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lfontconfig -lfreetype -lgdk-x11-2.0 -lpangocairo-1.0 -lpango-1.0 -lcairo -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0

MOZ_DBUS_GLIB_CFLAGS    = -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
MOZ_DBUS_GLIB_LIBS      = -ldbus-glib-1 -ldbus-1 -lgobject-2.0 -lglib-2.0
MOZ_ENABLE_DBUS         = 1

MOZ_GTHREAD_CFLAGS      = 
MOZ_GTHREAD_LIBS        = 

FT2_CFLAGS             = -I/usr/include/freetype2
FT2_LIBS               = -lfreetype

MOZ_XFT_CFLAGS		= -I/usr/include/freetype2
MOZ_XFT_LIBS		= -lXft
MOZ_PANGO_CFLAGS        = -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/libpng16 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/freetype2
MOZ_PANGO_LIBS          = -lpangocairo-1.0 -lcairo -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype

MOZ_XIE_LIBS		= 
XT_LIBS			= -lXt

GLIB_CFLAGS	= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
GLIB_LIBS	= -lgobject-2.0 -lglib-2.0
GLIB_GMODULE_LIBS	= 
LIBIDL_CFLAGS = -I/usr/include/libIDL-2.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LIBIDL_LIBS = -lIDL-2 -lgobject-2.0 -lglib-2.0
STATIC_LIBIDL = 

MOZ_NATIVE_MAKEDEPEND	= 

# Used for LD_LIBRARY_PATH
LIBS_PATH       = 

MOZ_AUTO_DEPS	= 1
COMPILER_DEPEND = 1
MDDEPDIR        := .deps

MOZ_DEMANGLE_SYMBOLS = 

# XXX - these need to be cleaned up and have real checks added -cls
CM_BLDTYPE=dbg
AWT_11=1
MOZ_BITS=32
OS_TARGET=Linux
OS_ARCH=Linux
OS_RELEASE=4.10.0-32-generic
OS_TEST=x86_64

TARGET_DEVICE = 

# For AIX build
AIX_OBJMODEL = 

# For OS/2 build
MOZ_OS2_TOOLS = 
MOZ_OS2_USE_DECLSPEC = 
MOZ_OS2_HIGH_MEMORY = 1

HAVE_XIE=

MOZ_MOVEMAIL=1
MOZ_PSM=1

# Gssapi (krb5) libraries and headers for the Negotiate auth method
GSSAPI_INCLUDES = @GSSAPI_INCLUDES@
USE_GSSAPI	= @USE_GSSAPI@

MOZILLA_OFFICIAL = 
BUILD_OFFICIAL = 
MOZ_MILESTONE_RELEASE = 

# Win32 options
MOZ_PROFILE	= 
MOZ_BROWSE_INFO	= 
MOZ_TOOLS_DIR	= 
MOZ_DEBUG_SYMBOLS = 
MOZ_QUANTIFY	= 
MSMANIFEST_TOOL = 
WIN32_REDIST_DIR = ./redist/microsoft/system
WIN32_CRT_SRC_DIR = 
WIN32_CUSTOM_CRT_DIR = 
# These are for custom CRT building
ifdef MOZ_MEMORY
ifneq (,$(WIN32_CRT_SRC_DIR)$(WIN32_CUSTOM_CRT_DIR))
export LIB = 
export PATH = 
DLLFLAGS = 
endif
endif

# python options.
MOZ_PYTHON_EXTENSIONS = xpcom dom
MOZ_PYTHON = 
MOZ_PYTHON_PREFIX = 
MOZ_PYTHON_INCLUDES = 
MOZ_PYTHON_LIBS = 
MOZ_PYTHON_DEBUG_SUFFIX = 
MOZ_PYTHON_DLL_SUFFIX = 
MOZ_PYTHON_VER_DOTTED = 
MOZ_PYTHON_VER = 

# Codesighs tools option, enables win32 mapfiles.
MOZ_MAPINFO	= 

GF_DROOD	= 1
MOZ_PHOENIX	= 
MOZ_XULRUNNER	= 
MOZ_XUL_APP	= 1
MOZ_THUNDERBIRD = 
MOZ_STANDALONE_COMPOSER= 
MOZ_STATIC_MAIL_BUILD = 
MOZ_SUNBIRD	= 
MOZ_SUITE	= 
WINCE		= 

MOZ_DISTRIBUTION_ID = org.mozilla

MINIMO		= 

MACOS_SDK_DIR	= 
NEXT_ROOT	= 
GCC_VERSION	= 6.3.0
XCODEBUILD_VERSION= 
HAS_XCODE_2_1	= 
UNIVERSAL_BINARY= 
HAVE_DTRACE= 

VISIBILITY_FLAGS = -I$(DIST)/include/system_wrappers -include $(topsrcdir)/config/gcc_hidden.h
WRAP_SYSTEM_INCLUDES = 1

MOZ_V1_STRING_ABI = 

MOZ_EMBEDDING_LEVEL_DEFAULT = 1
MOZ_EMBEDDING_LEVEL_BASIC = 1
MOZ_EMBEDDING_LEVEL_MINIMAL = 1
