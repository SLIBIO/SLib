cd $(dirname $0)
CURRENT_PATH=`pwd`
mkdir -p $CURRENT_PATH/build/debug
cd $CURRENT_PATH/build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make -j4
cd $CURRENT_PATH
