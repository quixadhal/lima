/* Do not remove the headers from this file! see /USAGE for more info. */

#define SIZE 10

string array data;

int is_valid(int x, int y) {
    if (x < 0 || x >= SIZE) return 0;
    if (y < 0 || y >= SIZE) return 0;
    return 1;
}

int value(int x, int y) {
    if (!is_valid(x, y)) return '#';
    return data[x][y];
}

int dx(int i) {
    if (i == 1) return 1;
    if (i == 3) return -1;
    return 0;
}

int dy(int i) {
    if (i == 0) return -1;
    if (i == 2) return 1;
    return 0;
}

void setup() {
    array valid = allocate(4);
    int x, y;
    
    data = allocate(SIZE);
    for (int i = 0; i < SIZE; i++) {
	data[i] = sprintf("%-*'.'s", SIZE, "");
    }

    data[0][0] = '@';
    while (1) {
	int num_valid = 0;

	for (int i = 0; i < 4; i++) {
	    if (value(x + 2*dx(i), y + 2 * dy(i)) == '.')
		valid[num_valid++] = i;
	}
	if (num_valid) {
	    int d = valid[random(num_valid)];
	    
	    data[x+dx(d)][y+dy(d)] = ' ';
	    x += 2*dx(d);
	    y += 2*dy(d);
	    data[x][y] = (' ' + d);
	} else {
	    int last = data[x][y] - ' ';
	    if (last == ('@' - ' ')) {
		data[0][0] = ' ';
		break;
	    }
	    data[x][y] = ' ';
	    x -= 2*dx(last);
	    y -= 2*dy(last);
	}
    }
    foreach (string str in data) {
	write(str + "\n");
    }
}

object virtual_create(string arg) {
    int x, y;

    if (sscanf(arg, "%d,%d", x, y) != 2)
	return 0;
    return new(__DIR__ "labroom", x, y, value(x,y-1), value(x+1,y), value(x,y+1), value(x-1,y));
}

int entrance_has_east() {
    return data[1][0] == ' ';
}

int entrance_has_south() {
    return data[0][1] == ' ';
}
