#include <mail/formats/toml/ser.h>
#include <sstream>

namespace mail
{

TomlSerializer::TomlSerializer() { _stack.push(&_document); }

void TomlSerializer::BeginStruct()
{
    if (_isFieldPending)
    {
        _stack.top()->as_table()[_pendingFieldName] = ::toml::table{};
        _stack.push(&_stack.top()->as_table()[_pendingFieldName]);
    } else
    {
        _stack.push(_stack.top());
        *_stack.top() = toml::table{};
    }
}

void TomlSerializer::BeginField(const std::string& name)
{
    _isFieldPending   = true;
    _pendingFieldName = name;
}

void TomlSerializer::EndField() { _isFieldPending = false; }

void TomlSerializer::EndStruct() { _stack.pop(); }

void TomlSerializer::BoolValue(bool value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::I8Value(std::int8_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::I16Value(std::int16_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::I32Value(std::int32_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::I64Value(std::int64_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::U8Value(std::uint8_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::U16Value(std::uint16_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::U32Value(std::uint32_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::U64Value(std::uint64_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::F32Value(std::float_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::F64Value(std::double_t value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::CharValue(char value) { _stack.top()->as_table()[_pendingFieldName] = value; }

void TomlSerializer::StringValue(const std::string& value) { _stack.top()->as_table()[_pendingFieldName] = value; }

std::string TomlSerializer::ToString() const
{
    std::stringstream output;
    output << _document;
    return output.str();
}

} // namespace mail
