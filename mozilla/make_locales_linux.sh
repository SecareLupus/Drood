#!/bin/sh -e

BUILD_OFFICIAL=1

ln -sf mozconfig-nodebug-linux .mozconfig

for locale in "$@"
do
  rm -rf ../objdir/dist/bin/dictionaries/*.* ../objdir/drood/installer/packages-static
  sed "s/%LOCALE%/$locale/" mozconfig-nodebug-linux.in > mozconfig-nodebug-linux
  VERBOSE=1 make -f client.mk build
  cd ../objdir/drood/installer/
  VERBOSE=1 make
  cd ../../../mozilla
done
