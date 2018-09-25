BUILD_TYPE=$1

APP_PATH=$2
if [ -z $APP_PATH ]; then
 APP_PATH=`pwd`
fi

BUILD_PATH=$APP_PATH/build/$BUILD_TYPE-$(uname -p)
mkdir -p $BUILD_PATH

CURRENT_PATH=`pwd`

cd $BUILD_PATH

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../..
make -j4

cd $CURRENT_PATH
