/* Do not remove the headers from this file! see /USAGE for more info. */

/* For keeping track of sets of numbers.  See also the set_bit()/test_bit()
 * series of efuns.  The major advantage of this package is that the
 * storage requirements are much lower for sets with many consecutive
 * numbers, or for numbers which are sparsely distributed over a large
 * range.  It isn't particularly efficient speedwise, but it is compact.
 *
 * The format is: x-x,y-z ...
 */

string set_add(string set, int number) {
    int idx, dash, nextidx;
    int lastidx, lastfirst, lastlast = -10;
    
    if (set == "") return set + number + "-" + number;

    /* The cases we have to consider:
     1-2 -> 1-2,4-4
     1-2,4-5 -> 1-2,4-6
     1-3,5-7 -> 1-7
    */

    idx = 0;
    do {
	string part;
	int first, last;

	nextidx = member_array(',', set, idx) + 1;

	if (!nextidx)
	    part = set[idx..];
	else
	    part = set[idx..nextidx-2];
        sscanf(part, "%d-%d", first, last);
	if (number < first) {
	    string res;
	    
	    if (number == lastlast + 1) {
		if (number == first - 1) {
		    res = set[0..lastidx-1] + lastfirst + "-" + last;
		    if (nextidx) res += set[nextidx-1..];
		    return res;
		}
		return set[0..lastidx-1] + lastfirst + "-" + number + set[idx-1..];
	    }
	    if (number == first - 1) {
		res = set[0..idx-1] + number + "-" + last;
		if (nextidx) res += set[nextidx-1..];
		return res;
	    }
	    return set[0..idx-1] + number + "-" + number + set[idx-1..];
	}
	if (number <= last)
	    return set;
	
	lastidx = idx;
	lastfirst = first;
	lastlast = last;
	idx = nextidx;
    } while (idx);

    if (number == lastlast + 1)
	return set[0..lastidx-1] + lastfirst + "-" + number;
    else
	return set + "," + number + "-" + number;
}

string set_remove(string set, int number) {
    int idx, dash, nextidx;
    
    if (set == "") return set;

    idx = 0;
    do {
	string part;
	int first, last;

	nextidx = member_array(',', set, idx) + 1;

	if (!nextidx)
	    part = set[idx..];
	else
	    part = set[idx..nextidx-2];
        sscanf(part, "%d-%d", first, last);
	if (number <= last) {
	    string res;

	    if (number < first)
		return set;
	    
	    if (number == last) {
		if (number == first) {
		    if (idx) {
			res = set[0..idx-1];
			if (nextidx) res += set[nextidx-1..];
		    } else {
			res = (nextidx ? set[nextidx..] : "");
		    }
		    return res;
		}
		res = set[0..idx-1] + first + "-" + (last - 1);
		if (nextidx) res += set[nextidx-1..];
		return res;
	    }
	    if (number == first) {
		res = set[0..idx-1] + (first + 1) + "-" + last;
		if (nextidx) res += set[nextidx-1..];
		return res;
	    }
	    res = set[0..idx-1] + first + "-" + (number - 1) + "," + (number + 1) + "-" + last;
	    if (nextidx) res += set[nextidx-1..];
	    return res;
	}
	
	idx = nextidx;
    } while (idx);

    return set;
}

int set_member(string set, int number) {
    int idx, dash, nextidx;
    if (set == "") return 0;
    
    for (idx = 0, nextidx = member_array(',', set); 
	 idx != -1;
	 (idx = nextidx), (nextidx = member_array(',', set, idx + 1))) {
	string part;
	int first, last;

	if (nextidx == -1)
	    part = set[idx..];
	else
	    part = set[idx..nextidx-1];
	sscanf(part, "%d-%d", first, last);
	
	if (number < first)
	    return 0;
	if (number <= last)
	    return 1;
    }
}
