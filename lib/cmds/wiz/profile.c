/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;

#ifdef __PROFILE_FUNCTIONS__
void profile_all(function compare) {
    mapping *prof;
    mapping *worst = ({});
    object ob;
    mapping item;

    foreach (ob in objects()) {
        prof = function_profile(ob);
        prof = filter(prof, (: $1["calls"] :));
        prof = sort_array(prof, compare);
        prof = prof[0..9];
        map(prof, (: $1["name"] = $(file_name(ob)) + "::" + $1["name"] :) );
        worst += prof;
    }
    worst = sort_array(worst, compare);
    worst = worst[0..19];

    printf("%-40s %5s %8s %8s %8s\n", "Name", "Calls", "Self", "Children", "Tota
l");
    foreach (item in worst) {
        printf("%-40s %5i %8i %8i %8i\n",
               item["name"], item["calls"], item["self"], item["children"],
               item["self"] + item["children"]);
    }
}
#endif

int main(string str) {
    mapping *prof;
    mapping item;
    string obname;
    object ob;
    function compare;

#ifndef __PROFILE_FUNCTIONS__
    write("Function profiling not enabled.\n");
    return 1;
#else
    if (!str) {
        write("profile [-total] <ob>\n");
        return 1;
    }

    if (sscanf(str, "-total %s", str)) {
        compare = (: $2["self"] + $2["children"] - $1["self"] - $1["children"] :
);
    } else {
        compare = (: $2["self"] - $1["self"] :);
    }

    if (str == "all") {
        profile_all(compare);
        return 1;
    }

    obname = evaluate_path(str);

    if (!(ob = find_object(obname))) {
        write("Can't find object " + obname + "\n");
        return 1;
    }

    prof = function_profile(ob);
    prof = filter(prof, (: $1["calls"] :));
    prof = sort_array(prof, compare);

    printf("%-30s %5s %8s %8s %8s\n", "Name", "Calls", "Self", "Children", "Tota
l");
    foreach (item in prof) {
        printf("%-30s %5i %8i %8i %8i\n",
               item["name"], item["calls"], item["self"], item["children"],
               item["self"] + item["children"]);
    }
#endif
    return 1;
}
    
