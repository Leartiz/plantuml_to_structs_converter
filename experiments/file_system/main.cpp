#include <iostream>
#include <filesystem>

using namespace std;

void check_project_v1() {
    // ***

    auto path = filesystem::current_path();
    cout << path << endl;

    path = path.parent_path().parent_path();
    path = path / "converter" / "test" / "projects" / "hierarchy";
    if (filesystem::exists(path) == true) {
        cout << "project root path - [OK]" << endl;
    }

    cout << endl;

    // ***

    const string uc_dia_dir =    "use_case_dia";
    const string class_dia_dir = "class_dia";

    cout << uc_dia_dir;
    if (!std::filesystem::exists(path / uc_dia_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    cout << class_dia_dir;
    if (!std::filesystem::exists(path / class_dia_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    // ***

    const string sequence_dias_dir =   "sequence_dias";
    const string robustness_dias_dir = "robustness_dias";

    cout << sequence_dias_dir;
    if (!std::filesystem::exists(path / sequence_dias_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    cout << robustness_dias_dir;
    if (!std::filesystem::exists(path / robustness_dias_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    // ***

    const string extra_reqs_dir = "extra_reqs";

    cout << extra_reqs_dir;
    if (!std::filesystem::exists(path / extra_reqs_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    std::cout << std::endl;

    // just for this!
    // ***

    const string uc_dia_src =    "use_case_dia.wsd";
    const string class_dia_src = "class_dia.wsd";

    cout << uc_dia_src;
    if (!std::filesystem::exists(path /
                                 uc_dia_dir /
                                 "src" /
                                 uc_dia_src))

        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    cout << class_dia_src;
    if (!std::filesystem::exists(path /
                                 class_dia_dir /
                                 "src" /
                                 class_dia_src))

        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    // ***

    const string layouts_dir = "layouts";
    const string formats_dir = "formats";
    const string descs_dir =   "descs";

    cout << layouts_dir;
    if (!std::filesystem::exists(path / extra_reqs_dir /
                                 layouts_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    cout << formats_dir;
    if (!std::filesystem::exists(path / extra_reqs_dir /
                                 formats_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    cout << descs_dir;
    if (!std::filesystem::exists(path / extra_reqs_dir /
                                 descs_dir))
        std::cout << " - [BAD]" << std::endl;
    else std::cout << " - [OK]" << std::endl;

    // ***
}

int main() {
    check_project_v1();
}
