/* Do not remove the headers from this file! see /USAGE for more info. */

// A non-line-mode editor for LP Muds...
// Rust@Lima Bean (viega@list.org) 1-26-97
//
// This still wants a file browser embedded, and a lot of cosmetic overhaul.

#include <ports.h>

#define MY_URL sprintf("http://%s:%d/scgi/webed.c", __HOST__, PORT_HTTP)
#include <security.h>

inherit M_ACCESS;

// These functions need to be in an inheritable...

private mixed get_priv(mapping form)
{
  if(adminp(form["user"]))
    {
      return 1;
    }
  return form["user"];
}

private string HTML_encode(string str) 
{
    string retval = "";
    int len = strlen(str);

    for(int i=0; i<len; i++)
      {
	switch(str[i])
	  {
	  case '&':
	    retval += "&amp;";
	    break;
	  case '<':
	    retval += "&lt;";
	    break;
	  case '>':
	    retval += "&gt;";
	    break;
	  case '"':
	    retval += "&quot;";
	    break;
	  default:
	    retval += str[i..i];
	    break;
	  }
      }
    return retval;
}


string format_editor(string user, string password, string filename, 
		     string contents)
{

  string output;

  if(!user)
    user = "";
  if(!password)
    password = "";
  output = "<html><title>Web Ed</title><body><h1>Web Ed - An LP Mud editor</h1>\n";
  output += sprintf("<form action='%s' method='POST'>\n", MY_URL);
  output +="<table><tr><td>";
  output += sprintf("Wizard name: </td><td><INPUT name=user type=TEXT value='%s' "
		    "size=10><p>\n</td></tr>", HTML_encode(user));
  output += sprintf("<tr><td>Password:</td><td> <INPUT name=password type=PASSWORD value='%s' "
		    "size=10><p></td></tr>\n", HTML_encode(password));
  output += "<tr><td>File To Load:</td><td> <INPUT name=loadfile type=TEXT size=60 value=''></td> \n";  output +="<td><INPUT name=load type=submit value='Load File'></td></tr>\n";


  if(contents)
    {
      output += sprintf("<tr><td>Save As:</td><td> <input type=text name=saveas size=60 value=%s> </td>\n", HTML_encode(filename));
      output +="<td><INPUT name=save type=submit value='Save File'></td></tr>\n";
      output +="<tr><td></td><td></td><td><INPUT name=reload type=submit value=Reload File></td></tr>\n<p>";
    }
  output = output + "</table>\n";
  if (contents)
    {
	array inh;
	object ob;

	output +="<hr><font size=+1><strong>Editing: " + HTML_encode(filename) + "</strong></font><p>";
	/*
	if ((ob = load_object(filename)) && sizeof(inh = inherit_list(ob))) {
	    string tmp;
	    
	    inh = ({ "<OPTION VALUE='" + HTML_encode(inh[0]) + "' SELECTED>" + HTML_encode(inh[0]) }) + map(inh[1..], (: "<OPTION VALUE='"+HTML_encode($1)+"'>"+HTML_encode($1) :));
	    tmp = implode(inh, " ");
	    
	    output += "<SELECT NAME='inh'>" + tmp + "</SELECT>";
	    output += "<INPUT name=loadinh type=submit value='Load Inherited File'><p>";
	}
	*/

      output += sprintf("<TEXTAREA name=contents ROWS=36 COLS=80 WRAP=off>"
			"%s</TEXTAREA>\n", HTML_encode(contents));


    }
  output += "</body></html>";
  return output;
}


string display_error(string error, mapping form)
{
  return sprintf("<table border=1><tr><td><font size=+2>%s</font></td></tr>"
		 "</table><p>%s\n", error, 
		 format_editor(form["user"], form["password"], form["saveas"],
			       form["contents"]));
}

int validate_user(mapping form)
{
  string array info;
  string val;

  if(!form["user"] || !form["password"])
    {
      return 0;
    }
  if(!wizardp(form["user"]))
    {
      return 0;
    }
  set_privilege(1);
  info = unguarded(1, (:USER_D->query_variable($(form["user"]),
					       ({"password"})):));
  set_privilege(0);

  if(!info)
    {
      return 0;
    }
  val = crypt(form["password"], info[0]);
  if(val  == info[0])
    {
      if(adminp(form["user"]))
	{
	  set_privilege(1);
	}
      else
	{
	  set_privilege(form["user"]);
	}
      return 1;
    }
  return 0;
}

// Error checking all over this function, please.
string save_text_to_file(mapping form)
{
  string filename = form["saveas"];
  string contents = replace_string(form["contents"], "\r\n", "\n");
  int result;

  if(!validate_user(form))
    {
      return display_error("Login info is incorrect", form);
    }

  result = unguarded(get_priv(form), (: write_file($(filename), 
                                          $(contents), 1) :));

  if(!result)
    {
      return display_error(sprintf("%s: COULD NOT SAVE FILE.\n", filename), 
			   form);
    }
  return format_editor(form["user"], form["password"], filename, 
		       form["contents"]);
}


string load_file(mapping form)
{
  string filename;
  string text;

  if(!validate_user(form))
    {
      return display_error("Login info is incorrect", form);
    }

  filename = evaluate_path(form["loadfile"]);
  if(is_directory(filename))
    {
      return display_error(sprintf("%s: Is a DIRECTORY.\n", filename), form);
    }
  if(!is_file(filename))
    {
      form["contents"] = "";
      form["saveas"] = filename;
      return display_error(sprintf("%s: New file\n", filename), form);
    }
  text = unguarded(get_priv(form), (: read_file($(filename)) :));
  if(!text)
    {
      return display_error(sprintf("%s: Permission to read denied.\n", 
				   filename), form);
    }
  else
    {
      form["saveas"] = filename;
      return format_editor(form["user"], form["password"], filename, text);
    }
}

string reload_file(mapping form)
{
  form["loadfile"] = form["saveas"];
  return load_file(form);
}

string main(mapping form)
{
  string fname;

  set_privilege(0); /* Just in case */
  if(!mapp(form))
    {
      return format_editor(0,0,0,0);
    }
  if(form["save"])
    {
      return save_text_to_file(form);
    }
  else
    {
      if(form["reload"])
	{
	  return reload_file(form);
	}
      else
	{
	  if(form["load"])
	    {
	      return load_file(form);
	    }
	  else
	    {
		if (form["loadinh"]) {
		    form["loadfile"] = form["inh"];

		    return load_file(form);
		} else
		    return format_editor(0,0,0,0);
	    }
	}
    }
}
