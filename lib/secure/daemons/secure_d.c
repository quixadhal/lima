/* Do not remove the headers from this file! see /USAGE for more info. */

// NOTE:
// Despite the length of this code, the only outside calls made
// are from call_unguarded(). call_other()s to other object will
// be extremely dangerous since they imply an unguarded() call.

#include <mudlib.h>
#include <security.h>

inherit M_ACCESS;

#define ERR_PRIV	"insufficient privilege"
#define ERR_NODOMAIN    "accessing non-existing domain"
#define ERR_NOWIZ       "referring to non-existing wizard"
#define ERR_BADARG	"illegal or unusable parameter"
#define ERR_HAZARD	"hazardous setting of privileges"

private mapping privileges; // mapping containing all privileges
                            // values are arrays of parents and children
private mapping read_access; // maps directory -> protection level
private mapping write_access; // dto.
private mapping quota;  // map!s wizard/domain names -> quota size
private mapping wizards; // mapping containing all wizards and their level
private mapping domains; // maps domains -> mappings of members/lords
                         //                 (valued 2 for lords, 1 for members)
private mapping domainlists; // wizard -> domains

int
valid_name( string s )
{
    // replace valid chars by '*', and compare to a string of all '*'s
    return
	map(s, (: ($1 < 'a' || $1 > 'z') ? ' ' : '*' :))
	== map(s, (: '*' :));
}

private void save_data()
{
  unguarded(1, (: save_object, ACCESS_SAVE :));
  unguarded(1, (: save_object, ACCESS_SAVE_BAK :));
}

private void syslog(string text)
{
  unguarded(1, (: write_file, ACCESS_LOG,
    (this_user() ? this_user()->query_userid()
                        : "<Server>") + ": " + text + "\n" :) );
}

void create()
{
  if( clonep(this_object()) )
    return;
  set_privilege(1);
  // The following call is handled specially in the master.
  // Using unguarded() is impossible, since variables are not yet
  // initialized.
 if (!restore_object(ACCESS_SAVE))
  {
    if (eval_cost() < 1000) // *sigh*
      for (;;);
    privileges = ([ ]);
    read_access = allocate_mapping(1);
    write_access = allocate_mapping(1);
    quota = ([ ]);
    domains = ([ ]);
    domainlists = ([ ]);
    wizards = ([ ]);
    save_data();
    return;
  }
  if (!sizeof(read_access))
    read_access = ([]);
  if (!sizeof(write_access))
    write_access = ([]);
}

int query_prevent_shadow() { return 1; }

// The following function HAS to be private. Otherwise somebody
// might gain write access to the server's datastructures.
private mixed *walk_path(string file,int write)
{
  string *path;
  mixed *result;
  mixed *tmp;
  mapping node,subnode;
  int i;
  path = explode(file,"/")-({"","."});
  if (!sizeof(path))
    return 0;
  if (member_array("..",path)>=0)
    return 0;
  result = allocate(sizeof(path));
  node = write ? write_access : read_access;
  result[0] = !!write;
  for (i=1;i<sizeof(result);i++)
  {
    if (undefinedp(tmp = node[path[i-1]]))
    {
      result[i]=-1;
      break;
    } else {
      result[i] = tmp[0];
      subnode = tmp[1];
    }
    if (!subnode)
      break;
    if (!sizeof(subnode))
    {
      tmp[1] = 0;
      break;
    }
    node = subnode;
  }
  if (i == sizeof(result))
    --i;
  return ({ result, path[0..<2], i });
}

nomask mixed query_protection(mixed file,int write)
{
  mixed *desc;
  int i;
  if (objectp(file))
    file = file_name(file);
  desc = walk_path(file,write);
  if (!desc)
    return !!write; // root directory
  i = desc[2];
  desc = desc[0];
  while (desc[i]==-1)
    --i;
  return desc[i];
}

nomask varargs int check_privilege(mixed priv,int ignore);
nomask int higher_privilege(mixed a,mixed b);
nomask int valid_privilege(mixed priv);

nomask string set_protection(string file,int write,mixed prot)
{
  mixed *desc;
  int i,j;
  mixed *priv;
  string *path;
  mapping node,subnode;
  desc = walk_path(file+"/foo",write);
  if (sizeof(desc[1])>6)
    return ERR_BADARG;
  if (prot != -1 && !valid_privilege(prot))
    return "invalid privilege identifier";
  if (!desc)
    return "trying to change root directory's privilege";
  priv = desc[0];
  i = desc[2];
  while (priv[i]==-1)
    i--;
  if (i)
  {
    j = i-1;
    while (priv[j]==-1)
      j--;
  }
  else
    j = i;
  if (priv[i]==prot)
    return "directory already has this effective protection level";
  if (write && !check_privilege(priv[sizeof(desc[1])==i ? j : i]))
    return ERR_PRIV;
  if (!write && !check_privilege(1)) // Only staff can make read restrictions
    return ERR_PRIV;
  if (eval_cost()<1000)
    for (;;);
  if (prot != -1)
  {
    if (write && higher_privilege(prot,priv[i]))
      return ERR_HAZARD;
    if (!write && higher_privilege(priv[i],prot))
      return ERR_HAZARD;
    node = write ? write_access : read_access;
    path = desc[1];
    for (i=0; i<sizeof(path)-1; i++)
    {
      if (undefinedp(node[path[i]]))
        node[path[i]] = ({ -1, subnode = allocate_mapping(1) });
      else if (!(subnode = node[path[i]][1]))
	node[path[i]][1] = subnode = allocate_mapping(1);
      node = subnode;
    }
    node[path[<1]]=({ prot, 0});
    save_data();
    return 0;
  }
  else
  {
    if (desc[2]!=sizeof(desc[1]))
      return "directory not linked to a privilege level";
    node = write ? write_access : read_access;
    path = desc[1];
    for (i=0; i<sizeof(path)-1; i++)
      node = node[path[i]][1];
    map_delete(node,path[<1]);
    save_data();
    return 0;
  }
}

nomask string set_quota(string name,int amount)
{
  if (undefinedp(quota[name]))
    return ERR_NOWIZ;
  if (!check_privilege(1)) // Only staff members can set quota
    return ERR_PRIV;
  if (eval_cost()<1000)
    for (;;);
  quota[name] = amount;
  save_data();
}

private int valid_domain_name(string name)
{
	return sizeof(regexp( ({ name }), "^[-a-z_]*$"));
}

nomask varargs string create_domain(string domain)
{
  if (!check_privilege(1)) // Only staff members can create domains
    return ERR_PRIV;
  if (eval_cost()<1000)
    for (;;);
  if (!valid_domain_name(domain))
    return "invalid domain name";
  domain = lower_case(domain);
  if (domains[domain])
    return "domain already exists";
  domains[domain] = ([ ]);
  privileges[capitalize(domain)] = ([ "":({}),":":({}) ]);
  quota[capitalize(domain)] = 8;
  save_data();
  syslog("Created domain " + domain);
  return 0;
}

nomask string delete_domain(string domain)
{
  string *members;
  int i;
  if (!check_privilege(1)) // Only staff members can delete domains
    return ERR_PRIV;
  if (eval_cost()<10000)
    for (;;);
  if (!domains[domain])
    return ERR_NODOMAIN;
  members = keys(domains[domain]);
  for (i=0; i<sizeof(members); i++)
  {
    mapping m;
    m = domainlists[members[i]];
    map_delete(m,domain);
  }
  map_delete(domains,domain);
  save_data();
  syslog("Deleted domain " + domain);
  return 0;
}

nomask string add_domain_member(string domain,string member,int lord)
{
  if (!domains[domain])
    return ERR_NODOMAIN;
  if (!wizards[member])
    return ERR_NOWIZ;
  if (!check_privilege(capitalize(domain)))
    return ERR_PRIV;
  if (eval_cost()<1000)
    for (;;); // This is getting boring...
  domains[domain][member] = (lord ? 2 : 1);
  if (domainlists[member])
    domainlists[member] += ([ domain : 1 ]);
  else
    domainlists[member] = ([ domain : 1 ]);
  save_data();
  if (lord)
    syslog("Made " + member + " lord of " + domain);
  else
    syslog("Made " + member + " member of " + domain);
  return 0;
}

nomask string remove_domain_member(string domain,string member)
{
  if (!domains[domain])
    return ERR_NODOMAIN;
  if (!domains[domain][member])
    return ERR_NOWIZ;
  if (!check_privilege(capitalize(domain)))
    return ERR_PRIV;
  if (eval_cost()<1000)
    for (;;);
  map_delete(domains[domain],member);
  map_delete(domainlists[member],domain);
  save_data();
  syslog("Removed " + member +" from domain " + domain);
  return 0;
}

nomask string create_wizard(string wizard)
{
  if (!valid_name(wizard))
    return "invalid character name";
  if (wizards[wizard])
    return "this character is already recorded as a wizard";
  if (!check_privilege(1))
    return ERR_PRIV;
  if (eval_cost()<1000)
    for (;;);
  wizards[wizard] = 1;
  privileges[wizard] = ([ "":({}),":":({}) ]);
  quota[wizard] = 8;
  save_data();
  syslog("Created wizard " + wizard);
  return 0;
}

nomask string delete_wizard(string wizard)
{
  mixed d;
  if (!wizards[wizard])
    return ERR_NOWIZ;
  if (!check_privilege(1))
    return ERR_PRIV;
  if (eval_cost()<10000)
    for (;;);
  map_delete(wizards,wizard);
  map_delete(privileges,wizard);
  d = domainlists[wizard];
  if (d)
  {
    int i;
    d = keys(d);
    for (i=0; i<sizeof(d); i++)
      map_delete(domains[d[i]],wizard);
  }
  save_data();
  syslog("Deleted wizard " + wizard);
  return 0;
}

private string get_priv_owner(string priv)
{
  int n;
  if (!stringp(priv))
    return 0;
  n = member_array(':',priv);
  if (n >=0)
    priv = priv[0..n-1];
  if (!privileges[priv])
    return 0;
  return priv;
}

nomask string define_privilege(string priv)
{
  string owner;
  owner = get_priv_owner(priv);
  if (!owner)
  {
    if (priv[0]!='@' || member_array(':',priv)>=0)
      return "invalid privilege identifier";
    if (!check_privilege(1))
      return ERR_PRIV;
    if (eval_cost()<1000)
      for (;;);
    privileges[priv] = ([ "": ({ }), ":": ({ }) ]);
    save_data();
    return 0;
  }
  if (!check_privilege(owner))
    return ERR_PRIV;
  if (eval_cost()<1000)
    for (;;);
  priv = priv[strlen(owner)..];
  if (privileges[owner][priv])
    return "attempt to redefine privilege";
  privileges[owner][priv] = ({ });
  save_data();
  return 0;
}

nomask string undefine_privilege(string priv)
{
  mixed owner;
  owner = get_priv_owner(priv);
  if (!owner)
    return "invalid privilege identifier";
  if (strlen(owner)+1 >= strlen(priv))
  {
    if (!check_privilege(1))
      return ERR_PRIV;
  }
  else
  {
    if (!check_privilege(owner))
      return ERR_PRIV;
  }
  if (eval_cost()<1000)
    for (;;);
  if (owner == priv)
    map_delete(privileges,priv);
  else
  {
    mapping list;
    list = privileges[owner];
    if (!list[priv[strlen(owner)..]])
      return "invalid privilege identifier";
    map_delete(list,priv[strlen(owner)..]);
  }
  save_data();
  return 0;
}

nomask string * query_privilege_owners()
{
    return keys(privileges);
}

nomask mapping query_privilege_list(string priv)
{
  mapping list,result;
  string *subs;
  int i;

  if (!(list = privileges[priv]))
    return 0;
  result = ([ ]);
  subs = keys(list);
  for (i=0; i<sizeof(subs); i++)
    result[priv+subs[i]] = list[subs[i]][0..];
  return result;
}

nomask mapping query_access_tree(string path,int write)
{
  mapping result;
  string *p;
  int i;
  mixed *tmp;
  
  p = explode(path,"/")-({"","."});
  if (write)
    result = write_access;
  else
    result = read_access;
  if (!sizeof(p))
    return copy(result);
  for (i=0;i<sizeof(p)-1 && sizeof(result);i++)
    result = result[p[i]] ? result[p[i]][1] : 0;
  if (sizeof(result) && !undefinedp(tmp = result[p[<1]]))
      return ([ p[<1] : ({ tmp[0] == -1 ? query_protection(path+"/foo",write):tmp[0], copy(tmp[1])}) ]);
  else
    return ([ p[<1] : ({ query_protection(path+"/foo",write), 0}) ]);
}

nomask string extend_access(string priv,string add)
{
  string owner;
  owner = get_priv_owner(priv);
  if (!owner || !privileges[owner] || !privileges[owner][priv[strlen(owner)..]])
    return "invalid privilege identifier";
  if (!stringp(add) || !valid_privilege(add))
    return "invalid privilege identifier";
  if (owner == priv)
  {
    if (!check_privilege(1))
      return ERR_PRIV;
  }
  else
  {
    if (!check_privilege(owner))
      return ERR_PRIV;
  }
  if (eval_cost()<1000)
    for (;;);
  priv = priv[strlen(owner)..];
  privileges[owner][priv] -= ({ add });
  privileges[owner][priv] += ({ add });
  save_data();
  return 0;
}

nomask string restrict_access(string priv,string remove)
{
  string owner;
  owner = get_priv_owner(priv);
  if (!owner || !privileges[owner] || !privileges[owner][priv[strlen(owner)..]])
    return "invalid privilege identifier";
  if (owner == priv)
  {
    if (!check_privilege(1))
      return ERR_PRIV;
  }
  else
  {
    if (!check_privilege(owner))
      return ERR_PRIV;
  }
  if (eval_cost()<1000)
    for (;;);
  priv = priv[strlen(owner)..];
  if (member_array(remove,privileges[owner][priv])<0)
    return "invalid privilege identifier";
  privileges[owner][priv] -= ({ remove });
  save_data();
  return 0;
}

nomask int query_is_wizard(string name)
{
  return wizards[name];
}

nomask int query_is_domain(string name)
{
  return !!domains[name];
}

nomask string *query_domain_members(string domain)
{
  mapping members;
  members = domains[domain];
  return members && keys(members);
}

nomask string *query_domain_lords(string domain)
{
  mapping members;
  string *list;
  int i;
  members = domains[domain];
  if (!members)
    return 0;
  list = keys(members);
  for (i=0; i<sizeof(list); i++)
    if (members[list[i]]<2)
      list[i] = 0;
  return list - ({ 0 });
}

nomask varargs string *query_domains(string wizard)
{
  mapping d;
  if (!wizard)
    return keys(domains);
  if (!wizards[wizard])
    return 0;
  d = domainlists[wizard];
  return d ? keys(d) : ({ });
}

nomask int valid_privilege(mixed p)
{
  int n;
  mapping list;
  if (intp(p))
    return p==0 || p==1;
  n = member_array(':',p);
  if (n<0)
    n = strlen(p);
  if (list = privileges[p[0..n-1]])
    return !undefinedp(list[p[n..]]);
}

// returns 1 if a >= b
nomask int higher_privilege(mixed a,mixed b)
{
  // This is THE central routine of the security system.
  // It determines the security hierarchy.
  int m,n;
  if (!valid_privilege(a) || !valid_privilege(b))
    error("Invalid privilege"+a+","+b+"\n");
  if (a==b)
    return 1;
  if (intp(a))
  {
    if (a)
      return 1;
    return 0;
  }
  else if (intp(b))
  {
    if (!b)
      return 1;
    return 0;
  }
  m = member_array(':',a);
  if (m<0)
    m = strlen(a);
  n = member_array(':',b);
  if (n<0)
    n = strlen(b);
  a = ({ a[0..m-1], a[m..] });
  b = ({ b[0..n-1], b[n..] });
  if (a[0]==b[0] && strlen(a[1])<strlen(b[1]) && b[1][0..strlen(a[1])-1]==a[1])
    return 1;
  switch (b[0][0])
  {
    case 'A'..'Z':
      if (strlen(a[1])==0 && domains[lower_case(b[0])])
      {
        if (domains[lower_case(b[0])][a[0]] > !strlen(b[1]))
          return 1;
      }
  }
  return member_array(a[0] + a[1], privileges[b[0]][b[1]])>=0;
}

nomask mixed reduced_privilege(mixed priv,mixed max)
{
  if (higher_privilege(max,priv))
    return priv;
  return max;
}

nomask mixed call_unguarded(function code,mixed *handle)
{
  mixed result;
  if (function_exists("verify_privilege_granted",previous_object())!=M_ACCESS)
    {
      syslog("Secvio: faked call in " + file_name(previous_object()));
      return 0; // faked call
    }
  if (!previous_object()->verify_privilege_granted(handle))
    {
      syslog("Secvio: handle incorrect in " + file_name(previous_object()));
      return 0; // faked, too
    }
  return evaluate(code);
}

nomask varargs int check_privilege(mixed prot,int ignore)
{
  object *stack;
  int stacksize;
  int i;
  object ob,next;
  object next2;

  if (!prot)
    return 1;
  stacksize = (sizeof(stack = all_previous_objects()));
  for (i=ignore; i<stacksize; i++)
  {
    ob = stack[i];
    next = (i < stacksize - 1 ? stack[i+1] : 0);
    next2 = (i < stacksize - 2 ? stack[i+2] : 0);
    if (!ob)
      {
	syslog("Secvio: object destructed in call stack\n" + 
	       implode(map_array(stack, (: file_name :)), ", "));
	return 0;
      }

    if (next == this_object()) // This call is unguarded
    {
	/* ### driver bug: may have 2 copies of self on stack */
	if ( next == next2 && ob != this_object() )
	    next = i < stacksize - 3 ? stack[i+3] : 0;
	else
	    /* ### end of patch... */
	    next = next2;
      if (next!=ob && next != this_object())
	{
	  syslog(sprintf("Secvio: %O used fptr bound to %O", ob, next));
	  return 0; // The object defining the fptr did not call it.
	}
      if (!next && prot)
	{
	  syslog("Secvio: ob destructed in call stack\n" +
		 implode(map_array(stack, (: file_name :)), ", "));
	  return 0; // This object has been destructed.
	}
      if (function_exists("query_unguarded_privilege",next)!=M_ACCESS)
	{
	  syslog("Secvio: faked query_unguarded_privilege in " + 
		 file_name(next));
	  return 0; // faked
	}
      if (!higher_privilege(next->query_unguarded_privilege(),prot))
	{
	  syslog(sprintf("Secvio: %O has priv %O, asked for %O",
			 next, next->query_unguarded_privilege(), prot));
	  return 0; // insufficient privilege
	}
      if (!higher_privilege(query_protection(next,1),prot))
	{
	  syslog(sprintf("Secvio: %O has prot %O, asked for %O",
			 next, query_protection(next, 1), prot));
	  return 0; // privilege higher than maximum allowed value
	}
      return 1;
    }
    if ( ob == master() ) continue;
    if ( file_name(ob) == SIMUL_OB ) continue;

    if (function_exists("query_privilege",ob)!=M_ACCESS)
    {
      string temp,temp2;
      if (sscanf(file_name(ob),"/wiz/%s/%s",temp,temp2)==2)
      {
	  return higher_privilege(temp+":",prot);
      }
      if (sscanf(file_name(ob),"/domains/%s/%s",temp,temp2)==2)
      {
	  return higher_privilege(capitalize(temp)+":",prot);
      }
      return 0;
    }
    if (!higher_privilege(ob->query_privilege(),prot))
      return 0;
  }
  ob = this_user();
  if (!ob)
    {
      syslog("Secvio: Missing user\n" + call_trace()[0..<2]);
      return 0;
    }
  if (function_exists("query_privilege",ob)!=M_ACCESS)
    {
      syslog(sprintf("Secvio: %O faked query_privilege", ob));
      return 0;
    }
  if (!higher_privilege(ob->query_privilege(),prot))
    {
      syslog(sprintf("Secvio: %O has priv %O, asked for %O",
		     ob, ob->query_privilege(), prot));
      return 0;
    }
  return 1;
}
