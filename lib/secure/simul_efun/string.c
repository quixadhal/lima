/* Do not remove the headers from this file! see /USAGE for more info. */

string chr( int i );

static private string ascii = "";
static private string whitespace = " \t\n\r";
static private string lowercase = "abcdefghijklmnopqrstuvwxyz";
static private string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


static private string _lower, _upper, _swapcase;

static void create()
{
  int i;

  for(i=0; i < 256; i++){
     if(i==0)ascii+=" ";
	ascii += chr(i);
  }

  _lower = ascii[0..('A'-1)] + lowercase + ascii[('Z'+1)..]; 
  _upper = ascii[0..('a'-1)] + uppercase + ascii[('z'+1)..];
  _swapcase = _upper[0..'A'-1] + lowercase + _upper['Z'+1..];
}

private int strstr(string s, string lookfor, int start)
{
    int x;

    x = strsrch(s[start..], lookfor);
    if ( x == -1 )
	return -1;
    return start + x;
}


#define DEFAULT_WIDTH	75
#define DEFAULT_INDENT	4

//:FUNCTION wrap
// Wrap function from Jihad @ Lima Bean.  Takes the string to wrap,
// and optionally a width to wrap it to and optionally an indent for
// any line after the first.
varargs string wrap(string str, int width, int indent)
{

    if ( !indent )
        return sprintf("%-=*s", width || DEFAULT_WIDTH, str);

    return sprintf(
	"%s%-=*s",
	str[0..indent - 1],
	(width || DEFAULT_WIDTH) - indent,
	str[indent..]
        );
}

string iwrap(string str)
{
    return wrap(str, 0, DEFAULT_INDENT);
}

#ifdef TRYING_NEW_VERSION

// iwrap.c  (wrap with indent)
// Peregrin@ZorkMUD           
// Jan '94
 
#define PAD      4
#define COLS    75
#define MINCOLS 40
#define MAXCOLS 79
 
varargs string iwrap(string str, int pad, int cols) {
    int    i, j, indent, len, numwords, numlines;
    string *words, *lines;
    string new_str, temp, padstr;
    int pre, post;
 


if( !stringp(str) ) 
    return str;
// initialise vars for routine...
    if (!pad || pad < 1)  pad = PAD;
    if (!cols || cols < MINCOLS || cols > MAXCOLS)   cols = COLS;
    padstr = "";
    i = pad;
    while (i--)
        padstr += " ";
    new_str = "";
    indent = 0;  j = 0;
// fix for MudOS explode() - with one of the switches this doesn't work
    while (pre < strlen(str) && str[pre]=='\n') pre++;
    while (pre + post < strlen(str) && str[<post + 1]=='\n') post++;
    str = str[pre..<post+1];
    
    lines = explode(str,"\n");
    numlines = sizeof(lines);
 
// wrap with indent:
  while (j < numlines ) {
    if (j==numlines-1 && lines[j]=="") {
        j++;
        new_str += "\n";
        continue;
    }
    if (indent)  new_str += "\n"+padstr;
    words = explode(lines[j]," ");
    numwords = sizeof(words);
    i = 0;  len = 0;
    while (i < numwords ) {
       if (words[i] && (indent + len + strlen(words[i]) > cols)) {
            temp = words[i];
            if (strlen(temp) > cols) {
                while ((indent + len + strlen(temp)) > cols) {
                    new_str += (temp[0..(cols-len-indent)] + "\n");
                    temp = temp[cols-len-indent..];
                    len = 0;
                    indent = pad;
                }
            }
            new_str += "\n" + padstr;
            new_str += (temp + " ");
            len = strlen(temp) + 1;
            indent = pad;
        } else {
            if (i==numwords-1 && words[i]=="") {
                i++;
                continue;
            }
            new_str += words[i]+" ";
            len += strlen(words[i]) + 1;
        }
        i++;
    }
    indent = pad;
    j++;
  }
    // MudOS explode again
    while (pre--) new_str = "\n" + new_str;
    while (post--) new_str += "\n";
  return new_str;
}
 

// Modified by Beek so that it behave correctly on text that had
// already been wrapped.
varargs
string
wrap( string arg, int width )
{
    int pos, next;
    int space;

    if( !width )
	width = 75;

    pos = 0;
    while (pos < strlen(arg) ) {
	next = strstr(arg, "\n", pos);
        if (next == -1) next = strlen(arg) + 1;
        if (next - pos > width) {
	    space = pos + width;
	    while (space > pos + width-15 && arg[space] != ' ') space--;
	    arg[space]='\n';
	    pos = space+1;
	} else {
	    pos = next + 1;
	}
    }
    return arg;
}

#endif /* TRYING_NEW_VERSION */


string trim_spaces( string s ){
  int i, j;

  j = strlen(s) - 1;
  if ( j < 0 )
      return "";

  while( i <= j && member_array(s[i], whitespace) != -1 ) i++;
  while( i <= j && member_array(s[j], whitespace) != -1 ) j--;
  return s[i..j];
}

string expand_range(string str)
{
  string ret;
  mixed  tmp;
  int	 i,x,y;

  if(!stringp(str))
    return 0;
  ret = "";
  str=implode(explode(str," "),"");
  tmp = explode( str, "," );
  i = sizeof( tmp );

  while( i-- )
    if( sscanf( tmp[i], "%d-%d", x, y ) == 2 )
      while( x <= y )
	ret = set_bit( ret, x ), x++;
    else
	ret = set_bit( ret, to_int(tmp[i]) );
  return ret;
}


// Rust


string chr( int i )
{
  return sprintf("%c",i);
}

int ord( string x )
{
  if( strlen( x ) != 1 )
	error("bad type arg to simul_efun ord()");
  return x[0];
}

string 
swapcase( string s ){
  return map(s, (: _swapcase[$1] :) );
}


varargs int find( string s, string sub, int i ){
	if( i < 0 ) i += strlen(s);
	return strstr( s, sub, i );
}

varargs int rfind( string s, string sub, int i ){
  int j, k;
  
  j = -1;
  k = strlen(s);

  if(i<0) i+=k;
  while((i<k)&&(i=strstr(s,sub,i))!=-1)
  {
	j=i;
	i++;
  }

  return j;
}


varargs
int count_substrings( string s, string sub, int i ){

  int n, m, r;

  if(i<0) i+= strlen(s);
  n = strlen(sub);
  m = strlen(s) + 1 - n;
  if( !n ) return m-i;
  n--;

  r = 0;
  while( i<m ){
    if( sub == s[i..(i+n)]){
	r++;
	i+=n;
    }
    else{
	i++;
    }
  }
  return r;
}

string power_str( string s, int t )
{
	return repeat_string(s,t);
}

string ljust( string s, int width )
{
  int n;

  n = width - strlen(s);
  if( n <= 0 ) return s;

  return s + power_str(" ",n);
}


string rjust( string s, int width )
{
  int n;

   n = width - strlen(s);
  if(n<=0) return s;
  return power_str(" ",n) + s;
}

string center( string s, int width )
{
  int n, half;

  n = width - strlen(s);
  if( n <= 0 ) return s;
  half = n>>1;
  if( n%2&&width%2) half++;
  return power_str(" ",half) + s + power_str(" ",n-half);
}

// Note that truncate adds an elipsis ("...") to the end if it 
// truncates.
string truncate( string s, int length )
{
  length -= 3;
  if(length < 1)
	{
		error("truncate:  value too low");
	}
  if(strlen(s) <= length)
	return s;
  return s[0..length-1] + "...";
}

int is_letter(int c)
{
  return strsrch(lowercase+uppercase,c) != -1;
}

string format_list(string array list, string separator) {
    if (!separator) separator = "and";
    if (sizeof(list)==0) return "";
    if (sizeof(list)==1) return list[0];
    if (sizeof(list)==2) return list[0] + " " + separator + " " + list[1];
    return implode(list[0..<2], ", ") + ", " + separator + " " + list[<1];
}
