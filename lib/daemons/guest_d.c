/* Do not remove the headers from this file! see /USAGE for more info. */

// File for Allowing visitors (new users) into a locked mud
// i.e. NO_NEW_PLAYERS is defined
// Vette  April 4, 1997
//

inherit M_DAEMON_DATA;

private string * guests;

void clear_guests() { 
  guests = ({}); 
  save_me();
}

void add_guest(string g) {
  if (!guests) clear_guests();
  if (member_array(g,guests) == -1) {
     guests += ({g});
  }
  save_me();
}

void remove_guest(string g) {
  if (!guests) clear_guests();
  guests -= ({g});
  save_me();
}

int guest_exists(string g) {
  if (!guests) return 0;
  if (member_array(g,guests) == -1) return 0;
  return 1;
}

string * query_guests() {
  if (!guests) return ({});
  return guests;
}
