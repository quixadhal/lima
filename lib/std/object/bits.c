/* Do not remove the headers from this file! see /USAGE for more info. */

#include <daemons.h>
#include <setbit.h>
#include <obbits.h>


#define SHIFT(x) (1 << (x-1))

int obits;

void set_ob_flag(int bit){
    if(bit<1||bit>32) return;
    if ( (interactive(this_object())) &&
      (this_object() != this_body()) &&
	 !check_privilege(1) )
	return;

    obits |= SHIFT(bit);
}

void clear_ob_flag(int bit){
    if(bit<1||bit>32) return;
    if ( (interactive(this_object())) &&
      (this_object() != this_body()) &&
	 !check_privilege(1) )
	return;

    obits &= ~(SHIFT(bit));
}

int query_ob_flag(int bit){
    if(bit<1||bit>32) return 0;
    return obits & SHIFT(bit);
}
