/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BASE_ROOM;

void setup()
{
    set_area("bobs_storage");
    set_brief("Bob's Storage Room");
    set_long (
    "This is Bob's boring storage room.  DONT TOUCH!"
             );
}

/* Keep this function in here, it prevents the driver from
   removing this object from the game.  If this were to happen
   major problems could occur
*/
int clean_up() { return 0; }
