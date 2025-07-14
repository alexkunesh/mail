#include <cstdint>
#include <iostream>
#include <filesystem>
#include <format>
#include <stdexcept>
#include <sstream>

#include <mail/ser.h>
#include <mail/formats/json/de.h>
#include <mail/formats/json/ser.h>
// #include <mail/formats/toml/de.h>
#include <mail/formats/binary/de.h>
#include <mail/formats/binary/ser.h>
#include <mail/formats/toml/de.h>
#include <mail/formats/toml/ser.h>


std::ofstream OpenFileWrite(const std::filesystem::path& path, bool binaryMode)
{
    std::ofstream file;
    if (binaryMode)
        file = std::ofstream(path, std::ios::binary);
    else
        file = std::ofstream(path);

    if (!file.is_open())
        throw std::runtime_error(std::format("failed to open file: {}", path.string()));

    return file;
}

std::ifstream OpenFileRead(const std::filesystem::path& path, bool binaryMode)
{
    std::ifstream file;

    if (binaryMode)
        file = std::ifstream(path, std::ios::binary);
    else
        file = std::ifstream(path);

    if (!file.is_open())
    {
        throw std::runtime_error(std::format("failed to open file: {}", path.string()));
    }

    return file;
}

std::string ReadText(const std::filesystem::path& path)
{
    const auto         file = OpenFileRead(path, false); // Open the file in text mode.
    std::ostringstream stringStream;
    stringStream << file.rdbuf();
    auto sourceString = stringStream.str();

    return sourceString;
}

std::vector<std::byte> ReadBytes(const std::filesystem::path& path)
{
    std::ifstream file = OpenFileRead(path, true);

    // Grab file size to allocate enough memory.
    file.seekg(0, std::ios::end);
    const auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<std::byte> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
    {
        throw std::runtime_error(std::format("failed to read file: {}", path.string()));
    }

    return buffer;
}

void WriteBytes(const std::filesystem::path& path, const std::span<std::byte>& data)
{
    auto file = OpenFileWrite(path, true);
    file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
}

class NestedStructure
{
public:
    std::int64_t cool, veryCool;
};

class Data
{
public:
    NestedStructure more;
    std::int32_t    a, b, c, d;
    std::string     text;
};

int main()
{
    const Data data2 = {.more = {.cool = 3445, .veryCool = 345},
                        .a    = 2,
                        .b    = 3,
                        .c    = 7,
                        .d    = 8,
                        .text = "hello, super cool binary format !"};

    mail::BinarySerializer serializer;
    serializer.Value(data2);
    WriteBytes("output.bin", serializer.GetBytes());
    // std::cout << serializer.ToString() << "\n";

    // auto serializedJson = R"({"a": 534, "b": 35234, "c": 1337, "d": 42069, "text": "Holy shit this works."})";
    // Data data;
    // mail::json::JsonDeserializer deserializer(serializedJson);
    // deserializer.Value(data);
    //
    // std::cout << data.text << "\n";

    // auto                   serialized = ReadText("test_data.toml");
    // Data                   data;
    // mail::TomlDeserializer deserializer(serialized);
    // deserializer.Value(data);
    //
    // std::cout << data.text << "\n";

    auto                     serializedBytes = ReadBytes("output.bin");
    Data                     data;
    mail::BinaryDeserializer deserializer(serializedBytes);
    deserializer.Value(data);
}
