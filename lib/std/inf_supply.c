/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;

private mapping obs;

void set_objects(mapping m) {
    // Normalize the filenames
    array bad_keys = filter(keys(m), (: $1[<2..] == ".c" :));

    foreach (string s in bad_keys) {
        m[s[0..<3]] = m[s];
        map_delete(m, s);
    }

    obs = m;
    /* Now and at reset, make one of them */
    ::set_objects(map_mapping(m, (: 1 :)));
}

mixed release_object(object target, int force) {
    mixed ret = ::release_object(target, force);

    if (ret == 1 && obs[base_name(target)] == -1 || obs[base_name(target)]-- > 0) 
	new(base_name(target))->move(this_object());

    return ret;
}

