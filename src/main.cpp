#include "libs/app/init.h"
#include "libs/ui/init-ui.h"
#include "libs/utils/shell-scripts.h"

int main() {
	// Test main function to see the flow of program
	std::string vault_dir;

	// Loads the password-store path
	vault_dir = load_vault_path();

	size_t count = get_number_files(vault_dir) + get_number_directories(vault_dir);

	// Gets a string array, with all entries
	std::vector<std::string> entries = get_all_entries(vault_dir, count, false);
	std::vector<std::string> entries_api = get_all_entries(vault_dir, count, true);

	struct vault vault = {.count_entries = count - get_number_directories(vault_dir), .count_directories = get_number_directories(vault_dir), .vault_dir = vault_dir, .api_entry = entries_api, .entry = entries};
	init_ui(&vault);

	return 0;
}
