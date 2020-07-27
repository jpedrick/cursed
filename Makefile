BUILD=Release
BRANCH_NAME=$(shell git rev-parse --symbolic-full-name --abbrev-ref @{u} )
DIRNAME=${shell basename $(CURDIR) }
DISTRIB_CODENAME=${shell lsb_release -cs}

USE_MAKE=
J=${shell nproc}

ifdef USE_MAKE
BUILD_DIR=../build-${DIRNAME}-${DISTRIB_CODENAME}-${BRANCH_NAME}-${BUILD}-make
CMAKE=cmake
BUILDER=/usr/bin/make
GENERATOR=Unix Makefiles
BUILD_FILE=Makefile
else
BUILD_DIR=../build-${DIRNAME}-${DISTRIB_CODENAME}-${BRANCH_NAME}-${BUILD}-ninja
CMAKE=cmake
BUILDER=/usr/bin/ninja
GENERATOR=Ninja
BUILD_FILE=build.ninja
endif

THREADS=${J}
BUILDER_ARGS=

TARGET=

$(info TARGET=${TARGET} J=${J} CMakeCache.txt=${CMAKE_CACHE} )
$(info BUILD=${BUILD} BUILD_DIR=${BUILD_DIR})

all: cmake 
	rm -f ${BUILD}
	ln -sf ${BUILD_DIR} ${BUILD}
	${BUILDER} ${BUILDER_ARGS} -j ${THREADS} -C ${BUILD_DIR} ${TARGET}

cmake: build-${BUILD}

debug:
	${MAKE} BUILD=Debug

build-${BUILD}: ${BUILD_DIR}/${BUILD_FILE}

${BUILD_DIR}/${BUILD_FILE}:
	${CMAKE} -j${THREADS} -G "${GENERATOR}" -H${CURDIR} -DCMAKE_BUILD_TYPE=${BUILD} -B${BUILD_DIR}

clean:
	${BUILDER} -C ${BUILD_DIR} clean

distclean:
	${BUILDER} -C ${BUILD_DIR} distclean
	rm ${BUILD}

check: all
	cd ${BUILD_DIR} && CTEST_OUTPUT_ON_FAILURE=1 ctest ${TEST_ARGS}

