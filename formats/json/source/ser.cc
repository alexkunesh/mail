#include <mail/formats/json/ser.h>

namespace mail
{

void JsonSerializer::BeginStruct() { _writer.StartObject(); }
void JsonSerializer::BeginField(const std::string& name) { _writer.Key(name.c_str(), name.size(), true); }
void JsonSerializer::EndField() {}
void JsonSerializer::EndStruct() { _writer.EndObject(); }
void JsonSerializer::BoolValue(const bool value) { _writer.Bool(value); }
void JsonSerializer::I8Value(const std::int8_t value) { _writer.Int(value); }
void JsonSerializer::I16Value(const std::int16_t value) { _writer.Int(value); }
void JsonSerializer::I32Value(const std::int32_t value) { _writer.Int(value); }
void JsonSerializer::I64Value(const std::int64_t value) { _writer.Int64(value); }
void JsonSerializer::U8Value(const std::uint8_t value) { _writer.Uint(value); }
void JsonSerializer::U16Value(const std::uint16_t value) { _writer.Uint(value); }
void JsonSerializer::U32Value(const std::uint32_t value) { _writer.Uint(value); }
void JsonSerializer::U64Value(const std::uint64_t value) { _writer.Uint64(value); }
void JsonSerializer::F32Value(const std::float_t value) { _writer.Double(value); }
void JsonSerializer::F64Value(const std::double_t value) { _writer.Double(value); }
void JsonSerializer::StringValue(const std::string& value) { _writer.String(value.data(), value.size(), true); }

std::string JsonSerializer::ToString() const { return std::string(_buffer.GetString()); }

} // namespace mail
