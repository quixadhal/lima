/* Do not remove the headers from this file! see /USAGE for more info. */

inherit FLAME;

void setup() {
    set_adj("blue");
    set_long("It must be magic as there's no fuel feeding it.");
    set_attached( 1 );
}

int can_light_the_magic_torch() {
    return 1;
}
