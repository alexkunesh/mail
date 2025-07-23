#pragma once

namespace mail
{

/**
 * Deserialization for simple types.
 * This specialization can be put inside of a translation unit.
 */
template<typename Deserializer, typename T> void Deserialize(Deserializer& deserializer, T& value);

} // namespace mail
