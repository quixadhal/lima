/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** ed_session.c
**
** Represents an editing session.  This is used in conjunction with the
** input system to allow nested editing/more/help/etc.
**
** 23-Jan-95. Deathblade. Created.
*/

#include <mudlib.h>
#include <playerflags.h>
#include <clean_up.h>

inherit M_INPUT;
inherit M_ACCESS;

nosave private function	end_func;
nosave private object	user;
nosave private int restrict;

private int already_editing = 0;

private nomask void receive_ed_input(mixed s)
{
  if(s == -1) {
    destruct(this_object());
    return;
  }

  /* Using standard ed() restrictions is not sufficient.  Privilege 0 documents
   * can still be read into the editor from persons who should not be, such as
   * players.  Therefore eliminate a certain set of commands.  This includes
   * all reading and writing commands in the editor.  If you have a privilege, 
   * you can, but that generally means that you are a wizard.  For some slight
   * variations of security (like if all of your users have privileges 
   * regardless of status, you might have to do something different here
   * -- Tigran
   */
  if(restrict&&!this_user()->query_privilege())
    {
      switch(s[0])
	{
	case 'e':
	case 'E':
	case 'f':
	case 'r':
	case 'w':
	case 'W':
	case 'x':
	  tell(this_user(),"Operation not allowed.");
	}
    }
  tell(this_user(), ed_cmd(s), NO_ANSI);
  if ( query_ed_mode() == -1 )
    {
      modal_pop();
      if( already_editing )
	--already_editing;
      else if(this_body())
	this_body()->clear_flag(F_IN_EDIT);
      
      if ( end_func )
	evaluate(end_func);
      
      destruct(this_object());
    }
}

private nomask string query_prompt()
{
  int line;
  
  line = query_ed_mode();
  if ( line == -2 )
    return "";
  
  if ( line )
    return "";
  
    return ":";
}

varargs nomask void begin_editing(string fname,
				  int restricted,
				  function f)
{
  restrict=restricted;
  modal_push((: receive_ed_input :), (: query_prompt :));
  if( this_body() )
    {
      if(this_body()->test_flag( F_IN_EDIT ))
	already_editing++;
      else
	this_body()->set_flag(F_IN_EDIT);		
    }
  user = this_user();
  
  end_func = f;
  
  ed_start(fname, restricted);
  printf("Editing: /%s", ed_cmd("f"));
  if(!is_file(fname))
    printf("[New file]\n");
}

int set_ed_setup(int code) {
  user->set_ed_setup(code);
}

void query_ed_setup() {
  return user->query_ed_setup();
}

private void create() {
  set_privilege(1);
}

private int clean_up() {
  if (query_ed_mode() == -1)
    destruct(this_object());
  return ASK_AGAIN;
}
