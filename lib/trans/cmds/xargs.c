/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$see: converse
//USAGE:	xargs <command>
//
//Acts like 'converse' (see help converse), except that it
//allows you to use something other than say.  For example,
//
//xargs tell rust
//
//will send all your input to Rust until you type: **

inherit CMD;
inherit M_INPUT;

private nomask void handle_piping(string verb, string arg)
{
  if( arg == "**" || arg == "." )
  {
    write("Done.\n");
    modal_pop();
    destruct();
    return;
  }

  this_user()->force_me(verb + " " + arg);
}


varargs nomask void start_cmd(mixed * arg,int lock)
{
  if(!clonep() || (base_name(previous_object()) != base_name()))
  {
    write("Illegal attempt to spoof command.\n");
    destruct();
    return;
  }
  write("Entering pipe mode. Type '**' to quit.\n");
  if(lock)
    modal_push((: handle_piping, arg[0] :), "*\b",0,0,1);
  else
    modal_push((: handle_piping, arg[0] :), "*\b");
}

private nomask void main(mixed * arg, mapping flags, string stdin,
			 string array implode_inf)
{
  arg[0] = implode_by_arr(arg[0],implode_inf);
  if (stdin)
  {
    arg[0] = replace_string(arg[0],"$*","$$");
    if(strsrch(arg[0],"$$") != -1)
      foreach (string line in explode(stdin, "\n") - ({ "" }))
        this_user()->force_me(replace_string(arg[0],"$$",line));
      else
        foreach (string line in explode(stdin, "\n") - ({ "" }))
          this_user()->force_me(arg[0] + " " + line);
  } else {
    if(flags["!"])
      new(file_name())->start_cmd(arg,1);
    else
      new(file_name())->start_cmd(arg);
  }
}