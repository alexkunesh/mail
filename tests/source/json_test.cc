#include <string>
#include <gtest/gtest.h>
#include <mail/s/serialize_std.h>
#include <mail/formats/json.h>
#include "data.h"

// const TestDataA ExampleTestData{.a                   = 533674,
//     .b                   = -3243463,
//     .text                = "Easter pink colors, race cars",
//     .fourShorts          = {555, 4831, 60000, 9},
//     .dynamicUnsignedInts = {1510223999, 13433002, 2424854066}};

const std::string ExampleTestDataJson =
        "{\"a\":533674,\"b\":-3243463,\"text\":\"Easter pink colors, race "
        "cars\",\"fourShorts\":[555,4831,60000,9],\"dynamicUnsignedInts\":[1510223999,13433002,2424854066]}";

TEST(JsonPrimitives, Serialization)
{
    EXPECT_EQ(mail::ToJson<std::uint8_t>(6), "6");
    EXPECT_EQ(mail::ToJson<std::uint16_t>(400), "400");
    EXPECT_EQ(mail::ToJson<std::uint32_t>(100000), "100000");
}

TEST(TestDataA, JsonSerialization) { EXPECT_EQ(mail::ToJson(ExampleTestData), ExampleTestDataJson); }
