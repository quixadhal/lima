/* Do not remove the headers from this file! see /USAGE for more info. */

string *regexplode(string str, string pat) {
    mixed *tmp;
    tmp = reg_assoc(str, ({ pat }), ({ 1 }) );
    return tmp[0];
}

string* split( string str, string pattern ){
  int i, j;
  mixed x,r;

  x = regexplode(str,pattern);
  j = sizeof(x);
  r = ({});
  for(i=0;i<j;i+=2) r+= ({ x[i] });
  return r;
}

int search( string s, string pat )
{
  mixed  m;
  m = regexplode(s,pat);
  if( sizeof(m) == 1 )  return -1;
  return strlen(m[0]);
}

int rsearch( string s, string pat )
{
  mixed m;
  m = regexplode(s,pat);
  if(sizeof(m) == 1)  return -1;
  return strlen( implode(m[0..<3],"") );
}

string sub( string s, string pat, string repl ){
  mixed x;

  x = regexplode( s, pat );
  if( sizeof(x) > 1 )
	x[1] = repl;

  return implode(x,"");
}

string gsub( string s, string pat, string repl ){
  
  return implode( split( s, pat ), repl );
}

varargs string insensitive_pattern(string pat)
{
  int i;
  int bracket;
  int patlen;
  string real_pat;

  patlen = strlen(pat);
  real_pat = "";
  for(i=0;i<patlen;i++)
    {
      if(pat[i] == '[') {
	bracket++;
	real_pat += "[";
      }
      else if (pat[i] == ']') {
	bracket--;
	real_pat += "]";
      }
      
      else if (pat[i] == '\\' && i+1 != patlen)
	{
	  real_pat += pat[i..i+1];
	  i++;
	  continue;
	}
      else if (pat[i] >= 'a' && pat[i] <= 'z' )
           if(!bracket)
		real_pat += sprintf("[%c%c]",pat[i], pat[i] - 32);
           else real_pat += sprintf("%c%c",pat[i], pat[i] - 32);
      else if (pat[i] >= 'A' && pat[i] <= 'Z' )
	  if(!bracket)
		real_pat += sprintf("[%c%c]",pat[i]+32,pat[i]);
	  else
		real_pat += sprintf("%c%c", pat[i]+32,pat[i]);
      else real_pat += pat[i..i];
    }
  return real_pat;
}

varargs string* insensitive_regexp(string* arr, string pat, int flag)
{
  return regexp(arr, insensitive_pattern(pat), flag);
}
