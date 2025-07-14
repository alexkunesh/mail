#include <iostream>
#include <mail/formats/json/de.h>
namespace mail
{

JsonDeserializer::JsonDeserializer(const std::string& input)
{
    _document.Parse(input.c_str());
    assert(!_document.HasParseError());
    _stack.push(&_document);
}

void JsonDeserializer::BeginStruct() { _stack.push(_stack.top()); }

void JsonDeserializer::BeginField(const std::string& name)
{
    _stack.push(&_stack.top()->GetObject().FindMember(name.c_str())->value);
}

void JsonDeserializer::EndField() { _stack.pop(); }

void JsonDeserializer::EndStruct() { _stack.pop(); }

void JsonDeserializer::BoolValue(bool& value) { value = _stack.top()->GetBool(); }

void JsonDeserializer::I8Value(std::int8_t& value) { value = _stack.top()->GetInt(); }

void JsonDeserializer::I16Value(std::int16_t& value) { value = _stack.top()->GetInt(); }

void JsonDeserializer::I32Value(std::int32_t& value) { value = _stack.top()->GetInt(); }

void JsonDeserializer::I64Value(std::int64_t& value) { value = _stack.top()->GetInt64(); }

void JsonDeserializer::U8Value(std::uint8_t& value) { value = _stack.top()->GetUint(); }

void JsonDeserializer::U16Value(std::uint16_t& value) { value = _stack.top()->GetUint(); }

void JsonDeserializer::U32Value(std::uint32_t& value) { value = _stack.top()->GetUint(); }

void JsonDeserializer::U64Value(std::uint64_t& value) { value = _stack.top()->GetUint64(); }

void JsonDeserializer::F32Value(std::float_t& value) { value = _stack.top()->GetFloat(); }

void JsonDeserializer::F64Value(std::double_t& value) { value = _stack.top()->GetDouble(); }

void JsonDeserializer::StringValue(std::string& value)
{
    auto        type   = _stack.top()->GetType();
    const char* string = _stack.top()->GetString();

    value = std::string(_stack.top()->GetString());
}

} // namespace mail
