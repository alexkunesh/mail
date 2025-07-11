#pragma once
#include <stack>
#include <mail/ser.h>
#include <toml.hpp>

namespace mail
{

class TomlSerializer final : public Serializer
{
private:
    toml::value              _document;
    std::stack<toml::value*> _stack;
    bool                     _isFieldPending{false};
    std::string              _pendingFieldName;

public:
    TomlSerializer();

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
    void CharValue(char value) override;
    void StringValue(const std::string& value) override;

    // Outputs
    std::string ToString() const;
};

} // namespace mail
