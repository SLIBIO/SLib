cd $(dirname $0)
CURRENT_PATH=`pwd`
BUILD_PATH=$CURRENT_PATH/build/release-$(uname -p)
mkdir -p $BUILD_PATH
cd $BUILD_PATH
cmake -DCMAKE_BUILD_TYPE=Release ../..
make -j4
cd $CURRENT_PATH
