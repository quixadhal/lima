/* Do not remove the headers from this file! see /USAGE for more info. */
//  Command line processing

#include <mudlib.h>

private
int classify( int opt, string options ){
  int i;

  i = member_array(opt, options);
  if(i == -1){
    printf("option -%c not recognized\n", opt);
    return 0;
  }
  if( ++i != strlen(options) ) return options[i] == ':';
}


//:FUNCTION getopt
//Takes an array and parses flags from it. Returns an array, the first
//element being a mapping of flag : value, the second element being an
//array of the remaining args, all flag information removed.  The second
//argument should be a string of single character flags that the command
//accepts, followed by a colon (:) if the flag can take an argument.
mixed
getopt( mixed args, string options )
{
  mapping res;
  mixed optstring, opt, optarg;

  res = ([]);

  
  if(!options)  options = "";

  while (sizeof(args) && stringp(args[0]) && args[0][0] == '-' && 
 	 args[0] != '-'){
    if( args[0] == "--" ){
      args = (sizeof(args)>1)? args[1..] : ({});
	 break;
    }
    optstring = strlen(args[0]) == 1 ? "" : args[0][1..];
    args = sizeof(args) == 1 ? ({}) : args[1..];
    while(optstring!=""){
      opt = optstring[0];
      optstring = strlen(optstring) == 1 ? "" : optstring[1..];
      if(classify(opt,options)){
        if(optstring == ""){
  	    if(!sizeof(args)){
  		printf("option -%c requires an argument.\n", opt);
  		return -1;
  	    }
            optstring = args[0];
	       args = sizeof(args)>1?args[1..]:({});
        }
        optarg = optstring;
        optstring = "";
     res[chr(opt)] = optarg;
      }
      else{
	   optarg = "";
           res[chr(opt)] = optarg;
      }
    }
  }
  return ({ res, args });

}
   

// Got the idea for this one from python, 
// but unlike getopt, didn't pirate the implimentation =)
//  

//:FUNCTION argument_explode
//assumes the arg passed is the argument to some unix-like
//command where ares are space seperated unless enclosed in non-escaped
//quotes.
//Returns an array of the arguments. and an array of implode info

mixed* argument_explode(string s)
{
  string* result = ({});
  string* implode_info = ({});
  int i, l;
  int waiting_for;
  int end_of_last_word = -1;
  int beginning_of_word = 0;

  l = strlen(s);
  for(i=0; i < l; i++)
    {
      switch(s[i])
	{
	case '\\':
	  // This is because all \\'s aren't converted to \ here. 
	  if(i+1 != l && s[i+1] == '\\')
	    break;
	  i++;
	  break;
	case '`':
	case '"':
	  if(!waiting_for)
	    {
	      waiting_for = s[i];
	      if(beginning_of_word != i)
		{
                  implode_info += 
                    ({s[end_of_last_word+1..beginning_of_word-1]});
                  end_of_last_word = i-1;
		  result += ({s[beginning_of_word..end_of_last_word]});
		}
	      beginning_of_word = i;
	      break;
	    }
	  if(waiting_for != s[i])
	    continue;
	  result += ({s[beginning_of_word..i]});
	  implode_info += 
	    ({s[(end_of_last_word+1)..(beginning_of_word-1)]});
	  end_of_last_word = i;
	  beginning_of_word = i+1;
	  waiting_for = 0; 
	  break;
        case '|':
          if(waiting_for)
	    break;
	  if(beginning_of_word != i)
	    {
	      result += ({s[beginning_of_word..i-1],"|"});
	      implode_info +=({s[end_of_last_word+1..beginning_of_word-1],""});
	      end_of_last_word = i;
	      beginning_of_word = i+1;
	      break;
	    }
	  implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
	  result += ({"|"});
	  beginning_of_word = i+1;
	  end_of_last_word = i;
	  break;
	case '<':
	  if(waiting_for)
	    break;
	  if(beginning_of_word != i)
	    {
	      result += ({s[beginning_of_word..i-1],"<"});
	      implode_info +=({s[end_of_last_word+1..beginning_of_word-1],""});
	      end_of_last_word = i;
	      beginning_of_word = i+1;
	      break;
	    }
	  result += ({"<"});
	  implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
	  end_of_last_word = i;
	  beginning_of_word = i+1;
	  break;
	case '>':
	  if(waiting_for)
	    break;
	  if(beginning_of_word != i)
	    {
	      implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
	      result += ({s[beginning_of_word..i-1]});
	      beginning_of_word = i+1;
	      end_of_last_word = i;
	    }	
	  if(s[i+1] == '>')
	    {
	      // Skip the next >
	      i++;
	      implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
	      end_of_last_word = i;
	      beginning_of_word = i+1;
	      result += ({">>"});
	      break;
	    }
	  else
	    {
	      implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
	      end_of_last_word = i;
	      beginning_of_word = i+1;
	      result += ({">"});
	      break;
	    }
	case ' ':
	  if(waiting_for)
	    break;
	  if(beginning_of_word == i)
	    {
	      beginning_of_word = i+1;
	      break;
	    }
	  implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
	  result += ({s[beginning_of_word..i-1]});
	  end_of_last_word = i-1;
	  beginning_of_word = i+1;
	  break;
	default:
	  break;
	}
    }
  if(beginning_of_word < l)
    {
      implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
      result += ({ s[beginning_of_word..] });
      implode_info += ({""});
    }
  else
    {
      implode_info += ({s[end_of_last_word+1..beginning_of_word-1]});
    }

  return ({result,implode_info});
}

//:FUNCTION parse_argument
//calls argument_explode() and then getopt(), returning the 
//value of the getopt() call.
mixed parse_argument( string input, string options ){
  return getopt( argument_explode(input), options );
}


