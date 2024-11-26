#include "SimpleSerialization.h"
#include "DefaultSerialization.h"
#include <iostream>

class TestClass1 : public SerializedObject
{
public:
    TestClass1(){}
    ~TestClass1(){}
    SERIALIZE_FUNCTION_DEFINITIONS()
    void setPos(int v1, int v2) {x = v1; y = v2;}
    int getX() {return x;}
    int getY() {return y;}
private:
    int x=0;
    int y=0;
};
SERIALIZE_CLASS(TestClass1, x, y)

void testSerialize()
{
    StreamableFile outputFile = StreamableFile("SerializedDataFile", StreamableFile::TYPE_WRITE);

    TestClass1 t1; t1.setPos(12, 34);
    std::vector<int> list1 = {1, 6, 2, 4, 0, 13, 18, 8};
    std::vector<TestClass1> list2 = std::vector<TestClass1>(4);
    list2[0].setPos(30, 40); list2[1].setPos(3232, 10203); list2[2].setPos(0xFF, 0xFFF); list2[3].setPos(-1, -2);
    std::string text = "Hellow World";
    staticSerialize(outputFile, t1);
    staticSerialize(outputFile, list1);
    staticSerialize(outputFile, list2);
    staticSerialize(outputFile, text);

    outputFile.close();
}

void testDeserialize()
{
    StreamableFile inputFile = StreamableFile("SerializedDataFile", StreamableFile::TYPE_READ);

    TestClass1 t1;
    std::vector<int> list1 = std::vector<int>(8);
    std::vector<TestClass1> list2 = std::vector<TestClass1>(4);
    std::string text;
    staticDeserialize(inputFile, t1);
    staticDeserialize(inputFile, list1);
    staticDeserialize(inputFile, list2);
    staticDeserialize(inputFile, text);

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
        else
        {
            std::cout << "-s to test serialization." << std::endl;
            std::cout << "-d to test deserialization." << std::endl;
        }
    }
    else
    {
        std::cout << "-s to test serialization." << std::endl;
        std::cout << "-d to test deserialization." << std::endl;
    }
    return 0;
}

