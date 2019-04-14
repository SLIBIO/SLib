$(dirname $0)/../macOS/build-release.sh
THIS_PATH=$(dirname $0)
SIMULATOR_VERSION=`xcodebuild -showsdks | grep iphonesimulator`
xcodebuild -project "${THIS_PATH}/SLib.xcodeproj" -scheme SLib -configuration Release -sdk iphonesimulator${SIMULATOR_VERSION##*iphonesimulator} -derivedDataPath "${THIS_PATH}/build"