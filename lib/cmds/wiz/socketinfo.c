/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: netstat
//USAGE socketinfo
//
//Displays details of sockets

inherit CMD;

private void main(string str) {
    string *lines = explode(dump_socket_status(), "\n");
    string line;
    
    foreach (line in lines) {
        if (line[0..1] != "-1")
            out(line + "\n");
    }
}
