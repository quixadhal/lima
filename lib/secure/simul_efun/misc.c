/* Do not remove the headers from this file! see /USAGE for more info. */

#include <driver/type.h>

int is_directory(string);

// There should be an | operator for this.
DOC(clean_array, "returns a version of the passed array with duplicate "
    "entries removed.  Eg, clean_array(({1,2,2}))  => ({1,2})")

mixed*
clean_array(mixed* r){
    int i;
    i = sizeof(r);
    while(i--)
	if(member_array(r[i], r) != i) r[i] = sprintf("%c",255);
    return r-({sprintf("%c",255)});
}


DOC(exclude_array, "returns a copy of arr with the range "
    "of elements sliced out.  Ie, the size of the array shrinks.  Eg: "
    "exclude_array( ({0,1,2,3}),1,2 )  => ({0,3})")

mixed* exclude_array( mixed* r, int x, int y )
{
  mixed n;

  if(x) n = r[0..x-1];
  else n = ({});

  if(y+1 != sizeof(r))
    n += r[y+1..];

  return n;
}

DOC(cmp, "returns whether its two arguments are equivolent.  This is about"
"the same as using the equivolence operator (==), but will return true"
"in cases where == does not, such as when comparing 2 empty arrays. "
"Logically 2 empty arrays should be equivolent, but aren't with ==. "
"cmp knows they are.  This is mainly useful when you want to compare "
"mappings and arrays.")

int cmp( mixed a, mixed b )
{
  int		i;
  mixed		x;

  if( arrayp( a ) && arrayp( b ) )
  {

    if( sizeof(a) != ( i = sizeof(b) ) )
      return 0;

    while( i-- )
      if( a[i] != b[i] )
        return 0;
    return 1;
  }

  if( mapp( a ) && mapp( b ) )
  {

    if( sizeof(a) != (i = sizeof(b)) )
      return 0;

    x = keys( a );
    while( i-- )
      if( undefinedp(b[x[i]]) || !cmp(a[x[i]],b[x[i]]) )
	return 0;
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

  where--;
  if(where == -1) return to_insert+into_array;
  if( where < 0 || where >= sizeof( into_array ) )
	return (void)error("Insertion index out of bounds.");

  if( (where+1) == sizeof( into_array ) )
    return into_array + to_insert;

  return into_array[0..where] + to_insert + into_array[where+1..];
}

/* eval lets you evaluate a string as an expression.
   exec allows you to evaluate a string as LPC code. */
varargs mixed exec(string arg, string includefile)
{
  string	file = "/tmp/exec.c";
  object	tmp;
  
    if(is_directory(wiz_dir(this_user())))
      file = wiz_dir(this_user())+"/exec.c";
  if(tmp = find_object(file))
    destruct(tmp);

  rm(file);

  if(!stringp(arg))
      error("Bad type argument 1 to exec");

  if (strsrch(arg,";")==-1) arg = "return "+arg;

  if(includefile)
    write_file(file, sprintf("\n#include \"%s\"\n", includefile));
  write_file( file, sprintf( "mixed exec_foo(){ %s", arg ) );
  write_file( file, ";}\n");
  return file->exec_foo();
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
  int		i,j;
  mixed		targ;

  targ = ({});

  if( !arrayp( org ) ) error("Bad type arg to decompose");

  j = sizeof( org );
  for( ; i < j ; i++ )
    if( arrayp( org[i] ) )
       targ += org[i];
    else
       targ += ({ org[i] });

return targ;
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

// I stole this implementation straight out of some lisp I wrote, therefore
// the recursive implementation.  --John
mixed
flatten_array(mixed arr)
{
    if (!arrayp(arr)) error("Bad argument 1 to flatten_array");
    if (!sizeof(arr))
	return arr;
    if (arrayp(arr[0]))
	return sizeof(arr) > 1 ? flatten_array(arr[0]) +
	    	flatten_array(arr[1..<1]) : flatten_array(arr[0]);
    return  sizeof(arr) > 1 ? ({arr[0]}) + flatten_array(arr[1..<1])
                            : arr;
}








DOC(call_out_chain, "Does a call_out to a list of functions, one following "
    "another, with each returning the delay till the next one is called.")

void handle_chain(object ob, array funcs, array args) {
    int delay;
    if (stringp(funcs[0])) {
        delay = call_other(ob, funcs[0], args...);
    } else {
        delay = evaluate(funcs[0], args...);
    }
    call_out( (: handle_chain :), delay, ob, funcs[1..]);
}

void call_out_chain(array funcs, int delay, array args...) {
    call_out( (: handle_chain :), delay, previous_object(), funcs, args);
}
