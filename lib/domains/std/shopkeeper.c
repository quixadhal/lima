/* Do not remove the headers from this file! see /USAGE for more info. */

inherit LIVING;
inherit M_VENDOR;

void setup() {
    set_name("Biff");
    add_id("shopkeeper");
    set_gender(1);
    set_proper_name("Biff the Shopkeeper");
    set_in_room_desc("Biff the Shopkeeper");
    set_long("Biff looks like a shrewd haggler.\n");
    set_for_sale(1);
    set_will_buy(1);
    set_currency_type("gold");
}
