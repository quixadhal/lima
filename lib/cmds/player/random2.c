/* Do not remove the headers from this file! see /USAGE for more info. */

#include <driver/runtime_config.h>
inherit CMD;

mapping data = ([ ]);
mapping reverse = ([ ]);

array todo;

void process() {
    array block = todo[0..99];
    mapping m;
    
    todo = todo[100..];
    if (sizeof(todo))
	call_out( (: process :), 2);
    
    foreach (string emote in block) {
	string soul = SOUL_D->query_emote(emote);
	
	foreach (string rule, string value in soul) {
	    array parts;
	    string l1, l2, tmp;

	    if (!stringp(value)) continue;
	    
	    parts = explode(value, " ");

	    if (sizeof(parts) <= 3) continue;
	    
	    l1 = l2 = 0;
	    
	    foreach (string part in parts + ({ 0 })) {
		tmp = l2 + " " + l1;
		if (!data[tmp])
		    data[tmp] = ([ ]);

		m = data[tmp];
		m[part]++;
		m[1]++;

		tmp = part + " " + l1;
                if (!reverse[tmp]) {
                    if ( sizeof(reverse) >= get_config(__MAX_MAPPING_SIZE__) )
                        return;
		    reverse[tmp] = ([ ]);
                }

		m = reverse[tmp];
		m[l2]++;
		m[1]++;

		l2 = l1;
		l1 = part;
	    }
	}
    }
}

string get_part(string l2, string l1) {
    string tmp = l2 + " " + l1;
    mapping d = data[tmp];
    int c = d[1];
    int hmm = random(c);

    foreach (string part, int num in d) {
	if (!stringp(part)) continue; /* part == 1 */
	
	hmm -= num;
	if (hmm < 0) return part;
    }
}

string get_prev_part(string l2, string l1) {
    string tmp = l2 + " " + l1;
    mapping d = reverse[tmp];
    int c = d[1];
    int hmm = random(c);

    foreach (string part, int num in d) {
	if (!stringp(part)) continue; /* part == 1 */

	hmm -= num;
	if (hmm < 0) return part;
    }
}

void do_the_nasty_deed(string ret) {
    string adv = choice(SOUL_D->get_adverbs());
    object target = choice(filter(bodies(), (: environment($1) == environment(this_body()) :)));

    this_body()->targetted_action(ret, target, adv);
}

private void main(string str) {
    string l1 = 0;
    string l2 = 0;
    string part;
    string ret;

    if (str) {
	array choices = map(filter(keys(data), (: strsrch($1, $(str) + " ") == 0 :)), (: $1[strsrch($1, ' ')+1..] :));
	string next;
	
	if (!sizeof(choices)) {
	    write("Nothing appropriate.\n");
	    return;
	}
	next = choice(choices);

	l2 = next;
	l1 = str;

	while (1) {
	    part = get_prev_part(l2, l1);
	    if (!part) break;
	    if (ret)
		ret = part + " " + ret;
	    else
		ret = part;
	    l2 = l1;
	    l1 = part;
	}
	if (ret) ret += " " + str + " " + next;
	else ret = str + " " + next;

	l2 = str;
	l1 = next;
    }

    while (1) {
	part = get_part(l2, l1);
	if (!part) break;
	if (ret)
	    ret += " " + part;
	else
	    ret = part;
	l2 = l1;
	l1 = part;
    }
    do_the_nasty_deed(ret);
}

void create() {
    todo = SOUL_D->list_emotes();
    process();
}
