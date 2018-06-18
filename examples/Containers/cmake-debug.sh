cd $(dirname $0)
CURRENT_PATH=`pwd`
BUILD_PATH=$CURRENT_PATH/build/debug-$(uname -p)
mkdir -p $BUILD_PATH
cd $BUILD_PATH
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make -j4
cd $CURRENT_PATH
