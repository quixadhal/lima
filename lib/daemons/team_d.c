/* Do not remove the headers from this file! see /USAGE for more info. */

// TEAM_D - Daemon for storing groups of players on the mud and
//          awarding experience proportionally to those players
//          if experience is used.
// March 1, 1997: Iizuka@Lima Bean created.

#ifdef PARTIES_SAVE
inherit M_DAEMON_DATA;
#else
#define save_me()
#define restore_me()
#endif

class team_member
{
   int shares;
   int access;      // 1 is highest. >1 is lower.
}

class team
{
   string  name;
   mapping members;
   string  password;
   int     min_level;
   int     max_level;
   int     max_active;
}

private void fix_shares(class team);

private int DIFF = 5;
private mapping parties = ([ ]);

nomask void fix_shares(class team team_data)
{
#if 0
   int total_shares;
   class team_member array members = values(team_data->members);

   foreach(class team_member member in members)
   {
      total_shares += member->shares;
   }
#endif
}

nomask int register_team(string name, string owner, string password)
{
   class team new_team = new(class team);
   class team_member new_owner = new(class team_member);

   if(parties[name])
      return 0;

   // When we first create the team, the creator
   // has 100% of the experience shares and has
   // the highest access level.
   new_owner->shares         = 100;
   new_owner->access         = 1;

   new_team->name           = name;
   (class team_member)new_team->members = ([ owner : new_owner ]);
   ((class team_member)(new_team->members[owner]))->shares = 100;
   new_team->password       = password;
   parties += ([ lower_case(name) : new_team ]);
   save_me();
   return 1;
}

private int filter_shares(class team_member m, mixed extra)
{
   return m->shares;
}

nomask int award_experience(string name, int amount)
{
   class team tmp = parties[lower_case(name)];
   string array team_members = keys(tmp->members);
   int x = 0, total = 0;
   int array shares = filter_array(values(tmp->members), (: filter_shares :));

   foreach(x in shares)
      total += x;

   if(total != 100)
   {
      fix_shares(tmp);
      x = total = 0;
      foreach(x in shares)
         total += x;
      if(total != 100)
         return 0;         // ERROR
   }

   foreach(string s in team_members)
      find_body(s)->add_experience(amount *((class team_member)tmp->members)->shares * 0.01);

   return 1;
}

nomask int add_member(string new_member, string pname, string password)
{
   object body = find_body(lower_case(new_member));
   class team_member member = new(class team_member);

   pname = lower_case(pname);
   if(password != ((class team)parties[pname])->password)
      return 0;

   // If the difference in levels between the new member
   // and any existing member of the team is greater
   // than DIFF, do not add the new member.
   if(body->query_level() > ((class team)parties[pname])->min_level + DIFF
      || body->query_level() < ((class team)parties[pname])->max_level - DIFF)
         return 0;

   // Update the new minimum and maximum levels for the team if needed.
   if(body->query_level() > ((class team)parties[pname])->max_level)
      ((class team)parties[pname])->max_level = body->query_level();
   if(body->query_level() < ((class team)parties[pname])->min_level)
      ((class team)parties[pname])->min_level = body->query_level();

   member->shares = 0;
   member->access = sizeof(((class team)parties[pname])->members) + 1;
   ((class team)parties[pname])->members[new_member] = member;
   fix_shares(parties[pname]);

   save_me();
   return 1; // Member has been added
}

nomask int remove_member(string member, string pname)
{
   pname = lower_case(pname);
   if(!((class team)parties[pname])->members[member])
   {
      return 0; // no such member
   }
   else
   {
      map_delete(((class team)parties[pname])->members, member);
      fix_shares(parties[pname]);
   }

   save_me();
   return 1;
}

nomask string locate_access_member(string pname, int access)
{
   string array members;

   pname = lower_case(pname);
   members = keys(((class team)parties[pname])->members);
   foreach(string name in members)
   {
      if((((class team_member)((class team)parties[pname])->members[name]))->access == access)
         return name;
   }
   return 0;
}

// Because two users cannot have the same access levels, when one member's
// access changes, at least one other member's must also. When a user is
// assigned access level X, every user between access level X and the
// user's current access level have their access level incremented (which,
// in game terms, means lowered) =)
private int change_access(string pname, string member, int access)
{
   string current_member;
   int old_access;

   pname = lower_case(pname);
   current_member = locate_access_member(pname, access);
   old_access = (((class team_member)((class team)parties[pname])->members[member]))->access;
   (((class team_member)((class team)parties[pname])->members[current_member]))->access = old_access;
   (((class team_member)((class team)parties[pname])->members[member]))->access = access;

   save_me();
   return 1;
}

nomask string locate_user(string name)
{
   string array p = keys(parties);

   foreach(string s in p)
   {
      if(member_array(name, keys(((class team)parties[s])->members)) != -1)
         return s;
   }
//   return filter(keys(parties), (: member_array(name,
//            keys(((class team)parties[$1])->members)) :));
}

nomask string array list_all_parties()
{
   return keys(parties);
}

nomask string array query_team_members(string pname)
{
   return keys(((class team)parties[lower_case(pname)])->members);
}

nomask int query_member_access(string pname, string member)
{
   return (((class team_member)((class team)parties[lower_case(pname)])->members[member]))->access;
}

// Find a user in the team 'pname' with a given access level.
nomask string locate_member_by_access(string pname, int access)
{
   string array m;

   pname = lower_case(pname);
   m = keys(((class team)parties[pname])->members);
   foreach(string s in m)
   {
      if((((class team_member)((class team)parties[pname])->members[s]))->access == access)
      return s;
   }
   return 0;
}

// The maximum number of members that can be active in one login.
nomask int query_max_active(string pname)
{
   return ((class team)parties[lower_case(pname)])->max_active;
}

nomask void set_max_active(string pname, int n)
{
   ((class team)parties[lower_case(pname)])->max_active = n;
   save_me();
}

nomask string array query_active_members(string pname)
{
   int max;
   string array all;

   pname = lower_case(pname);
   max = ((class team)parties[pname])->max_active;
   all = ({ });
   // If there is no limit, list all users. Default is no limit.
   if(max == 0)
      return query_team_members(pname);

   for(int x = 0; x < max; x++)
      all += ({ locate_member_by_access(pname, x) });

   return all;
}

nomask void remove_team(string pname)
{
   map_delete(parties, lower_case(pname));
   save_me();
}

nomask int team_exists(string pname)
{
   return pname ? (parties[lower_case(pname)] ? 1 : 0) : 0;
}

void create()
{
   restore_me();
}
