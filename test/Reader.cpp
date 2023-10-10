#include <gtest/gtest.h>

#include <vmfpp/Reader.h>

#include "Config.h"

using namespace vmfpp;

TEST(Reader, rootEmpty) {
    constexpr std::string_view data = R"(
node
{
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 1);
    ASSERT_TRUE(root.hasSection("node"));
    ASSERT_EQ(root.getSections().at("node").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getNumItems(), 0);
}

TEST(Reader, rootMulti) {
    constexpr std::string_view data = R"(
node1
{
}
node2
{
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 2);
    ASSERT_TRUE(root.hasSection("node1"));
    ASSERT_TRUE(root.hasSection("node2"));
    ASSERT_EQ(root.getSections().at("node1").size(), 1);
    ASSERT_EQ(root.getSections().at("node1").at(0).getNumItems(), 0);
    ASSERT_EQ(root.getSections().at("node2").size(), 1);
    ASSERT_EQ(root.getSections().at("node2").at(0).getNumItems(), 0);
}

TEST(Reader, valueBasic) {
    constexpr std::string_view data = R"(
node
{
    "key" "value"
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 1);
    ASSERT_TRUE(root.hasSection("node"));
    ASSERT_EQ(root.getSections().at("node").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getNumItems(), 1);
    ASSERT_TRUE(root.getSections().at("node").at(0).hasValue("key"));
    ASSERT_STREQ(root.getSections().at("node").at(0).getValue("key").data(), "value");
}

TEST(Reader, valueMultiline) {
    constexpr std::string_view data = R"(
node
{
    "key" "value
on
several lines"
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 1);
    ASSERT_TRUE(root.hasSection("node"));
    ASSERT_EQ(root.getSections().at("node").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getNumItems(), 1);
    ASSERT_TRUE(root.getSections().at("node").at(0).hasValue("key"));
    ASSERT_STREQ(root.getSections().at("node").at(0).getValue("key").data(), "value\non\nseveral lines");
}

TEST(Reader, childEmpty) {
    constexpr std::string_view data = R"(
node
{
    key
    {
    }
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 1);
    ASSERT_TRUE(root.hasSection("node"));
    ASSERT_EQ(root.getSections().at("node").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getNumItems(), 1);
    ASSERT_TRUE(root.getSections().at("node").at(0).hasChild("key"));
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key").at(0).getNumItems(), 0);
}

TEST(Reader, childBasic) {
    constexpr std::string_view data = R"(
node
{
    key
    {
        "inner" "value"
    }
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 1);
    ASSERT_TRUE(root.hasSection("node"));
    ASSERT_EQ(root.getSections().at("node").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getNumItems(), 1);
    ASSERT_TRUE(root.getSections().at("node").at(0).hasChild("key"));
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key").at(0).getNumItems(), 1);
    ASSERT_TRUE(root.getSections().at("node").at(0).getChild("key").at(0).hasValue("inner"));
    ASSERT_STREQ(root.getSections().at("node").at(0).getChild("key").at(0).getValue("inner").data(), "value");
}

TEST(Reader, valueBasicAndChild) {
    constexpr std::string_view data = R"(
node
{
    "key" "value"
    key2
    {
    }
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 1);
    ASSERT_TRUE(root.hasSection("node"));
    ASSERT_EQ(root.getSections().at("node").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getNumItems(), 2);
    ASSERT_TRUE(root.getSections().at("node").at(0).hasValue("key"));
    ASSERT_STREQ(root.getSections().at("node").at(0).getValue("key").data(), "value");
    ASSERT_TRUE(root.getSections().at("node").at(0).hasChild("key2"));
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key2").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key2").at(0).getNumItems(), 0);
}

TEST(Reader, valueMultilineAndChild) {
    constexpr std::string_view data = R"(
node
{
    "key" "value
on
several lines"
    key2
    {
    }
}
)";

    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readData(root, data));
    ASSERT_EQ(root.getSections().size(), 1);
    ASSERT_TRUE(root.hasSection("node"));
    ASSERT_EQ(root.getSections().at("node").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getNumItems(), 2);
    ASSERT_TRUE(root.getSections().at("node").at(0).hasValue("key"));
    ASSERT_STREQ(root.getSections().at("node").at(0).getValue("key").data(), "value\non\nseveral lines");
    ASSERT_TRUE(root.getSections().at("node").at(0).hasChild("key2"));
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key2").size(), 1);
    ASSERT_EQ(root.getSections().at("node").at(0).getChild("key2").at(0).getNumItems(), 0);
}

TEST(Reader, file) {
    Root root;
    Reader reader;
    ASSERT_TRUE(reader.readFile(root, TEST_VMF_PATH));
    ASSERT_EQ(root.getSections().size(), 9);
    ASSERT_TRUE(root.hasSection("entity"));
    ASSERT_EQ(root.getSections().at("entity").size(), 3);
    ASSERT_EQ(root.getSections().at("entity").at(0).getNumItems(), 5);
    ASSERT_TRUE(root.getSections().at("entity").at(0).hasValue("classname"));
    ASSERT_STREQ(root.getSections().at("entity").at(0).getValue("classname").data(), "info_player_start");
    ASSERT_TRUE(root.getSections().at("entity").at(0).hasChild("editor"));
    ASSERT_EQ(root.getSections().at("entity").at(0).getChild("editor").size(), 1);
    ASSERT_EQ(root.getSections().at("entity").at(0).getChild("editor").at(0).getNumItems(), 4);
    ASSERT_TRUE(root.getSections().at("entity").at(0).getChild("editor").at(0).hasValue("color"));
    ASSERT_STREQ(root.getSections().at("entity").at(0).getChild("editor").at(0).getValue("color").data(), "0 255 0");
}
