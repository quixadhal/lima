/* Do not remove the headers from this file! see /USAGE for more info. */

#include <daemons.h>

string main(string user) {
    return replace_string(read_file("/FOR_ALPHA"), "\n", "<BR>\n");
}
