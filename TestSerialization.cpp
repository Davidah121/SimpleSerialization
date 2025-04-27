#include "SimpleSerialization.h"
#include "DefaultSerialization.h"
#include "RawDataFormatter.h"
#include <iostream>

struct SimpleVec2 : virtual public SerializedObject
{
    float x;
    float y;
    SERIALIZE_CLASS(x, y)
};


class TestClass1 : virtual public SerializedObject
{
public:
    TestClass1(){}
    ~TestClass1(){}
    void setPos(int v1, int v2) {x = v1; y = v2;}
    int getX() {return x;}
    int getY() {return y;}

private:
    int x=0;
    int y=0;

    SERIALIZE_CLASS(x, y)
};

class TestClass2 : virtual public TestClass1, virtual public SimpleVec2
{
public:
    TestClass2(){}
    ~TestClass2(){}
private:

    SERIALIZE_SUPER_CLASS(TestClass1, SimpleVec2)
    SERIALIZE_CLASS()
};


void testSerialize()
{
    SerializedStreamableFile outputFile = SerializedStreamableFile("SerializedDataFile", SerializedStreamableFile::TYPE_WRITE);
    RawDataFormatter formatter = RawDataFormatter();

    TestClass2 t1; t1.setPos(12, 34);
    t1.SimpleVec2::x = 1.0f;
    t1.SimpleVec2::y = 2.0f;
    std::vector<int> list1 = {1, 6, 2, 4, 0, 13, 18, 8};
    std::vector<TestClass1> list2 = std::vector<TestClass1>(4);
    list2[0].setPos(30, 40); list2[1].setPos(3232, 10203); list2[2].setPos(0xFF, 0xFFF); list2[3].setPos(-1, -2);
    std::string text = "Hellow World";
    std::tuple<int, float, double> testTuple = {12, 32.2f, 1002.0};
    staticSerialize(outputFile, formatter, t1);
    staticSerialize(outputFile, formatter, list1);
    staticSerialize(outputFile, formatter, list2);
    staticSerialize(outputFile, formatter, text);
    staticSerialize(outputFile, formatter, testTuple);

    outputFile.close();
}

void testDeserialize()
{
    SerializedStreamableFile inputFile = SerializedStreamableFile("SerializedDataFile", SerializedStreamableFile::TYPE_READ);
    RawDataFormatter formatter = RawDataFormatter();

    TestClass2 t1;
    std::vector<int> list1 = std::vector<int>(8);
    std::vector<TestClass1> list2 = std::vector<TestClass1>(4);
    std::string text;
    std::tuple<int, float, double> testTuple;
    try
    {
        staticDeserialize(inputFile, formatter, t1);
        staticDeserialize(inputFile, formatter, list1);
        staticDeserialize(inputFile, formatter, list2);
        staticDeserialize(inputFile, formatter, text);
        staticDeserialize(inputFile, formatter, testTuple);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    inputFile.close();

    ///check if values are correct
    std::cout << t1.getX() << ", " << t1.getY() << std::endl;

    std::cout << list1.size() << std::endl;
    std::cout << "\t";
    for(int i=0; i<list1.size(); i++)
        std::cout << list1[i] << ", ";
    std::cout << std::endl;

    std::cout << list2.size() << std::endl;
    std::cout << "\t";
    for(int i=0; i<list2.size(); i++)
        std::cout << "(" << list2[i].getX() << ", " << list2[i].getY() << "), ";
    std::cout << std::endl;
    std::cout << text << std::endl;
    std::cout << std::get<0>(testTuple) << ", " << std::get<1>(testTuple) << ", " << std::get<2>(testTuple) << std::endl;
}

void displayFields()
{
    TestClass2 c;
    for(auto field : c.getSerializedVariables())
    {
        std::cout << field.first << std::endl;
        std::cout << "\tTYPE: " << field.second.type.getName() << std::endl;
    }
}

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        std::string arg = argv[1];
        if(arg == "-s")
            testSerialize();
        else if(arg == "-d")
            testDeserialize();
        else if(arg == "-f")
            displayFields();
        else
        {
            std::cout << "-s to test serialization." << std::endl;
            std::cout << "-d to test deserialization." << std::endl;
            std::cout << "-f to test getting serialized fields." << std::endl;
        }
    }
    else
    {
        std::cout << "-s to test serialization." << std::endl;
        std::cout << "-d to test deserialization." << std::endl;
        std::cout << "-f to test getting serialized fields." << std::endl;
    }
    return 0;
}
