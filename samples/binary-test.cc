#include <mail/formats/binary/conversion.h>

int main()
{
    float testNumber = 69.420f;
    // auto     bytes      = mail::Uint32ToBytes(testNumber, std::endian::big);
    auto bytes = mail::ToBytes(testNumber, std::endian::big);

    auto numberFromBytes = mail::FromBytes<std::uint32_t>(
            {std::byte{0x88}, std::byte{0x8F}, std::byte{0x06}, std::byte{0xCE}}, std::endian::big);
}
