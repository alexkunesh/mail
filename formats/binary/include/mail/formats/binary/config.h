#pragma once
#include <bit>

namespace mail
{

struct BinaryConfiguration
{
    std::endian Endianness{std::endian::little};
    /** A number (in bytes) to preallocate when constructing a new BinarySerializer. */
    std::size_t PreallocationSize{2048};
};

constexpr BinaryConfiguration DefaultBinaryConfiguration;

} // namespace mail
