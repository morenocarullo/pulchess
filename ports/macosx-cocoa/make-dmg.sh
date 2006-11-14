#! /bin/sh

PROJECTDIR=`pwd`/build/Deployment
FILENAME=`pwd`/build/PulCHESSX-tmp.dmg
FINALNAME=`pwd`/build/PulCHESSX-dev.dmg
VOLUMELABEL=PulCHESSX

# ricompila il progetto
xcodebuild -configuration  Deployment

# crea il file .dmg
rm -rf ${FILENAME}
hdiutil create -srcfolder "${PROJECTDIR}" -volname "${VOLUMELABEL}" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW "${FILENAME}"

# compressione .dmg
rm -rf ${FINALNAME}
hdiutil convert "${FILENAME}" -format UDZO -imagekey zlib-level=9 -o "${FINALNAME}"
rm -rf ${FILENAME}
