/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * Standard book, written by Tigran sometime in 1999.
 * This book makes the gross assumption that all pages are numbered, and in 
 * fact said page number is the name of the entry, rather than a synonym.  This
 * is important for sorting purposes and whatnot, and from what I can tell,
 * essential considering the nature of M_READABLE's entries. --Tigran
 */
inherit OBJ;
inherit M_OPENABLE;
inherit M_READABLE;
inherit M_GETTABLE;

void close_hook_func();
void open_hook_func();
string extra_long_hook_func();

string current_page;
string title;
string author;
nosave function open_hook=(:open_hook_func:);
nosave function close_hook=(:close_hook_func:);
nosave function extra_long_hook=(:extra_long_hook_func:);

private nomask mixed page_first()
{
  string array pages=sort_array(map(list_entries(),(:to_int:)),1);
  return sprintf("%i",pages[0]);
}

private nomask mixed page_before(string first)
{
  string array pages=sort_array(map(list_entries(),(:to_int:)),1);
  if(!query_entry(first))
    first=query_synonym(first);
  if(!query_entry(first))
    return 0;
  if(member_array(to_int(first),pages)-1<=0)
    return -1;
  return sprintf("%i",pages[member_array(to_int(first),pages)-1]);
}

private nomask mixed page_after(string first)
{
  string array pages=sort_array(map(list_entries(),(:to_int:)),1);
  if(!query_entry(first))
    first=query_synonym(first);
  if(!query_entry(first))
    return 0;
  if(member_array(to_int(first),pages)+1>=sizeof(pages))
    return -1;
  return sprintf("%i",pages[member_array(to_int(first),pages)+1]);
}

mixed direct_turn_str_wrd_obj(string str, string wrd, object ob)
{
  switch(PREPOSITION_D->translate_preposition(wrd))
    {
    case "of":
    case "in":
      break;
    default:
      return sprintf("#You cannot turn the %s '%s' %s.",
		     str,
		     wrd,
		     this_object()->short());
    }
  switch(str)
    {
      /* The easy stuff first */
    case "page forward":
    case "page forwards":
    case "page":
      {
	if(page_after(current_page)==-1)
	  return "#The book is already on the last page.";
	break;
      }
    case "page backwards":
    case "page backward":
      {
	if(page_before(current_page)==-1)
	  return "#The book is already on the first page.";
	break;
      }
    default:
      {
	string page;
	if(sscanf(str,"to page %s",page)!=1)
	  return 0;
	if(!query_entry(page))
	  page=query_synonym(page);
	if(!query_entry(page))
	  return "#That page does not exist.";
      }
    }
  return 1;
}

mixed direct_turn_str_wrd_obj_wrd(string str,string wrd1, object ob,string wrd2)
{
  return direct_turn_str_wrd_obj(sprintf("%s %s",str,wrd2),wrd1,ob);
}

mixed direct_turn_obj_str(object obj,string str)
{
  return direct_turn_str_wrd_obj(str,"in",obj);
}

mixed direct_read_obj(object ob)
{
  object o=owner(this_object());
  
  if(o && o != this_body())
    return sprintf("#Try asking %s nicely if you can read it.\n",o->query_name());
  return 1;
}

varargs void do_turn(mixed str, mixed wrd,string wrd2)
{
  if(wrd2)
    wrd=sprintf("%s %s",wrd,wrd2);
  if(!wrd)
    wrd="in";
  switch(str)
    {
    case "page forward":
    case "page forwards":
    case "page":
      {
	current_page=page_after(current_page);
	this_body()->targetted_action("$N $vturn the page in $td.",this_object());
	break;
      }
    case "page backward":
    case "page backwards":
      {
	current_page=page_before(current_page);
	this_body()->targetted_action("$N $vturn the page in $td.",this_object());
	break;
      }
    default:
      {
	string page;
	sscanf(str,"to page %s",page);
	if(!query_entry(page))
	  page=query_synonym(page);
	current_page=page;
	this_body()->targetted_action("$N $vturns to a page in $td.",this_object());
      }
    }
}
	  
/* This function merely returns the last used page of the book */
private nomask string query_last_page()
{
  return sort_array(list_entries(),1)[<1];
}

//:FUNCTION set_pages
//Set the pages that are in the book.  Each element of the array is the text
//that appears on the page.  The elements can be either the text that is on the
//page, a filename, or a function pointer that generates one of these.
void set_pages(mixed array page_data...)
{
  int counter=0;
  clear_entries();
  foreach(string page in flatten_array(page_data))
    {
      counter++;
      add_entry(sprintf("%i",counter),page);
    }
}

//:FUNCTION add_pages
//Add pages to the existing pages of the book.
//See set_pages()
void add_pages(string array page_data...)
{
  int counter=to_int(query_last_page());
  foreach(string page in flatten_array(page_data))
    {
      counter++;
      add_entry(sprintf("%i",counter),page);
    }
}

//:FUNCTION remove_pages
//Remove pages from the existing pages of the book.
//The arguments passed to remove_pages() are the page numbers to be removed.
//See set_pages()
void remove_pages(mixed array pagenum...)
{
  foreach(mixed page in flatten_array(pagenum))
    {
      if(intp(page))
	page=sprintf("%i",page);
      if(!query_entry(page)||query_entry(query_synonym(page)))
	return;
      remove_entry(page);
    }
}

//:FUNCTION list_pages
//Return all of the pages in the book in an array.
string array list_pages()
{
  return list_entries();
}

//:FUNCTION query_page
//Return the contents of the page specified.
string query_page(int pagenum)
{
  return query_entry(sprintf("%i",pagenum));
}

//:FUNCTION set_contents
//Set the contents of the book.  The contents are a mapping with the section
//heading as the key and the page it starts with as the value (a string).
void set_contents(mapping new_contents)
{
  set_synonyms(new_contents);
}

//:FUNCTION add_contents
//Add additional contents to the existing contents.
//See set_contents
void add_contents(mapping additions)
{
  foreach(mixed key,mixed value in additions)
    add_synonym(key,value);
}

//:FUNCTION remove_contents
//Remove contents from the existing contents
//See set_contents()
void remove_contents(string array headings...)
{
  foreach(string heading in flatten_array(headings))
    remove_synonym(headings);
}

//:FUNCTION query_content_page
//Return the page number of the contents heading
string query_content_page(string heading)
{
  return query_synonym(heading);
}

//:FUNCTION list_contents
//Return the mapping of contents
mapping list_contents()
{
  return dump_synonyms();
}

//:FUNCTION set_title
//Set the title of the book.
void set_title(string new_title)
{
  title=new_title;
  add_id(title);
}

//:FUNCTION query_title
//Return the title of the book.
string query_title()
{
  return title;
}

//:FUNCTION set_author
//Set the author of the book
void set_author(string who)
{
  author=who;
}

//:FUNCTION query_author
//Return the author of the book
string query_author()
{
  return author;
}

//:FUNCTION is_book
//Return whether or not the object is a book.
int is_book()
{
  return 1;
}

void open_hook_func()
{
  current_page=page_first();
}  

void close_hook_func()
{
  current_page=0;
}

string extra_long_hook_func()
{
  string ret="";
  string auth=query_author();
  string tit=query_title();
  if(query_closed())
    {
      if(title)
	{
	  if(auth)
	    ret=sprintf("'%s' written by %s.  ",
			tit,
			auth);
	  else
	    ret=tit;
	}
    }
  else
    {
      ret=sprintf("It is turned to a page marked '%s'.  ",
		  current_page);
    }
  return ret;
}

string a_short()
{
  return sprintf("%s titled '%s'",
		 ::a_short(),
		 query_title());
}

string read()
{
  return read_entry(current_page);
}

void mudlib_setup()
{
  ::mudlib_setup();
  set_id( "book" );
  set_size( SMALL );
  add_hook("open",open_hook);
  add_hook("close",close_hook);
  add_hook("extra_long",extra_long_hook);
}
