OTHERFLAGS := -I/usr/include/gtk-2.0 -I/usr/include/cairo -I/usr/include/pango-1.0
OTHERFLAGS += -I/usr/lib/$(shell arch)-linux-gnu/gtk-2.0/include -I/usr/include/gdk-pixbuf-2.0
OTHERFLAGS += -I/usr/include/atk-1.0 -I/usr/include/freetype2
OTHERFLAGS += -I/usr/include/gtk-unix-print-2.0 -I/usr/include/dbus-1.0
OTHERFLAGS += -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include
GLIBFLAGS := -I/usr/lib/$(shell arch)-linux-gnu/glib-2.0/include -I/usr/include/glib-2.0
OTHERFLAGS += $(GLIBFLAGS)
MFLAGS := CXXFLAGS=-"Wno-error=narrowing -fshort-wchar -fpermissive $(OTHERFLAGS)" CPPFLAGS+=" $(OTHERFLAGS)"

BDIR := mozilla
IDIR := objdir/celtx/installer
DESTDIR := /usr/local
all: $(BDIR)/mozconfig-nodebug-linux $(BDIR)/.mozconfig
	cd $(BDIR); make -f client.mk $(MFLAGS) build
	cd $(IDIR); make

$(BDIR)/mozconfig-nodebug-linux:
	sed 's/%LOCALE%/en-US/' \
	  $(BDIR)/mozconfig-nodebug-linux.in > $(BDIR)/mozconfig-nodebug-linux

$(BDIR)/.mozconfig:
	ln -sf mozconfig-nodebug-linux $(BDIR)/.mozconfig

VER := 0.1.3


PKGNAME := open-celtx
TFILE := $(PKGNAME)_$(VER).tar

archive: $(TFILE)

$(TFILE):
	git archive -o $(TFILE) --prefix $(PKGNAME)-$(VER)/ HEAD
	git tag v$(VER) HEAD

install:
	install -d $(DESTDIR)/usr/celtx
	cp -a objdir/dist/celtx/. $(DESTDIR)/usr/celtx

clean:
	rm -rf objdir core mozilla/config/Expression.pyc mozilla/config/configobj.pyc
	rm -f $(BDIR)/.mozconfig $(BDIR)/mozconfig-nodebug-linux
	rm -f $(BDIR)/.mozconfig.mk $(BDIR)/.mozconfig.out
