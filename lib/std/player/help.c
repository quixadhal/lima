/* Do not remove the headers from this file! see /USAGE for more info. */

//
// help module for player object.  really simple...
//
// query_help_topic(str)  -- return time <str> was read. if not read, return 0
// read_help_topic(str)   -- set time <str> was read to right now.
//

mapping topic_to_time = ([ ]);

void
read_help_topic(string str) {
   topic_to_time[str] = time();
}

int
query_help_topic(string str) {
   return topic_to_time[str];
}
