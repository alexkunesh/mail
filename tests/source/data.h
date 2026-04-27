// Common data types used in testing.
#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include <mail/serializer.h>
#include <mail/deserializer.h>

class TestDataA
{
public:
    std::int32_t            a, b;
    std::string             text;
    std::array<uint16_t, 4> fourShorts;
    std::vector<uint32_t>   dynamicUnsignedInts;
};

const TestDataA ExampleTestData{.a                   = 533674,
                                .b                   = -3243463,
                                .text                = "Easter pink colors, race cars",
                                .fourShorts          = {555, 4831, 60000, 9},
                                .dynamicUnsignedInts = {1510223999, 13433002, 2424854066}};

namespace mail
{

template<> inline void Serialize(Serializer& serializer, const TestDataA& value)
{
    serializer.BeginStruct();

    serializer.Field("a", value.a);
    serializer.Field("b", value.b);
    serializer.Field("text", value.text);
    serializer.Field("fourShorts", value.fourShorts);
    serializer.Field("dynamicUnsignedInts", value.dynamicUnsignedInts);

    serializer.EndStruct();
}

template<> inline void Deserialize(Deserializer& deserializer, TestDataA& value)
{
    deserializer.BeginStruct();

    deserializer.Field("a", value.a);
    deserializer.Field("b", value.b);
    deserializer.Field("text", value.text);
    deserializer.Field("fourShorts", value.fourShorts);
    deserializer.Field("dynamicUnsignedInts", value.dynamicUnsignedInts);

    deserializer.EndStruct();
}

} // namespace mail
