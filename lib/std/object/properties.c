/* Do not remove the headers from this file! see /USAGE for more info. */

/* 12-18-98 Tigran made the variables sets and psets initialize when declared
 *          and removed create().
 *          Added query_perm_sets() for consistancy
 */

/* This is our property implementation.  Use as little as possible */
private nosave mapping sets =([]);
private mapping psets =([]);


nomask void
set(mixed key, mixed value) {
    sets[key] = value;
}

nomask void
add(mixed key, mixed value)
{
    if(typeof(value) == typeof(sets[key]))
	if(!functionp(value))
	    sets[key]+=value;
}

nomask mixed
query(mixed key){
    return sets[key];
}

nomask mapping
query_sets() {
    return sets;
}

void
delete(mixed key){
   map_delete( sets, key );
}


nomask void set_perm(mixed key, mixed value){
    psets[key] = value;
}

nomask mixed query_perm(mixed key){
  return psets[key];
}

nomask void delete_perm(mixed key){
    map_delete(psets,key);
}

nomask void add_perm(mixed key, mixed value)
{
    if(typeof(value) == typeof(psets[key]))
	if(!functionp(value))
	    psets[key]+=value;
}

nomask mapping query_perm_sets()
{
  return psets;
}
