#!/bin/bash

########################################################################
# Package the binaries built on Travis-CI as an AppImage
# By Simon Peter 2016
# For more information, see http://appimage.org/
########################################################################

sudo apt-get -y install coreutils binutils
export ARCH=$(arch)

APP=Sturdy
LOWERAPP=${APP,,}
GIT_REV=$(git rev-parse --short HEAD)
echo $GIT_REV

sudo make install

sudo chown -R $USER /app/
# Patch hardcoded /app away
find /app/ -type f -exec sed -i -e "s|/app|././|g" {} \;

sudo mkdir -p /$APP/$APP.AppDir/
sudo mv /app /$APP/$APP.AppDir/usr
sudo chown -R $USER /$APP/

cd /$APP/
wget -q https://github.com/probonopd/AppImages/raw/master/functions.sh -O ./functions.sh
. ./functions.sh

cd $APP.AppDir/

########################################################################
# Copy desktop and icon file to AppDir for AppRun to pick them up
########################################################################

get_apprun

# find . -name *desktop -exec cp {} $LOWERAPP.desktop \;
# FIXME: Workaround for missing desktop file
cat > $LOWERAPP.desktop <<EOF
[Desktop Entry]
Type=Application
Name=$APP
Exec=$LOWERAPP
Comment=Note-taking app with markdown support
Icon=$LOWERAPP
EOF
cat $LOWERAPP.desktop

# FIXME: Workaround for missing icon file
wget -c "https://cloud.githubusercontent.com/assets/2480569/11008987/42eef908-84d4-11e5-9da8-05e96ec97af0.png" -O $LOWERAPP.png

########################################################################
# Copy in the dependencies that cannot be assumed to be available
# on all target systems
########################################################################

mkdir -p ./usr/lib/qt5/plugins/
PLUGINS=/opt/qt56/plugins/
cp -r $PLUGINS/* ./usr/lib/qt5/plugins/
# This stuff seems not to be required by this particular Qt app - how can we be sure? (FIXME)
rm -rf usr/lib/qt5/plugins/{bearer,designer,generic,geoservices,position,prontsupport,qmltooling}

find /opt -name QtWebEngineProcess -exec cp {} ./usr/bin \;
mkdir -p ./usr/bin/translations/qtwebengine_locales/
cp -r /opt/qt56/translations/qtwebengine_locales/en-US.pak ./usr/bin/translations/qtwebengine_locales/
cp -r /opt/qt56/resources ./usr/bin/
cp -r /usr/lib/x86_64-linux-gnu/nss/* ./usr/lib

export LD_LIBRARY_PATH=/opt/qt56/lib/:LD_LIBRARY_PATH
copy_deps

# Move the libraries to usr/bin
move_lib
mv ./opt/qt56/lib/* usr/lib ; rm -rf ./opt
( cd usr/lib/qt5/plugins/platforms/../../ ; ln -s ../../lib/ . )
mv usr/lib/x86_64-linux-gnu/* usr/lib/

########################################################################
# Delete stuff that should not go into the AppImage
########################################################################

# Delete dangerous libraries; see
# https://github.com/probonopd/AppImages/blob/master/excludelist
delete_blacklisted
# We don't bundle the developer stuff
rm -rf usr/include || true
rm -rf usr/lib/cmake || true
rm -rf usr/lib/pkgconfig || true
find . -name '*.la' | xargs -i rm {}
strip usr/bin/* usr/lib/* || true

########################################################################
# desktopintegration asks the user on first run to install a menu item
########################################################################

get_desktopintegration $LOWERAPP
cat $LOWERAPP.desktop

########################################################################
# Determine the version of the app; also include needed glibc version
########################################################################

GLIBC_NEEDED=$(glibc_needed)
VERSION=git.$GIT_REV-glibc$GLIBC_NEEDED

########################################################################
# Patch away absolute paths; it would be nice if they were relative
########################################################################

find usr/lib -type f -exec sed -i -e 's|/opt/qt56|././/////|g' {} \;
# Possibly need to patch additional hardcoded paths away, replace
# "/usr" with "././" which means "usr/ in the AppDir"

########################################################################
# AppDir complete
# Now packaging it as an AppImage
########################################################################

cd .. # Go out of AppImage
sudo mkdir -p ../out/
sudo chown -R $USER ../out/
generate_appimage

########################################################################
# Upload the AppDir
########################################################################

transfer ../out/*
