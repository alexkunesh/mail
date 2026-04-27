#pragma once

#include <array>
#include <vector>

#include "serialize.h"

namespace mail
{

template<typename Serializer, typename T2> void Serialize(Serializer& serializer, const std::vector<T2>& value)
{
    serializer.BeginList(false, value.size());
    for (auto element: value)
    {
        Serialize(serializer, element);
    }
    serializer.EndList();
}

template<typename Serializer, typename T2, std::size_t Length>
void Serialize(Serializer& serializer, const std::array<T2, Length>& value)
{
    serializer.BeginList(true, -1);
    for (auto element: value)
    {
        Serialize(serializer, element);
    }
    serializer.EndList();
}

} // namespace mail
