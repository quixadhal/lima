/* Do not remove the headers from this file! see /USAGE for more info. */

inherit TORCH;
inherit M_VALUABLE;

void setup() {
    set_adj("magic");
    set_id("torch");
    set_long( "It looks like an ordinary torch, but you guess that it's probably magic because it's humming.");
    set_light_msgs("As $n $vlight the $o, it starts burning with a blue flame.", "As $n $vlight the $o, it starts burning with a blue flame.");
    set_source( (: $1->can_light_the_magic_torch() ? (mixed)1 : "It doesn't seem to be the right type of flame.\n" :) );
    set_value(15000);
    set_untouched_desc( "A torch lies in the dust.");
}

int sound()
{
    write( "It's humming with a low regular buzz.\n" );
    return 1;
}

mapping lpscript_attributes()
{
    return m_valuable::lpscript_attributes() + torch::lpscript_attributes();

}

// Give us unlimited fuel. Also, note that we don't need to call
// set_fuel() from within setup().
mixed check_fuel()
{
   return 1;
}

