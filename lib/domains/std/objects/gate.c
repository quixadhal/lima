/* Do not remove the headers from this file! see /USAGE for more info. */

inherit DOOR;


void do_on_open()
{
   object env = environment( this_body() );

load_object("/domains/std/rooms/caves/North_Tunnel.c")->set_room_state("gate_open");
   env->set_room_state("gate_open");
}

void do_on_close()
{
   object env = environment( this_body() );

load_object("/domains/std/rooms/caves/North_Tunnel.c")->clear_room_state("gate_open");
   env->clear_room_state( "gate_open");
}

void setup(string dir, string dest) {
    set_id("gate");
    set_adj("rusty" , "rusty gate" );
  set_long("The gate is very rusty and doesnt look like it could stop anything anymore");
    setup_door("rusty gate", dir, dest);
    set_flag(ATTACHED);
}
