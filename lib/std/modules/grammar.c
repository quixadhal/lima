/* Do not remove the headers from this file! see /USAGE for more info. */

/* Note: this is now a wrapper for the efun, allowing "on the fly"
 * modification */
#define VOWELS ({'a', 'e', 'i', 'o', 'u'})

DOC(pluralize,"convert a noun to it's plural form.  Oddly enough, it's also quite"
    "good at converting verbs to their singular form, since going from plural to "
    "singular often entails adding an 's'.  Eg, they run, he runs.")

string pluralize(string str)
{
    if ( str[<2..<1] == "ff" )
	return str + "s";

    // nouns ending in a consonant plus y form the plural by changing:
    // *y -> *ies (gumby -> gumbies)
    if (str[<1] == 'y' && member_array(str[<2], "aeiou") == -1)
	return str[0..<2]+"ies";

    switch (str)
    {
    case "were": return "was"; // Later drivers have this one
    case "have": return "has";
    case "are": return "is";
    case "laf": return "lafs";
    case "have": return "has";
    default:
	return efun::pluralize(str);
    }
}	

// punctuate Written by Rust.
DOC(punctate,"adds a period to a sentence if needed.")
string punctuate( string str )
{
    int		last_char;

    if(!stringp(str) || !strlen(str))
	return "";

    while (str[<1] == ' ') str = str[0..<2];
    last_char = str[<1];
    if( ( last_char >= 'a' && last_char <= 'z' ) ||
      ( last_char >= 'A' && last_char <= 'Z' ) )
	return str+".";

    return str;
}

string number_of(int num, string what) {
    if (num == 1) return "1 " + what;
    return num + " " + pluralize(what);
}

