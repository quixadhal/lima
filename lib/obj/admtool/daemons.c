/* Do not remove the headers from this file! see /USAGE for more info. */

inherit __DIR__ "internal/directory.c";

string module_name() {
    return "daemons";
}

string module_key() {
    return "d";
}
