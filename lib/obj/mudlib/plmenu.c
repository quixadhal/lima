/* Do not remove the headers from this file! see /USAGE for more info. */

/*	
**	Player menu by John Viega (rust@virginia.edu) 5-Jul-95
**	For the Lima mudlib.
**
**	Still want to have:
**	o channel menu
**     o more reporter commands
**	o what's new or something like that.
**	o take args at line, ie, b yes for biff or somesuch
**     o disable newbie options
*/

#include <menu.h>
#include <mudlib.h>
#include <playerflags.h>
#include <commands.h>
//### for now
#include <security.h>

inherit MENUS;
//### for now
inherit M_ACCESS;

MENU toplevel;
// submenus of the toplevel (main) menu
MENU soulmenu;
MENU reportmenu;
MENU remotemenu;
MENU personalmenu;
// submenus of personalmenu
MENU biffmenu;  
MENU snoopablemenu; 


MENU_ITEM quit_item;	
MENU_ITEM goto_main_menu_item;
MENU_ITEM main_seperator;

// right now, we're just going to call the help command.
//private MENU helpmenu;


private mapping dispatch = ([
    "n" : "news",
    "w" : "who",
    "b" : "bug",
    "t" : "typo",
    "i" : "idea",
    "?" : "help",
    "l" : "mudlist",
    ]);


private nomask void simple_cmd(string cmd)
{
    call_other(CMD_DIR_PLAYER "/" + dispatch[cmd], "player_menu_entry");
}


void
start_mail()
{
    this_body()->query_mailer()->begin_mail();
}

void handle_finger(string person)
{   ZBUG(person);
    CMD_OB_FINGER->player_menu_entry(person);
    prompt_then_return();
}

void test_soul(string s)
{
    CMD_OB_SEMOTE->player_menu_entry(s);
    prompt_then_return();
}

void find_soul(string s)
{
    if(!s) {
	write("Invalid.\n");	
	return;
    }

    CMD_OB_EMOTEAPROPOS->player_menu_entry(s);
}

void show_souls(string s)
{
    CMD_OB_FEELINGS->player_menu_entry(s);
}

 
void show_adverbs(string s)
{
  write(
"Remember: You can get any of these adverbs by typing part of the adverb \n"
"then *, if the part you type is unique.  Eg, kick rust ene* would give you:\n"
"kick rust energetically, but kick rust en* won't because it also\n"
"matches endearingly and enthusiastically.\n");

CMD_OB_ADVERBS->player_menu_entry(s);
}


void change_email(string s)
{
  this_user()->set_email(s);
  write("Email info changed.\n");
}

void change_url(string s)
{
  this_user()->set_url(s);
  write("Home page address changed.\n");
}

void change_title(string s)
{

  if(strsrch(s,"$N") == -1)
    s = "$N " + s;
  this_body()->set_title(s);
  
  write("Title changed.\n");
}

void
query_personal_info()
{
  printf("Your title is: %s\n"
	 "Your provided e-mail address is: %s\n"
	 "Your provided WWW home page is: %s\n"
	 "You %sget notified when new mail arrives.\n" 
	 "Wizards %s snoop you.\n",
	 this_body()->query_title(),
	 this_user()->query_email(),
	 this_user()->query_url(),
	 this_body()->test_flag(F_BIFF) ? "" : "DON'T ",
	 this_body()->test_flag(F_SNOOPABLE) ? "CAN" : "can't"
	 );

  prompt_then_return();
}

void prompt_change_title()
{
    input_one_arg("Change title to what? ", (: change_title :));
}

void prompt_change_email()
{
    input_one_arg("Change e-mail to what? ", (: change_email :));
}

void prompt_change_url()
{
    input_one_arg("Change your WWW homepage address to what? ", (: change_url :));
}


void set_biff(string s)
{
    if ( s == "y" )
	s = "on";
    else
	s = "off";

    CMD_OB_BIFF->player_menu_entry(s);
    goto_previous_menu();
} 

 
void set_ansi(string s)
{
    CMD_OB_ANSI->player_menu_entry(s);
}
      
   
void set_snoopable(string s)
{
    if ( s == "y" )
	s = "on";
    else
	s = "off";

    CMD_OB_SNOOPABLE->player_menu_entry(s);
    goto_previous_menu();
}

void finish_who(string mudname)
{
    CMD_OB_FINGER->player_menu_entry("@" + mudname);
    printf("%s queried.  It's up to that mud to reply to you.\n", mudname);
    prompt_then_return();
}

void
remote_who()
{
  write("Which mud do you want to query?\n");
  complete_choice(0, IMUD_D->query_mudnames(), (: finish_who :));
}


void create()
{
    set_privilege(1);

  toplevel 	= new_menu(mud_name()+" Game Menu");
  soulmenu 	= new_menu("Soul Menu");
  reportmenu 	= new_menu("Reporter Menu");
  personalmenu  = new_menu("Personal Menu");
  biffmenu      = new_menu("Notify you when you receive new mail?");
  snoopablemenu = new_menu("Allow wizards to snoop you?");
  remotemenu    = new_menu("Other muds");

  // Since we'll use these things more than once, we can just
  // call new_menu_item once, and insert them wherever we want
  // to use them.
  quit_item = new_menu_item("Quit", (:quit_menu_application:), "q");
  goto_main_menu_item =new_menu_item("Return to main menu", 
					 toplevel, "m");


  main_seperator = new_seperator("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");


  // Add items to the toplevel (main) menu.  
  add_menu_item (toplevel, main_seperator);
  add_menu_item (toplevel, new_menu_item("Read news (news)", 
					 (: simple_cmd :), "n"));
  add_menu_item (toplevel, new_menu_item("Send or read mail (mail)",
					 (: start_mail :), "m"));
  add_menu_item (toplevel, new_menu_item("See who's on (who)", 
					    (: simple_cmd :), "w"));
  add_menu_item (toplevel,
                new_menu_item("Get info about a person (finger)",
                           (: get_input_then_call,
                           (: handle_finger :),
                           "Finger who? " :), "f"));
  add_menu_item (toplevel, new_menu_item("Soul/emote Menu", soulmenu, "s"));
  add_menu_item (toplevel, new_menu_item("Report a bug, typo or idea", 
					 reportmenu, "r"));
  add_menu_item (toplevel, new_menu_item("Change or view your personal information. ",
					 personalmenu, "i"));
  add_menu_item (toplevel, new_menu_item("Information on other muds",
					 remotemenu, "o"));
  add_menu_item (toplevel, quit_item);
  add_menu_item (toplevel, new_menu_item("Help", (: simple_cmd :), "?"));

  // Add items to the soul menu.
  add_menu_item (soulmenu, main_seperator);
  add_menu_item (soulmenu, 
		 new_menu_item("List souls", (: input_one_arg,
					      "List all souls starting with "
					      "(enter for ALL souls): ",
					      (: show_souls :) :),
			       "s"));
  add_menu_item (soulmenu, 
		 new_menu_item("List adverbs", (: input_one_arg,
					      "List all adverbs starting with "
					      "(enter for ALL adverbs): ",
					      (: show_adverbs :) :),
			       "a"));
  add_menu_item (soulmenu, 
		 new_menu_item("Find souls", (: input_one_arg,
					      "Find souls whoose output "
					      "contains string: ",
					      (: find_soul :) :),
			       "f"));

  add_menu_item (soulmenu, 
  		 new_menu_item("Test a soul", (: input_one_arg,
					       "Soul to test: ",
					       (: test_soul :) :),  "t"));

  add_menu_item (soulmenu, quit_item);
  
  add_menu_item (soulmenu, goto_main_menu_item);
					 
  add_menu_item (reportmenu, main_seperator);
  add_menu_item (reportmenu, new_menu_item("Report a bug", (: simple_cmd :),
					   "b"));
  add_menu_item (reportmenu, new_menu_item("Report a typo", (: simple_cmd :),
					   "t"));
  add_menu_item (reportmenu, new_menu_item("Report an idea", (: simple_cmd :),
					   "i"));
  add_menu_item (reportmenu, quit_item);
  add_menu_item (reportmenu, goto_main_menu_item);

  add_menu_item (personalmenu, main_seperator);
  add_menu_item (personalmenu, new_menu_item("View your personal information",
					     (: query_personal_info :), "v"));
  add_menu_item (personalmenu, new_menu_item("Change your title", 
					     (: prompt_change_title :), "t"));
  add_menu_item (personalmenu, new_menu_item("Change your supplied "
					     "e-mail address",
					     (: prompt_change_email :), "e"));
  add_menu_item (personalmenu, new_menu_item("Change your supplied WWW home "
					     "page address",
					     (: prompt_change_url :), "w"));
  add_menu_item (personalmenu, new_menu_item("Set or unset mail notification",
					     biffmenu, "n"));
  add_menu_item (personalmenu, new_menu_item("Set whether or not you can be "
					     "snooped", snoopablemenu, "s"));
  //  add_menu_item (personalmenu, new_menu_item("Change your supplied real name",
  //					     (: prompt_change_real_name :), "n"));
  add_menu_item(personalmenu, new_menu_item("Set ANSI on/off",
					    (: input_one_arg,
					     "Ansi 'on' or 'off' ? (default off): ",
 					     (: set_ansi :) :),
					    "a"));
 
  add_menu_item (personalmenu, quit_item);
  add_menu_item (personalmenu, goto_main_menu_item);

  add_menu_item (biffmenu, main_seperator);
  add_menu_item (biffmenu, new_menu_item("Yes", (: set_biff :), "y"));
  add_menu_item (biffmenu, new_menu_item("No", (: set_biff :), "n"));
  add_menu_item (biffmenu, quit_item);
  add_menu_item (biffmenu, goto_main_menu_item);

  add_menu_item (snoopablemenu, main_seperator);
  add_menu_item (snoopablemenu, new_menu_item("Yes", 
					      (: set_snoopable :), "y"));
  add_menu_item (snoopablemenu, new_menu_item("No", 
					      (: set_snoopable :), "n"));
  add_menu_item (snoopablemenu, quit_item);
  add_menu_item (snoopablemenu, goto_main_menu_item);

  add_menu_item (remotemenu, main_seperator);
  add_menu_item (remotemenu, new_menu_item("List muds " + mud_name() + 
					   " knows about", 
                (: simple_cmd :), "l"));
  add_menu_item (remotemenu, new_menu_item("See who's on another mud",
					   (:remote_who:), "w"));
  add_menu_item (remotemenu, quit_item);
  add_menu_item (remotemenu, goto_main_menu_item);
}


void
start_menu()
{
  init_menu_application(toplevel);
}
