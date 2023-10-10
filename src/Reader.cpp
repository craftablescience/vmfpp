#include <vmfpp/Reader.h>

#include <sstream>
#include <fstream>

#include <vmfpp/detail/StringUtils.h>
#include <vmfpp/VMF.h>

using namespace vmfpp;
using namespace vmfpp::detail;

constexpr char CHAR_ENTER_SCOPE = '{';
constexpr char CHAR_EXIT_SCOPE = '}';
constexpr char CHAR_QUOTE = '\"';
constexpr std::string_view CHAR_DISCARD_CUTSET = "{} \r\n";
constexpr std::string_view NODE_ROOT_KEY = "$root";

bool readNode(std::istream& stream, Node* scope, std::string_view key = NODE_ROOT_KEY) {
    for (std::string line; std::getline(stream, line); ) {
        if (line.empty()) {
            continue;
        }
        // New scope
        else if (line.contains(CHAR_ENTER_SCOPE)) {
            // Scope is opened after the key is read
            // There may be situations where there is no key, so we must account for that
            if ((key == "" && (scope->getChildren().size() + scope->getValues().size()) > 0) || key == NODE_ROOT_KEY) {
                Node newScope;
                if (!readNode(stream, &newScope)) {
                    return false;
                }
                scope->addChild(key.data(), newScope);
            }
        }
        // Exit scope
        else if (line.contains(CHAR_EXIT_SCOPE)) {
            break;
        }
        // Value
        else if (line.contains(CHAR_QUOTE)) {
            auto p = split(line, CHAR_QUOTE);
            if (std::count(line.begin(), line.end(), CHAR_QUOTE) == 3) {
                // Multi-line property value, because value quotes aren't closed
                // Read lines until we encounter a closing quote
                for (; std::getline(stream, line); ) {
                    if (line.contains(CHAR_QUOTE)) {
                        p[3] += '\n' + split(line, CHAR_QUOTE)[0];
                        break;
                    }
                    p[3] += '\n' + line;
                }
            }
            scope->addValue(p[1], p[3]);
        }
        // Child
        else {
            Node newScope;
            std::string newKey = line;
            trim(newKey, CHAR_DISCARD_CUTSET);
            trim(newKey);
            if (!readNode(stream, &newScope, newKey)) {
                return false;
            }
            scope->addChild(newKey, newScope);
        }
    }
    return true;
}

bool Reader::readData(Root& root, std::string_view data) {
    std::istringstream file{data.data()};
    return readNode(file, reinterpret_cast<Node*>(&root), NODE_ROOT_KEY);
}

bool Reader::readFile(Root& root, std::string_view filePath) {
    std::ifstream file{filePath.data(), std::ios_base::in};
    if (!file.is_open()) {
        return false;
    }
    return readNode(file, reinterpret_cast<Node*>(&root), NODE_ROOT_KEY);
}
