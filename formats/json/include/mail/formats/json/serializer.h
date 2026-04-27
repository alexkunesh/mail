#pragma once
#include <sstream>
#include <stack>
#include <mail/serializer.h>

namespace mail
{
class JsonSerializer final : public Serializer
{
private:
    enum class Context
    {
        None,
        Struct,
        Field,
        List,
    };

    std::ostringstream  _outputStream;
    bool                _pendingComma = false;
    std::stack<Context> _contextStack;

    Context CurrentContext() const;
    void    PushContext(Context);
    void    PopContext();

    void TryAddLastCommaInLists();


public:
    JsonSerializer() {}
    static std::string EscapeString(const std::string& input);

    void BeginStruct() override;
    void BeginField(const std::string& name) override;
    void EndField() override;
    void EndStruct() override;

    void BoolValue(bool value) override;
    void I8Value(std::int8_t value) override;
    void I16Value(std::int16_t value) override;
    void I32Value(std::int32_t value) override;
    void I64Value(std::int64_t value) override;
    void U8Value(std::uint8_t value) override;
    void U16Value(std::uint16_t value) override;
    void U32Value(std::uint32_t value) override;
    void U64Value(std::uint64_t value) override;
    void F32Value(std::float_t value) override;
    void F64Value(std::double_t value) override;

    void UnsignedCharValue(unsigned char value) override;
    void CharValue(char value) override;
    void StringValue(const std::string& value) override;

    void BeginList(bool knownSize, std::size_t length) override;
    void EndList() override;

    // Outputs
    std::string ToString() const;
};

} // namespace mail
