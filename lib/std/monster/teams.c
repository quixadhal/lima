/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE
// This module supports setting up teams
// Currenlty you need to write your own custom support for using this
// info, but i've made this file so you needent add to the basic code.
// If you have anything you'd like added to this, let me know.
// Functions for Team/Alignment Support
//
// This module is activated using USE_TEAMS definition in config
// Vette April 15, 1997
//

private string team;

string query_team() {
  return team;
}

void set_team(string str) {
  team = str;
}
