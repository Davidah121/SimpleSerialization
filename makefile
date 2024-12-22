CXX := clang++
CXXFLAGS := -pipe -c -D USE_PREPROCESSOR_TRICKS=1
INCLUDES := -I ./include

all: TestSerialization

TestSerialization: TestSerialization.o TypeInfo.o TestSerialization.o
	$(CXX) ./*.o -o TestSerialization

SimpleSerialization.o : SimpleSerialization.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./SimpleSerialization.cpp -o ./SimpleSerialization.o

TypeInfo.o : TypeInfo.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./TypeInfo.cpp -o ./TypeInfo.o

TestSerialization.o: TestSerialization.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./TestSerialization.cpp -o ./TestSerialization.o

clean:
	rm ./*.o
	rm TestSerialization
