private nomask
confirm_new_password(string s1, string s2)
{
  write("\n");
  this_body()->modal_pop();
  if(s1 != s2)
    {
      write("Passwords must match.\n");
      return;
    }
  this_user()->set("password", s1);
  write("Password changed.\n");
}

private nomask
get_new_password(string s)
{
  write("\n");
  this_body()->modal_pop();
  if(strlen(s) < 5)
    {
      write("Your password must be 5 characters or more.\nAborting.\n");
      return;
    }
  this_body()->modal_push( (: confirm_new_password, s :),
			  (: "Again to confirm: " :), 1 );
}


private nomask
confirm_current_password(string s)
{
  write("\n");
  this_body()->modal_pop();
  if(!this_user()->matches_password(s))
    {
      write("Invalid password.\n");
      return;
    }
  this_body()->modal_push( (: get_new_password :),
			  (: "New password: " :), 1);
}


void
main()
{
  this_body()->modal_push( (: confirm_current_password :), 
			  (: "Enter your current password: ":), 1);
}
