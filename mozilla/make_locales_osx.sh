#!/bin/sh -e

BUILD_OFFICIAL=1

ln -sf mozconfig-nodebug-osx .mozconfig

version=`cat drood/config/version.txt`

for locale in "$@"
do
  rm -rf ../objdir/*/dist
  sed "s/%LOCALE%/$locale/" mozconfig-nodebug-osx.in > mozconfig-nodebug-osx
  make -f client.mk build
  hdiutil attach ~/Development/Drood.dmg
  rm -rf /Volumes/Drood/Drood.app/Contents
  cp -RL ../objdir/ppc/dist/universal/drood/Drood.app/Contents /Volumes/Drood/Drood.app/Contents
  hdiutil detach /Volumes/Drood
  hdiutil convert -format UDZO -o ~/Development/Release/Drood-$version-$locale.dmg ~/Development/Drood.dmg
done

