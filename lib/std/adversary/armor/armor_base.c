/* Do not remove the headers from this file! see /USAGE for more info. */

void set_wearing(string s)
{
   object ob = new(s);
   if(!ob)
      return;

   ob->move(this_object());
   ob->do_wear();
}
