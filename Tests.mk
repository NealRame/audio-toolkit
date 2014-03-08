CFLAGS    += -O0 -g -DDEBUG -DTEST
CXXFLAGS  += -O0 -g -DDEBUG -DTEST

all: $(OBJECTS) libgtest.a gtest_main.o
	$(CXX) $(CXXFLAGS) -pthread -o proto-audio-metadata-test $^

libgtest.a: gtest-all.o gmock-all.o
	ar -rv libgtest.a gtest-all.o gmock-all.o

gtest_main.o: $(CURDIR)/../vendors/gtest/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) -pthread -c $(CURDIR)/../vendors/gtest/src/gtest_main.cc

gtest-all.o: $(CURDIR)/../vendors/gtest/src/gtest-all.cc
	$(CXX) $(CXXFLAGS) -pthread -c $^

gmock-all.o: $(CURDIR)/../vendors/gmock/src/gmock-all.cc
	$(CXX) $(CXXFLAGS) -pthread -c $^

include $(DEPS)
