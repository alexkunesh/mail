#pragma once
#include <cstdint>
#include <cmath>
#include <span>
#include <string>

namespace mail
{

/**
 * Serialization for simple types.
 * This specialization can be put inside of a translation unit.
 */
template<typename T> void Serialize(class Serializer& serializer, const T& value);

/**
 * Abstract class that defines the basic functionality of a serializer.
 * Assume this class is NOT thread-safe.
 */
class Serializer
{
public:
    virtual ~Serializer() = default;

    /**
     * Forward declaration for mail::Serialize.
     */
    template<typename T> void Value(const T& value) { mail::Serialize(*this, value); }

    // Struct
    virtual void BeginStruct()                       = 0;
    virtual void BeginField(const std::string& name) = 0;
    virtual void EndField()                          = 0;
    virtual void EndStruct()                         = 0;

    // Scalars
    virtual void BoolValue(bool value)                 = 0;
    virtual void I8Value(std::int8_t value)            = 0;
    virtual void I16Value(std::int16_t value)          = 0;
    virtual void I32Value(std::int32_t value)          = 0;
    virtual void I64Value(std::int64_t value)          = 0;
    virtual void U8Value(std::uint8_t value)           = 0;
    virtual void U16Value(std::uint16_t value)         = 0;
    virtual void U32Value(std::uint32_t value)         = 0;
    virtual void U64Value(std::uint64_t value)         = 0;
    virtual void F32Value(std::float_t value)          = 0;
    virtual void F64Value(std::double_t value)         = 0;
    virtual void StringValue(const std::string& value) = 0;
    // virtual void BytesValue(std::span<std::uint8_t> value) = 0;

    // Utility functions
    template<typename T> void Field(const std::string& name, const T& value)
    {
        BeginField(name);
        Value(value);
        EndField();
    }
};


} // namespace mail
