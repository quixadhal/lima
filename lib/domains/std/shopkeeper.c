/* Do not remove the headers from this file! see /USAGE for more info. */

inherit LIVING;
inherit M_VENDOR;

#define OBJS "/domains/std/objects/"

void setup() {
    set_name("Biff");
    add_id("shopkeeper");
    set_gender(1);
    set_proper_name("Biff the Shopkeeper");
    set_in_room_desc("Biff the Shopkeeper");
    set_long("Biff looks like a shrewd haggler.\n");

/* Determines what type of currency we'll use */
    set_currency_type("gold");

/* Tells vendor we will sell things */
    set_for_sale(1);

/* Tells vendor we will buy things */
    set_will_buy(1);

/* itemization of what we sell initially
   filename : #
   if # is set to -1, it will sell an infinite # of that item
*/
    set_sell(([
     __DIR__ "apple" : -1,
     OBJS "sword" : 3,
     ]));

/* We would set this so that the vendor stores weaps/armor in a storage
   room.  See help on set_unique_inventory.  storage.c in the rooms subdir has
   been set up to work.
    set_unique_inventory(__DIR__ "rooms/storage");
*/

/* We would set this if we wanted EVERYTHING to be stored in a storage
   room.
    set_all_unique(1);
*/
}
