/* Do not remove the headers from this file! see /USAGE for more info. */

// Simple cp, no -r yet.
#include <mudlib.h>
inherit CMD;

private void
main(mixed argv, mapping flags)
{
  string str, fname, file;
  
  if(sizeof(argv[0]) > 1 && !is_directory(argv[1]))
    {
      printf("cp: files dir not files file\n");
      return;
    }
  if(sizeof(argv[0]) > 1)
    {
      if(argv[1][<1] != '/')
	argv[1] += "/";
      foreach(file in argv[0])
	{
	  fname = argv[1] + split_path(file)[1];
	  if(is_file(fname) && !flags["f"])
	    {
	      printf("%s already exists.  Copy failed.\n", fname);
	    }
	  else
	    {
	      if(!(str = read_file(file)) || (is_file(fname) && !rm(fname)))
		{
		  printf("Permissions problem.  Copy failed.\n");
		}
	      else
		{
		  if(!write_file(fname, str))
		    {
		      printf("%s could not be written to.  Copy failed.\n",
			   fname);
		    }

		  else
		    {
		      printf("%s copied to %s.\n", file, fname);
		    }
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
	      printf("%s already exists.  Copy failed.\n", fname);
	    }
	  else
	    {
	      if(!(str = read_file(file)) || (is_file(fname) && !rm(fname)))
		{
		  printf("Permissions problem.  Copy failed.\n");
		}
	      else
		if(!write_file(fname, str))
		  {
		    printf("%s could not be written to.  Copy failed.\n",
			   fname);
		  }
		else
		  {
		    printf("%s copied to %s.\n", file, fname);
		  }
	    }

    }
}
