/* Do not remove the headers from this file! see /USAGE for more info. */

string main() 
{
  string	ret = DID_D->get_changelog_for_web();
  
  ret = "<title>Changes since last release</title>\n"
        "<h2>Changes since the last LIMA release</h2>\n"
	 + ret + "\n";
  
  return ret;
}
