/* Do not remove the headers from this file! see /USAGE for more info. */

#include <bodyslots.h>
inherit CLASS_BODYSLOTS;


// Set some defaults. If you want other ones, override this.

static mapping slots =
 ([  TORSO : new(class slot, howmany:1, name:TORSO, max_size:LARGE, stuff:({ })),
     HEAD : new(class slot, howmany:1,name:HEAD,max_size:SMALL,stuff:({ })),
     ARMS : new(class slot,howmany:1,name:ARMS,max_size:MEDIUM,stuff:({ })),
     LEGS : new(class slot, howmany:1,name:LEGS,max_size:MEDIUM, stuff:({ })),
     HANDS: new(class slot,howmany:1,name:HANDS,max_size:SMALL,stuff:({ })),
     FEET: new(class slot, howmany:1,name:FEET,max_size:SMALL,stuff:({ })),
 ]);
                                                                                                                                                

 
//For those of us who just want to roll our own.
static nomask void set_body_slots(mapping what)
{
    slots = what;
}

static nomask int add_body_slot(string sname, int num, int maxsize)
{
    class slot tmp;

    if(slots[sname])
        return 0;
    
    tmp = new(class slot);
    tmp->howmany = num;
    tmp->name = sname;
    tmp->stuff = ({ });
    tmp->max_size = maxsize;
    slots[sname] = tmp;
    return 1;
}

    

nomask int populate_body_slot(string sname, object what)
{
    object *athere;
    int num;
    int slot_max_size;
    if(!slots[sname])
        return 0;
    athere = ((class slot)slots[sname])->stuff;
    num = ((class slot)slots[sname])->howmany;
    if(sizeof(athere) >= num)
        return 0;
    
    slot_max_size = ((class slot)slots[sname])->max_size;
    if((what->get_size() > slot_max_size) && slot_max_size)
        return 0;
        
    ((class slot)slots[sname])->stuff += ({ what });
    return 1;
}

nomask int depopulate_body_slot(string sname, object what)
{
    object *athere;
    int num;
    
    if(!slots[sname])
        return 0;
    athere = ((class slot)slots[sname])->stuff;
    
    if(member_array(what, athere) == -1)
        return 0;
    
    ((class slot)slots[sname])->stuff -= ({ what });
    return 1;
}


nomask mixed get_random_clothing()
{
    class slot tmp;
    object what;
    if(!sizeof(slots))
        return 0;
    
    tmp = choice(values(slots));
    if(!tmp)
        return 0;
        
    if(!sizeof(tmp->stuff))
        return 0;
    what = choice(tmp->stuff);
    return what;
}

    
        
static nomask int reset_body_slots()
{

    
    slots = ([ ]);
    return 1;
}
    
    
nomask int has_body_slot(string what)
{

    return (slots[what] ? 1 : 0);
}

            
