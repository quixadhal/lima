/* Do not remove the headers from this file! see /USAGE for more info. */

/* object.c  about 1/2 Rust and 1/2 Nevyn  */
// Modifications:  Peregrin@ZorkMUD (GUE),  Mar '94
// Beek - rewrote the DISCARDED_MESSAGE stuff b/c it was hideously
//      - inefficient.
// Beek --- Added the OFFICIAL "what these vars should contain"
// documentation
// Sep 9 1994
// Rust moved everything into modules.

#include <flags.h>

inherit BASE_OBJ;

#ifdef USE_SIZE
inherit __DIR__ "object/size";
#else
#ifdef USE_MASS
inherit __DIR__ "object/mass";
#endif
#endif  //USE_SIZE

inherit __DIR__ "object/light";		/* before non_object */
inherit __DIR__ "object/properties";
inherit __DIR__ "object/move";
inherit __DIR__ "object/visible";
inherit __DIR__ "object/vsupport";
inherit __DIR__ "object/hooks";
inherit __DIR__ "object/msg_recipient";

//:FUNCTION stat_me
//write() some debugging info about the state of the object
int stat_me() 
{
    ::stat_me();
    write("Short: "+short()+"\n");
#ifdef USE_SIZE
    write("Size: "+get_size()+" Light: " + query_light() + "\n");
#else
# ifdef USE_MASS
    write("Weight: "+query_mass()+"  Light: " + query_light() + "\n");
# else
    write("Light: "+query_light() + "\n");
# endif
#endif
    return 1;
}

create(){
    ::create();
    properties::create();
    configure_set(STD_FLAGS, 0, 0, (: resync_visibility :), 0, 0);
//    restore_object("/data/"+base_name(this_object()));
}


/* arbitrate some stuff that was stubbed in BASE_OBJ */
varargs mixed call_hooks(array args...)
{
    return hooks::call_hooks(args...);
}
int is_visible()
{
    return visible::is_visible();
}
void set_light(int x)
{
    light::set_light(x);
}
