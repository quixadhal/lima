/* Do not remove the headers from this file! see /USAGE for more info. */

// Suggestion is to rewrite this with reg_assoc()
// $Id: pluralize.c,v 1.2 1994/02/12 11:29:54 vax Exp $
// purpose: this file does the brunt of the pluralization work.
/*
 * $Author: vax $
 * $Date: 1994/02/12 11:29:54 $
 * $Revision: 1.2 $
 * $Log: pluralize.c,v $
 * Revision 1.2  1994/02/12  11:29:54  vax
 * Changed this to use un_article (new simul_efun).
 *
 * Revision 1.1  1994/02/12  11:28:41  vax
 * Initial revision
 *
 */

/* old stuff:
 * author:  Pinkfish
 * history: written by pinkfish one day, and modified for the TMI
 *  distribution mudlib by Buddha on 3-14-92.  No changes were needed. ;-)
 * Changelog:
 *      7 Nov 1993 by VaX#n8
 *              o General revamp
 *      18 Jan 1994 VaX#n8 (vax)
 *              o Special cases for "datum" and "sphinx" added.
 *      27 Jan 1994 VaX#n8 (vax)
 *              o Rule for '*o' -> '*oes' (mosquito -> mosquitoes) fixed.

This has been messed with a few times by Zork/GUE/LIMA/... people.  
None of the simul_efuns mentioned in comments here are simul_efuns.
 * This is obsolete:
 * NB: it is always preferable for objects to set their plural using
 *     "set_plural()" (on std/object), but if the relevant programmer hasn't
 *     done so, "query_plural()" (also on std/object) calls this efun for
 *     a hopefully reasonable approximate.  objects should _always_ call
 *     call "query_plural()" to get the plural form of an object's "short()",
 *     failure to do so is treason, and subject to punishment by summary
 *     execution.  The Computer is your friend.
 */

#define VOWELS ({'a', 'e', 'i', 'o', 'u'})

DOC(pluralize,"convert a noun to it's plural form.  Oddly enough, it's also quite"
    "good at converting verbs to their singular form, since going from plural to "
    "singular often entails adding an 's'.  Eg, they run, he runs.")

string pluralize(string str)
{
    string pre, suf;
    mixed rel, two;
    int one;

    if (!stringp(str) || (strlen(str) <= 1)) return "";

    // first, get rid of articles.  pluralized forms never have them ;)
    str = un_article(str);

    // Deal with "sword of power" -> "swords of power" types (Watcher 09/92)
    if (sscanf(str, "%s of %s", pre, suf) == 2)
	return pluralize(pre) + " of " + suf;

    // only pluralize the last word, ie: lose adjectives.
    rel = explode(str, " ");
    if (!rel) { pre = ""; rel = str; }
    else
    {
	pre = implode(rel[0..<2], " ");
	if (sizeof(rel) > 1) pre += " ";
	rel = rel[sizeof(rel)-1];
    }

    // trap the exceptions to the rules below and special cases.
    switch (rel)
    {
    case "fish" :
    case "deer" :
    case "sheep" :
	return str;
    case "child" :
	return str + "ren";
    case "tooth" :
	return pre + "teeth";
    case "ox" :
    case "vax" :
	return str + "en";
    case "bus" :
	return str + "es";
    case "mouse" :
	return pre + "mice";
    case "goose" :
	return pre + "geese";
    case "cliff" :
    case "fife" :
    case "dynamo" :
	return str + "s";
    case "laf" :
        return "lafs";
    case "datum" : return "data";
    case "sphinx" : return "sphinges";
    case "are" : return "is";
    case "have" : return "has";
    case "do" : return "does";
    case "go" : return "goes";
    }

    one = rel[<1];
    two = rel[<2..<1];

    // now handle "rules" ... god I hate english!!

    // Latin pluralization, I think
    // *us -> *i (virus -> viri), also rad
    if (two == "us") return str[0..<3]
	+"i";
    // nouns ending in s, z, x, ch, and sh form plural by adding es.
    // ex: business, six, church, wish, quizz
    if ((one == 's') || (one == 'x') || (one == 'z') ||
      (two == "ch") || (two == "sh"))
	return str + "es";

    // *fe -> *ves (knife -> knives)
    // exception: fife (above)
    if (two == "fe") return str[0..<3] + "ves";

    if (two == "ff") return str + "s";
    /*
     * *f -> *ves (half -> halves)
     * *ef -> *efs (chef -> chefs) (really a rule for a special case)
     */
    // exception: cliff
    if (one == 'f')
    {
	if (two == "ef") return str + "s";
	return str[0..<2]+"ves";
    }

    // nouns ending in a consonant plus y form the plural by changing:
    // *y -> *ies (gumby -> gumbies)
    if (one == 'y' && (member_array(rel[strlen(rel)-2], VOWELS) == -1))
	return str[0..<2]+"ies";

    // *man -> *men (foreman -> foremen)
    if (rel[<3..<1] == "man") return str[0..<4] + "men";

    // *is -> *es (this is from gordons pluralize ... )
    // if (two == "is") return str[0..<3] + "es";

    // *o -> *s (also from gordon)
    if (one == 'o') return str[0..<2] + "es";

    // default, * -> *s
    return str + "s";
}

/* end of pluralize() */

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
