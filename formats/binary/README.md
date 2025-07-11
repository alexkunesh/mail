# Mail Binary Format (v1)

The binary format Mail uses is made-up on the spot. It is unstructured and expects the code to know exactly what the
payload is supposed to contain. It supports both big- and little-endian. However, due to its higher commonality,
little-endian is the default (even on big-endian systems, as this format is intended for transmission.).

# Strings

Strings are encoded first with their length (in a 64-bit unsigned integer), then the actual content of the string.
Currently, there is no effort to normalize the encoding of the string. Though, if there was, it would be normalized to
UTF-8.