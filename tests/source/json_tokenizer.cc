#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mail/formats/json/tokenizer.h>

// *sigh* no wildcard in using statements :(
using mail::JsonTokenType::Colon;
using mail::JsonTokenType::Comma;
using mail::JsonTokenType::End;
using mail::JsonTokenType::Error;
using mail::JsonTokenType::False;
using mail::JsonTokenType::Float;
using mail::JsonTokenType::Integer;
using mail::JsonTokenType::LeftBrace;
using mail::JsonTokenType::LeftBracket;
using mail::JsonTokenType::Null;
using mail::JsonTokenType::RightBrace;
using mail::JsonTokenType::RightBracket;
using mail::JsonTokenType::String;
using mail::JsonTokenType::True;

using testing::ElementsAre;

TEST(JsonTokenizer, ScanNumber)
{
    ASSERT_THAT(mail::JsonTokenizer("12352").GetTokenTypes(), ElementsAre(Integer, End));
    ASSERT_THAT(mail::JsonTokenizer("1.0").GetTokenTypes(), ElementsAre(Float, End));
    ASSERT_THAT(mail::JsonTokenizer("-5234").GetTokenTypes(), ElementsAre(Integer, End));
    ASSERT_THAT(mail::JsonTokenizer("-32.52").GetTokenTypes(), ElementsAre(Float, End));
}

TEST(JsonTokenizer, ScanNumberLeadingPeriodFails)
{
    ASSERT_THAT(mail::JsonTokenizer(".0").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanNumberProperlyTerminates)
{
    ASSERT_THAT(mail::JsonTokenizer("24325.2723f").GetTokenTypes(), ElementsAre(Float, Error, End));
}

TEST(JsonTokenizer, ScanString)
{
    ASSERT_THAT(mail::JsonTokenizer("\"\"").GetTokenTypes(), ElementsAre(String, End));
    ASSERT_THAT(mail::JsonTokenizer("\"hello world\"").GetTokenTypes(), ElementsAre(String, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Single quotes ' ' '\"").GetTokenTypes(), ElementsAre(String, End));
}

TEST(JsonTokenizer, ScanStringEscapes)
{
    ASSERT_THAT(mail::JsonTokenizer("\"A string with \\\"quotes\\\" inside\"").GetTokenTypes(),
                ElementsAre(String, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Path\\\\to\\\\a\\\\file.txt\"").GetTokenTypes(), ElementsAre(String, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Forward \\/ slash\"").GetTokenTypes(), ElementsAre(String, End));
}

TEST(JsonTokenizer, ScanStringControlEscapes)
{
    ASSERT_THAT(mail::JsonTokenizer("\"Backspace \\b, Formfeed \\f, Newline \\n, Carriage Return \\r, Tab \\t\"")
                        .GetTokenTypes(),
                ElementsAre(String, End));
}

TEST(JsonTokenizer, ScanStringUnicode)
{
    ASSERT_THAT(mail::JsonTokenizer("\"Unicode escape: \\u0041\"").GetTokenTypes(), ElementsAre(String, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Surrogate pair: \\uD834\\uDD1E\"").GetTokenTypes(), ElementsAre(String, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Control hex: \\u0000 \\u001F\"").GetTokenTypes(), ElementsAre(String, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Emoji: 🚀✨ and Japanese: こんにちは\"").GetTokenTypes(),
                ElementsAre(String, End));
}

TEST(JsonTokenizer, ScanStringValidSurrogatePairs)
{
    ASSERT_EQ(mail::JsonTokenizer("\"\\uD83D\\uDE80\"").GetTokens()[0].StringValue, "🚀");
    ASSERT_EQ(mail::JsonTokenizer("\"\\uD83D\\uDE02\"").GetTokens()[0].StringValue, "😂");
    ASSERT_EQ(mail::JsonTokenizer("\"\\uD83C\\uDC04\"").GetTokens()[0].StringValue, "🀄");
    ASSERT_EQ(mail::JsonTokenizer("\"\\uD835\\uDD38\"").GetTokens()[0].StringValue, "𝔸");
}

TEST(JsonTokenizer, ScanStringOrphanedHighSurrogateError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"\\uD83D\"").GetTokenTypes(), ElementsAre(Error, End));
    ASSERT_THAT(mail::JsonTokenizer("\"\\uD83Dhello\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringInvalidTrailingSurrogateError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"\\uD83D\\u0041\"").GetTokenTypes(), ElementsAre(Error, End));
    ASSERT_THAT(mail::JsonTokenizer("\"\\uD83D\\uD83D\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringMissingTrailingSurrogateHexError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"\\uD83D\\u12\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringOrphanedLowSurrogateError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"\\uDE80\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringUnterminatedError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"This string has no end quote").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringLiteralControlCharacterError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"Literal\nnewline\"").GetTokenTypes(), ElementsAre(Error, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Literal\ttab\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringInvalidEscapeSequenceError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"Invalid escape \\x\"").GetTokenTypes(), ElementsAre(Error, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Escaping a random letter \\z\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringInvalidUnicodeEscapeError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"Bad unicode \\u123\"").GetTokenTypes(), ElementsAre(Error, End));
    ASSERT_THAT(mail::JsonTokenizer("\"Bad unicode \\u12X4\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanStringNewLineError)
{
    ASSERT_THAT(mail::JsonTokenizer("\"Line #1\nLine#2\"").GetTokenTypes(), ElementsAre(Error, End));
}

TEST(JsonTokenizer, ScanSyntax)
{
    ASSERT_THAT(mail::JsonTokenizer("{}[]:,").GetTokenTypes(),
                ElementsAre(LeftBrace, RightBrace, LeftBracket, RightBracket, Colon, Comma, End));
}


TEST(JsonTokenizer, EmptyObject)
{
    ASSERT_THAT(mail::JsonTokenizer("{}").GetTokenTypes(), ElementsAre(LeftBrace, RightBrace, End));
}

TEST(JsonTokenizer, SimpleObject)
{
    ASSERT_THAT(mail::JsonTokenizer("{\"key\": \"value\"}").GetTokenTypes(),
                ElementsAre(LeftBrace, String, Colon, String, RightBrace, End));
}

TEST(JsonTokenizer, MultiKeyValueObject)
{
    ASSERT_THAT(mail::JsonTokenizer("{\"name\": \"Alice\", \"age\": 30}").GetTokenTypes(),
                ElementsAre(LeftBrace, String, Colon, String, Comma, String, Colon, Integer, RightBrace, End));
}

TEST(JsonTokenizer, NestedObject)
{
    ASSERT_THAT(mail::JsonTokenizer("{\"user\": {\"id\": 1}}").GetTokenTypes(),
                ElementsAre(LeftBrace, String, Colon, LeftBrace, String, Colon, Integer, RightBrace, RightBrace, End));
}

TEST(JsonTokenizer, ObjectWithWhitespace)
{
    ASSERT_THAT(mail::JsonTokenizer("{\n    \"key1\" : 123,\n    \"key2\" : \"test\"\n}").GetTokenTypes(),
                ElementsAre(LeftBrace, String, Colon, Integer, Comma, String, Colon, String, RightBrace, End));
}

TEST(JsonTokenizer, ObjectWithMixedTypes)
{
    ASSERT_THAT(mail::JsonTokenizer("{\"str\": \"a\", \"num\": -1.5, \"arr\": []}").GetTokenTypes(),
                ElementsAre(LeftBrace, String, Colon, String, Comma, String, Colon, Float, Comma, String, Colon,
                            LeftBracket, RightBracket, RightBrace, End));
}
