/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void setup(){
    set_adj("lazy");
    set_id("river","water");
    set_long( "The river is moving rather slowly from east to west. It isn't very deep; you won't have any problems fording it if you want to leave northwest.");
    set_attached();
}


mixed swim( string verb ){
    object wall = present( "wall", environment( this_object()));
    string str = "$N $v" +verb+ " around the water for a while.";

    if( wall->query_closed()){
	str += " The cave floor quivers a bit as $n $vclamber out.";
	wall->trigger();
    }
    this_body()->simple_action( str );
    return 1;
}
