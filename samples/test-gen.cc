#include <cstdint>
#include <mail/de.h>
#include <mail/ser.h>

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

namespace mail
{
template<> void Serialize(Serializer& serializer, const NestedStructure& value)
{
    serializer.BeginStruct();

    serializer.Field("cool", value.cool);
    serializer.Field("veryCool", value.veryCool);

    serializer.EndStruct();
}

template<> void Deserialize(Deserializer& deserializer, NestedStructure& value)
{
    deserializer.BeginStruct();

    deserializer.Field("cool", value.cool);
    deserializer.Field("veryCool", value.veryCool);

    deserializer.EndStruct();
}

template<> void Serialize(Serializer& serializer, const Data& value)
{
    serializer.BeginStruct();

    serializer.Field("more", value.more);
    serializer.Field("a", value.a);
    serializer.Field("b", value.b);
    serializer.Field("c", value.c);
    serializer.Field("d", value.d);
    serializer.Field("text", value.text);

    serializer.EndStruct();
}

template<> void Deserialize(Deserializer& deserializer, Data& value)
{
    deserializer.BeginStruct();

    deserializer.Field("more", value.more);
    deserializer.Field("a", value.a);
    deserializer.Field("b", value.b);
    deserializer.Field("c", value.c);
    deserializer.Field("d", value.d);
    deserializer.Field("text", value.text);

    deserializer.EndStruct();
}
} // namespace mail
