cd $(dirname $0)
CURRENT_PATH=`pwd`
mkdir -p $CURRENT_PATH/build/release
cd $CURRENT_PATH/build/release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make -j4
cd $CURRENT_PATH
