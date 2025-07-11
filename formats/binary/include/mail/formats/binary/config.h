#pragma once
#include <bit>

namespace mail
{

struct BinaryConfiguration
{
    std::endian Endianness{std::endian::little};
};

} // namespace mail
