#include <string>
#include <span>
#include <gtest/gtest.h>

#include <mail/serialize.h>
#include <mail/serialize_std.h>
#include <mail/formats/binary/conversion.h>
#include <mail/formats/binary/serializer.h>
#include <mail/formats/binary/deserializer.h>

#include "data.h"

#define B(x) static_cast<std::byte>(x)

constexpr std::int8_t  i8TestValue  = 48;
constexpr std::int16_t i16TestValue = 1337;
constexpr std::int32_t i32TestValue = -4601908;
constexpr std::int64_t i64TestValue = -47736842221;

std::array i8bytes{B(48)};
std::array i16bytesBE{B(0x05), B(0x39)};
std::array i16bytesLE{B(0x39), B(0x05)};
std::array i32bytesBE{B(0xFF), B(0xB9), B(0xC7), B(0xCC)};
std::array i32bytesLE{B(0xCC), B(0xC7), B(0xB9), B(0xFF)};
std::array i64bytesBE{B(0xFF), B(0xFF), B(0xFF), B(0xF4), B(0xE2), B(0xA9), B(0x98), B(0x13)};
std::array i64bytesLE{B(0x13), B(0x98), B(0xA9), B(0xE2), B(0xF4), B(0xFF), B(0xFF), B(0xFF)};

// Tests the mail::ToBytes function in both little- and big- endian modes.
TEST(BinaryConversion, ToBytes)
{
    ASSERT_EQ(mail::ToBytes<std::int8_t>(i8TestValue, std::endian::little), i8bytes);
    ASSERT_EQ(mail::ToBytes<std::int8_t>(i8TestValue, std::endian::big), i8bytes);

    ASSERT_EQ(mail::ToBytes<std::int16_t>(i16TestValue, std::endian::big), i16bytesBE);
    ASSERT_EQ(mail::ToBytes<std::int16_t>(i16TestValue, std::endian::little), i16bytesLE);

    ASSERT_EQ(mail::ToBytes<std::int32_t>(i32TestValue, std::endian::big), i32bytesBE);
    ASSERT_EQ(mail::ToBytes<std::int32_t>(i32TestValue, std::endian::little), i32bytesLE);

    ASSERT_EQ(mail::ToBytes<std::int64_t>(i64TestValue, std::endian::big), i64bytesBE);
    ASSERT_EQ(mail::ToBytes<std::int64_t>(i64TestValue, std::endian::little), i64bytesLE);
}

// Tests the mail::FromBytes function in both little- and big- endian modes.
TEST(BinaryConversion, FromBytes)
{
    ASSERT_EQ(mail::FromBytes<std::int8_t>(i8bytes, std::endian::big), i8TestValue);
    ASSERT_EQ(mail::FromBytes<std::int8_t>(i8bytes, std::endian::little), i8TestValue);

    ASSERT_EQ(mail::FromBytes<std::int16_t>(i16bytesBE, std::endian::big), i16TestValue);
    ASSERT_EQ(mail::FromBytes<std::int16_t>(i16bytesLE, std::endian::little), i16TestValue);

    ASSERT_EQ(mail::FromBytes<std::int32_t>(i32bytesBE, std::endian::big), i32TestValue);
    ASSERT_EQ(mail::FromBytes<std::int32_t>(i32bytesLE, std::endian::little), i32TestValue);

    ASSERT_EQ(mail::FromBytes<std::int64_t>(i64bytesBE, std::endian::big), i64TestValue);
    ASSERT_EQ(mail::FromBytes<std::int64_t>(i64bytesLE, std::endian::little), i64TestValue);
}

std::array TestDataA1{B(0xAA), B(0x24), B(0x08), B(0x00), B(0x39), B(0x82), B(0xCE), B(0xFF), B(0x1D), B(0x00), B(0x00),
                      B(0x00), B(0x00), B(0x00), B(0x00), B(0x00), B(0x45), B(0x61), B(0x73), B(0x74), B(0x65), B(0x72),
                      B(0x20), B(0x70), B(0x69), B(0x6E), B(0x6B), B(0x20), B(0x63), B(0x6F), B(0x6C), B(0x6F), B(0x72),
                      B(0x73), B(0x2C), B(0x20), B(0x72), B(0x61), B(0x63), B(0x65), B(0x20), B(0x63), B(0x61), B(0x72),
                      B(0x73), B(0x2B), B(0x02), B(0xDF), B(0x12), B(0x60), B(0xEA), B(0x09), B(0x00), B(0x03), B(0x00),
                      B(0x00), B(0x00), B(0x00), B(0x00), B(0x00), B(0x00), B(0x7F), B(0x30), B(0x04), B(0x5A), B(0xAA),
                      B(0xF8), B(0xCC), B(0x00), B(0x32), B(0x56), B(0x88), B(0x90)};

TEST(TestDataA, BinarySerialization)
{
    mail::BinarySerializer serializer;
    serializer.Value(ExampleTestData);
    auto serializerBytes = serializer.ToBytes();
    ASSERT_TRUE(std::equal(serializerBytes.begin(), serializerBytes.end(), TestDataA1.begin()));
}

TEST(TestDataA, BinaryDeserialization)
{
    mail::BinaryDeserializer deserializer(TestDataA1);
    TestDataA                deserializedData;
    deserializer.Value(deserializedData);

    ASSERT_EQ(deserializedData.a, ExampleTestData.a);
    ASSERT_EQ(deserializedData.b, ExampleTestData.b);
    ASSERT_EQ(deserializedData.text, ExampleTestData.text);
    ASSERT_EQ(deserializedData.fourShorts, ExampleTestData.fourShorts);
    ASSERT_EQ(deserializedData.dynamicUnsignedInts, ExampleTestData.dynamicUnsignedInts);
}
