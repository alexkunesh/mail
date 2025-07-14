#include <mail/formats/binary/ser.h>
#include <mail/formats/binary/conversion.h>

namespace mail
{

BinarySerializer::BinarySerializer(const std::size_t allocateBytes, const BinaryConfiguration configuration) :
    _configuration(configuration)
{
    _output.reserve(allocateBytes);
}

// As this format is not self-describing, we can leave these function implementations blank.

void BinarySerializer::BeginStruct() {}

void BinarySerializer::BeginField(const std::string& name) {}

void BinarySerializer::EndField() {}

void BinarySerializer::EndStruct() {}

void BinarySerializer::BoolValue(bool value) { I8Value(static_cast<std::int8_t>(value)); }

#define NUMERIC_VALUE_IMPL                                                                                             \
    auto bytes = mail::ToBytes(value, _configuration.Endianness);                                                      \
    _output.insert(_output.end(), bytes.begin(), bytes.end());

void BinarySerializer::I8Value(std::int8_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::I16Value(std::int16_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::I32Value(std::int32_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::I64Value(std::int64_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::U8Value(std::uint8_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::U16Value(std::uint16_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::U32Value(std::uint32_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::U64Value(std::uint64_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::F32Value(std::float_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::F64Value(std::double_t value) { NUMERIC_VALUE_IMPL }

void BinarySerializer::StringValue(const std::string& value)
{
    U64Value(value.length());
    for (std::size_t i = 0; i < value.length(); i++)
    {
        _output.push_back(static_cast<std::byte>(value[i]));
    }
}

std::span<std::byte> BinarySerializer::GetBytes() { return std::span(_output.begin(), _output.end()); }

std::vector<std::byte> BinarySerializer::ToBytes()
{
    std::vector<std::byte> copy;
    copy.reserve(_output.size());
    copy.insert(copy.end(), _output.begin(), _output.end());
    return copy;
}

} // namespace mail
