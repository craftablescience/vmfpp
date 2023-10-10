#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace vmfpp {

namespace DEFAULT_SECTIONS {

constexpr std::string_view VERSIONINFO = "versioninfo";
constexpr std::string_view VISGROUPS = "visgroups";
constexpr std::string_view VIEWSETTINGS = "viewsettings";
constexpr std::string_view WORLD = "world";
constexpr std::string_view ENTITY = "entity";
constexpr std::string_view CAMERAS = "cameras";
constexpr std::string_view CORDON = "cordon"; // Pre-L4D
constexpr std::string_view CORDONS = "cordons"; // Post-L4D
constexpr std::string_view HIDDEN = "hidden";

} // namespace DEFAULT_SECTIONS

// A KeyValue object with one key, multiple keyvalues
class Node {
public:
    Node() = default;

    [[nodiscard]] const std::unordered_map<std::string, std::string>& getValues() const;

    [[nodiscard]] std::unordered_map<std::string, std::string>& getValues();

    [[nodiscard]] bool hasValue(const std::string& key) const;

    [[nodiscard]] std::string_view getValue(const std::string& key) const;

    void addValue(std::string key, std::string value);

    void removeValue(const std::string& key);

    [[nodiscard]] const std::unordered_map<std::string, std::vector<Node>>& getChildren() const;

    [[nodiscard]] std::unordered_map<std::string, std::vector<Node>>& getChildren();

    [[nodiscard]] bool hasChild(const std::string& key) const;

    [[nodiscard]] const std::vector<vmfpp::Node>& getChild(const std::string& key) const;

    void addChild(std::string key, Node value);

    void removeChild(const std::string& key);

    [[nodiscard]] std::size_t getNumItems() const;

protected:
    std::unordered_map<std::string, std::string> values;
    std::unordered_map<std::string, std::vector<Node>> children;
};

// A KeyValue object with no key, only children
class Root : protected Node {
public:
    Root() = default;

    [[nodiscard]] const std::unordered_map<std::string, std::vector<Node>> getSections() const;

    [[nodiscard]] std::unordered_map<std::string, std::vector<Node>> getSections();

    [[nodiscard]] bool hasSection(std::string_view key) const;

    [[nodiscard]] const std::vector<vmfpp::Node>& getSection(std::string_view key) const;

    void addSection(std::string key, Node value);

    void addSection(std::string_view key, Node value);

    void removeSection(std::string_view key);
};

} // namespace vmfpp
