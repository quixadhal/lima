/* Do not remove the headers from this file! see /USAGE for more info. */

private static object victim;

int start_shadowing(object ob) {
    if (!shadow(ob)) return 0;
    victim = ob;
}

int stop_shadowing() {
    if (!remove_shadow()) return 0;
    victim = 0;
}

void query_shadows() {
    string *shadows;
    if (!victim) return ({});
    if (!(shadows = victim->query_shadows())) shadows = ({});
    return shadows + ({ file_name(this_object) });
}
