#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Emote Tutorial 3");
  set_long(@MILAMBER
complete tag list:

$n  = A name: defaults to the name of the doer.
$t  = Name of the target
$v  = A verb: signals that the next word is a verb.
$p# = Returns the possesive of a name.  The number indicates which name
      to return the possesive of.  $p0 is the doer, $p1 is the first
      target, $p2 is the second target, and so on.
$r# = Returns the reflexive.  (ie: yourself, himself, herself) It works
      like $p#.
$o  = This represents the object used in the emote (other than the target).
      eg: "smile broadly" would call rule "STR" with 'broadly' as $o
      and the message is "$N $vsmile $o."
MILAMBER
  );
  set_exits( ([
    "north" : R + "emote04",
    "south" : R + "emote02",
   ]) );
  set_objects( ([
   ]) );
}
