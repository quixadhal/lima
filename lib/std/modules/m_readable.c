/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** From OBJ::description
*/
string the_short();


mapping         entries;
mapping         synonyms;
private string  read_text;

/* Parser interaction */
mixed direct_read_obj(object ob) {
  object o;
  
  o = owner(this_object());
  if (o && o != this_body())
    return "#Try asking " + o->query_name() + " nicely if you can read it.\n";
  
  if (!read_text) {
    if (entries)
      return "It has a number of entries about various topics.\n";
    return "It appears to be blank.\n";
  }
  return 1;
}


mixed direct_read_word_obj(string p, object ob) {
  return direct_read_obj(ob);
}

mixed direct_read_str_word_obj(string str, string p, object ob) {
  object o;
  
  o = owner(this_object());
  if (o && o != this_body())
    return "#Try asking " + o->query_name() + " nicely if you can read it.\n";
  if (!entries) {
    if (read_text)
      return "It's fairly short.  Just read it all.\n";
    return 0;
  }
  return 1;
}

mixed direct_read_word_str_word_obj(string p1, string str, string p2,
				    object ob) {
  return direct_read_str_word_obj(str, p2, ob);
}

//:FUNCTION set_text
//Set the text for an object to be read.
//The argument can either be the actual text that will be used or a filename.
//If it is a filename, either absolute, or relative paths may be used.
void set_text( string t )
{
  string file=absolute_path(t,this_object());
  if (file_size(file)>=0)
    read_text = read_file(file);
  else
    read_text = t;
}

//:FUNCTION query_text
//Return the string that is the text that will be read
//See set_text()
mixed query_text()
{
  return read_text;
}


string read()
{
  this_body()->simple_action("$N $vread the $o.", this_object());
  return read_text;
}

string read_entry( string entry )
{
  string filename;
  string entry_contents = entries[entry];
  if(!entry_contents)
    entry_contents = entries[synonyms[entry]];
  if(!entry_contents)
    return("Nothing there\n");
  if (functionp(entry_contents))
    {
      this_body()->simple_action("$N $vread the $o.", this_object());
      return evaluate(entry_contents);
    }
  filename=absolute_path(entry_contents,this_object());
  if (stringp(entry_contents) && file_size(filename) > 0)
    {
      this_body()->simple_action("$N $vread the $o.", this_object());
      return read_file(filename);
    }
  if(stringp(entry_contents))
    {
      this_body()->simple_action("$N $vread the $o.", this_object());
      return entry_contents;
    }
  return 0;
}

//:FUNCTION has_entries
//Boolean function which returns 1 if there are entries to be read on the 
//object, otherwise 0.
//See set_entries()
int has_entries()
{
  return mapp( entries );
}

//:FUNCTION set_entries
//Set the entries for the object.
//This function takes a mapping as argument.  The keys are the names of the
//entries and the values are either a filename which can be absolute or
//relative, a function pointer which will be resolved into a text string, or
//a simple string.
void set_entries(mapping e)
{
  entries = e;
}

//:FUNCTION add_entry
//Add an additional entry that can be read to the already existing entries.
//The first argument is the name of the entry and the second is the contents
//of the entry.
//See set_entries()
void add_entry(mixed name,mixed contents)
{
  entries+=([name:contents]);
}

//:FUNCTION remove_entry
//Remove a readable entry from the object.
//The string argument is the name of the entry to be removed.  It can be
//either the name of the entry or one of its synonyms and all synonyms will
//also be removed.
//See set_entry()
void remove_entry(string entry)
{
  string real_name;
  if(member_array(entry,keys(entries))>-1)
    real_name=entry;
  else if(member_array(entry,keys(synonyms))>-1)
    real_name=synonyms[entry];
  else
    return;
  map_delete(entries,real_name);
  foreach(string syn,string value in synonyms)
    {
      if(value==entry)
	map_delete(synonyms,syn);
    }
}

//:FUNCTION clear_entries
//Clear all of the entires.  Both pages and synonyms.
void clear_entries()
{
  synonyms=([]);
  entries=([]);
}

//:FUNCTION query_entry
//Query the contents of an entry. 
//The returned value is not evaluated.
//The string argument can be the actual entry name or a synonym of it.
mixed query_entry(string entry)
{
  string real_name;
  if(member_array(entry,keys(entries))>-1)
    real_name=entry;
  else if(member_array(entry,keys(synonyms))>-1)
    real_name=synonyms[entry];
  else
    return;
  return entries[real_name];
}

//:FUNCTION list_entries
//A list of all the readable entries on the object
string array list_entries()
{
  return keys(entries);
}

//:FUNCTION dump_entries
//Raw dump of all the entry data.
mapping dump_entries()
{
  return entries;
}

//:FUNCTION set_synonyms
//Set the synonyms for the readable entries.
//A mapping is passed as argument.  The key is the synonym, and the value is 
//the entry.
void set_synonyms(mapping s)
{
  synonyms=([]);
  foreach(string syn,string entry in s)
  {
    if(!query_entry(entry))
      error(sprintf("Bad value '%s' in mapping for set_entry_synonyms.\n"
        "Entry must exist",
        entry));
    synonyms+=([syn:entry]);
  }
}

//:FUNCTION set_entry_synonyms
//Set the synonyms for the readable entries.
//A mapping is passed as argument.  The key is the synonym, and the value is 
//the entry.
void set_entry_synonyms(mapping s)
{
  set_synonyms(s);
}

//:FUNCTION add_synonym
//Add an additional synonym 
//The first argument is the synonym and the second argument is the entry to 
//which it refers
void add_synonym(string syn,string entry)
{
  if(!query_entry(entry))
    error(sprintf("Bad argument 2 '%s' to add_synonym.\n"
		  "Entry must exist.",
		  entry));
  synonyms+=([syn:entry]);
}

//:FUNCTION remove_synonym
//Removes a synonym of an entry
void remove_synonym(string syn)
{
  map_delete(synonyms,syn);
}

//:FUNCTION query_synonym
//Return the entry that the synonym refers to
string query_synonym(string syn)
{
  return synonyms[syn];
}

//:FUNCTION dump_synonyms
//Return the mapping of all synonyms
mapping dump_synonyms()
{
  return synonyms;
}

mapping lpscript_attributes() {
    return ([
        "text" : ({ LPSCRIPT_STRING, "setup", "set_text" }),
    ]);
}

