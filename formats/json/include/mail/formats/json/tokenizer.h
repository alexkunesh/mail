#pragma once
#include <initializer_list>
#include <optional>
#include <string>
#include <vector>

namespace mail
{

enum JsonTokenType
{
    /** { */ LeftBrace,
    /** } */ RightBrace,

    /** [ */ LeftBracket,
    /** ] */ RightBracket,

    /** : */ Colon,
    /** , */ Comma,

    /** A quote, followed by the content, and then another quote. */ String,
    /** Whole number. */ Integer,
    /** Rational number. */ Float,

    /** null */ Null,
    /** true */ True,
    /** false */ False,

    /** EOF. Automatically placed when reaching the end of the stream. */ End,

    Error,

};

struct JsonToken
{
    std::size_t   Start, End, Line, LineOffset;
    JsonTokenType Type;
    std::string   StringValue;
};

class JsonTokenizer
{
public:
    explicit JsonTokenizer(const std::string_view& input);

    [[nodiscard]] const std::vector<JsonToken>& GetTokens() const;
    [[nodiscard]] std::vector<JsonTokenType>    GetTokenTypes() const;
    [[nodiscard]] std::string                   GetError() const;

private:
    std::string_view       _input;
    std::vector<JsonToken> _tokens;

    std::size_t _cursor     = 0;
    std::size_t _line       = 1;
    std::size_t _lineOffset = 0;

    void ScanToken();
    bool ScanNumber();
    bool ScanUnicodeValue(std::uint32_t& value);
    bool ScanString();
    void ScanSyntax();

    void PushToken(std::size_t start, JsonTokenType type, const std::string& stringValue = "");

    /** Have we reached the end of the input yet? */
    [[nodiscard]] bool AtEnd() const;

    /** Views the next character (without consuming). */
    [[nodiscard]] unsigned char Peek(std::size_t offset = 0) const;

    /** Consumes the next character. */
    unsigned char Consume();
    bool          MatchAndConsume(unsigned char c);
    bool          MatchAndConsume(const std::string& sequence);

    /** Consumes all whitespace. It will be discarded. */
    void ConsumeWhitespace();
};

} // namespace mail
