/* Do not remove the headers from this file! see /USAGE for more info. */

//  Command line processing

#include <mudlib.h>

private
int classify( int opt, string options ){
  int i;

  i = member_array(opt, options);
  if(i == -1){
    printf("option -%c not recognized", opt);
    return 0;
  }
  if( ++i != strlen(options) ) return options[i] == ':';
}


DOC(getopt, "Takes an array and parses flags from it. Returns an array, the first "
"element being a mapping of flag : value, the second element being an array of "
"the remaining args, all flag information removed.  The second argument should "
"be a string of single character flags that the command accepts, followed by a "
"colon (:) if the flag can take an argument.");
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
DOC(argument_explode, "assumes the arg passed is the argument to some unix-like "
"command where ares are space seperated unless enclosed in non-escaped quotes. "
"Returns an array of the arguments.");

private mixed get_words(string s)
{
  string t;
  t = trim_spaces(s);
  return ((t[0] == '`' && t[<1] == '`') || 
	  (t[0] == '"' && t[<1] == '"')) ? t : 
	    map(reg_assoc(s, ({"[^ ]*[ ]*"}), ({0}))[0] - ({""}), 
		(: $1[<1] == ' ' ? $1[0..<2] : $1 :));
}

mixed
argument_explode( mixed input ){
  mixed res;
  res = reg_assoc( " "+input+" ", ({"(`([^\\\\`](\\\\`)?(`[^ ])?)+` )|(\"([^\\\\\"](\\\\\")?(\"[^ ])?)+\" )"}), ({ 0 }))[0] - ({""," "});
//  res = reg_assoc( input+" ", ({"(`([^`](`[^ ])?)+` )|(\"([^\"](\"[^ ])?)+\" )"}), ({0}))[0] - ({""," "});
//  res = map(res, (: trim_spaces :));
//  res = decompose(map(res,(: (($1[0] == '`' && $1[<1] == '`')  || ($1[0] == '"' && $1[<1] == '"') ) ? $1 : explode($1," ") :))) - ({"", " "});
  res = decompose(map(res, (: get_words :))) - ({""});

  return res;
}

DOC(parse_argument, "calls argument_explode() and then getopt(), returning the "
"value of the getopt() call.");
mixed parse_argument( string input, string options ){
  return getopt( argument_explode(input), options );
}


