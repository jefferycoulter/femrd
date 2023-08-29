# get current direcetory
DIR="$( cd "$( dirname "$0" )" && pwd )"

# run cmake
if [ "$(uname -s)" == "Darwin" ]; then # build apple
    cmake -S $DIR -B $DIR/build         \
        -G "Unix Makefiles"             \
        -DCMAKE_C_COMPILER="clang"      \
        -DCMAKE_CXX_COMPILER="clang++"

    if [[ -z "$MAKEFLAGS" ]]; then # tell make to run on multiple threads if possible
        export MAKEFLAGS=-j$(sysctl -n hw.ncpu)
    fi
elif [ "$(uname -s)" == "Linux" ]; then # build linux
    cmake -S $DIR -B $DIR/build     \
        -G "Unix Makefiles"         \
        -DCMAKE_C_COMPILER="gcc"    \
        -DCMAKE_CXX_COMPILER="g++"
    
    if [[ -z "$MAKEFLAGS" ]]; then # tell make to run on multiple threads if possible
        export MAKEFLAGS=-j$(nproc)
    fi
else
    echo "error"
    exit
fi

# run make
cd $DIR/build; VERBOSE=1 make