/* Do not remove the headers from this file! see /USAGE for more info. */

// This should realy be implimented and inherit from a heapping object
// instead.  Remind me when you need it.   ---John

#include <mudlib.h>

inherit OBJ;
inherit M_GRAMMAR;

string type = "zorkmid";
int ammount = 1;

int query_ammount(){ return ammount; }

string query_money_type(){ return type; }

int stat_me() {
   write("Coin type: "+type+"   Ammount: "+ammount+"\n");
   return ::stat_me();
}

int move(string where){
   object e, *b;
   int i;

   ::move(where);
   e = environment(this_object());
   if(!e) return 0;
   b = all_inventory(e) - ({this_object()});
   i = sizeof(b);
   while(i--)
      if(b[i]->query_money_type() == type ){
	 ammount += b[i]->query_ammount();
	 b[i]->remove();
	 break;
      }
}

/*
id( arg ){
   return ::id(arg) || ::plural_id(arg);
}
*/

string short(){
   switch( ammount ){
   case 1 :
      return "a "+type;
   case 2..5 :
      return "a small pile of "+pluralize(type);
   default:
      return "A large pile of "+pluralize(type);
   }
}

string long(){
   if(ammount == 1) return "It looks just like a "+type+".\n";
   return "They look just like "+pluralize(type)+".\n";
}

int get(){  return 1; }

void create(){
   if(member_array(type,ids) == -1) ids+=({type});
   if(member_array("coin",ids)==-1)ids+=({"coin"});
   ::create();
}

int payup( int i )
{
  object o;
  if( i > ammount ) 
	  return 0;

  if( i == ammount )
	return this_object();

  ammount -= i;
  o = clone_object("/std/coin");
  o->set_ammount(i);
  return o;
}
