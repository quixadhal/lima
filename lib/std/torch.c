/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;
inherit M_LIGHTABLE;

void mudlib_setup()
{
    ::mudlib_setup();
    set_id("torch");
    set_fuel(600);
    set_combat_messages("combat-torch");
    set_damage_kind("fire");
    set_weapon_class(5);
}

mapping lpscript_attributes()
{
    return weapon::lpscript_attributes() + m_lightable::lpscript_attributes();
}
