/* Do not remove the headers from this file! see /USAGE for more info. */

// From safe.c (Zifnab) and magic_box.c (Beek)

inherit INF_SUPPLY;
inherit M_OPENABLE;
inherit M_GETTABLE;
inherit M_LOCKABLE;


void setup()
{
    set_adj("large", "magic");
    set_id( "chest" );
    set_long( "It's a large chest, that glows softly with a magical aura.");
    set_attached();

    set_max_capacity( VERY_LARGE );
    set_objects( ([
	"chainmail" : -1,
	"objects/sword" : -1,
      ]) );

    set_gettable( "#It's way too heavy to lift.\n" );

    set_locked("chest");
    set_strength_vs_magic(0);
    set_strength_vs_picking(100);
}


mapping lpscript_attributes()
{
    return m_gettable::lpscript_attributes() +
    m_openable::lpscript_attributes();
}
