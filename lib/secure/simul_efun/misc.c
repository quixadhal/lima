/* Do not remove the headers from this file! see /USAGE for more info. */

#include <driver/type.h>

int is_directory(string);
string evaluate_path(string);
object find_body(string);

DOC(call_trace, "returns the stack of objects and functions")

string
call_trace() {
    string res;
    int i, n;
    object array objects;
    string array programs;
    string array functions;
    
    res = "";
    programs = call_stack(0);
    objects = call_stack(1);
    functions = call_stack(2);

    n = sizeof(programs);
    for (i = 0; i < n; i++) {
	res += sprintf("%25-O %25-s %25-s\n", objects[i], programs[i], 
		       functions[i]);
    }
    return res;
}

// There should be an | operator for this.
DOC(clean_array, "returns a version of the passed array with duplicate "
    "entries removed.  Eg, clean_array(({1,2,2}))  => ({1,2})")

mixed*
clean_array(mixed* r) {
    int i, n;

    r = r & r; // sort.  sort_array() can't hack it.  And no, &= doesn't work.

    n = sizeof(r) - 1;
    while (i < n) {
	if (r[i] == r[i+1]) {
	    r[i..i] = ({});
	    n--;
	} else
	    i++;
    }

    return r;
}

DOC(cmp, "returns whether its two arguments are equivolent.  This is about"
"the same as using the equivolence operator (==), but will return true"
"in cases where == does not, such as when comparing 2 arrays. "
"Logically 2 arrays should be equivolent, but aren't with ==. "
"cmp knows they are.  This is mainly useful when you want to compare "
"mappings and arrays.")

int cmp( mixed a, mixed b )
{
  int		i;
  mixed		x, y, z;

  if( arrayp( a ) && arrayp( b ) )
  {

    if( sizeof(a) != ( i = sizeof(b) ) )
      return 0;

    while( i-- )
      if( !cmp(a[i], b[i]) )
        return 0;
    return 1;
  }

  if( mapp( a ) && mapp( b ) )
  {

    if( sizeof(a) != (i = sizeof(b)) )
      return 0;

    foreach (x,y in a) {
	z = b[x];
	if (undefinedp(z)) return 0;
	if (!cmp(y, z))
	    return 0;
    }
    return 1;
  }

  return a == b;
}


DOC(insert, "Inserts the contents of the array of the first argument into "
    "The array in the second argument before the nth element of the array, "
    "where n is the 3rd argument passed to insert.")

// Rust hacked at this to make it a bit more intuitive...

mixed insert( mixed 	to_insert,
	mixed	into_array,
	int	where )
{
  if( !arrayp( to_insert ) )
    return (void)error("Bad type arg 1 to simul efun insert()");

  if( !arrayp( into_array ) )
    return (void)error("Bad type arg 2 to simul efun insert()");

  if( !intp( where ) )
    return (void)error("Bad type arg 3 to simul efun insert()");

  return into_array[0..where-1] + to_insert + into_array[where..];
}


string parse_ref(string s)
{
  string result;
  string noun;
  string* pieces = explode(s,":");

  noun = pieces[0];
  pieces = pieces[1..];
  switch(noun)
    {
    case "me":
      result = "this_body()";
      break;
    case "here":
      result = "environment(this_body())";
      break;
    default:
      if(find_body(noun))
	result = "find_body(\""+noun+"\")";
      else if(load_object(evaluate_path(noun)))
	result = "find_object(evaluate_path(\""+noun+"\"))";
      else return s;
    }
  foreach(noun in pieces)
    {
      switch(noun) {
      case "shell":
	result = sprintf("(%s->query_shell_ob() || %s)", result, result);
	break;
      case "link":
	result = sprintf("(%s->query_link())",result);
	break;
      default:
	result = sprintf("present(\"%s\",%s)", noun, result);
	break;
      }
      
    }
    return result;
}


/* eval lets you evaluate a string as an expression.
   exec_code allows you to evaluate a string as LPC code. */
varargs mixed exec_code(string arg, string dir, string includefile)
{
    // DO NOT USE UNGUARDED IN HERE
    // If perms are bad, we want to fail.  Use unguarded(1, (: exec_code :))
    // if really necessary -Beek
  string	file = dir + "/exec.c";
  object	tmp;
  mixed		retval;
  mixed		info;
  int		i;
  string	contents;
  
    if(tmp = find_object(file))
	destruct(tmp);

    rm(file);

    if(!stringp(arg))
	error("Bad type argument 1 to exec_code");

    info = reg_assoc(arg, ({"\\.[a-z:/]+"}),({0}))[0];
    for(i=0; i < sizeof(info);i++)
	{
	    if(info[i][0] == '.' && strlen(info[i]) > 1 )
		{
		    info[i] = parse_ref(info[i][1..]);
		}
	}
    arg = implode(info,"");

    if (strsrch(arg,";")==-1) arg = "return "+arg;

    contents = "";
    if(includefile)
	contents += sprintf("\n#include \"%s\"\n", includefile);
    contents += sprintf( "mixed exec_foo(){ %s", arg );
    contents += ";}\n";
    write_file(file, contents);
    retval = file->exec_foo();
    rm(file);
    return retval;

}
    
// eval by Rust, so that you can
// convert a string to almost anything.
// bet on it being as slow as dirt, though... 

DOC(eval, "evaluates the string s as an LPC value.  Eg, if you have a string "
	"someonetyped in: \"({1,2,3,4})\"  eval returns the actual array "
    	"({1,2,3,4}).")

varargs mixed eval( string arg, string includefile )
{
    mixed	tmp;
    string	file;

    file = "/tmp/eval.c";

    if( tmp = find_object( file ) )
	destruct( tmp );
    rm(file);
    if( !stringp( arg ) )
    {
	error( "Bad type argument 1 to eval" );
	return;
    }
    if(includefile)
      write_file(file,sprintf("#include \"%s\"\n",includefile));

	
      
    if( arg == "0" ) return 0;
  
    if( tmp = to_int( arg ) )
	if(tmp + "" == arg)
	    return tmp;
    if( tmp = to_float( arg ) )
	if(tmp + "" == arg)
	    return tmp;

    if( tmp = find_object(arg) && objectp( tmp ) )
	return tmp;

    if( strlen( arg ) < 4 )
	return arg;

    write_file( file, sprintf( "foo(){ return %s; }\n", arg ) );
    if( catch( tmp = file->foo() ) ) 
	return arg;

    if( tmp && !stringp(tmp) )
	return tmp;

    return arg;
}

DOC(decompose, "Takes any arrays that are elements in arr and merges "
    "all of its elements as elements of arr.  Eg, decompose(({1,({2,3,}),4})) "
    "will return: ({1,2,3,4}).  The algorithm is not recursive, so if any of "
    "the arrays have arrays in them, those arrays remain intact.  Eg, "
    "decompose( ({1,({({2,3}),4}),5}) )  returns:({1,({2,3}),4,5}). "
    "See flatten_array for a recursive version.")

mixed* decompose( mixed* org )
{
    int i = 0, j;

    if( !arrayp( org ) ) error("Bad type arg to decompose");
    org = copy(org);

    while (i < sizeof(org)) {
	if (arrayp(org[i])) {
	    j = sizeof(org[i]);
	    org[i..i] = org[i];
	    i += j; // skip the elements inserted
	} else i++;
    }
    return org;
}


DOC(len,"returns the length of any aggregate type, be it the "
"length of a string, the number of elements in an array, or "
"the number of entries in a mapping. ")

int 
len( mixed f ){
    return sizeof(f);
}


DOC(choice, "Returns a random element of the structure passed, if that "
    "structure is an aggregate type (i.e., A string, array or mapping).")

mixed choice( mixed f ){
    mixed *k;
  switch(typeof(f)){
    case STRING: return f[random(strlen(f))];
    case ARRAY: return f[random(sizeof(f))];
    case MAPPING: k = keys(f); return f[k[random(sizeof(k))]];
    default:
      error("choice of non-sequential type");
  }
}


DOC(min, "Returns the smallest element of an aggregate type (string, array, "
	"or mapping).")

mixed min( mixed f ){
  if(stringp(f)) f = explode(f," ");
else
  if(mapp(f)) f = keys(f);

  return sort_array(f,1)[0];
}


DOC(max, "Returns the largest element of a structure that is a string, "
	"array or mapping.")

mixed max( mixed f ){
  if(stringp(f)) f = explode(f," ");
else
  if(mapp(f)) f = keys(f);
  
  return sort_array(f,-1)[0];
}




DOC(flatten_array, "Takes a array that may contain arrays, and reduces all "
    "arrays so that the result is a one dimensional array")


mixed
flatten_array(mixed arr)
{
    int i = 0;
    
    if (!arrayp(arr)) error("Bad argument 1 to flatten_array");
    arr = copy(arr);
    
    while (i < sizeof(arr)) {
	if (arrayp(arr[i])) {
	    arr[i..i] = arr[i];
	} else i++;
    }
    return arr;
}

DOC(call_out_chain, "Does a call_out to a list of functions, one following "
    "another, with each returning the delay till the next one is called.")

static void handle_chain(object ob, array funcs, array args) {
    int delay;
    if(!sizeof(funcs))
      return;
    if (stringp(funcs[0])) {
        delay = call_other(ob, funcs[0], args...);
    } else {
        delay = evaluate(funcs[0], args...);
    }
    call_out( (: handle_chain :), delay, ob, funcs[1..], args);
}

void call_out_chain(array funcs, int delay, array args...) {
    call_out( (: handle_chain :), delay, previous_object(), funcs, args);
}

mixed *my_call_outs()
{
  object p = previous_object();
  return filter_array(call_out_info(), (: $1[0] == $(p):));
}

// Beek fixed not to loop forever if exp < 0, etc
// also optimized extensively
int
pow(int num, int exp)
{
    int res;
    
    switch (exp) {
    case 0:
	if (!num) error("pow(0, 0) is undefined.\n");
	return 1;
    case 1:
	return num;
    case 2:
	return num * num;
    case 3:
	return num * num * num;

    case 4: 				// 2 multiplications, not 3
	num *= num;			// 1, 2, 4
	return num * num;

    case 5: 				// 3 multiplications, not 4
	exp = num * num; 		// 1, 2, 4, 5
	exp *= exp;
	return exp * num;

    case 6:				// 3 multiplications, not 5
	num *= num;			// 1, 2, 4, 6
	return num * num * num;

    case 7:				// 4 multiplications, not 6
	exp = num * num;		// 1, 2, 4, 6, 7
	exp *= exp * exp;
	return exp * num;
	
    case 8:				// 3 multiplications, not 7
	num *= num;			// 1, 2, 4, 8
	num *= num;
	return num * num;
	
    case 9:				// 4, not 8
	exp = num * num;
	exp *= exp;
	exp *= exp;
	return num * exp;

    case 10:				// 4, not 9
	num *= num;
	exp = num * num;
	exp *= exp;
	return exp * num;
	
    case 11:				// 5, not 10
	exp = num * num;
	res = exp * exp;
	res *= res;
	return res * num * exp;
	
    case 12:				// 4, not 11 ...
	num *= num;
	num *= num;
	exp = num * num;
	return exp * num;
	
    default:
	if (exp < 0)
	    return 0;
	// exp > 12 here ...
	switch (num) {
	case 0:
	    return 0;
	case 1:
	    return 1;
	case 2:
	    return 1 << exp;
	default:
	    // sub optimal, but pretty good
	    // O(log n) multiplications, not O(n)
	    res = 1;

	    while (exp) {
		if (exp & 1)
		    res *= num;
		num *= num;
		exp >>= 1;
	    }
	    return res;
	}
    }
}

int fuzzy_divide(int top, int bottom) {
    if (random(bottom) < (top % bottom))
	return top / bottom + 1;
    else
	return top / bottom;
}
