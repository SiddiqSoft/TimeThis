# CMake generated Testfile for 
# Source directory: /Users/kei/projects/TimeThis/tests
# Build directory: /Users/kei/projects/TimeThis/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[TimeThis_gtests]=] "tests")
set_tests_properties([=[TimeThis_gtests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/kei/projects/TimeThis/tests/CMakeLists.txt;24;add_test;/Users/kei/projects/TimeThis/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
