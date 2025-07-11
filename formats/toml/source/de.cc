#include <mail/formats/toml/de.h>

namespace mail
{

TomlDeserializer::TomlDeserializer(const std::string& input)
{
    _document = toml::parse_str(input);
    _stack.push(&_document);
}

void TomlDeserializer::BeginStruct() { _stack.push(_stack.top()); }

void TomlDeserializer::BeginField(const std::string& name) { _stack.push(&_stack.top()->as_table()[name]); }

void TomlDeserializer::EndField() { _stack.pop(); }

void TomlDeserializer::EndStruct() { _stack.pop(); }

void TomlDeserializer::BoolValue(bool& value) { value = _stack.top()->as_boolean(); }

void TomlDeserializer::I8Value(std::int8_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::I16Value(std::int16_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::I32Value(std::int32_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::I64Value(std::int64_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::U8Value(std::uint8_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::U16Value(std::uint16_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::U32Value(std::uint32_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::U64Value(std::uint64_t& value) { value = _stack.top()->as_integer(); }

void TomlDeserializer::F32Value(std::float_t& value) { value = _stack.top()->as_floating(); }

void TomlDeserializer::F64Value(std::double_t& value) { value = _stack.top()->as_floating(); }

void TomlDeserializer::CharValue(char& value) { value = _stack.top()->as_string()[0]; }

void TomlDeserializer::StringValue(std::string& value)
{
    auto type = _stack.top()->type();
    value     = _stack.top()->as_string();
}
} // namespace mail
