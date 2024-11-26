#pragma once

#ifdef _MSC_VER
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
    #pragma warning(disable:4996)
#endif
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>

class Streamable
{
public:
    /**
     * @brief Write n bytes out.
     *      Must also move forward by n bytes.
     * 
     * @param buffer 
     * @param sizeInBytes 
     */
    virtual bool write(const void* buffer, size_t sizeInBytes) = 0;

    /**
     * @brief Read n bytes in.
     *      Must also move forward by n bytes.
     * 
     * @param buffer 
     * @param sizeInBytes 
     */
    virtual bool read(void* buffer, size_t sizeInBytes) = 0;

    /**
     * @brief Move to the location.
     *      The location is an absolute location.
     * 
     * @param location 
     */
    virtual bool seek(size_t location) = 0;
private:
};

class StreamableVector : public Streamable
{
public:
    StreamableVector(){}
    StreamableVector(size_t size)
    {
        buffer = std::vector<unsigned char>(size);
    }
    StreamableVector(const std::vector<unsigned char>& other)
    {
        buffer = other;
    }
    ~StreamableVector(){}

    virtual bool write(const void* inputBuffer, size_t sizeInBytes)
    {
        if(sizeInBytes + index > buffer.size())
        {
            //allocate more memory
            buffer.resize(sizeInBytes + index);
        }
        std::memcpy((void*)&buffer[index], inputBuffer, sizeInBytes);
        index += sizeInBytes;
        return true;
    }
    virtual bool read(void* inputBuffer, size_t sizeInBytes)
    {
        if(sizeInBytes + index >= buffer.size())
            return false;
        std::memcpy(inputBuffer, (void*)&buffer[index], sizeInBytes);
        index += sizeInBytes;
        return true;
    }
    virtual bool seek(size_t index)
    {
        if(index < buffer.size())
        {
            this->index = index;
            return true;
        }
        else
            return false;
    }

    std::vector<unsigned char>& getBuffer()
    {
        return buffer;
    }
private:
    size_t index;
    std::vector<unsigned char> buffer;
};


class StreamableFile : public Streamable
{
public:
    static const bool TYPE_READ = false;
    static const bool TYPE_WRITE = true;
    
    StreamableFile(std::string filename, bool type)
    {
        if(type == TYPE_READ)
            file = fopen(filename.c_str(), "rb");
        else
            file = fopen(filename.c_str(), "wb");
        
        if(file != nullptr)
        {
            fseek(file, 0, SEEK_END);
            fileSize = ftell(file);
            fseek(file, 0, SEEK_SET);
        }
    }
    ~StreamableFile()
    {
        close();
    }

    virtual bool write(const void* inputBuffer, size_t sizeInBytes)
    {
        if(file == nullptr)
            return false;
        size_t bytesWritten = fwrite(inputBuffer, 1, sizeInBytes, file);
        fileSize += bytesWritten;
        return bytesWritten == sizeInBytes;
    }
    virtual bool read(void* inputBuffer, size_t sizeInBytes)
    {
        if(file == nullptr)
            return false;
        size_t bytesRead = fread(inputBuffer, 1, sizeInBytes, file);
        return bytesRead == sizeInBytes;
    }
    virtual bool seek(size_t index)
    {
        if(file == nullptr)
            return false;
        if(index >= fileSize)
            return false; //do not allow writing to an offset past the file size
        
        int err = fseek(file, index, SEEK_SET);
        return err != 0;
    }

    void close()
    {
        if(file != nullptr)
            fclose(file);
        file = nullptr;
    }

    bool valid()
    {
        return file != nullptr;
    }

    FILE* getFilePointer()
    {
        return file;
    }
private:
    size_t fileSize = 0;
    FILE* file = nullptr;
};