#ifndef EXTRA_REQS_H
#define EXTRA_REQS_H

#include <string>
#include <vector>
#include <memory>

struct Layout;
struct Format;

// created by this learning environment!
struct Desc final {
    std::string pre_condition;
    std::string basic_script;
    std::vector<std::string> alt_scripts;

    // ***

    // ref can be specified in widget.
    std::vector<std::weak_ptr<Layout>> layouts;
    std::vector<std::weak_ptr<Format>> formats;
};

// -----------------------------------------------------------------------

struct Layout final {
    struct Meta {
        std::string img_ref; // exs: MainWindow.png
        std::string src_ref; // exs: MainWindow.wsd
    };
};

struct Format final {
    struct Meta {
        std::string img_ref; // exs: DbFacade.png
        std::string src_ref; // exs: DbFacade.wsd
    };
};

#endif // EXTRA_REQS_H
