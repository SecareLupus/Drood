Open-Celtx

Script Writing and Production Suite
=====

 The following was previously recommended in the README but currently I am not sure which
 ones are still valid. "It compiles for me" :-)

apt-get update
apt-get upgrade
apt-get install build-essential libasound2-dev libcurl4-openssl-dev libnotify-dev \
	libxt-dev libiw-dev mesa-common-dev autoconf2.13 yasm libidl-dev \
	libfreetype6-dev zlib1g-dev

# Not sure if this is necessary
apt-get build-dep firefox-esr

Type make to build on my amd64 Stretch release  debian Linux

When compiled run objdir/dist/celtx/celtx if happy install with make install

This Makefile has clunky extra compile flags to make it find everything which are
not necessary in my debian packaged version..
My debhelper rules is much simpler :

    export DEB_BUILD_MAINT_OPTIONS = hardening=-fortify,-stackprotector,-format,-stackprotectorstrong
    export DEB_CXXFLAGS_APPEND  = -Wno-error=narrowing
