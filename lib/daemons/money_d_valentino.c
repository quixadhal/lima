/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** money_d.c -- money daemon
**
** This daemon manages the legal types of money within the game and their
** exchange rates.
**
** Created Wed Jul  3 20:28:42 MET DST 1996 <V>
**
** Not terribly secure. Minor precautions made. Needs the admtool 
** interface before being 100% secure.
**
** money holds THESE mappings: 
** 
** AREA_WITH_CURRENCY:({ General exchange rate, ([ MATERIAL1: Name for curr.,
**                       MATERIALN: Name for curr ]) }).
*/
inherit M_DAEMON_DATA;

mapping money;

#define CAP capitalize
#define DEBUG
#define DELIM printf(repeat_string("-",73)+"\n")
#define THE_DOLLAR "Tharanian silvercrown"
#define DOLLAR_AREA "Tharanian"
#define DOLLAR_TYPE "silver"

private void create() {
  ::create();
  if (!money)
    money = ([DOLLAR_AREA:
	     ({ 100,
		(["gold":"golden falcon",
		"silver":"silvercrown",
		"steel":"steelbeek",
		"copper":"copperbeek",])})]);
}

// Amount of a material you get for 1000 coins.
private int query_material_value(string mat)
{
  switch(mat)
    {
    case "gold":
      return 100000;
      break;
    case "platinum": // I know its not this way in our world, but this just
      return 10000;  // illustrates that gold is prettier than platinum.
      break;
    case "silver":
      return 1000;
      break;
    case "copper":
      return 100;
      break;
    case "steel":
      return 200;
      break;
    case "lead":
      return 10;
      break;
    case "ceramic":
      return 1;
      break;
    default:
      return 0;
      break;
    }
}

int query_exchange_rate(string type)
{
    return money[type][0];
}

//:FUNCTION query_currency_areas
//Returns the areas with different currencies
string *query_currency_areas()
{
    return keys(money);
}

nomask string
query_currency_string(string area,string mat)
{
 string ret;
 ret = area+" ";
 if (money[area] && mapp(money[area][1]) && 
     money[area][1][mat])
   ret += money[area][1][mat];
 else
   ret += mat;
 return ret;
}

//:FUNCTION query_rate
//Returns the amount of THE_DOLLAR you need to get 'amount' of these.
varargs int query_rate(string coin, int amount)
{
  string area,desc;
  sscanf(coin,"%s %s",area,desc);
  if (!amount) amount=100;
  if (coin==THE_DOLLAR) return amount;
  if (desc && area)
    {
      foreach (string key, string descs in money[area][1])
	{
	  if (desc==descs)
	    return (query_rate(THE_DOLLAR)*amount*money[area][0])/
	    (query_material_value(key)*10);
	}
    }
  return query_material_value(coin)/10;
}

nomask int
adjust_exchange_rate(string area,int amount)
{
  if (base_name(previous_object())!=ADMTOOL)
    return;
  if (!money[area])
    return 0;
  if (area==DOLLAR_AREA)
    error("Money_D: Illegal try to change exchange rate on "
	  +DOLLAR_AREA+" currency.\n");
  money[area][0]+=amount;
  save_me();
  return 1;
}

nomask void
add_currency(string area, string material,string nickname)
{
  if (base_name(previous_object())!=ADMTOOL)
    return;
  if (!money[area])
    money[area]=({100,([])});
  money[area][1][material]=nickname;
  save_me();
}

nomask void
remove_currency(string area,string material)
{
  if (base_name(previous_object())!=ADMTOOL)
    return;
  if (area==DOLLAR_AREA && material==DOLLAR_TYPE)
    error("Money_D: Illegal try to remove "+THE_DOLLAR+".\n");
  if (!money[area])
    return;
  map_delete(money[area][1],material);
  save_me();
}

varargs nomask int
stat_me(string areain,int amount)
{
  printf("%15s %4s %|10s %-20s %-10s\n","Area","Exch","Material",
	 "Nickname","Value");
  DELIM;
  if (!areain)
    {
      foreach (string area,array ar in money)
	{
	  foreach (string ma, string nick in ar[1])
	    {
	      printf("%15s %4d %|10s %-20s %-10d\n",CAP(area),
		     query_exchange_rate(area),
		     ma || "None?",
		     nick,
		     query_rate(area+" "+nick,amount));
	    }
	}
      return 1;
    }
  else
    {
      foreach (string ma, string nick in money[areain][1])
	{
	  printf("%15s %4d %|10s %-20s %-10d\n",CAP(areain),
		 query_exchange_rate(areain),
		 ma || "None?",
		 nick,
		 query_rate(areain+" "+nick,amount));
	}    
    }
  DELIM;
  return 1;
}

void clean_up() {
    destruct(this_object());}
