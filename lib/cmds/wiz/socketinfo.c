/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;

int main(string str) {
    string *lines = explode(dump_socket_status(), "\n");
    string line;
    
    foreach (line in lines) {
        if (line[0..1] != "-1")
            write(line + "\n");
    }
    return 1;
}
