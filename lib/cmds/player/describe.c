#define MAX_DESCRIP_SIZE 6
#include <commands.h>
#include <mudlib.h>

inherit CMD;

private string* descrip = ({});

void end_describe();

private void main(string s)
{
  if(stringp(s) && strlen(s))
    {
      
      this_body()->modal_push( (: 1 :), (: 1 :) );
      descrip = explode(wrap(s),"\n");
      end_describe();
      return;
    }

  write("Describe yourself in "+ MAX_DESCRIP_SIZE+" lines or less.\n"
	"Type '.' on a line of it's own when done.\n"
	"Type '~q' to abort this description.\n"
	"-----------------------------------------\n");

  new(CMD_OB_DESCRIBE)->begin_describing();
}


private void end_describe()
{
  this_user()->modal_pop();
  if(!sizeof(descrip))
    {
      write("Description unchanged.\n");
      destruct(this_object());
      return;
    }
  this_body()->set_description(implode(descrip, "\n") + "\n");
  write("Description finished.\n");
  destruct(this_object());
}


private void get_a_line(string input)
{
  if(input == ".")
    {
      end_describe();
      return;
    }
  if(input == "~q")
    {
      this_user()->modal_pop();
      write("Aborted.\n");
      destruct(this_object());
      return;
    }
  descrip += ({input});
  if(sizeof(descrip) == MAX_DESCRIP_SIZE)
    {
      end_describe();
    }
}

void begin_describing()
{
  if(base_name(previous_object()) != CMD_OB_DESCRIBE)
    {
      destruct(this_object());
      return;
    }
  
  this_user()->modal_push( (: get_a_line :), (: "" :) );
}

