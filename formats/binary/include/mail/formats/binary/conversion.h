#pragma once
#include <cstddef>
#include <algorithm>
#include <array>
#include <bit>
#include <type_traits>
#include <span>

namespace mail
{

/**
 * Takes a floating-point or integer number and converts it into its byte representation.
 * @tparam T The number type.
 * @param input The value to convert.
 * @param endianness Which endianness to use to perform the conversion. By default, this uses the system's native
 * endianness (e.g. little for x86* or ARM, big for PowerPC or MIPS).
 * @return The byte representation of the input.
 */
template<typename T>
std::array<std::byte, sizeof(T)> ToBytes(T input, const std::endian endianness = std::endian::native)
{
    static_assert(std::is_arithmetic_v<T>); // Only accept integers and floating point numbers.

    // Use bit-wise magic to decompose the input down into an array of bytes.
    // This implementation is kept for the sake of keeping it, but using std::bit_cast is not only cleaner but more
    // efficient, as it just copies the in-memory structure instead of performing numerous bit-wise operations.

    // std::array<std::byte, sizeof(T)> output;
    // for (std::size_t i = 0; i < sizeof(T); i++)
    // {
    //     output[i] = static_cast<std::byte>((input >> (i * 8)) & 0xFF);
    // }

    auto output = std::bit_cast<std::array<std::byte, sizeof(T)>>(input);

    // If the endianness of the current system is the opposite of what the native one is, flip the output.
    if (endianness != std::endian::native)
    {
        std::reverse(output.begin(), output.end());
    }

    return output;
}

/**
 * Takes the byte representation of a floating-point or integer number and converts it back into a number.
 * @tparam T The number type.
 * @param input The bytes to convert.
 * @param endianness Which endianness to use to perform the conversion. By default, this uses the system's native
 * endianness (e.g. little for x86* or ARM, big for PowerPC or MIPS).
 * @return The numeric representation of the input.
 */
template<typename T>
T FromBytes(std::span<std::byte, sizeof(T)> input, const std::endian endianness = std::endian::native)
{
    static_assert(std::is_arithmetic_v<T>); // Only accept integers and floating point numbers.

    // If the endianness of the current system is the opposite of what the native one is, flip the input.
    if (endianness != std::endian::native)
    {
        std::reverse(input.begin(), input.end());
    }

    // Use bit-wise magic to reconstruct a number from the input.
    // ( see line 24 )

    // T output;
    // for (std::size_t i = 0; i < sizeof(T); i++)
    // {
    //     output |= static_cast<T>(input[i]) << (i * 8);
    // }

    // auto output = std::bit_cast<T>(input);

    auto output = *reinterpret_cast<T*>(input.data());

    return output;
}

} // namespace mail
