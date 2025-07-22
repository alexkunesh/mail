#pragma once
#include <vector>
#include <iostream>

namespace mail
{

/**
 * Serialization for simple types.
 * This specialization can be put inside of a translation unit.
 */
template<typename Serializer, typename T> void Serialize(Serializer& serializer, const T& value);

// Serialize for container types

// // ... sorry
// // T = typename, N = numeric type
//
// template<typename T1, typename T2> void SerializeT(Serializer& serializer, const T1<T2>& value);

} // namespace mail
