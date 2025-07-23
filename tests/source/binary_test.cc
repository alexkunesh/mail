#include <span>
#include <gtest/gtest.h>
#include <mail/formats/binary/conversion.h>

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
