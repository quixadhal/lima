/* Do not remove the headers from this file! see /USAGE for more info. */

/* This is our property implementation.  Use as little as possible */
private static mapping sets ;
private mapping psets ;


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

nomask mixed
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
    if(!mapp(psets))
	psets = ([]);
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

void
create()
{
  sets = ([]);
  psets = ([]);
}
