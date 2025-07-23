#pragma once
#include <stack>
#include <mail/d/deserializer.h>
#include <toml.hpp>

namespace mail
{

class TomlDeserializer : public Deserializer
{
private:
    toml::value              _document;
    std::stack<toml::value*> _stack;

public:
    explicit TomlDeserializer(const std::string& input);

    void BeginStruct() override;
    void BeginField(const std::string& name) override;
    void EndField() override;
    void EndStruct() override;

    void BoolValue(bool& value) override;
    void I8Value(std::int8_t& value) override;
    void I16Value(std::int16_t& value) override;
    void I32Value(std::int32_t& value) override;
    void I64Value(std::int64_t& value) override;
    void U8Value(std::uint8_t& value) override;
    void U16Value(std::uint16_t& value) override;
    void U32Value(std::uint32_t& value) override;
    void U64Value(std::uint64_t& value) override;
    void F32Value(std::float_t& value) override;
    void F64Value(std::double_t& value) override;
    void StringValue(std::string& value) override;
};

} // namespace mail
