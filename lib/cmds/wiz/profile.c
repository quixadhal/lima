/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

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

    outf("%-40s %5s %8s %8s %8s\n", "Name", "Calls", "Self", "Children", "Total");
    foreach (item in worst) {
        outf("%-40s %5i %8i %8i %8i\n",
               item["name"], item["calls"], item["self"], item["children"],
               item["self"] + item["children"]);
    }
}

int total(mapping *info) {
    int ret;
    mapping item;

    foreach (item in info)
        ret += item["self"];
    return ret;
}

protected void top_ten() {
    mixed *tmp;
    int i, j;

    tmp = map(objects((: !clonep($1) :)),
              (: ({ $1, total(function_profile($1)) }) :));
    // We could use sort_array() here, but that's an inefficient way
    // to get the top 10 when there are lots of objects.

    for (i = 0; i < 10; i++) {
        mixed foo;

        for (j = i; j < sizeof(tmp); j++)
            if (tmp[i][1] < tmp[j][1]) {
                foo = tmp[i];
                tmp[i] = tmp[j];
                tmp[j] = foo;
            }
        outf("%60-O %i\n", tmp[i][0], tmp[i][1]);
    }
}
#endif

private void main(string str) {
#ifndef __PROFILE_FUNCTIONS__
    out("Function profiling not enabled.\n");
    return;
#else
    mapping *prof;
    mapping item;
    string obname;
    object ob;
    function compare;

    if (!str) {
	top_ten();
        return;
    }

    if (sscanf(str, "-total %s", str)) {
        compare = (: $2["self"] + $2["children"] - $1["self"] - $1["children"] :
);
    } else {
        compare = (: $2["self"] - $1["self"] :);
    }

    if (str == "all") {
        profile_all(compare);
        return;
    }

    obname = evaluate_path(str);

    if (!(ob = find_object(obname))) {
        out("Can't find object " + obname + "\n");
        return;
    }

    prof = function_profile(ob);
    prof = filter(prof, (: $1["calls"] :));
    prof = sort_array(prof, compare);

    outf("%-30s %5s %8s %8s %8s\n", "Name", "Calls", "Self", "Children", "Total");
    foreach (item in prof) {
        outf("%-30s %5i %8i %8i %8i\n",
               item["name"], item["calls"], item["self"], item["children"],
               item["self"] + item["children"]);
    }
#endif
    return;
}
    
