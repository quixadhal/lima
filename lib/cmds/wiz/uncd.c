/* Do not remove the headers from this file! see /USAGE for more info. */

/* Beek: Nov 2 96 */

inherit CMD;

private void main() {
    object ob = this_user()->query_shell_ob();
    
    ob->swap_pwd();

    outf("new cwd: %s\n", ob->get_variable("pwd") || "NONE");
}
