/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_READABLE;

void setup() {
    set_long("A small leaflet.");
    set_size(VERY_SMALL);
    set_adj("small");
    set_id("leaflet", "booklet", "mail");
    set_text("WELCOME TO ZORK!\n\nWell, ok, maybe not.  But it starts sorta similar.  Your mission, should you choose to accept it, is cause the best LP mudlib ever created to come into existence.  Luckily, since this is a game, it will take you less than the several years it took us.  Good luck!\n\nP.S. There appear to be a few gremlins in the area intent on foiling your efforts.  Be careful.");
}
