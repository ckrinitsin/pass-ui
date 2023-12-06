#include "../utils/shell-scripts.h"
#include "../ui/design.h"
#include <vector>
#include <string.h>

void get_pass_information(std::vector<std::string> *information, std::string argument); 

bool copy_to_clipboard(std::string argument);

bool delete_entry(std::string argument);
