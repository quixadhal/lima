/* Do not remove the headers from this file! see /USAGE for more info. */

/* Wraith@Lima Bean / Hatter@Wonderland
   infodawn@lima.mudlib.org

   Module for objects that can be searched productively.
*/

int searched, once, need_spec_item;
string *with, *item;

//:FUNCTION searchable_once
//Set whether or not (0 or 1) this object can be searched more than once per
//reset.
void searchable_once(int i) { once = i; }
int query_search_once() { return once; }

//:FUNCTION searchable_with
//Give an array of filenames that can be used to search this object.
void searchable_with(string *filenames) { with = filenames; }
string *query_searchable_with() { return with; }

//:FUNCTION searchable_items
//The items that can be searched for in this object.
void searchable_items(string *items) { item = items; }
string *query_searchable_items() { return item; }

void set_searched(int i) { searched = i; }
int query_searched() { return searched; }

//:FUNCTION need_specific_item
//Set whether or not you need to give a specific item to search for
//in this object, this should be used only when searchable_items() is used.
void need_specific_item(int i) { need_spec_item = i; }
int query_need_specific_item() { return need_spec_item; }

int can_search_me(string sfor, object owith) {
  int bad;

  if (once && searched) bad += 1;
  if (need_spec_item && sizeof(item) && member_array(sfor, item) != -1) bad += 1;
  if (sizeof(with) && member_array(file_name(owith), with) != -1) bad += 1;
  return !bad;
}

void reset() { searched = 0; }
