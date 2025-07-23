#pragma once
#include <cstdint>
#include <cmath>
#include <span>
#include <string>

#include "deserialize.h"

namespace mail
{

/**
 * Abstract class that defines the basic functionality of a deserializer.
 * Assume this class is NOT thread-safe.
 */
class Deserializer
{
public:
    virtual ~Deserializer() = default;

    /**
     * Forward declaration for mail::Deserialize.
     */
    template<typename T> void Value(T& value) { mail::Deserialize(*this, value); }

    // Struct
    virtual void BeginStruct()                       = 0;
    virtual void BeginField(const std::string& name) = 0;
    virtual void EndField()                          = 0;
    virtual void EndStruct()                         = 0;

    // Scalars
    virtual void BoolValue(bool& value)          = 0;
    virtual void I8Value(std::int8_t& value)     = 0;
    virtual void I16Value(std::int16_t& value)   = 0;
    virtual void I32Value(std::int32_t& value)   = 0;
    virtual void I64Value(std::int64_t& value)   = 0;
    virtual void U8Value(std::uint8_t& value)    = 0;
    virtual void U16Value(std::uint16_t& value)  = 0;
    virtual void U32Value(std::uint32_t& value)  = 0;
    virtual void U64Value(std::uint64_t& value)  = 0;
    virtual void F32Value(std::float_t& value)   = 0;
    virtual void F64Value(std::double_t& value)  = 0;
    virtual void StringValue(std::string& value) = 0;

    // Lists
    virtual void BeginList(bool knownLength, std::size_t& length) = 0;
    virtual void TraverseList()                                   = 0;
    virtual void EndList()                                        = 0;

    // Utility functions
    template<typename T> void Field(const std::string& name, T& value)
    {
        BeginField(name);
        Value(value);
        EndField();
    }
};

} // namespace mail
