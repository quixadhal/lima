/* Do not remove the headers from this file! see /USAGE for more info. */

#include <socket.h>
#include <tkinter.h>
#include <security.h>

static object tkinter_socket;
string password;
static mapping interfaces = ([]);

inherit M_ACCESS;

private void handle_tkinter_close(object socket)
{
  interfaces[socket]->remove();
  socket->remove();
}

private void handle_tkinter_read(object socket, string input)
{
  object interface, link;
  string username, pwd;

  if(!input)
    {
      return;
    }
  BBUG(input);
  if(!(interface = interfaces[socket]))
    {
      sscanf(input, "%s %s", username, pwd);
      if(!link = find_body(username))
	{
	  socket->remove();
	  return;
	}
      unguarded(1, (: restore_object(LINK_PATH($(username))) :));
      if(password != crypt(pwd, password))
	{
	  socket->remove();
	  return;
	}
      interfaces[socket] = new(TK_INTERFACE, link, socket);
      BBUG(interfaces[socket]);
    }
  interfaces[socket]->receive_tk_request(input);
}



void create()
{
    string err;

    if (clonep()) destruct(this_object());

    set_privilege(1);

    err = catch(tkinter_socket = new(SOCKET, SKT_STYLE_LISTEN,
				 TKINTER_PORT,
				 (: handle_tkinter_read :),
				 (: handle_tkinter_close :)));
    if ( err )
    {
	tkinter_socket->remove();
	error(err);
	destruct(this_object());
    }
}
