/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust
#include <mudlib.h>
inherit DAEMON;

int
main(mixed argv, mapping flags)
{
  string str, fname, file;
  
  if(arrayp(argv[0]) && sizeof(argv[0]) > 1 && !is_directory(argv[1]))
    {
      printf("mv: files dir, not mv files file\n");
      return 1;
    }
  if(sizeof(argv[0]) > 1)
    {
      if(argv[1][<1] != '/')
	argv[1] += "/";
      foreach(file in argv[0])
	{
	  fname = argv[1] + split_path(file)[1];
	  BBUG(fname);
	  if(is_file(fname) && !flags["f"])
	    {
	      printf("%s already exists.  Move failed.\n", fname);
	    }
	  else
	    {
	      if(!str = read_file(file))
		{
		  printf("%s could not be read.  Move failed.\n", file);
		}
	      else
		if(!write_file(fname, str))
		  {
		    printf("%s could not be written to.  Move failed.\n",
			   fname);
		  }
		else
		  if(!rm(file))
		    {
		      printf("%s couldn't be removed.\n", file);
		    }
		  else
		    {
		      printf("%s moved to %s.\n", file, fname);
		    }
	    }
	}
    }
  else 
    {      

	  fname = argv[1];
	  file = argv[0][0];
	  if(is_directory(fname))
	    {
	      if(fname[<1] != '/')
		fname += "/";
	      fname += depath(file);
	    }
	  if(is_file(fname) && !flags["f"])
	    {
	      printf("%s already exists.  Move failed.\n", fname);
	    }
	  else
	    {
	      if(!str = read_file(file))
		{
		  printf("%s could not be read.  Move failed.\n", file);
		}
	      else
		if(!write_file(fname, str))
		  {
		    printf("%s could not be written to.  Move failed.\n",
			   fname);
		  }
		else
		  if(!rm(file))
		    {
		      printf("%s couldn't be removed.\n", file);
		    }
		  else
		    {
		      printf("%s moved to %s.\n", file, fname);
		    }
	    }

    }
}
