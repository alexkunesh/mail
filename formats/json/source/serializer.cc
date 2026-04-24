#include <mail/formats/json/serializer.h>

namespace mail
{

JsonSerializer::Context JsonSerializer::CurrentContext() const
{
    if (_contextStack.empty())
    {
        return Context::None;
    }

    return _contextStack.top();
}

void JsonSerializer::PushContext(const Context context) { _contextStack.push(context); }
void JsonSerializer::PopContext() { _contextStack.pop(); }

void JsonSerializer::TryAddLastCommaInLists()
{
    if (CurrentContext() == Context::List && _pendingComma)
    {
        _outputStream << ",";
        // _pendingComma = false;
    }

    _pendingComma = true;
}

std::string JsonSerializer::EscapeString(const std::string& input)
{
    std::string output;

    // Reserve a little extra memory to *hopefully* prevent extra allocations.
    output.reserve(input.length() + input.length() / 4);

    for (unsigned char c: input)
    {
        switch (c)
        {
            case '"':
                output += "\\\"";
                break;
            case '\\':
                output += "\\\\";
                break;
            case '\b':
                output += "\\\b";
                break;
            case '\f':
                output += "\\\f";
                break;
            case '\n':
                output += "\\\n";
                break;
            case '\r':
                output += "\\\r";
                break;
            case '\t':
                output += "\\\t";
                break;
            default:
                if (c <= '\x1F')
                {
                    // Escape remaining control characters as \u00XX
                    char escapeBuffer[7];
                    std::snprintf(escapeBuffer, sizeof(escapeBuffer), "\\u%04x", c);
                    output += escapeBuffer;
                } else
                {
                    // Pass remaining valid characters through unchanged.
                    output += c;
                }
                break;
        }
    }

    return output;
}

void JsonSerializer::BeginStruct()
{
    TryAddLastCommaInLists();
    _pendingComma = false;
    _outputStream << "{";
    PushContext(Context::Struct);
}

void JsonSerializer::BeginField(const std::string& name)
{
    if (_pendingComma)
    {
        _pendingComma = false;
        _outputStream << ",";
    }

    _outputStream << "\"" << EscapeString(name) << "\":";
    PushContext(Context::Field);
}

void JsonSerializer::EndField()
{
    _pendingComma = true;
    PopContext();
}

void JsonSerializer::EndStruct()
{
    _outputStream << "}";
    _pendingComma = true;
    PopContext();
}

void JsonSerializer::BoolValue(const bool value)
{
    TryAddLastCommaInLists();
    _outputStream << (value ? "true" : "false");
}

void JsonSerializer::I8Value(const std::int8_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}
void JsonSerializer::I16Value(const std::int16_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}
void JsonSerializer::I32Value(const std::int32_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}
void JsonSerializer::I64Value(const std::int64_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}

void JsonSerializer::U8Value(const std::uint8_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}

void JsonSerializer::U16Value(const std::uint16_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}

void JsonSerializer::U32Value(const std::uint32_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}

void JsonSerializer::U64Value(const std::uint64_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}

void JsonSerializer::F32Value(const std::float_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}

void JsonSerializer::F64Value(const std::double_t value)
{
    TryAddLastCommaInLists();
    _outputStream << std::to_string(value);
}

void JsonSerializer::UnsignedCharValue(const unsigned char value)
{
    TryAddLastCommaInLists();
    _outputStream << "\"" << value << "\"";
}

void JsonSerializer::CharValue(char value)
{
    TryAddLastCommaInLists();
    _outputStream << "\"" << value << "\"";
}


void JsonSerializer::StringValue(const std::string& value)
{
    TryAddLastCommaInLists();
    _outputStream << "\"" << EscapeString(value) << "\"";
}

void JsonSerializer::BeginList(bool knownSize, std::size_t length)
{
    TryAddLastCommaInLists();
    _pendingComma = false;
    _outputStream << "[";
    PushContext(Context::List);
}

void JsonSerializer::EndList()
{
    _pendingComma = true;
    _outputStream << "]";
    PopContext();
}

std::string JsonSerializer::ToString() const { return std::string(_outputStream.str()); }

} // namespace mail
