/* Do not remove the headers from this file! see /USAGE for more info. */
// Original version by Beek
// Updated version by Ohara

inherit M_ANSI;


void update_status_line()
{
    string p;
    p=( ansi("%^SAVE%^\e[1;1f\e[1;2r\e[7m" +	" " + ljust( environment( this_object())->query_light() ? environment( this_object())->get_brief() : "Darkness", 69 ) +	rjust( this_object()->query_score() + "/" + QUEST_D->total_points() +" ", 10 ) + "%^RESET%^\e[1;1f\e[2;24r%^RESTORE%^"));
    tell_object( this_object(), p );
}


int has_status_line()
{
    return( (this_object()) && (this_object()->query_shell_ob()) && (this_object()->query_shell_ob()->get_variable( "status" ) != 0));
}


void remove_status_line()
{
    write( ansi( "%^SAVE%^\e[1;1f\e[1;24r%^RESTORE%^"));
}
