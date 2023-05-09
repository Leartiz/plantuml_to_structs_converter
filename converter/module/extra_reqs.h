#ifndef EXTRA_REQS_H
#define EXTRA_REQS_H

#include <string>

struct Desc final {
    std::string src_ref;
};

struct Layout final {
    std::string img_ref;
    std::string src_ref;
    std::string editor_type;
};

struct Format final {
    std::string img_ref;
    std::string src_ref;
    std::string editor_type;
};

#endif // EXTRA_REQS_H
