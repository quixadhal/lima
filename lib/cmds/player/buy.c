/* Do not remove the headers from this file! see /USAGE for more info. */

/*  
 *  Buy command 
 *  
 *  created 15-Feb-97 by Neppit@isle of avalon
 *  Darklord fixed a lot 3/4/97
 */

inherit CMD;

private void main(string str)
{
   int listnum = 1, number = 1;
   string arg;
   string *args;
   if(!this_body()) return;
   if(str)
     args = explode(str, " ");

   switch(sizeof(args)) 
     {
     case 3:
       if(args[2][0] == '#')
	 sscanf(args[2], "#%d", listnum);
       arg = args[1];
       sscanf(args[0], "%d", number);
       break;
     case 2:
       if(args[1][0] == '#'){      
	 sscanf(args[1], "#%d", listnum);
	 if(args[0][0] > '9')
	   arg = args[0];
	 else {
	   sscanf(args[0], "%d", number);
	   arg = "all";
	 }
       }
       else {
	 arg = args[1];
	 sscanf(args[0], "%d", number);
       }
       break;
     case 1:
       if(args[0][0] == '#') {
	 arg = "all";
	 sscanf(args[0], "#%d", listnum);
       } 
       else
	 arg = args[0];
     }
   foreach(object ob in all_inventory(environment(this_body()))) {
     if(ob->query_vendor()) {
       if(!arg) {
         write("Buy what?\n");
         write("        " + repeat_string("-=", 27) + "\n");
         ob->query_items("all", 0); 
         return;
       }
       ob->sell_stored_objects(arg, listnum, number);
       return;
     }
   } 
   out("Sorry, there is not a vendor in this room.\n");
}
