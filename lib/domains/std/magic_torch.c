/* Do not remove the headers from this file! see /USAGE for more info. */

inherit TORCH;
inherit M_VALUABLE;

void setup() {
    set_adj("magic");
    set_id("torch");
    set_long( (: "The slender torch is made of wood that seems to sing with vitality.\n" + (query_is_lit() ? "It burns with a strange blue flame.\n" : "") :) );
    set_light_msgs("As $n $vlight the $o, it starts burning with a blue flame.\n", "As $n $vlight the $o, it starts burning with a blue flame.\n");
    set_source( (: $1->can_light_the_magic_torch() ? (mixed)1 : "It doesn't seem to be the right type of flame.\n" :) );
    set_fuel(-1); // never runs out
set_value(15);
}
