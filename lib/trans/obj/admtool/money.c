/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** money.c -- Currency administration
**
** 25-Jul-96   Valentino.    Created.
**  6-Aug-96   Valentino.    Generalized and ported to Da Bean.
*/

#include <daemons.h>
#define ADMINS "Admin"
void std_handler(string str);
varargs void modal_simple(function input_func, int secure);
varargs void modal_func(function input_func, mixed prompt_func, int secure);
void do_one_arg(string arg_prompt, function fp, string arg);
void do_two_args(string arg1_prompt, string arg2_prompt,
		 function fp, string arg);
void do_three_args(string arg1_prompt, string arg2_prompt,string arg3_prompt,
		 function fp, string arg);

#define PROMPT_MONEY     "(AdmTool:currency)   [alrxmq?] > "

private nomask void write_money_menu()
{
    write("Administration Tool: currency administration\n"
	  "\n"
	  "    a [ar] [mat] [nic] - add a currency name ["+ADMINS+"]\n"
	  "    l [area]           - list currencies\n"
	  "    r [area] [materi]  - remove a currency name ["+ADMINS+"]\n"
	  "    x [area] [amount]  - adjust exchange rate ["+ADMINS+"]\n"
	  "\n"
	  "    m                  - main menu\n"
	  "    q                  - quit\n"
	  "    ?                  - help\n"
	  "\n"
	  );
}

varargs private nomask void
list_cur(string area)
{
  if (area=="") area=0;
  if (area && member_array(area,MONEY_D->query_currency_areas())==-1)
    {
      write("*** That area is not defined.\n");
      return;
    }
  MONEY_D->stat_me(area,10000);
}

varargs private nomask void
add_cur(string area,string material,string nickname)
{
  if (!stringp(nickname) || !stringp(material) || !stringp(nickname))
    {
      write("*** Wrong arguments given.\n");
      return;
    }
  MONEY_D->add_currency(area,material,nickname);
  write("Added currency successfully.\n");
}

varargs private nomask void
adj_ex(string area,int amount)
{
  if (!stringp(area) || !intp(amount))
    {
      write("*** Wrong arguments given.\n");
      return;
    }
  if (MONEY_D->adjust_exchange_rate(area,amount))
    write("Added currency successfully.\n");
  else
    write("Nothing changed.\n");
}

varargs private nomask void
rem_cur(string area,string material)
{
  if (!stringp(area) || !stringp(material))
    {
      write("*** Wrong arguments given.\n");
      return;
    }
  write("Trying remove.\n");
  MONEY_D->remove_currency(area,material);
}

private nomask void receive_money_input(string str)
{
  int i;
  string arg,arg2;
  
  sscanf(str,"%s %s",str,arg);
  switch(str)
    {
    case "?":
      write_money_menu();
      break;
    case "a":
      if (!adminp(this_body()))
	{
	  write("Sorry... "+ADMINS+" only.\n");
	  return;
	}
      do_three_args("What area name? ","What material? ","What nickname? ",
		    (:add_cur:),arg);
      break;
    case "r":
      if (!adminp(this_body()))
	{
	  write("Sorry... "+ADMINS+" only.\n");
	  return;
	}
      do_two_args("What area name? ","What material? ",(:rem_cur:),arg);
      break;
    case "x":
      if (!adminp(this_body()))
	{
	  write("Sorry... "+ADMINS+" only.\n");
	  return;
	}
      sscanf(arg,"%s %d",arg,i);
      adj_ex(arg,i);
      break;
    case "l":
      do_one_arg("What area? [default: all] ",(:list_cur:),arg);
      break;
    default:
      std_handler(str);
      break;
    }
}

static nomask void begin_money_menu()
{
  modal_func((: receive_money_input :), PROMPT_MONEY);
  write_money_menu();
}



