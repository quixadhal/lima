/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** IMAIL_D -- Handle intermud mail.  This has different functionality 
** than local mail, because muds might be down, drop off the mudlist, etc.
**
**
** Some explanation of the implementation:
** Mail messages might go to users at different muds.  
** We might deliver successfully to one mud, and not to others.
** Therefore, we assign multiple outgoing ids to each post, one for each
** mud we're sending to.  When all those ids are ack'd, we know the post 
** has been fully delivered, so we remove that from our "internal queue".
** I deemed this better than copying the message x times if the message
** was getting sent to x muds.
**
** John Viega (rust@lima.mudlib.org) July 19, 1996.
*/
#define SAVE_FILE	"/data/mail/imud_queue"
#define ONE_DAY			(60 * 60 * 24)
#define ONE_HOUR		(60 * 60)
#define WARNING_INTERVAL	ONE_DAY
#include <driver/origin.h>

inherit M_DAEMON_DATA;
inherit CLASS_MAILMSG;
inherit M_COMPLETE;


varargs string array targ_map_to_list(mapping, int);
private void process_queue();
varargs private void handle_ack(mapping, int);

class outgoing_info
{
  string 	mudname;
  int		msgId;
  int		lastWarningTime;
}
int nextOutgoingId = 1;  /* I'd like to see this change to a string. */
int nextMsgId      = 1;

// Okay, this one maps id : (mudname, users, msgId)
private mapping outgoing_queue = ([]);

// This one maps msgId : mail_msg.  
private mapping internal_queue = ([]);

private int is_currently_up(string name)
{
 return member_array(IMUD_D->canon_mudname(name), 
		     IMUD_D->query_up_muds()) != -1;
}

nomask mixed get_complete_mudname(string name)
{
  mixed 	info;
  
  info = find_best_match_or_complete(name, IMUD_D->query_mudnames());
   
  if(!arrayp(info) || !sizeof(info))
    return ({});
  
  if(sizeof(info) == 1)
    return info[0];

  return info;
}

void create()
{
    ::create();
    process_queue();
    call_out("check_queue", ONE_HOUR);
}

private void check_queue()
{
  process_queue();
  call_out("check_queue", ONE_HOUR);
}

string format_addrs(mapping m)
{
  return implode(targ_map_to_list(m), ", ");
}

private string array quote_msg(class mail_msg msg)
{
  return ({
      "",  
      "---Original message follows:",
      sprintf("FROM: %s", msg->sender),
      sprintf("TO: %s", arrayp(msg->to_list) ? implode(msg->to_list, ", ") :
	      format_addrs(msg->to_list)),
      sprintf("CC: %s", arrayp(msg->cc_list) ? implode(msg->cc_list, ", ") :
	      format_addrs(msg->cc_list)),
      sprintf("SUBJECT: %s", msg->subject)
	}) + msg->body;
}

private void send_warning (int id, string array warning)
{
  class outgoing_info	this_info;
  string 		recipient;


  if(this_info->lastWarningTime + WARNING_INTERVAL > time())
    {
      return;   // Don't send out a warning every single time we try to send.
    }

  this_info = outgoing_queue[id];
  recipient = ((class mail_msg)internal_queue[this_info->msgId])->sender;

  unguarded (1, (: MAIL_D->send_mail ("I3 Mail Service",
				   "Delivery Still Pending",
				      ({ "*** Your mail hasn't been fully "
					   "delivered yet."}) +
				   $(warning)  + 
			   quote_msg(internal_queue[$(this_info->msgId)]),
				   ({$(recipient)}),
				   ({})) :));
  
  this_info->lastWarningTime = time();
  save_me();
}

private void report_errors (int id, string array errorset)
{
  class outgoing_info	this_info;
  string 		recipient;

  this_info = outgoing_queue[id];
  recipient = ((class mail_msg)internal_queue[this_info->msgId])->sender;

  unguarded (1, (: MAIL_D->send_mail ("I3 Mail Service",
				   "Failed mail",
				   $(errorset)  + 
			      quote_msg(internal_queue[$(this_info->msgId)]),
				   ({$(recipient)}),
				   ({})) :));
}

private void error_ambiguous_or_unknown_mudnames(class mail_msg msg, 
						 mapping info)
{
  string array 	problems = ({});
  string array 	whatItCouldHaveBeen;
  string	whatYouCalledIt;

  foreach(whatYouCalledIt, whatItCouldHaveBeen in info)
    {
      if(sizeof(whatItCouldHaveBeen))
	{
	  problems += 
	    ({sprintf("Ambigous mud name, '%s'.  Could have been: %s",
		      whatYouCalledIt, implode(whatItCouldHaveBeen, ", "))});
	}
      else
	{
	  problems += ({ sprintf("Unknown mud, '%s'.", whatYouCalledIt) });
	}
    }

    unguarded (1, (: MAIL_D->send_mail ("I3 Mail Service",
				      "Delivery problems",
				   $(problems)  + 
			      quote_msg($(msg)),
				   ({$(msg->sender)}),
				   ({})) :));
}

void receive_ack(mapping ack_list)
{
  if (previous_object() != find_object(IMUD_D))
    {
      error("Illegal call to receive_ack.");
    }
  handle_ack(ack_list, 1);
}

varargs private void handle_ack(mapping ack_list, int flag)
{
  int			id;
  string array		errorset;
  class outgoing_info	info;
  class mail_msg	this_msg;

  /* Okay, if there are failures, report them. */
  foreach (id, errorset in ack_list)
    {
      info = outgoing_queue[id];
      if(arrayp(errorset) && sizeof(errorset))
	{
	  if(flag)
	    {
	      report_errors (id, ({sprintf("%s reports the following errors:",
				       info->mudname)}) + errorset);
	    }
	  else
	    {
	      report_errors (id, ({"The following delivery errors occured:"})
		+ errorset);

	    }
	}
      map_delete(outgoing_queue, id);
      this_msg = internal_queue[info->msgId];
      if(!(--this_msg->dels_pending))
	{
	  map_delete(internal_queue, info->msgId);
	}
    }
    save_me();
}


private array prep_message(int id, class outgoing_info info)
{
  class	mail_msg msg = internal_queue[info->msgId];
  return ({
    "mail",
      id,
      msg->sender,
      msg->to_list,
      msg->cc_list,
      ({}),
      msg->date,
      msg->subject,
      implode(msg->body, "\n")
  });
}

private void send_message(int id)
{
  class outgoing_info	this_info;
  array			packet;

  this_info = outgoing_queue[id];
  if (!IMUD_D->mud_exists(this_info->mudname))
    {
      handle_ack (([id : ({sprintf("Mud '%s' is not known at all by I3 "
				    "(might have dropped off the mudlist)", 
				    this_info->mudname) }) ]));
      return;
    }
  if(!IMUD_D->has_service(this_info->mudname, "mail"))
    {
      handle_ack (([id : ({sprintf("Mud '%s' doesn't support I3 mail.",
				    this_info->mudname) }) ]));
      return;
    }
  if (!is_currently_up(this_info->mudname))
    {
      send_warning(id, ({ "Couldn't deliver messgage because " + 
			    this_info->mudname + " is currently down."}));
    }
  
  packet = prep_message(id, this_info);
  IMUD_D->send_mail_message_to_mud(packet, this_info->mudname); 
}

private void process_queue()
{
  int			i;

  foreach(i in keys(outgoing_queue))
    {
      send_message(i);
    }
}


private mapping find_remote_recipients(class mail_msg msg)
{
  mapping 	remote_info = ([]);
  string	name;
  mapping	ambiguous_mudnames = ([]);
  mapping	new_to = ([]);
  mapping	new_cc = ([]);
  int		i;

  for(i = 0; i < sizeof(msg->to_list); i++)
    {
      string user, mudname; 
      mixed  mudinf;
      name = msg->to_list[i];
      if(sscanf(name, "%s@%s", user, mudname))
	{
	  mudname = lower_case(mudname);
	  if(ambiguous_mudnames[mudname])
	    {
	      continue;
	    }
	  mudinf = get_complete_mudname(mudname);
	  if(arrayp(mudinf))
	    {
	      ambiguous_mudnames[mudname] = mudinf;
	      continue;
	    }
	  else
	    {
	      mudinf = IMUD_D->canon_mudname(mudinf);
	    }
	  if(!remote_info[mudinf])
	    {
	      new_to[mudinf] 	  = ({ user });
	      remote_info[mudinf] = ({ user });
	    }
	  else
	    {
	      new_to[mudinf]      += ({ user });
	      remote_info[mudinf] += ({ user });
	    }
	}
      else
	{
	  if(new_to[mud_name()])
	    {
	      new_to[mud_name()] += ({name});
	    }
	  else
	    {
	      new_to[mud_name()] = ({name});
	    }
	}
    }
  msg->to_list = new_to;

  for(i = 0; i < sizeof(msg->cc_list); i++)
    {
      string user, mudname; 
      mixed  mudinf;
      name = msg->cc_list[i];
      if(sscanf(name, "%s@%s", user, mudname))
	{
	  mudname = lower_case(mudname);
	  if(ambiguous_mudnames[mudname])
	    {
	      continue;
	    }
	  mudinf = get_complete_mudname(mudname);
	  if(arrayp(mudinf))
	    {
	      ambiguous_mudnames[mudname] = mudinf;
	      continue;
	    }
	  else
	    {
	      mudinf = IMUD_D->canon_mudname(mudinf);
	    }
	  if(!remote_info[mudinf])
	    {
	      new_cc[mudinf] 	  = ({ user });
	      remote_info[mudinf] = ({ user });
	    }
	  else
	    {
	      if(new_cc[mudinf])
		{
		  new_cc[mudinf]      += ({ user });
		}
	      else
		{
		  new_cc[mudinf] = ({ user });
		}
	      remote_info[mudinf] += ({ user });
	    }
	}
      else
	{
	  if(new_cc[mud_name()])
	    {
	      new_cc[mud_name()] += ({name});
	    }
	  else
	    {
	      new_cc[mud_name()] = ({name});
	    }
	}
    }
  msg->cc_list = new_cc;

    if(sizeof(ambiguous_mudnames))
      {
	error_ambiguous_or_unknown_mudnames(msg, ambiguous_mudnames);
      }
    return remote_info;
}

void enqueue_message(class mail_msg msg)
{
  mapping		remote_info;
  string		mudName;
  int			myMsgId;

  if (previous_object() != find_object(MAIL_D))
    {
      error("Illegal call to enqueue_message");      
    }
  remote_info = find_remote_recipients(msg);

  if (!sizeof(remote_info))
    {
      // No imud users, ambigous or bad mud names...
      return;
    }

  myMsgId = nextMsgId++;
  msg->dels_pending = sizeof(remote_info);
  internal_queue[myMsgId] = msg;
  foreach ( mudName in keys(remote_info) )
    {
      class outgoing_info 	myInfo;

      myInfo = new(class outgoing_info);
      myInfo->mudname = mudName;
      myInfo->msgId   = myMsgId;
      myInfo->lastWarningTime = 0;
      outgoing_queue[nextOutgoingId] = myInfo;
      send_message(nextOutgoingId++);
    }
  save_me();
}

private string array get_local_recipients(mixed array mail_packet)
{
  string array	mudnames;
  string array 	local_recipients = ({});

  mudnames = filter(keys(mail_packet[3]), // TO list
		    (: mud_name() == IMUD_D->canon_mudname($1) :)); 
  if(sizeof(mudnames))
    {
      local_recipients += mail_packet[3][mudnames[0]];
    }

  mudnames = filter(keys(mail_packet[4]), //CC list
		    (: mud_name() == IMUD_D->canon_mudname($1) :)); 
  if(sizeof(mudnames))
    {
      local_recipients += mail_packet[4][mudnames[0]];
    }
  local_recipients += mail_packet[5];  // BCC list
  local_recipients = clean_array(local_recipients);

  return local_recipients;
}


// Converts a to_list or cc_list of the form:
//    ([
//        MUD-A : ({ user-1, user-2, }),
//        MUD-B : ({ user-1, user-2, }),
//    ])
//
// To one of the form: ({user-1@MUD-A, user-2@MUD-A, ... })

// If flag, I want @This Mud to be left off.
varargs string array targ_map_to_list(mapping info, int flag)
{
  string array ret = ({});

  foreach(string mudname, string array userlist in info)
    {
      if(flag && mudname == mud_name())
	{
	  ret += userlist;
	}
      else
	{
	  ret += map(userlist, (: $1 + "@" + $(mudname) :));
	}
    }

  return ret;
}

string array incoming_mail(string mudname, mixed array mail_packet)
{
  string array 	local_recipients;
  string array	errors = ({});
  int	 	i;

  if(previous_object() != find_object(IMUD_D))
    {
      error("illegal call to incoming_mail");
    }
  if(!mapp(mail_packet[3]))
    {
      mail_packet[3] = ([]);
      errors += ({"Imud implementation error: TO list should be a mapping"});
    }
  if(!mapp(mail_packet[4]))
    {
      mail_packet[4] = ([]);
      errors += ({"Imud implementation error: CC list should be a mapping"});
    }
  if(!arrayp(mail_packet[5]))
    {
      mail_packet[5] = ({});
      errors += ({"Imud implementation error: BCC list should be an array"});
    }
  // Get local recipients.
  local_recipients = get_local_recipients(mail_packet);

  // Check their validity.
  i = sizeof(local_recipients);
  while(i--)
    {
      if(!user_exists(local_recipients[i]))
	{
	  errors += ({sprintf("'%s' is not a user on %s.", local_recipients[i],
			      mud_name())});
	  local_recipients[i] = 0;
	}
    }
  local_recipients -= ({0});

  if(sizeof(local_recipients))
    {
      unguarded(1, (: MAIL_D->send_mail($(mail_packet)[2] + "@" + $(mudname), 
				    $(mail_packet)[7],  // subject
				    $(mail_packet)[8],  // body
				    targ_map_to_list($(mail_packet)[3], 1),
				    targ_map_to_list($(mail_packet)[4], 1),
				    $(mail_packet)[6])  // send time.
		:));
    }
  return errors;
}

void test()
{
  class mail_msg	test;

  test = new(class mail_msg);
  test->sender = "rust";
  test->to_list = ({"rust@zork"});
  test->cc_list = ({});
  test->subject = "Blah";
  test->date    = time();
  test->thread_id = test->date;
  test->body    = ({"This is test mail."});
  enqueue_message(test);
}


