#include <cstddef>
#include <cmath>
#include <mail/serialize.h>
#include <mail/serializer.h>
#include <mail/deserialize.h>
#include <mail/deserializer.h>


#define MAIL_IMPL_PRIMITIVE(TYPE, FUNCTION)                                                                            \
    template<> void Serialize(Serializer& serializer, const TYPE& value) { serializer.FUNCTION(value); }               \
    template<> void Deserialize(Deserializer& deserializer, TYPE& value) { deserializer.FUNCTION(value); }

namespace mail
{

MAIL_IMPL_PRIMITIVE(bool, BoolValue)
MAIL_IMPL_PRIMITIVE(std::int8_t, I8Value)
MAIL_IMPL_PRIMITIVE(std::int16_t, I16Value)
MAIL_IMPL_PRIMITIVE(std::int32_t, I32Value)
MAIL_IMPL_PRIMITIVE(std::int64_t, I64Value)
MAIL_IMPL_PRIMITIVE(std::uint8_t, U8Value)
MAIL_IMPL_PRIMITIVE(std::uint16_t, U16Value)
MAIL_IMPL_PRIMITIVE(std::uint32_t, U32Value)
MAIL_IMPL_PRIMITIVE(std::uint64_t, U64Value)
MAIL_IMPL_PRIMITIVE(std::float_t, F32Value)
MAIL_IMPL_PRIMITIVE(std::double_t, F64Value)
MAIL_IMPL_PRIMITIVE(std::string, StringValue)

template<> void Serialize(Serializer& serializer, const std::byte& value)
{
    serializer.U8Value(static_cast<std::uint8_t>(value));
}

template<> void Deserialize(Deserializer& deserializer, std::byte& value)
{
    deserializer.U8Value(reinterpret_cast<std::uint8_t&>(value));
}

// TODO: Implement char

// namespace ser
} // namespace mail
