/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** RCPD (Remote Creator Protocol Daemon)
**
** Dunedain (vivake@uchicago.edu) 17 September 1998
**
** Notes:
**  This is compatible with Garion's Remote Editor II Windows client
**  and with Descartes' CreRemote perl script.
**  Remote Editor II can be found at :
**          http://www.kolej.mff.cuni.cz/~brunclik/products/reditor/remote.htm
**       or http://members.xoom.com/Milanek/products/reditor/remote.htm
**
** Credits:
**  o Descartes for the original RCP spec, for the implementation
**    in Nightmare, and for the CreRemote perl script.
**  o Garion@Lost in Time (Milan Brunclik - milanek@geocities.com)
**    for writing Remote Editor II, a Windows RCP client.  This daemon
**    is fully compatible with it.
**  o Rust and company@Lima Bean for /secure/daemons/ftp_d.c, which
**    this daemon is based upon.
**
*/  

#include <socket.h>
#include <assert.h>
#include <log.h>
#include <clean_up.h>
#include <ports.h>

inherit M_ACCESS;

#define NEEDS_ARG()     if(!arg) arg = "" 
#define RCPLOG(x)       LOG_D->log(LOG_RCP, x)

class rcp_session
{
   int          connected;
   string       user;
   mixed        priv;
   string       pwd;
   object       cmdPipe;
   string       command;

   string       fileName;
   int          filePosition;
   string       fileData;
}

private void RCP_CMD_ls(class rcp_session, string);
private void RCP_CMD_login(class rcp_session, string);
private void RCP_CMD_mkdir(class rcp_session, string);
private void RCP_CMD_cd(class rcp_session, string);
private void RCP_CMD_rm(class rcp_session, string);
private void RCP_CMD_rmdir(class rcp_session, string);
private void RCP_CMD_mv(class rcp_session, string);
private void RCP_CMD_update(class rcp_session, string);
private void RCP_CMD_edit(class rcp_session, string);
private void RCP_CMD_100(class rcp_session, string);
private void RCP_DATA_read(class rcp_session, string);
private void RCP_DATA_write(class rcp_session, string);

private nosave mapping sessions = ([]);
private nosave mapping dispatch = 
([
  "login" : (: RCP_CMD_login :),
  "mkdir" : (: RCP_CMD_mkdir :),
  "cd"    : (: RCP_CMD_cd    :),
  "ls"    : (: RCP_CMD_ls    :),
  "rm"    : (: RCP_CMD_rm    :),
  "rmdir" : (: RCP_CMD_rmdir :),
  "mv"    : (: RCP_CMD_mv    :),
  "update": (: RCP_CMD_update:),
  "edit"  : (: RCP_CMD_edit  :),
  "100"   : (: RCP_CMD_100   :),
]);

private mixed outfile=([]);
private object sock;

private void RCPLOGF(string format, string array args...)
{
  RCPLOG(sprintf(format, args...));
}

private void RCP_addSession(object socket)
{
  class rcp_session  newSession;

  newSession = new(class rcp_session);
  newSession->cmdPipe = socket;
  map_delete(sessions,0); /* Just make sure there's no stale stuff here */
  sessions[socket] = newSession;
}

private void RCP_read(object socket, string data)
{
  string                cmd, arg;
  class rcp_session     thisSession;
  function              dispatchTo;
  int i;

  /* If there is no data, it's a new connection. */
  if (!data)
    {
      assert(!sessions[socket]);
      RCP_addSession(socket);
      return;
    }

  thisSession = sessions[socket];

  /* If filePosition if > 0, we must be receiving a file */
  if( thisSession->filePosition > 0 )
    {
      RCP_DATA_read(thisSession, data);
      return;
    }

  if (!thisSession->command) thisSession->command="";
  thisSession->command+=data;

  if ((i=strsrch(thisSession->command,"\n"))==-1) return;
  data=thisSession->command[0..i-1];
  thisSession->command=thisSession->command[i+1..];

  data=replace_string(data,"\r","");
  data=trim_spaces(data);

  /* get the command and argument. */
  if (!sscanf(data, "%s %s", cmd, arg))
    {
      cmd = data;
    }

  /* lower_case the command... */
  cmd = lower_case(cmd);

  /* If we're not connected, these are valid commands... */
  if (!thisSession->connected)
    {
      switch(cmd)
	{
	case "login":
	  RCP_CMD_login(thisSession, arg);
	  return;
	default:
	  socket->send("50 Must login with user name and password.\n");
	  return;
	}
    }
  /* We are connected, so dispatch to the proper handler. */
  dispatchTo = dispatch[cmd];
  if (!dispatchTo)
    {
      /* Log command so we know what clients are trying to use */
      socket->send(sprintf("50 %s %s.: Command not supported.\n",cmd,arg));
      return;
    }
  if(catch(evaluate(dispatchTo, thisSession, arg)))
     {
       RCPLOGF("%s caused a FAILURE with command '%s'.\n",
	    capitalize(thisSession->user), data);
       socket->send("50 Unknown failure.  Please report what you were doing "
                    "to the mud admin.\n");
     }
  return;
}

private void RCP_close(object socket)
{
  map_delete(sessions, socket);
}

private void create()
{
  sock = new(SOCKET, SKT_STYLE_LISTEN, PORT_RCP, (: RCP_read :),
	     (: RCP_close :));
}

private void RCP_DATA_read(class rcp_session info, string data)
{
  int len;
      
  info->command = "";
  if( !(len = strlen(data)) )
    {
      return;
    }

  if( len < info->filePosition )
    {
      info->fileData += data;
      info->filePosition -= len;
      return;
    }
  if( len == info->filePosition )
    {
      info->fileData += data;
      info->filePosition -= len;
      RCP_DATA_write(info, info->fileData);
      return;
    }
  else if( len >= info->filePosition )
    {
      string file;

      file = data[0..(info->filePosition-1)];
      info->command = data[info->filePosition..];
      RCP_DATA_read(info, file);
      RCP_read(info->cmdPipe, "\n");
      return;
    }

  return;
}

private void RCP_DATA_write(class rcp_session info, string text)
{
  text = replace_string(text,"\r","");

  if( catch(unguarded(info->priv,
                      (: write_file, info->fileName, text, 1 :))) )
    {
      info->cmdPipe->send(sprintf("50 Failed to write file: %s.\n", info->fileName));
    }
  else
    {
      info->cmdPipe->send(sprintf("110 File %s written.\n", info->fileName));
      RCPLOGF("%s PUT %s.\n", capitalize(info->user), info->fileName);
    }

  info->fileData = "";
  info->filePosition = 0;
  info->fileName = "";

  return;
}

private void RCP_CMD_login(class rcp_session info, string arg)
{
  string username, password, pass;
  mixed *userinfo;
  NEEDS_ARG();

  if( sscanf(arg, "%s %s", username, password) != 2 )
    {
      info->cmdPipe->send("50 Login failed: fill in username AND password.\n");
      return;
    }
  username = lower_case(username);
  if( !unguarded( 1, (: user_exists, username :) ) )
    {
      info->cmdPipe->send("50 Login failed: unknown user.\n");
      return;
    }
  if(info->connected)
    {
      info->cmdPipe->send(sprintf("50 User %s access denied.\n", arg));
      return;
    }
  info->user = username;

  userinfo = unguarded(1,(:USER_D->query_variable($(info->user),
                                                  ({"password"})):));
  if(arrayp(userinfo) && sizeof(userinfo))
    {
      pass = userinfo[0];
    }

  if(crypt(password, pass) != pass || !wizardp(info->user))
    {
      info->cmdPipe->send("50 Login failed: wrong password.\n");
      return;
    }

  info->cmdPipe->send(sprintf("60 Connection to %s.\n", mud_name()));
  info->connected = 1;
  RCPLOGF("%s connected from %s.\n", capitalize(info->user), 
	      info->cmdPipe->address()[0]);

  if(adminp(info->user))
    {
      info->priv = 1;
    }
  else
    {
      info->priv = info->user;
    }
  info->pwd = join_path(WIZ_DIR,info->user);
  if (!is_directory(info->pwd))
    { info->pwd = "/"; }
  return;
}

private string query_access(class rcp_session info, string arg)
{
  string acc;
  string tmp;

  if( (tmp = unguarded(info->priv,(: read_file,arg :))) != 0 )
    {
      acc = "r";
      if( unguarded(info->priv, (: write_file, arg, tmp, 1 :)) != 0 )
        {
          acc += "w";
        }
      else
        {
          acc += "-";
        }
    }
  else
    {
      acc = "--";
    }
  return acc;
}

private string filter_files(class rcp_session info, string dir, mixed array arg)
{
  string s;
  if (dir != "/") s = dir + "/" + (string)arg[0];
  else s = "/" + (string)arg[0];
  if( unguarded(info->priv, (: file_size, s :)) == -1 ) return "";
  return sprintf("%s\t%d\t%d\t%s", query_access(info, s),
                 (int)arg[1],(int)arg[2],(string)arg[0]);
}

private void RCP_CMD_ls(class rcp_session info, string arg)
{
  string array files;
  string orig_arg = arg;
  
  if(!arg || arg == "")
    {
      arg = info->pwd;
    }
  else if( (arg != "/") && (arg[<1] == '/') )
    {
      arg = arg[0..<2];
    }
  arg = evaluate_path(arg, info->pwd);

  switch(unguarded(info->priv, (: file_size, arg :)))
    {
      case -2:
        if( arg[<1] != '/' ) arg += "/";
        files = ({ arg });
        files += map(unguarded(info->priv, (: get_dir, arg, -1 :) ),
                     (: filter_files($(info), $(arg), $1) :));
        info->cmdPipe->send("500 " + implode(files, "\t") + "\n");
        return;
      case -1:
        info->cmdPipe->send(sprintf("50 ls %s: Permission denied.\n",orig_arg));
        return;
      default:
      {
        mixed stats = unguarded(info->priv, (: stat, arg:) );
        info->cmdPipe->send("500 " + sprintf("%s\t%d\t%d\t%s",
                                             query_access(info, arg),
                                             stats[1],stats[0],arg)+"\n");
        return;
      }
    }

  return;
}

private void RCP_CMD_edit(class rcp_session info, string arg)
{
  string file;

  NEEDS_ARG();
  
  file = evaluate_path(arg, info->pwd);

  if( !file = unguarded(info->priv,(: read_file, file :)) )
    {
      file = "";
    }

  if( file[<1] != '\n' )
    {
      file += "\n";
    }
   
  info->cmdPipe->send(sprintf("%-14s\n","100 " + strlen(file))[0..15]);
  info->cmdPipe->send(file);
  RCPLOGF("%s GOT %s.\n", capitalize(info->user), file);

  return;
}

private void RCP_CMD_100(class rcp_session info, string arg)
{
  int sz;
  string filename;
  NEEDS_ARG();

  if( sscanf(arg, "%d %s", sz, filename) != 2 )
    {
      info->cmdPipe->send("50 Bad file send command.\n");
      return;
    }
  filename = evaluate_path(filename, info->pwd);
  info->filePosition = sz;
  info->fileName = filename;
  info->fileData = "";

  arg = info->command;
  info->command = "";
  RCP_read(info->cmdPipe, arg);
  return;
}

private void RCP_CMD_cd(class rcp_session info, string arg)
{
  string	newpath;

  NEEDS_ARG();
  newpath = evaluate_path(arg, info->pwd);

  if(!unguarded(info->priv, (:is_directory($(newpath)):)))
    {
      info->cmdPipe->send(sprintf("50 cd %s: Permission denied.\n",arg));
      return;
    }
  info->pwd = newpath;
  info->cmdPipe->send(sprintf("400 %s\n",newpath));
}

private void RCP_CMD_mkdir(class rcp_session info, string arg)
{
  NEEDS_ARG();

  arg = evaluate_path(arg, info->pwd);

  if(!unguarded(info->priv, (:is_directory(base_path($(arg))):)))
    {
      info->cmdPipe->send(sprintf("50 mkdir %s: Permission denied.\n",arg));
      return;
    }
  if(unguarded(info->priv, (:file_size($(arg)):)) != -1)
    {
      info->cmdPipe->send(sprintf("50 mkdir %s: Permission denied.\n",arg));      return;
    }

  if(!unguarded(info->priv, (:mkdir($(arg)):)))
    {
      info->cmdPipe->send(sprintf("50 mkdir %s: Permission denied.\n",arg));      return;
    }
  info->cmdPipe->send("400 Directory created.\n");
  return;
}

private void RCP_CMD_rm(class rcp_session info, string arg)
{
  string orig_arg = arg;
  
  NEEDS_ARG();
  arg = evaluate_path(arg, info->pwd);

  if(!unguarded(info->priv, (: rm($(arg)) :)))
    {
      info->cmdPipe->send(sprintf("50 rm %s: Permission denied.\n",orig_arg));
      return;
    }
  RCPLOGF("%s DELETED %s.\n", capitalize(info->user), arg);
  info->cmdPipe->send(sprintf("400 %s deleted.\n", arg));
}

private void RCP_CMD_rmdir(class rcp_session info, string arg)
{
  string orig_arg = arg;
  
  NEEDS_ARG();
  arg = evaluate_path(arg, info->pwd);

  if(!unguarded(info->priv, (: rmdir($(arg)) :)))
    {
      info->cmdPipe->send(sprintf("50 rmdir %s: Permission denied.\n",orig_arg));
      return;
    }
  RCPLOGF("%s DELETED %s.\n", capitalize(info->user), arg);
  info->cmdPipe->send(sprintf("400 %s deleted.\n", arg));
}

private void RCP_CMD_mv(class rcp_session info, string arg)
{
  string *files;
  
  NEEDS_ARG();
  files = explode(arg, " ");

  if( sizeof(files) != 2 )
    {
      info->cmdPipe->send(sprintf("50 mv %s: Permission denied.\n",arg));
      return;
    }
  files[0] = evaluate_path(files[0], info->pwd);
  files[1] = evaluate_path(files[1], info->pwd);

  if(unguarded(info->priv, (: catch(rename($(files[0]), $(files[1]))) :)))
    {
      info->cmdPipe->send(sprintf("50 rm %s: Permission denied.\n",arg));
      return;
    }
  RCPLOGF("%s MOVED %s.\n", capitalize(info->user), arg);
  info->cmdPipe->send(sprintf("400 %s renamed to %s.\n",files[0],files[1]));
}

private void RCP_CMD_update(class rcp_session info, string arg)
{
  object ob;
  
  NEEDS_ARG();
  arg = evaluate_path(arg, info->pwd);
  if( arg[<2..] == ".c" )
    {
      arg = arg[0..<3];
    }
    
  if( (ob = find_object(arg)) && (!ob->remove()) )
    {
      info->cmdPipe->send("510 Failed to destruct original object.\n");
      return;
    }

  if( catch(load_object(arg)) )
    {
      info->cmdPipe->send("510 Error in loading object.\n");
      return;
    }
    
  RCPLOGF("%s MOVED %s.\n", capitalize(info->user), arg);
  info->cmdPipe->send(sprintf("510 %s: successfully loaded.\n",arg));
}


void remove()
{
  class rcp_session item;

  foreach(item in values(sessions))
    {
      if(objectp(item->cmdPipe))
	{
	  destruct(item->cmdPipe);
	}
    }

  destruct(sock);
}

int clean_up() {
 return 0;
}

string array list_users()
{
   return map(values(sessions), (: ((class rcp_session)$1)->connected ? 
				((class rcp_session)$1)->user : "(login)" :));
}
