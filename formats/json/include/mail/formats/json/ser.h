#pragma once
#include <mail/s/serializer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace mail
{
class JsonSerializer final : public Serializer
{
private:
    rapidjson::StringBuffer                    _buffer;
    rapidjson::Writer<rapidjson::StringBuffer> _writer;

public:
    JsonSerializer() : _writer(_buffer) {}

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
    void StringValue(const std::string& value) override;

    void BeginList(bool knownSize, std::size_t length) override;
    void EndList() override;

    // Outputs
    std::string ToString() const;
};

} // namespace mail
