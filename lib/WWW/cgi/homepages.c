/* Do not remove the headers from this file! see /USAGE for more info. */

#include <http_d.h>

string main() {
    string *wizards;
    string ret;
    ret = "<title>Wizard's Home Pages</title><center><h2>Wizards</h2></center>";
    wizards = get_dir(WIZ_DIR+"/");
    foreach(string wizard in wizards) {
        if(file_size(WIZ_DIR+"/"+wizard+HTTP_USER_HOME) == -2) {
            ret+=sprintf("<li><a href=/~%s> %s </a></li>", wizard, wizard);
        }
    }
    return ret+"</ul>";
}
