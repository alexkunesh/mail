#pragma once

#include <mail/formats/binary/config.h>
#include <mail/formats/binary/conversion.h>
#include <mail/formats/binary/deserializer.h>
#include <mail/formats/binary/serializer.h>

namespace mail
{

template<typename T>
std::vector<std::byte> ToBinary(const T& data, const BinaryConfiguration& configuration = DefaultBinaryConfiguration)
{
    BinarySerializer serializer(configuration);
    serializer.Value(data);
    return serializer.ToBytes();
}

template<typename T>
T FromBinary(const std::span<std::byte>& data, const BinaryConfiguration& configuration = DefaultBinaryConfiguration)
{
    BinaryDeserializer deserializer(data, configuration);
    T                  output;
    deserializer.Value(output);
    return output;
}

} // namespace mail
