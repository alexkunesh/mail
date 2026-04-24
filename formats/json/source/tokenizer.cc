#include <algorithm>
#include <charconv>
#include <format>
#include <sstream>
#include <utility>
#include <mail/formats/json/tokenizer.h>


namespace mail
{

JsonTokenizer::JsonTokenizer(const std::string_view& input) : _input{input}
{
    _tokens.reserve(input.length() / 6);

    ConsumeWhitespace();

    // If we encountered an error, there is no point in continuing tokenization.
    while (!AtEnd() && (_tokens.empty() || _tokens[_tokens.size() - 1].Type != Error))
    {
        ScanToken();
        ConsumeWhitespace();
    }

    PushToken(_cursor, End);
}

const std::vector<JsonToken>& JsonTokenizer::GetTokens() const { return _tokens; }

std::vector<JsonTokenType> JsonTokenizer::GetTokenTypes() const
{
    std::vector<JsonTokenType> output;
    output.reserve(_tokens.size());

    std::ranges::transform(_tokens, std::back_inserter(output), [](const JsonToken& token) { return token.Type; });

    return output;
}

std::string JsonTokenizer::GetError() const
{
    // If there is an error present, the last two tokens in the list should be Error, End

    if (_tokens.size() < 2)
    {
        return "success";
    }

    const auto token = _tokens[_tokens.size() - 2];

    if (token.Type != Error)
    {
        return "success";
    }

    return token.StringValue;
}

void JsonTokenizer::ScanToken()
{
    if (ScanNumber())
    {
        return;
    }

    if (ScanString())
    {
        return;
    }

    ScanSyntax();
}

bool JsonTokenizer::ScanNumber()
{

    if (std::isdigit(Peek()) || Peek() == '-')
    {
        const auto         start = _cursor;
        std::ostringstream source;

        // Consume the first character, this guarantees that we won't accept a dash later down the line.
        source << Consume();
        bool isInteger = true;

        while (!AtEnd() && (std::isdigit(Peek()) || Peek() == '.'))
        {
            if (Peek() == '.')
            {
                isInteger = false;
            }

            source << Consume();
        }

        PushToken(start, isInteger ? Integer : Float, source.str());
        return true;
    }

    return false;
}

bool JsonTokenizer::ScanUnicodeValue(std::uint32_t& value)
{
    std::array<char, 4> hexBuffer = {};

    // Read in hex characters.
    for (int i = 0; i < 4; ++i)
    {
        if (!std::isxdigit(Peek()))
        {
            // PushToken(start, Error, std::format("invalid unicode escape : {}", source.str()));
            return false;
        }

        hexBuffer[i] = static_cast<char>(Consume());
    }

    std::from_chars(hexBuffer.data(), hexBuffer.data() + 4, value, 16);
    return true;
}

bool JsonTokenizer::ScanString()
{
    const auto start = _cursor;

    if (MatchAndConsume('\"'))
    {
        std::ostringstream source;
        bool               pendingEscape = false;
        bool               reading       = true;

        while (reading)
        {
            if (AtEnd() || Peek() < 0x1F)
            {
                PushToken(start, Error, std::format("unterminated string : {}", source.str()));
                return true;
            }

            const auto c = Consume();

            if (c == '\\' && !pendingEscape)
            {
                pendingEscape = true;
            } else if (c == '"' && pendingEscape)
            {
                pendingEscape = false;
                source << '"';
            } else if (c == '\\' && pendingEscape)
            {
                pendingEscape = false;
                source << '\\';
            } else if (c == '/' && pendingEscape)
            {
                pendingEscape = false;
                source << '/';
            } else if (c == 'b' && pendingEscape)
            {
                pendingEscape = false;
                source << '\b';
            } else if (c == 'f' && pendingEscape)
            {
                pendingEscape = false;
                source << '\f';
            } else if (c == 'n' && pendingEscape)
            {
                pendingEscape = false;
                source << '\n';
            } else if (c == 'r' && pendingEscape)
            {
                pendingEscape = false;
                source << '\r';
            } else if (c == 't' && pendingEscape)
            {
                pendingEscape = false;
                source << '\t';
            } else if (c == 'u' && pendingEscape)
            {
                std::uint32_t value;
                if (!ScanUnicodeValue(value))
                {
                    PushToken(start, Error, std::format("invalid unicode escape : {}", source.str()));
                    return true;
                }

                // Encode into source stream.
                if (value <= 0x007F)
                {
                    source << static_cast<char>(value);
                } else if (value <= 0x07FF)
                {
                    source << static_cast<char>(0xC0 | (value >> 6));
                    source << static_cast<char>(0x80 | (value & 0x3F));
                } else if (value >= 0xD800 && value <= 0xDFFF)
                {
                    // Surrogate pair

                    if (!MatchAndConsume("\\u"))
                    {
                        PushToken(start, Error,
                                  std::format("surrogate pair not followed up with surrogate value", source.str()));
                        return true;
                    }

                    std::uint32_t trailingValue;
                    if (!ScanUnicodeValue(trailingValue))
                    {
                        PushToken(start, Error, std::format("invalid unicode escape (trailing) : {}", source.str()));
                        return true;
                    }

                    if (trailingValue < 0xDC00 || trailingValue > 0xDFFF)
                    {
                        PushToken(start, Error, std::format("invalid trailing surrogate value : {:X}", trailingValue));
                        return true;
                    }

                    // Combine value (high) & trailingValue (low) to create the final codepoint.
                    // 0x10000 + ((High - 0xD800) * 0x400) + (Low - 0xDC00)

                    const std::uint32_t codepoint = 0x10000 + ((value - 0xD800) << 10) + (trailingValue - 0xDC00);

                    source << static_cast<char>(0xF0 | (codepoint >> 18));
                    source << static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
                    source << static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                    source << static_cast<char>(0x80 | (codepoint & 0x3F));
                } else
                {
                    source << static_cast<char>(0xE0 | (value >> 12));
                    source << static_cast<char>(0x80 | ((value >> 6) & 0x3F));
                    source << static_cast<char>(0x80 | (value & 0x3F));
                }

                pendingEscape = false;
            } else if (c == '"')
            {
                // We're done.
                reading = false;
            } else
            {
                // Normal character. Pass into output buffer.
                source << c;
            }
        }

        PushToken(start, String, source.str());
        return true;
    }

    return false;
}

void JsonTokenizer::ScanSyntax()
{
    const auto start = _cursor;

    if (MatchAndConsume(':'))
    {
        PushToken(start, Colon);
    } else if (MatchAndConsume(','))
    {
        PushToken(start, Comma);
    } else if (MatchAndConsume('{'))
    {
        PushToken(start, LeftBrace);
    } else if (MatchAndConsume('}'))
    {
        PushToken(start, RightBrace);
    } else if (MatchAndConsume('['))
    {
        PushToken(start, LeftBracket);
    } else if (MatchAndConsume(']'))
    {
        PushToken(start, RightBracket);
    } else if (MatchAndConsume("null"))
    {
        PushToken(start, Null);
    } else if (MatchAndConsume("true"))
    {
        PushToken(start, True);
    } else if (MatchAndConsume("false"))
    {
        PushToken(start, False);
    } else
    {
        PushToken(start, Error, std::format("unexpected token: {}", Consume()));
    }
}

void JsonTokenizer::PushToken(const std::size_t start, const JsonTokenType type, const std::string& stringValue)
{
    _tokens.push_back(JsonToken{.Start       = start,
                                .End         = _cursor,
                                .Line        = _line,
                                .LineOffset  = _lineOffset - (_cursor - start),
                                .Type        = type,
                                .StringValue = stringValue});
}

bool JsonTokenizer::AtEnd() const { return _cursor == _input.length(); }

unsigned char JsonTokenizer::Peek(const std::size_t offset) const { return _input.at(_cursor + offset); }

unsigned char JsonTokenizer::Consume()
{
    const auto c = Peek();

    _cursor++;
    _lineOffset++;

    if (c == '\n')
    {
        // Advance line count.
        _line++;
        _lineOffset = 0;
    }

    return c;
}

bool JsonTokenizer::MatchAndConsume(const unsigned char c)
{
    if (!AtEnd() && Peek() == c)
    {
        Consume();
        return true;
    }

    return false;
}

bool JsonTokenizer::MatchAndConsume(const std::string& sequence)
{
    if (_input.length() - _cursor < sequence.length())
    {
        // Sequence is longer than the number of remaining character.s
        return false;
    }

    for (int i = 0; i < sequence.length(); ++i)
    {
        if (Peek(i) != sequence[i])
        {
            return false;
        }
    }

    for (int i = 0; i < sequence.length(); ++i)
    {
        Consume();
    }
    return true;
}

void JsonTokenizer::ConsumeWhitespace()
{
    if (AtEnd())
    {
        return;
    }

    while (!AtEnd() && (Peek() == ' ' || Peek() == '\r' || Peek() == '\t' || Peek() == '\n'))
    {
        Consume();
    }
}


} // namespace mail
