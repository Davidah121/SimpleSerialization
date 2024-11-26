CXX := clang++
CXXFLAGS := -pipe -c -D USE_PREPROCESSOR_TRICKS=1
INCLUDES := -I ./include

all: TestSerialization

TestSerialization: TestSerialization.o
	$(CXX) ./*.o -o TestSerialization

TestSerialization.o: TestSerialization.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./TestSerialization.cpp -o ./TestSerialization.o

clean:
	rm ./*.o
	rm TestSerialization
