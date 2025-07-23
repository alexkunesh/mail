#pragma once
#include <array>
#include <vector>

#include "deserialize.h"

namespace mail
{

template<typename Deserializer, typename T2> void Deserialize(Deserializer& deserializer, std::vector<T2>& value)
{
    // Retrieve the length
    std::size_t length;
    deserializer.BeginList(false, length);

    // TODO: assert that value is empty

    // Pre-allocate space for the value
    value.reserve(length);

    for (std::size_t i = 0; i < length; i++)
    {
        T2 v;
        Deserialize(deserializer, v);
        value.push_back(v);

        if (i != length - 1)
        {
            deserializer.TraverseList();
        }
    }

    deserializer.EndList();
}

} // namespace mail
