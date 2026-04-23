#pragma once
#include <mail/formats/json/serializer.h>
#include <mail/formats/json/deserializer.h>

namespace mail
{

template<typename T> std::string ToJson(const T& data)
{
    mail::JsonSerializer serializer;
    serializer.Value(data);
    return serializer.ToString();
}

} // namespace mail
