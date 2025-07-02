# Maintainer: Micro <microgamercz@proton.me> -> https://github.com/MicrogamerCz

pkgname=piqi-git
pkgver=0.1.0
pkgrel=1
pkgdesc="Unofficial Qt API for Pixiv (with async support)"
arch=(x86_64)
url="https://github.com/MicrogamerCz/Piqi"
license=(Unlicense)
depends=(qcoro)
makedepends=(extra-cmake-modules ninja git)
source=("https://github.com/MicrogamerCz/Piqi.git")
sha256sums=('SKIP')

build() {
  cmake -GNinja -B build -S Piqi \
    -DCMAKE_INSTALL_PREFIX=/usr
  cmake --build build
}

package() {
  DESTDIR="$pkgdir" cmake --install build
}
p