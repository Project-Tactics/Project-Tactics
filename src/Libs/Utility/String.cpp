#include "String.h"

namespace tactics {

char tolower(char c) { return static_cast<char>(::tolower(static_cast<int>(c))); }

char toupper(char c) { return static_cast<char>(::toupper(static_cast<int>(c))); }

} // namespace tactics
