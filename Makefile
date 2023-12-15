TESTS?=OFF
EXAMPLE?=OFF
BUILD_TYPE?=Debug
# Build type lower case
BUILD_TYPE_LC=`echo ${BUILD_TYPE} | tr [:upper:] [:lower:]`

# CMAKE_FLAGS=--warn-uninitialized
SOURCE_DIRS=src

all:
	@echo "Variables:"
	@echo "  BUILD_TYPE- current value='${BUILD_TYPE}' used for CMAKE - either Release or Debug"
	@echo ""
	@echo "Targets:"
	@echo "  example   - build example program"
	@echo "  examplerpi- build example program for Raspberry Pi 3+"
	@echo "  tests     - build tests and run"
	@echo "  clean     - cleans build directory"
	@echo "  cleanall  - removes build directories"
	@echo "  format    - use clang-format on C/C++ files in ${SOURCE_DIRS}"
	@echo "  tidy      - use clang-tidy in ${SOURCE_DIRS}"

example:
	@make BUILD_DIR=build_$@_${BUILD_TYPE_LC} EXAMPLE=ON __build
	@rm -f compile_commands.json
	@ln -s build_$@_${BUILD_TYPE_LC}/compile_commands.json .

examplerpi:
	@cmake -S . -B build_rpi -G Ninja \
		   --toolchain cmake/rpi.cmake \
		   -DUSE_EXAMPLE=ON \
		   -DUSE_TESTS=OFF \
           -DCMAKE_BUILD_TYPE=Release
	@cmake --build build_rpi

tests:
	@make TESTS=ON example
	@./build_example_${BUILD_TYPE_LC}/options_tests

__build:
	@if [ ${BUILD_TYPE} != "Debug" -a ${BUILD_TYPE} != "Release" ]; then \
		echo "Invalid BUILD_TYPE (${BUILD_TYPE})!"; \
		exit 1; \
		fi
	@cmake ${CMAKE_FLAGS} -S . -B ${BUILD_DIR} -G Ninja \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DUSE_TESTS=${TESTS} \
		-DUSE_EXAMPLE=${EXAMPLE}
	@cmake --build ${BUILD_DIR}

clean:
	@if [ -d build_example_${BUILD_TYPE_LC} ] ; then \
		cmake --build build_example_${BUILD_TYPE_LC} --target clean ; \
		fi
	@rm -f compile_commands.json

cleanall:
	@rm -rf build_* compile_commands.json

format:
	find ${SOURCE_DIRS} -type f -name "*.[ch]" -o -name "*.[ch]pp" -exec clang-format -i {} \;
	find ${SOURCE_DIRS} CMakeLists.txt -type f -name CMakeLists.txt -exec cmake-format -i {} \;

tidy:
	@run-clang-tidy -fix -quiet ${SOURCE_DIRS}
