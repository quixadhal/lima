/* Do not remove the headers from this file! see /USAGE for more info. */

#include <more.h>

private static string		stdoutstr = 0;
private static string 		buf ;
private static mixed		next_cmd;
private static mixed		next_implode_info;
private static int		no_redirect = 0;
private static int		output_flags = 0;

static int set_output_flags(int x) {
    output_flags = x;
}

static void no_redirection()
{
  no_redirect = 1;
}

static int restrict_redirection()
{
  return no_redirect;
}

mixed get_rest_of_args()
{
  mixed tmp = ({next_cmd, next_implode_info});
  
  next_cmd = 0;
  next_implode_info = 0;
  return tmp;
}

string get_output()
{
  return buf;
}

static int end_of_pipeline()
{
  return !next_cmd;
}

private int determine_stdout(mixed args)
{
  string	op = args[0];
  string	stdoutfile; 
  int		clobber = 0;

  switch(op)
    {
    case ">":
      clobber = 1;
    case ">>":
      stdoutfile = evaluate_path(implode(args[1..], " "));
      if(!is_directory(base_path(stdoutfile)))
	{
	  printf("%s is not a valid directory.\n", base_path(stdoutfile));
	  return 0;
	}
      if(clobber)
	{
	  if(is_file(stdoutfile) && (!rm(stdoutfile)))
	    {
	      printf("%s: couldn't remove.\n", stdoutfile);
	      return 0;
	    }
	}
      stdoutstr = stdoutfile;
      break;
    case "|":
      next_cmd = args[1..];
      break;
    default:
      error("Should never get here...");
    }
  return 1;
}

static void bare_init()
{
  stdoutstr = 0;
  buf = "";
  next_cmd = 0;
  output_flags = 0;
}

static mixed hello_stdio(mixed ininfo, mixed argstohold, mixed implode_info)
{
  string	stdinfile;
  string	stdin;
  int		i;

  stdoutstr = 0;
  buf = "";
  next_cmd = 0;

  next_implode_info = implode_info;
  /* First, if there's ininfo, read our std input */
  if(ininfo)
    {
      if(stringp(ininfo))
	{
	  // We got stdin from a pipe
	       stdin = ininfo;
	}
      else
	{
	  stdinfile = evaluate_path(implode(ininfo," "));
	  if(!is_file(stdinfile))
	    {
	      printf("%s is not a file.\n", stdinfile);
	      return 0;
	    }
	  stdin = read_file(stdinfile);
	}
    }
  if(argstohold)
    {
      i = 1;
      while((i < sizeof(argstohold)) && (argstohold != "|")) i++;
	next_cmd = argstohold[i..];
      if(!sizeof(next_cmd))
	next_cmd = 0;
      i = determine_stdout(argstohold[0..(i-1)]);
      if(i != 1)
	return i;
    }
  return stdin || 1;
}

static mixed done_outputing()
{
  object	stdoutobj;
  string	tmp;

  if((!stdoutstr) && (!next_cmd))
    {
      if(buf == "" || !buf)
	return 0;
      more(explode(buf,"\n"), 0, 0, output_flags);
      return 0;
    }
  if(stdoutstr)
    {
      if(!write_file(stdoutstr,buf))
	{
	  printf("error: couldn't write to %s.\n", stdoutstr);
	}
      return 0;
    }
  tmp = buf;
  buf = 0;
  return ({ next_cmd, tmp, next_implode_info });
}

static void out(string s)
{
  if(!buf)
    buf = "";
  buf += s;
}

static void outf(string format, mixed args ... )
{
  buf += sprintf(format, args...);
}





