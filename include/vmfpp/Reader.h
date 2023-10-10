#pragma once

#include "VMF.h"

namespace vmfpp {

class Reader {
public:
    Reader() = default;

    [[nodiscard]] bool readData(Root& root, std::string_view data);

    [[nodiscard]] bool readFile(Root& root, std::string_view filePath);
};

} // namespace vmfpp
