/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** grid_server.c -- a virtual server for grid-type rooms
**
** Format for the object is:
**
**	/serverfname/x/y
**
** The data for the server is pulled from the data file provided at
** create() time.  It is an array of description-id values (in the
** first N non-comment lines).  Comments are lines beginning with #.
** After the description array are lists of rooms for the boundaries
** of the grid.  They are in north, east, south, west order; one line
** per grid spot.
**
** Deathblade, 960101: created
*/

inherit DAEMON;

static private string * descriptions;
static private string * grid_desc_id;
static private string * edge_rooms;
static private string	brief_desc;

#define GRID_HEIGHT	sizeof(grid_desc_id)
#define GRID_WIDTH	sizeof(grid_desc_id[0])
#define GRID_ROOM(x,y)	(file_name() + "/" + (x) + "/" + (y))

#define ROOM_DESC(x,y)	descriptions[grid_desc_id[y][x] - '0']

#define NORTH_EDGE(x)	edge_rooms[x]
#define EAST_EDGE(y)	edge_rooms[(y)+GRID_WIDTH]
#define SOUTH_EDGE(x)	edge_rooms[(x)+GRID_WIDTH+GRID_HEIGHT]
#define WEST_EDGE(y)	edge_rooms[(y)+2*GRID_WIDTH+GRID_HEIGHT]


void set_descriptions(string * new_desc)
{
    descriptions = new_desc;
}

string query_brief()
{
    return brief_desc;
}
void set_brief(string new_brief)
{
    brief_desc = new_brief;
}

void use_data_file(string fname)
{
    string * lines;
    int i;

    lines = map(explode(read_file(fname), "\n"), (: trim_spaces :));
    lines = filter(lines - ({ "" }), (: $1[0] != '#' :));

    /* find the extent of the grid descriptions */
    for ( i = 0; lines[i][0..4] == "desc:" ; ++i )
	;
    grid_desc_id = map(lines[0..i-1], (: $1[5..] :));

    edge_rooms = lines[i..];
}

void setup()
{
}
object virtual_create(string arg)
{
    int idx = member_array('/', arg);
    int x;
    int y;
    string exit_n;
    string exit_e;
    string exit_s;
    string exit_w;
    object room;

setup();
    if ( idx == -1 )
	return 0;

    x = to_int(arg[0..idx-1]);
    y = to_int(arg[idx+1..]);

    if ( y == 0 )
	exit_n = NORTH_EDGE(x);
    else if ( y == GRID_HEIGHT - 1 )
	exit_s = SOUTH_EDGE(x);

    if ( !exit_n )
	exit_n = GRID_ROOM(x,y-1);
    if ( !exit_s )
	exit_s = GRID_ROOM(x,y+1);

    if ( x == 0 )
	exit_w = WEST_EDGE(y);
    else if ( x == GRID_WIDTH - 1 )
	exit_e = EAST_EDGE(y);

    if ( !exit_w )
	exit_w = GRID_ROOM(x-1,y);
    if ( !exit_e )
	exit_e = GRID_ROOM(x+1,y);

    room = new(ROOM);
    room->set_brief(brief_desc);
    room->set_long(ROOM_DESC(x, y));
    room->add_exit("north", exit_n);
    room->add_exit("east", exit_e);
    room->add_exit("south", exit_s);
    room->add_exit("west", exit_w);


    return room;
}

