/* Do not remove the headers from this file! see /USAGE for more info. */

// m_lightable - Handles verb interaction for lightable objects.
// 26 June, 1998: Iizuka seperated into m_lightable and m_light_source.

#include <flags.h>
#include <hooks.h>

string lighting_extra_short();
void set_light(int);
mixed call_hooks(string, int);
void hook_state(string, mixed, int);
string the_short();
int test_flag(int);
void clear_flag(int);
void assign_flag(int, int);

varargs mixed light(object);   // From m_light_source.c
mixed extinguish();

private string		light_msg = "$N $vlight a $o.";
private string		light_with_msg = "$N $vlight a $o with a $o1.";
private string		extinguish_msg = "$N $vextinguish a $o.";
private nosave function light_hook = (: lighting_extra_short :);
private mixed           source_filter;

int query_is_lit()
{
   return test_flag(F_LIGHTED);
}

void set_extinguish_msg(string x)
{
   extinguish_msg = x;
}

void set_light_msgs(string x, string y)
{
   light_msg = x;
   light_with_msg = y;
}

//:FUNC set_source
//set_source(function f) makes it so that the object can only be lit by
//objects for which evaluate(f, ob) returns 1.  f may return a string
//error message on failure.  If f is a string, any object with that string
//as an id is a valid source.  set_source(0) makes any object a valid source.
void set_source(mixed f)
{
   source_filter = f;
}

mixed query_source()
{
   return source_filter;
}

string lighted_attributes()
{
   return "(providing adequate light)";
}

void set_lit(int x)
{
   assign_flag(F_LIGHTED, x);
   hook_state("extra_short", light_hook, x);
}

void do_extinguish()
{
   mixed tmp = extinguish();
   if(!tmp)
      tmp = "That doesn't seem possible.\n";
   if(stringp(tmp))
   {
      write(tmp);
      return;
   }
   clear_flag(F_LIGHTED);
   this_body()->simple_action(extinguish_msg, this_object());
}

varargs void do_light(object with)
{
   mixed tmp = light(with);
   if(!tmp) tmp = "That doesn't seem possible.\n";
   if(stringp(tmp))
   {
      write(tmp);
      return;
   }
   if(with)
      this_body()->simple_action(light_with_msg, this_object(), with);
   else
      this_body()->simple_action(light_msg, this_object());
}

mixed direct_light_obj()
{
   if(query_is_lit())
      return "It is already lit.\n";
   if(source_filter)
      return "You need to light it with something.\n";
   return 1;
}

mixed direct_extinguish_obj()
{
   if(!query_is_lit())
      return "It isn't lit.\n";
   return 1;
}

mixed direct_light_obj_with_obj(object ob, object with) {
   if(query_is_lit())
      return "It is already lit.\n";
   if(!source_filter)
      return 1;
   if(!with)
      return 1;
   if(stringp(source_filter))
      return with->id(source_filter);
   return evaluate(source_filter, with);
}

mixed indirect_light_obj_with_obj(object ob, object with) {
   mixed sf;
   if(ob)
      sf = ob->query_source();
   if(with->query_is_lit())
   {
      if(!sf)
         return 1;
      if(stringp(sf))
         return with->id(sf);
      return evaluate(sf, with);
   }
   return capitalize(with->the_short()) + " isn't lit.\n";
}

int need_to_see()
{
   return 0;
}

mapping lpscript_attributes()
{
   return ([
      "light_msgs" : ({ LPSCRIPT_SPECIAL, (: ({ "special", "set_light_msgs(\"" + $1[0] + "\", \"" + (sizeof($1) > 1 ? $1[1] : $1[0]) + "\")" }) :) }),
      "fuel" : ({ LPSCRIPT_INT, "setup", "set_fuel" }),
   ]);
}
