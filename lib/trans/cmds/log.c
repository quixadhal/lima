/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz
int main()
{
  string	file;

  file = wiz_dir(this_user()) + "/log";

  if(file_size(file) <1)
	return write("No log.\n"), 1;

  return tail(file), 1;
}

int help(){
return write("Usage: log\nTails your error log.\n"), 1;
}
