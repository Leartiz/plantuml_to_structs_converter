#include <iostream>
#include <filesystem>

using namespace std;

int main() {
	auto path = filesystem::current_path();
	cout << path << endl;

	path = path.parent_path().parent_path();
	path = path / "converter" / "test" / "projects" / "hierarchy";
	if (filesystem::exists(path)) {
		cout << "project root path - [OK]" << endl;
	}
	cout << endl;

	// ***

	const string uc_dia_src = "use_case_dia.wsd";
	const string class_dia_src = "class_dia.wsd";
	const string layout_flow_src = "layout_flow.wsd";

	cout << uc_dia_src;
	if (!std::filesystem::exists(path / uc_dia_src))
		std::cout << " - [BAD]" << std::endl;
	else std::cout << " - [OK]" << std::endl; 

	cout << class_dia_src;
	if (!std::filesystem::exists(path / class_dia_src))
		std::cout << " - [BAD]" << std::endl;
	else std::cout << " - [OK]" << std::endl;

	cout << layout_flow_src;
	if (!std::filesystem::exists(path / layout_flow_src))
		std::cout << " - [BAD]" << std::endl;
	else std::cout << " - [OK]" << std::endl;

	// ***

	const string sequence_dias_dir = "sequence_dias";
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

	const string layouts_dir = "layouts";
	const string formats_dir = "formats";
	const string descs_dir = "descs";

	cout << layouts_dir;
	if (!std::filesystem::exists(path / layouts_dir))
		std::cout << " - [BAD]" << std::endl;
	else std::cout << " - [OK]" << std::endl;

	cout << formats_dir;
	if (!std::filesystem::exists(path / formats_dir))
		std::cout << " - [BAD]" << std::endl;
	else std::cout << " - [OK]" << std::endl;

	cout << descs_dir;
	if (!std::filesystem::exists(path / descs_dir))
		std::cout << " - [BAD]" << std::endl;
	else std::cout << " - [OK]" << std::endl;

	// ***
}
