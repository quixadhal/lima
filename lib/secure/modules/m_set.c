/* Do not remove the headers from this file! see /USAGE for more info. */

/* For keeping track of sets of numbers.  See also the set_bit()/test_bit()
 * series of efuns.  The major advantage of this package is that the
 * storage requirements are much lower for sets with many consecutive
 * numbers, or for numbers which are sparsely distributed over a large
 * range.  It isn't particularly efficient speedwise, but it is compact.
 *
 * The format is: x-x,y-z ...
 */

/* This function ensures that the sets aren't currupted.  It returns 1 
 * if the set is valid and 0 on failure.
 * There may need to be more work done here -- Tigran */
int valid_set(string set)
{
  int part1, part2;
  string rest;
  string array parts=explode(set,",");
  if(set="") return 1;
  foreach(string part in parts)
    {
      if(sscanf("%d-%d%s",part1,part2,rest)!=2)
	return 0;
    }
  return 1;
} 

string set_add(string set, int number) {
    int idx, nextidx;
    int lastidx, lastfirst, lastlast = -10;
    
    if(!valid_set(set))
      error(sprintf("Invalid set in set_add: %s\n",set));
    if (set == "") return number + "-" + number;

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
            return set[0..idx-1] + number + "-" + number + "," + set[idx..];
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

string set_add_range(string set, int nfirst, int nlast) {
    int idx, nextidx;
    int lastidx, lastfirst, lastlast = -10;
    
    if(!valid_set(set))
      error(sprintf("Invalid set in set_add_range: %s\n",set));
    if (set == "") return nfirst + "-" + nlast;

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
        if (nfirst < first) {
            string res;
	    int sidx;

	    sidx = idx;
	    while (nextidx && nlast > last) {
		idx = nextidx;
		nextidx = member_array(',', set, idx) + 1;

		if (!nextidx)
		    part = set[idx..];
		else
		    part = set[idx..nextidx-2];
		sscanf(part, "%d-%d", first, last);
	    }
            
            if (nfirst <= lastlast + 1) {
                if (nlast >= first - 1) {
                    res = set[0..lastidx-1] + lastfirst + "-" + last;
                    if (nextidx) res += set[nextidx-1..];
                    return res;
                }
                return set[0..lastidx-1] + lastfirst + "-" + nlast + set[idx-1..];
            }
            if (nlast >= first - 1) {
                res = set[0..sidx-1] + nfirst + "-" + last;
                if (nextidx) res += set[nextidx-1..];
                return res;
            }
            return set[0..sidx-1] + nfirst + "-" + nlast + set[idx-1..];
        }
        if (nlast <= last)
            return set;
        
        lastidx = idx;
        lastfirst = first;
        lastlast = last;
        idx = nextidx;
    } while (idx);

    if (nfirst <= lastlast + 1)
        return set[0..lastidx-1] + lastfirst + "-" + nlast;
    else
        return set + "," + nfirst + "-" + nlast;
}

string set_remove(string set, int number) {
    int idx, nextidx;

    if(!valid_set(set))
      error(sprintf("Invalid set in set_remove: %s\n",set));
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
    string array set_parts;

    if(!valid_set(set))
      error(sprintf("Invalid set in set_member: %s\n",set));
    set_parts=explode(set,",");
    foreach(string element in set_parts)
      {
	int first;
	int last;
	sscanf(element,"%d-%d",first,last);
	if(number<first)
	  continue;
	if(number<=last)
	  return 1;
      }
}


/* More filter() friendly interface */
int member_set(int number,string set)
{
  return set_member(set,number);
}

string set_union(string set1, string set2)
{
    string res = "";
    int f1 = -1, f2 = -1, l1, l2, handled;

    if(!valid_set(set1))
      error(sprintf("Invalid first set in set_union: %s\n",set1));
    if(!valid_set(set2))
      error(sprintf("Invalid second set in set_union: %s\n",set2));
    sscanf(set1, "%d-%d%s", f1, l1, set1);
    if (set1[0] == ',') set1 = set1[1..];
    sscanf(set2, "%d-%d%s", f2, l2, set2);
    if (set2[0] == ',') set2 = set2[1..];
    handled = 0;
    
    while (f1 != -1 && f2 != -1) {
	if (l1 < f2) {
	    res += (res == "" ? "" : ",") + f1 + "-" + l1;
	    f1 = -1;
	    sscanf(set1, "%d-%d%s", f1, l1, set1);
	    if (set1[0] == ',') set1 = set1[1..];
	} else if (l2 < f1) {
	    res += (res == "" ? "" : ",") + f2 + "-" + l2;
	    f2 = -1;
	    sscanf(set2, "%d-%d%s", f2, l2, set2);
	    if (set2[0] == ',') set2 = set2[1..];
	} else {
	    int a = (f1 < f2 ? f1 : f2);
	    int b = (l1 > l2 ? l1 : l2);

	    while (1) {
		if (f1 != -1 && f1 <= b) {
		    f1 = -1;
		    sscanf(set1, "%d-%d%s", f1, l1, set1);
		    if (set1[0] == ',') set1 = set1[1..];
		    if (f1 <= b && l1 > b)
			b = l1;
		} else
		if (f2 != -1 && f2 <= b) {
		    f2 = -1;
		    sscanf(set2, "%d-%d%s", f2, l2, set2);
		    if (set2[0] == ',') set2 = set2[1..];
		    if (f2 <= b && l2 > b)
			b = l2;
		} else
		    break;
	    }

	    res += (res == "" ? "" : ",") + a + "-" + b;
	}
    }
    if (f1 != -1)
	res += (res == "" ? "" : ",") + f1 + "-" + l1;
    if (f2 != -1)
	res += (res == "" ? "" : ",") + f2 + "-" + l2;
    if (set1 != "")
	res += (res == "" ? "" : ",") + set1;
    if (set2 != "")
	res += (res == "" ? "" : ",") + set2;

    return res;
}

string set_difference(string set1, string set2)
{
  string res = "";
  int f1 = -1, f2 = -1, l1, l2;
    if(!valid_set(set1))
      error(sprintf("Invalid first set in set_difference: %s\n",set1));
    if(!valid_set(set2))
      error(sprintf("Invalid second set in set_difference: %s\n",set2));
  sscanf(set1, "%d-%d%s", f1, l1, set1);
  sscanf(set2, "%d-%d%s", f2, l2, set2);  
  while (f1 != -1) 
    {
      if(set1[0]==',') set1=set1[1..];
      if(set2[0]==',') set2=set2[1..];
      if(f2==-1)
	{
	  /* This might want to be reconstructed as string addition to 
	   * conform with the rest of the function, but right now it
	   * works, and I can deal better w/ sprintf()S -- Tigran */
	  res+=sprintf("%s%i-%i%s%s",
		       (res==""?"":","),
		       f1,
		       l1,
		       set1==""?"":",",
		       set1);
	  f1=-1;
	  continue;
	}
      else if (l1 < f2) 
	{
	  res += (res == "" ? "" : ",") + f1 + "-" + l1;
	  f1 = -1;
	  sscanf(set1, "%d-%d%s", f1, l1, set1);
	  if (set1[0] == ',') set1 = set1[1..];
	}
      else if (l2 < f1) 
	{
	  f2 = -1;
	  sscanf(set2, "%d-%d%s", f2, l2, set2);
	  if (set2[0] == ',') set2 = set2[1..];
	} 
      else 
	{
	  if (f1 < f2)
	    {
	      res += (res == "" ? "" : ",") + f1 + "-" + (f2-1);
	      f1=-1;
	      sscanf(set1,"%d-%d%s",f1,l1,set1);
	    }
	  if (l2 < l1)
	    {
	      f1 = l2 + 1;
	      f2=-1;
	      sscanf(set2, "%d-%d%s", f2, l2, set2);
	    }
	  else if (l1 < l2)
	    {
	      f1 = -1;
	      sscanf(set1, "%d-%d%s", f1, l1, set1);
	      if (set1[0] == ',') set1 = set1[1..];
	    } 
	  else if ( f1==f2 ) 
	    {
	      if(l1==l2)
		{
		  f1=-1;
		  sscanf(set1,"%d-%d%s",f1,l1,set1);
		  f2=-1;
		  sscanf(set2,"%d-%d%s",f2,l2,set2);
		}
	    }
	  else 
	    {
	      f2 = -1;
	      sscanf(set2, "%d-%d%s", f2, l2, set2);
	      if (set2[0] == ',') set2 = set2[1..];
	    }
	}
    }
  return res;
}

string set_intersection(string set1, string set2) {
    string res = "";
    int f1 = -1, f2 = -1, l1, l2;
    if(!valid_set(set1))
      error(sprintf("Invalid first set in set_intersection: %s\n",set1));
    if(!valid_set(set2))
      error(sprintf("Invalid second set in set_intersection: %s\n",set2));
    sscanf(set1, "%d-%d%s", f1, l1, set1);
    if (set1[0] == ',') set1 = set1[1..];
    sscanf(set2, "%d-%d%s", f2, l2, set2);
    if (set2[0] == ',') set2 = set2[1..];
    
    while (f1 != -1 && f2 != -1) {
	if (l1 < f2) {
	    f1 = -1;
	    sscanf(set1, "%d-%d%s", f1, l1, set1);
	    if (set1[0] == ',') set1 = set1[1..];
	} else if (l2 < f1) {
	    f2 = -1;
	    sscanf(set2, "%d-%d%s", f2, l2, set2);
	    if (set2[0] == ',') set2 = set2[1..];
	} else {
	    res += (res == "" ? "" : ",") + 
		(f1 > f2 ? f1 : f2) + "-" + (l1 < l2 ? l1 : l2);
	    if (l1 < l2) {
		f1 = -1;
		sscanf(set1, "%d-%d%s", f1, l1, set1);
		if (set1[0] == ',') set1 = set1[1..];
	    } else {
		f2 = -1;
		sscanf(set2, "%d-%d%s", f2, l2, set2);
		if (set2[0] == ',') set2 = set2[1..];
	    }
	}
    }
    return res;
}

int set_min(string set) {
    int ret = 0;
    if(!valid_set(set))
      error(sprintf("Invalid set in set_min: %s\n",set));
    sscanf(set, "%d", ret);
    return ret;
}

int set_max(string set) {
    int idx = strsrch(set, ",", -1);
    int ret;
    if(!valid_set(set))
      error(sprintf("Invalid set in set_max: %s\n",set));
    sscanf(set[idx+1..], "%*d-%d", ret);
    
    return ret;
}

int set_count(string set) {
    int f, l;
    int ret = 0;
    
    if(!valid_set(set))
      error(sprintf("Invalid set in set_count: %s\n",set));
    while (strlen(set)) {
	sscanf(set, "%d-%d%s", f, l, set);
	if (set[0] == ',') set = set[1..];

	ret += l - f + 1;
    }

    return ret;
}
