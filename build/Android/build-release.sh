$(dirname $0)/../macOS/build-release.sh
OLD_PATH=`pwd`
THIS_PATH=$(dirname $0)
cd "${THIS_PATH}"
./gradlew assembleRelease
cd "${OLD_PATH}"
