/* Do not remove the headers from this file! see /USAGE for more info. */
// Original version by Beek
// Updated version by Ohara

#define ESC "\e"
#define SAVE ESC + "7"
#define CLEAR ESC + "[1J"
#define RESTORE ESC + "8"
#define HOME ESC + "[1;1f"
#define TOP ESC + "[2;r"
#define FULL ESC + "[1;r"
#define LIM ESC + "[1;2r"
#define RESET ESC + "[0m"
#define REVERSE ESC + "[7m"


void update_status_line()
{
    string p;
    p = SAVE + HOME + LIM + REVERSE + " " +
    ljust( environment( this_object())->query_light()?environment(this_object())->get_brief():"Darkness", this_user()->query_screen_width() - 10) +
    rjust( this_object()->query_score()+"/" + QUEST_D->total_points() + " ", 10) +
    RESET + TOP + RESTORE;
    tell(this_object(), p, NO_WRAP);
}

int has_status_line()
{
    return get_user_variable("status") != 0;
}


void remove_status_line()
{
    write(SAVE + HOME + FULL + RESTORE);
}
