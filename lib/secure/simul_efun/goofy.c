/* Do not remove the headers from this file! see /USAGE for more info. */


private mapping subs = ([
"a" : "4",
"d": "|)",
"f": "ph",
"e" : "3",
"i" : "1",
"k" : "|<",
"l" : "|",
"m" : "/\\/\\",
"o" : "0",
"s" : "$",
"t" : "+",
"w" : "\\/\\/",
"x" : "><"
]);

  string hax0r_letters(string s) {
    string array r = explode(s,"");
    string result = "";
    foreach(string item in r)
      result += subs[item] ? subs[item] : item;
    return result;
}


string hax0r(string s)
{
  string array words;


  words = explode(s, " ");
  for(int i=0;i<sizeof(words);i++)
  {
     switch(lower_case(words[i])) {
      case "i": words[i] = "3y3"; break;
      case "you": words[i] = "j00"; break;
      case "dude": words[i] = "d00d"; break;         
      case "elite": words[i] = "3133+"; break;         
      case "are": words[i] = "R"; break;         
      case "hack": words[i] = "hax0r"; break;
      case "hacker": words[i] = "hax0r"; break;
      case "i'm" : words[i] = "3y3'/\\/\\"; break;
      case "i'll" : words[i] = "3y3'77"; break;
      case "see": words[i] = "C"; break;
       case "oh": words[i] = "O"; break;
      case "sucks": words[i] = "$ux"; break;
      case "off": words[i] = "0ph"; break;
      case "too": words[i] = "2"; break;
      case "to": words[i] = "2"; break;
      case "for": words[i] = "4"; break;
      default:         
        words[i] = hax0r_letters(words[i]);         
    }
    }
  s = implode(words, " ");

     return s;
}

