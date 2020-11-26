#!/usr/bin/env bash
PACKAGE=chorespp
VERSION=1.1
SRCDIR=$(pwd)
BUILDDIR=/tmp/build-${PACKAGE}-${VERSION}

DEBEMAIL="dimitrisinbox@gmail.com"
DEBFULLNAME="Dimitris Psathas"
export DEBEMAIL DEBFULLNAME

mkdir -p "${BUILDDIR}" && pushd "${BUILDDIR}"
qmake PREFIX=/usr "${SRCDIR}/chorespp.pro" CONFIG+=release
make
INSTALL_ROOT=${PACKAGE}-${VERSION} make install
mkdir -p ${PACKAGE}-${VERSION}/DEBIAN
cat > ${PACKAGE}-${VERSION}/DEBIAN/control <<EOF
Package: ${PACKAGE}
Version: ${VERSION}
Section: base
Priority: optional
Architecture: amd64
Depends: libqt5gui5
Maintainer: ${DEBFULLNAME} <${DEBEMAIL}>
Description: Chores
 A simple and lightweight to-do application
EOF
dpkg-deb --build ${PACKAGE}-${VERSION}
cp "${PACKAGE}-${VERSION}.deb" "${SRCDIR}"
popd
rm -rf "${BUILDDIR}"
echo
echo Package created. You can install it with
echo " dpkg -i ${PACKAGE}-${VERSION}.deb"
echo
