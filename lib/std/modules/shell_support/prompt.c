/* This file optimizes prompt displaying.*/


#define P_HISTORY 	1
#define P_ROOM 		2
#define P_PWD		3
#define P_DATE		4
#define P_DAY		5
#define P_NAME		6
#define P_TIME		7

private static int* replacements;

private static string prompt;


int get_command_number();
mixed get_variable(string);

static void set_prompt(string s)
{
  string*	bits;
  int		i;
  
  s = replace_string(s,"%_","\n");
  s = replace_string(s,"%m", mud_name());
  s = replace_string(s,"%N", this_user()->query_userid());
  if(s[0] == '"' && s[<1] == '"')
    s = s[1..<2];
  prompt = s;

  replacements = ({});
  bits = explode("%"+s,"%");
  if(!sizeof(bits))
    return;
  for(i=1;i<sizeof(bits);i++)
    {
      if(bits[i] == "")
	{
	  i++;
	  continue;
	}
      switch(bits[i][0])
	{
	case 'h':
	  replacements += ({P_HISTORY});
	  break;
	case 'r':
	  replacements += ({P_ROOM});
	  break;
	case 'p':
	  replacements += ({P_PWD});
	  break;
	case 'd':
	  replacements += ({P_DATE});
	  break;
	case 'D':
	  replacements += ({P_DAY});
	  break;
	case 'n':
	  replacements += ({P_NAME});
	  break;
	case 't':
	  replacements += ({P_TIME});
	  break;
	}
    }
  replacements = clean_array(replacements);
}


static string get_prompt()
{

  string	s;
  int		rep;
  int		i;

  if(!prompt)
    {
      set_prompt(get_variable("PROMPT") || "? for shell help> ");
    }

  s = prompt;

  foreach(rep in replacements)
    {
      switch(rep)
	{
	case P_PWD:
	  s = replace_string(s,"%p", this_body()->query_pwd() || "(no pwd!)");
	  continue;
	case P_TIME:
	  s = replace_string(s,"%t", ctime(time())[11..15]);
	  continue;
	case P_HISTORY:
	  s = replace_string(s,"%h", sprintf("%d", get_command_number()));
	  continue;
	case P_ROOM:
	  s = replace_string(s,"%r", file_name(environment(this_body())));
	  continue;
	case P_DATE:
	  s = replace_string(s,"%d", ctime(time())[4..9]);
	  continue;
	case P_DAY:
	  s = replace_string(s,"%D", ctime(time())[0..2]);	
	  continue;
	case P_NAME:
	  s = replace_string(s,"%n", capitalize(this_body()->query_name()));
	  continue;
	}
    }
  return s;
}


static void create()
{
  this_object()->add_variable_hook("PROMPT", (:set_prompt:));
}
