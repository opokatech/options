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
	@echo "  example   - build example programs"
	@echo "  tests     - build tests and run"
	@echo "  testcov   - build tests with coverage and run them"
	@echo "  clean     - cleans build directory"
	@echo "  cleanall  - removes build directories"
	@echo "  format    - use clang-format on C/C++ files in ${SOURCE_DIRS}"
	@echo "  tidy      - use clang-tidy in ${SOURCE_DIRS}"

example:
	@make BUILD_DIR=build_$@_${BUILD_TYPE_LC} EXAMPLE=ON __build
	@rm -f compile_commands.json
	@ln -s build_$@_${BUILD_TYPE_LC}/compile_commands.json .

tests:
	@make TESTS=ON example
	@./build_example_${BUILD_TYPE_LC}/options_tests

testcov:
	@rm -rf build_testcov
	mkdir build_testcov
	cmake -S . -B build_testcov -DUSE_TESTS=ON -GNinja -DCMAKE_CXX_FLAGS=--coverage
	cmake --build build_testcov
	ctest --test-dir build_testcov/src/tests -V
	lcov -c -d build_testcov/src --exclude "/usr/include/*" \
                                 --exclude "*catch2/*" \
                                 --exclude "*tests/*" \
                                 -o build_testcov/tracelog.lcov
	genhtml build_testcov/tracelog.lcov -o build_testcov/html >/dev/null 2>&1 || echo "genhtml failed"

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
