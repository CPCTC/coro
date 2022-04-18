: ${CACHE=`which ccache 2> /dev/null`}
: ${CC=gcc}
: ${CXX=g++}
: ${FLAGS="-O2"}
: ${CFLAGS=-iquote include}
: ${LFLAGS=-lpthread}

: ${NAME=coro}
: ${TYPE=bin}	# 'bin', 'a', 'o', 'so', or empty to skip this step

# All paths must be relative
: ${SOURCE_DIR=src}
: ${BUILD_DIR=build}
: ${TEST_DIR=test}

if [[ `type -t STEP` != function ]]; then
	STEP () {
		echo "$*"
		"$@"
	}
fi
