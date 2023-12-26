#include "../ui/display.h"
#include "../utils/shell-scripts.h"
// #include "../ui/design.h"

void get_pass_information(linked_list *information, char *argument);

bool copy_to_clipboard(char *argument);

bool delete_entry(char *argument);

bool edit_password(char *argument);

bool rename_password(char *argument1, char *argument2);
