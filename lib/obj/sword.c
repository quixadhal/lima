/* Do not remove the headers from this file! see /USAGE for more info. */

#include "mudlib.h"

inherit WEAPON;

create() {
  ::create();
    add_msg("miss", "$N $vslash at $t but $vmiss by a mile.\n");
    add_msg("miss", "$N just $vmiss $t with a quick thrust.\n");
    add_msg("light", "$N $vprick $t with a quick stab of $p $o.\n");
    add_msg("light", "Blood flows from $p1 hand as $n $vsneak past $p1 guard.\n");
    add_msg("serious", "$N $vconnect!  $T $v1are hurt badly.\n");
    add_msg("serious", "$N $vthrust quickly and $vwithdraw, leaving $t wounded.\n");
    add_msg("fatal", "$N neatly $vinsert $p $o between $p1 ribs.\n");
    add_msg("fatal", "$N $vremove $p1 head from $p1 shoulders.\n");
    add_msg("disarm", "$N $vsend $p1 $o1 flying with a quick move $ns learned in Swordfighting 201.\n");
    add_msg("disarm", "$N $vslap $p1 hand with $p $o, causing $To to drop $p1 $o1.\n");
    add_msg("knockdown", "$N $vdrive $t back with a flurry of blows.\n");
    add_msg("knockout", "$N $vfeint past $p1 guard and $vknock $t senseless.\n");
    add_msg("dispatch", "$N $vhave no sense of honor; $n lops $p1 head clean off.\n");
    add_id("sword");
    add_adj("dull");
   set_gettable(1);
}

