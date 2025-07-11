#include <mail/formats/binary/de.h>

namespace mail
{

BinaryDeserializer::BinaryDeserializer(const std::span<std::byte>& input)
{
    _buffer = input;
    _cursor = 0;
}
void BinaryDeserializer::BeginStruct() {}
void BinaryDeserializer::BeginField(const std::string& name) {}
void BinaryDeserializer::EndField() {}
void BinaryDeserializer::EndStruct() {}

void BinaryDeserializer::BoolValue(bool& value)
{
    std::int8_t n;
    I8Value(n);
    value = n == 1;
}

void BinaryDeserializer::I8Value(std::int8_t& value) { NumericValue(value); }

void BinaryDeserializer::I16Value(std::int16_t& value) { NumericValue(value); }

void BinaryDeserializer::I32Value(std::int32_t& value) { NumericValue(value); }

void BinaryDeserializer::I64Value(std::int64_t& value) { NumericValue(value); }

void BinaryDeserializer::U8Value(std::uint8_t& value) { NumericValue(value); }

void BinaryDeserializer::U16Value(std::uint16_t& value) { NumericValue(value); }

void BinaryDeserializer::U32Value(std::uint32_t& value) { NumericValue(value); }

void BinaryDeserializer::U64Value(std::uint64_t& value) { NumericValue(value); }

void BinaryDeserializer::F32Value(std::float_t& value) { NumericValue(value); }

void BinaryDeserializer::F64Value(std::double_t& value) { NumericValue(value); }

void BinaryDeserializer::CharValue(char& value) {}

void BinaryDeserializer::StringValue(std::string& value) {}

} // namespace mail
