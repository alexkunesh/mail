#include <gtest/gtest.h>
#include <mail/formats/binary/conversion.h>

TEST(BinaryConversion, ToBytes)
{
    constexpr std::array i16bytes{static_cast<std::byte>(0x39), static_cast<std::byte>(0x05)};
    ASSERT_EQ(mail::ToBytes<std::int16_t>(1337), i16bytes);
}
