/* Do not remove the headers from this file! see /USAGE for more info. */

/* Do not remove the headers from this file! see /USAGE for more info. 
 * Vendor module
 *
 * Zifnab Wrote it at some point the first time but didn't put his name in here :)
 *
 * Darklord went nuts and redid it 15-Feb-97
 *
 * Bunch of changes and additions 25-Apr-97 by Vette
 *
 */

//:MODULE
//m_vendor is used to create vendor objects that buy and sell stuff
//they work as traditional shopkeepers and also bartenders
//See: /domains/std/shopkeeper.c

/* TODO
more compatibility with buying and selling only certain types of objects
compatibility of buying and selling vehicles
verb to support viewing an object before buying it
*/

#include <mudlib.h>
#include <move.h>

// Value of what the objects value is multiplied by to achieve its cost
#define COST_MULT 3/2 

string short();
string query_subjective();
int check_uniqueness(object ob);

private mixed for_sale;
private mixed will_buy;
private mixed currency_type = "gold";
private mapping stored_items = ([]);
private string unique_inv = "";
private int all_unique = 0;

//:FUNCTION add_sell
//enables you to add items to the vendors stored_items mapping

void add_sell(string item, int amt)
{
 object added;
 string state, name, *ids;
 int cost;


 if(!amt) amt = 1;
 if(item)
 {
  added = new (item);
  if(added)
  {
   state = base_name(added);
   name = added->short();
   ids = added->query_id();
   ids += ({ added->plural_short() });
   cost = added->query_value();
   stored_items += ([ state : ([
			       0 : name,
                               1 : state,
			       2 : cost,
			       3 : ids,
			       4 : item,
			       5 : amt
			       ])]);
  }
  else
    error("No such item: " + item + "\n");
 }
}


//:FUNCTION set_sell
// with a mapping you can set many items into the vendors to sell list
void set_sell(mapping items)
{
 object added;
 string state, name, *ids, *keys;
 int cost, size, i;
 size = sizeof(items);
 keys = keys(items);


 for(i =0; i < size; i++)
 {
  added = new(keys[i]);
  if(added)
  {
    state = base_name(added);
    name = added->short();
    ids = added->query_id();
    ids += ({ added->plural_short() });
    cost = added->query_value();
    stored_items += ([ state : ([
				0 : name,
				1 : state,
				2 : cost,
				3 : ids,
				4 : keys[i],
				5 : items[keys[i]],
			       ])]);  
  }
  else
   error("No such item: " + keys[i] + "\n");
 }
}

//:FUNCTION set_for_sale
//Set the array of object names which this living object is willing to sell.
//set_for_sale(1) means everything is for sale.  set_for_sale(0) means nothing
//is.  A function pointer is legal here.
void set_for_sale(mixed x) {
    for_sale = x;
}

//:FUNCTION set_will_buy
//Set the array of object names which this living object is willing to buy.
//set_will_buy(1) means it will buy anything.  set_will_buy(0) means it wont
//by anything.  A function pointer is legal here.
void set_will_buy(mixed x) {
    will_buy = x;
}

//:FUNCTION set_currency_type
//Sets the type of currency the vendor will buy/sell in
mixed set_currency_type(string type)
{
    if(!member_array(type, MONEY_D->query_currency_types() ))
      catch("Invalid currency set in shopkeeper!\n");
    currency_type = type;
}

//:FUNCTION query_currency_type
//Queries the type of currency the vendor will buy/sell in
mixed query_currency_type()
{
    return currency_type;
}

mixed indirect_buy_obj_from_liv(object ob, object liv) {
    mixed tmp = evaluate(for_sale);

    if (intp(tmp)) return tmp;
    if (!pointerp(tmp)) return 0;

    foreach (string name in tmp) {
	if (ob->id(name)) return 1;
    }
    return 0;
}

mixed direct_ask_liv_about_wrd(object liv, string item)
{
    return 1;
}

mixed indirect_sell_obj_to_liv(object ob, object liv) {
    mixed tmp = evaluate(will_buy);

    if (intp(tmp)) return tmp;
    if (!pointerp(tmp)) return 0;

    foreach (string name in tmp) {
	if (ob->id(name)) return 1;
    }
    return 0;
}

void buy_object(object ob) {

  int object_cost, i, ok = 1, uni;
  string item_state;
  string item_name, file, *key;
  string *object_ids;
  int exchange_rate = MONEY_D->query_exchange_rate(currency_type);
  int object_value;

  if (ob->move(this_object()) != MOVE_OK)
    write("You can't seem to give " + ob->the_short() + " to " + short() +
	  ".\n");
  else {
    // sscanf(file_name(ob), "%s#%s", file, junk);
    file = base_name(ob);
    key = keys(stored_items);
    for(i=0; i<sizeof(key); i++)
      if(file == key[i])
      {
         ok = 0;
         break;
      }

    uni = check_uniqueness(ob);

    object_value = ob->query_value();
    object_cost = object_value * exchange_rate;
    this_body()->add_money(currency_type, object_cost);
    if (uni == 0)
      item_state = base_name(ob);
    else
      item_state = file_name(ob);
    item_name = ob->short();
    object_ids = ob->query_id();
    object_ids += ({ ob->plural_short() });

    if(ok || (uni == 1)) {
      stored_items += ([ item_state : ([
				     0 : item_name,
				     1 : item_state,
				     2 : object_value,
				     3 : object_ids,
				     4 : file,
				     5 : 1,
                                     6 : ob
                                    ])]);
    }
    else {
      if ((uni == 0) && (stored_items[key[i]][5] != -1))
        stored_items[key[i]][5]++;
    }

    this_body()->my_action("$N $vsell a $o for "+ object_cost +" "+currency_type +".", ob);
    this_body()->other_action("$N $vsell a $o.\n", ob);
   
/* If object is either non-unique(0) or destroyable(2), but not unique(1) */ 
    if (uni != 1) 
      ob->remove();
    else 
      ob->move(load_object(unique_inv));
  }
}

int sell_object(object * ob) {
    int exchange_rate = MONEY_D->query_exchange_rate(currency_type);
    int object_cost = 0; 
    int i, sold=0;

    /* the vendor sells things for COST_MULT times their value */
    for(i =0; i < sizeof(ob); i++)
      {
	object_cost = ob[i]->query_value() * exchange_rate * COST_MULT ;
	
	if(object_cost > this_body()->query_amt_money(currency_type))
	  {
	    write( short() +
		   " laughs in your face saying, that costs  "+ 
		   object_cost +" "+ currency_type +
		   ", which you don't have!\n");
            if (check_uniqueness(ob[i]) != 1)
	      ob[i]->remove();
	    return 0;
	  }
	
	if (ob[i]->move(this_body()) != MOVE_OK)
	  {
	    write(capitalize(query_subjective()) + 
	      " can't seem to give " + ob[i]->the_short() + " to you.\n");
            if (check_uniqueness(ob[i]) != 1)
	      ob[i]->remove();
	    if(sizeof(ob) == 1)
	      return 0;
	    else
	      return sold;
	  }
	this_body()->subtract_money(currency_type, object_cost);
	this_body()->my_action("$N $vbuy a $o for "+ object_cost +" "
			       + currency_type +".\n",ob[i]);
	this_body()->other_action("$N $vbuy a $o.\n",ob[i]);
	sold++;
      }
    return 1;
}


/* This function completely replaced Zifnab's Apr 7 version
   Im not sure if i really needed to fold any of Zif's stuff into here
   or not.  Vette  April 18, 1997
*/
//:FUNCTION query_items(string item, int flag)
//gets called from the verb ask and the rule ask obj about str
//this function shows the players what items the shopkeeper has
//If flag is not set the it will just print the listing
void query_items(string item, int flag)
{

    int i, m, j, k, cost, num;
    string *stored_items_ids, *keys = ({});
    string *items_available = ({});

// write(sprintf("%O\n",stored_items));

    i = sizeof(stored_items);
    if(!i)
      {
	if(flag)
	  this_body()->my_action("The shopkeeper says, \"I currently have no items in inventory.\"\n");
	else
	  write("This shop has nothing to sell.\n");
      }
    else
      {
        keys = keys(stored_items);
	for(k=0; k<sizeof(keys); k++)
	  {
             
	    if (item == "all")
	      {
		items_available += ({ ({ stored_items[keys[k]][0] }) + 
					({ stored_items[keys[k]][2] }) +  
					  ({ stored_items[keys[k]][5] })   
					  });
	      }
	    else
	      {
		stored_items_ids = ({ stored_items[keys[k]][3] });
		if(member_array(item, stored_items_ids[0]) != -1)
		  {
		    items_available += ({ ({ stored_items[keys[k]][0] }) + 
					    ({ stored_items[keys[k]][2] }) +
					      ({ stored_items[keys[k]][5] })   
						});
		  }
	      }
	  }
	j = sizeof(items_available);
	if (j)
	  {
	    if(flag)
	      this_body()->my_action("The shopkeeper says, \"I have the following items that match your request.\"\n");
	    else 
            {

              write(sprintf("%|10s %5s %-20s %15s     %|10s\n","List #","","Name/id",capitalize(currency_type),"# of Items"));
            }
//write(sprintf("%O\n",items_available));
	    for( m=0; m < j; m++)
	      {
		cost = items_available[m][1];
//write(sprintf("%d %d\n",m,cost));
		cost = (cost * COST_MULT * MONEY_D->query_exchange_rate(currency_type));
                num = items_available[m][2];
		if (num != -1) {
                  printf("%|10d %5s %-20s %15d     %|10d\n", m + 1 ,"",items_available[m][0], cost, num);
                }
		else 
                  printf("%|10d %5s %-20s %15d     %|10s\n", m + 1 ,"",items_available[m][0],cost, "Numerous");
	      }
	  }
	else
	  {
	    if(flag)
	      this_body()->my_action("The shopkeeper says, \"I have no items that match your request.\"\n");
	    else
	      write("Nothing in this shop matches that!\n");
	  }
      }
}


/* once again, a LOT of changes made by Darklord, and not sure if
   Zifnab had since changed anything since the orig that DL used.
*/
//:FUNCTION sell_stored_objects()
//Gets called from the buy verb with the string from obj rule.
//This is the way the players can buy objects from the shopkeeper
//that he has stored away.
void sell_stored_objects(string item, int number, int ammount)
{
    int i, j = 0, k;
    string *for_sale_ids, *keys = ({});
    object *obs=({});


write(ammount + " of " + item + " at # " + number + "\n");

    number -= 1;
    if (!ammount) ammount = 1;

/*
write(sprintf("%O\n%d %s %d\n",stored_items,ammount,item,number));
*/

    if(i = sizeof(stored_items)) 
    {
      if (number >= i) {
        write("That is not available.\n");
        return;
      }
      keys = keys(stored_items);
      if(item == "all")
      {
        for(k =0; k < ammount; k++)
        {
          if( ((stored_items[keys[number]][5] - k) > 0) || 
               (stored_items[keys[number]][5] == -1) )
            if (stored_items[keys[number]][6])
              obs += ({ stored_items[keys[number]][6] });
            else 
              obs += ({ new(stored_items[keys[number]][4]) });
	  else
	    break;
	}
	if(sell_object(obs))
        {
          if ( stored_items[keys[number]][5] != -1 )
          {
	    if(1 == stored_items[keys[number]][5])
	      map_delete(stored_items, keys[number]);
	    else
	    {
	      stored_items[keys[number]][5] -= sizeof(obs);
	      if(stored_items[keys[number]][5] <= 0)
              {
	        stored_items[keys[number]][5] = 0;
                map_delete(stored_items, keys[number]);
              }	
	    }
          }
        }
      }
      else
      {
        while (i--)
        {
          for_sale_ids = ({ stored_items[keys[i]][3] });
          if(member_array(item, for_sale_ids[0]) != -1)
          {
	    j += 1;
	    if(j == number)
	    {
	      for(k =0; k < ammount; k++)
              {
                if( ((stored_items[keys[i]][5] - k) > 0) ||
                     (stored_items[keys[i]][5] == -1) )
                  if (stored_items[keys[i]][6])
                    obs += ({ stored_items[keys[i]][6] });
                  else
                    obs += ({ new(stored_items[keys[i]][4]) });
	        else
	          break;
	      }  
	      if(sell_object(obs))
              {
                if ( stored_items[keys[i]][5] != -1 )
                {
	  	  if(1 == stored_items[keys[i]][5])
		    map_delete(stored_items, keys[i]);
		  else
	          {
	            stored_items[keys[i]][5] -= sizeof(obs);
		    if(stored_items[keys[i]][5] <= 0)
		    {
	              stored_items[keys[i]][5] = 0;
		      map_delete(stored_items, keys[i]);
		    }
		  }
                }
              }		      
            }
	    break;
          }
        }
      }
    }
}

mapping query_stored()
{
  return copy(stored_items);
}

//:FUNCTION set_unique_inventory
//void set_unique_inventory(string str)
//This function determines if the vendor should hold onto what he
//buys instead of desting it and replacing it with an original.  For
//Example without unique set if you sell a sword to the vendor, no
//matter what the condition is, it will be dested and replaced with
//a new one.  Some muds would prefer the old way of what you sell is
//what you buy.  The unique inventory is set by sending the room
//where the inventory is kept.
//ex: set_unique_inventory("/domains/std/rooms/storage");
//NOTE: only armor, weaps, vehicles are uniqued
//Unless the object has a is_unique() { return 1; } function in it
//See set_all_unique to unique everything
void set_unique_inventory(string str)
{
  unique_inv = str;
}

string query_unique_inventory()
{
  return unique_inv;
}

//:FUNCTION set_all_unique
//Sets ALL objects to be uniqued.  Only works when set_unique_inventory()
//is used.
void set_all_unique(int i)
{
  all_unique = i;
}

int query_all_unique()
{
  return all_unique;
}

int query_vendor()
{
  return (for_sale || will_buy);
}

int check_uniqueness(object ob)
{
  /* If the object is supposed to be removed from the game upon selling */
  /* ie. quest objects */
//:FUNCTION destroyable
//place int destroyable() { return 1; } if you do not wish your object
//to be purchaseable from a vendor.  Players may sell it to receive its
//value, but it will then be destroyed.  Mostly used for quest objects
  if (ob->destroyable()) return 2;

  /* If we dont allow uniqueness at all, atuo return 0 */
  if (!unique_inv || unique_inv == "") return 0;

  /* If we want everything to be unique, return 1 */
  if (all_unique) return 1;

  /* If the object has been coded to be unique, return 1 */
//:FUNCTION is_unique
//put int is_unique() { return 1; } in your object if you wish it
//to be kept in a storeroom.  This is most likely because your
//object would have variable changes in it.  For example, weapons
//can have their wc reduced by wear and you'd rather someone not be
//able to purchase a brand new sword after selling a broken one.
  if (ob->is_unique()) return 1;

  if (ob->is_armor()) return 1;
  if (ob->is_weapon()) return 1;
  if (ob->is_vehicle()) return 1;
  return 0;
}

