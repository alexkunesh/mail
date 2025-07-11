#pragma once
#include "conversion.h"


#include <bit>
#include <span>
#include <mail/de.h>

namespace mail
{

class BinaryDeserializer : public Deserializer
{
private:
    std::span<std::byte> _buffer;
    std::size_t          _cursor;

private:
    template<typename T> void NumericValue(T& value)
    {
        std::span<std::byte, sizeof(T)> data = _buffer.subspan(_cursor, _cursor + sizeof(T));
        value                                = FromBytes<T>(data);

        // Advance the cursor.
        _cursor += sizeof(T);
    }

public:
    explicit BinaryDeserializer(const std::span<std::byte>& input);

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
    void CharValue(char& value) override;
    void StringValue(std::string& value) override;
};

} // namespace mail
