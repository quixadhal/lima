/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;

create() {
    ::create();
    add_id("sword");
    set_combat_messages("combat-sword");
    set_wield_type("blade");
}
