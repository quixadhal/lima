/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

private void main( string foo )
{
  string time,year,month,arg;
  int date;
  
  if (foo) sscanf(foo,"%s %s",foo,arg);
  else
    foo="FOO!";
  switch(foo)
    {
    case "-s":
      time=ctime(time());
      sscanf(time,"%*s %s %d %*s %s",month,date,year);
      printf("** %d-%s-%s\t%-15s%s.\n",
	     date,
	     month,
	     year[2..3],
	     this_body()->query_name()+".",
	     arg ? arg : "Created"
	     );	     
      break;
    case "a":
      if (arg=="babe")
	{
	  switch(random(6))
	    {
	    case 0:
	      write("You got it pal.\nWatch out for the weekend coming up.\n");
	      break;
	    case 1:
	      write("You call 900-BABE.\n");
	      break;
	    case 2:
	      write("You rummage through your belongings and find Karen "+
		    "Mulder's phonenumber at last!\n");
	      break;
	    case 3:
	      write("Sure thing bob!\n");
	      break;
	    case 4:
	      write("Uhm.. better get off the mud then, right?\n");
	      break;
	    case 5:
	      write("Yeah... wish it was that easy.\n");
	      break;
	    }      
	}
    default:
      write(ctime(time())+"\n");
      break;
    }
}

