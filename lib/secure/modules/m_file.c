/* Do not remove the headers from this file! see /USAGE for more info. */

#define BUFFER_SIZE 8192

class file_info {
    string fname;
    string mode;
    int pos;

    int buffered;
    int bpos;
    int bchanged;
    string buf;
}

mapping open_files = ([]);
class file_info cur_file;

private class file_info find_file(string fn) {
    if (fn) {
	cur_file = open_files[fn];
	if (!cur_file)
	    error("No such open file '" + fn + "'.\n");
    }
    return cur_file;
}


private void flush_buffer(class file_info fi) {
    if (fi->bchanged) {
	write_bytes(fi->fname, fi->pos, fi->buf);
	fi->bchanged = 0;
    }
}

private void refill_buffer(class file_info fi) {
    flush_buffer(fi);
    fi->buf = read_bytes(fi->fname, fi->pos, BUFFER_SIZE);
    fi->bpos = 0;
    fi->buffered = sizeof(fi->buf);
}

void file_open(string fn, string mode) {
    int fs = file_size(fn);
    int p;
    
    if (fs == -2)
	error("file_open(): Is a directory.\n");
    switch (mode) {
    case "r":
	if (fs == -1)
	    error("file_open(): No such file.\n");
	break;
    case "w":
    case "rw":
	break;
    case "a":	
	p = fs;
    }
    
    open_files[fn] = cur_file = new(class file_info,
				    fname : fn,
				    pos : p);
}

string file_read(string fn, int num) {
    class file_info fi = find_file(fn);
    string ret;
    
    if (fi->mode[0] != 'r')
	error("file_read(): File not open for reading.\n");
    if (num <= fi->buffered) {
	ret = fi->buf[fi->bpos..fi->bpos + num - 1];
	fi->bpos += num;
	fi->pos += num;
	fi->buffered -= num;
	return ret;
    } else {
	ret = fi->buf[fi->bpos..];
	fi->pos += fi->buffered;
	num -= fi->buffered;
	while (num > BUFFER_SIZE) {
	    refill_buffer(fi);
	    ret += fi->buf;
	    fi->pos += fi->buffered;
	    num -= fi->buffered;
	    if (fi->buffered != BUFFER_SIZE)
		break;
	}
	refill_buffer(fi);
	if (num > fi->buffered) {
	    ret += fi->buf;
	    fi->pos += fi->buffered;
	    fi->buffered = 0;
	    fi->bpos += fi->buffered;
	} else {
	    ret += fi->buf[0..num-1];
	    fi->pos += num;
	    fi->bpos += num;
	    fi->buffered -= num;
	}
	return ret;
    }
}

string file_readline(string fn) {
    class file_info fi = find_file(fn);
    string ret;
    int where;
    
    if (fi->mode[0] != 'r')
	error("file_read(): File not open for reading.\n");
    where = member_array('\n', fi->buf, fi->bpos);
    if (where != -1) {
	ret = fi->buf[fi->bpos..where - 1];
	fi->bpos = where + 1;
	fi->pos += (where + 1 - fi->bpos);
	fi->buffered -= (where + 1 - fi->bpos);
	return ret;
    } else {
	ret = fi->buf[fi->bpos..];
	fi->pos += fi->buffered;
	while (1) {
	    refill_buffer(fi);
	    where = member_array('\n', fi->buf, fi->bpos);
	    if (where != -1) {
		ret = fi->buf[fi->bpos..where - 1];
		fi->bpos = where + 1;
		fi->pos += (where + 1 - fi->bpos);
		fi->buffered -= (where + 1 - fi->bpos);
		return ret;
	    }
	    ret += fi->buf;
	    fi->pos += fi->buffered;
	    if (fi->buffered != BUFFER_SIZE)
		return ret;
	}
    }
}

void file_seek(string fn, int dist, string how) {
    class file_info fi = find_file(fn);
    int rel;
    
    if (fi->mode == "a")
	error("file_seek(): Cannot seek on a file opened for append.\n");
    
    switch (how) {
    case "start":
	rel = dist - fi->pos;
	break;
    case "cur":
	rel = dist;
	break;
    case "end":
	rel = file_size(fi->fname) + dist - fi->pos;
    }
    if (rel + fi->pos < 0)
	rel = -fi->pos;
    
    fi->pos += rel;
    if (rel > fi->buffered || (-rel) > fi->bpos) {
	refill_buffer(fi);
    } else {
	fi->bpos += rel;
	fi->buffered -= rel;
    }
}

void file_close(string fn) {
    class file_info fi = find_file(fn);

    flush_buffer(fi);
    map_delete(open_files, fn);
}

void file_flush(string fn) {
    class file_info fi = find_file(fn);

    flush_buffer(fi);
}

int file_pos(string fn) {
    class file_info fi = find_file(fn);
    
    return fi->pos;
}
