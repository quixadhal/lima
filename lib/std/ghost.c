/* Do not remove the headers from this file! see /USAGE for more info. */

//  Ghost.c  by Rust 2-5-93
//  This body thing is a hack.
//  Might be worth redoing, just so it follows some sort
// of logic.

#include <mudlib.h>
#include <daemons.h>

inherit OBJ;



string name;
object link;

int stat_me() {
  write("Ghost of "+capitalize(name)+"\n");
  return ::stat_me();
}

int modify_command(string arg)
{
   mixed exits;
   string verb;
   sscanf(arg,"%s %s",verb, arg);
   if(!verb){
      verb = arg;
      arg = 0;
   }
   switch(verb){
   case "go":
      "/bin/player/_go"->main(arg);
      return 1;
   case "wizcall":
      "/bin/player/_wizcall"->main(arg);
      return 1;
   case "look":case "l":
      "/bin/player/_look"->main(arg);
      return 1;
   case "pray":
      write("Aw... I'm a pushover.\n");
      say(sprintf("Trumpets sound in the distance as %s returns from the dead.\n",capitalize(name)));
      link->switch_body(PLAYER);
      return 1;
   case "n":
      verb = "north";
      break;
   case "s":
      verb = "south";
      break;
   case "e":
      verb = "east";
      break;
   case "w":
      verb = "west";
      break;
   case "ne":
      verb = "northeast";
      break;
   case "nw":
      verb = "northwest";
      break;
   case "se":
      verb = "southeast";
      break;
   case "sw":
      verb = "southwest";
      break;
   case "u":
      verb = "up";
      break;
   case "d":
      verb = "down";
      break;
   case "su":
	if(!arg || arg == name) link->switch_body();
	else
      "/bin/player/su"->main(arg);
      return 1;
   case "quit":
      say(name+" left the game.\n");
      ANNOUNCE_D->announce_me(2);
      write("Now leaving the Great Underground Empire...\n");
      link->remove();
      destruct(this_object());
	return 1;
   }
   if(environment(this_object())){
      exits = keys(environment(this_body())->get_exits());
      if(member_array(verb,exits) != -1){
	 environment(this_body())->go_somewhere(verb);
	 return 1;
      }
   }
   if(!catch(load_object(sprintf("/bin/player/_%s",verb)))){
      write("Not bloody likely!\n");
      return 1;
   }
   write(wrap("You can't do much at all as a ghost, so I'm not going to TRY to understand you.\n"));
   return 1;
}



init_cmd_hook(){
   set_ids(({"ghost","ghost of "+name, name}));
   set_modify_command(this_object());
}

init_user(n){
	if(file_name(previous_object())[0..10] != "/std/login#")
		return;
      link = previous_object();
   name = n;
  set_living_name(n);
   return 1;
}


query_link(){
   return link;
}

query_name(){
   return short();
}

query_real_name(){
   return name;
}

short(){
   return "a ghost of "+capitalize(name);
}

long(){
}


enter_game(){
   link = previous_object();
   init_cmd_hook();
   if(move(START)){
      write("Uh-oh, you have no environment.\n");
      return;
   }
   write("\n\nNow entering the Great Underground Empire...\n");
   say(query_name()+" enters the Empire.\n");
   ANNOUNCE_D->announce_me(1);
   command("look", this_object());
   return;
}

remove(){
	destruct(this_object());
}

net_dead(){
  tell_room(environment(this_object()), query_name()+" has gone link-dead.\n");
  ANNOUNCE_D->announce_me(4);
}
reconnect(){
   write("reconnected.\n");
   say(query_name()+" has reconnected.\n");
   ANNOUNCE_D->announce_me(3);
}

query_who_data(){
return({0,short(),short(), 0});
}
