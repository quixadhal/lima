/*
 *  Parsing efuns.  Many of the concepts and algorithms here are stolen from
 *  an earlier LPC parser written by Rust@ZorkMUD.
 */
/*
 * TODO:
 * . he, she, it, him, her, them -> "look at tempress.  get sword.  kill her with it"
 * . compound input -> "n. e then s."
 * . OBS: and, all and everything (all [of] X, X except [for] Y, X and Y)
 * . OBS in OBS
 * . possesive: my his her its their Beek's
 * . first, second, etc
 * . one, two, ...
 * . questions.  'Take what?'
 * . oops
 * . the 'her' ambiguity
 * . me, myself
 * . foo, who is ...   foo, where is ...   foo, what is ...   foo, go south
 * . where is ... "where is sword" -> "In the bag on the table"
 */

#include "std.h"
#include "lpc_incl.h"
#include "parser.h"

static parse_info_t *pi = 0;
static hash_entry_t *hash_table[HASH_SIZE];
static verb_t *verbs[VERB_HASH_SIZE];
static int objects_loaded = 0;
static int num_objects, num_people;
static struct object_s *loaded_objects[MAX_NUM_OBJECTS];
static int num_literals = -1;
static char **literals;
static word_t words[256];
static int num_words = 0;
static verb_node_t *parse_vn;
static verb_t *parse_verb_entry;
static object_t *parse_restricted;
static object_t *parse_user;
static unsigned int cur_livings[NUM_BITVEC_INTS];
static int best_match;
static int best_error_match;
static int best_num_errors;
static parse_result_t *best_result = 0;
static match_t matches[10];
static char *current_error = 0;
static char *the_word, *a_word;

static void parse_rule PROT((parse_state_t *));

#define isignore(x) (!isascii(x) || !isprint(x) || x == '\'')
#define iskeep(x) (isalnum(x) || x == '*')

/* parse_init() - setup the object
 * parse_refresh() - refresh an object's parse data
 * parse_add_rule(verb, rule, handler) - add a rule for a verb
 * parse_sentence(sent) - do the parsing :)
 */

INLINE static void bitvec_copy P2(unsigned int *, b1, unsigned int *, b2) {
    int i;
    for (i = 0; i < NUM_BITVEC_INTS; i++)
	b1[i] = b2[i];
}

INLINE static void bitvec_zero P1(unsigned int *, bv) {
    int i;
    for (i = 0; i < NUM_BITVEC_INTS; i++)
	bv[i] = 0;
}

INLINE static int intersect P2(unsigned int *, bv1, unsigned int *, bv2) {
    int i, found = 0;

    for (i = 0; i < NUM_BITVEC_INTS; i++)
	if (bv1[i] &= bv2[i]) found = 1;
    
    return found;
}

void all_objects P2(unsigned int *, bv, int, remote_flag) {
    int i;
    int num = (remote_flag ? num_objects : num_objects - num_people);
    int last = BV_WHICH(num);
    
    i = last;
    while (i--)
	bv[i] = ~0;
    if (BV_WHICH(num + 1) != last)
	bv[last] = ~0;
    else
	bv[last] = 2 * BV_BIT(num) - 1;
    for (i = last + 1; i < NUM_BITVEC_INTS; i++)
	bv[i] = 0;
}

INLINE static match_t *add_match P4(parse_state_t *, state, int, token, 
				    int, start, int, end) {
    match_t *ret;

    ret = &matches[state->num_matches++];
    ret->first = start;
    ret->last = end;
    ret->token = token;
    switch (token) {
    case LIV_TOKEN:
	state->match_level += 3;
	break;
    case OBJ_TOKEN:
	state->match_level += 2;
	break;
    case STR_TOKEN:
	state->match_level++;
	break;
    case ERROR_TOKEN:
	state->num_errors++;
    }	
    return ret;
}

static void parse_copy_array P2(array_t *, arr, char ***, sarrp) {
    char **table;
    int j;

    if (!arr->size) {
	*sarrp = 0;
	return;
    }
    
    table = *sarrp = CALLOCATE(arr->size,char *, 
			       TAG_PARSER, "parse_copy_array");
    for (j = 0; j < arr->size; j++) {
	if (arr->item[j].type == T_STRING) {
	    if (arr->item[j].subtype == STRING_SHARED) {
		table[j] = ref_string(arr->item[j].u.string);
	    } else {
		table[j] = make_shared_string(arr->item[j].u.string);
	    }
	} else {
	    table[j] = 0;
	}
    }
}

static void interrogate_master PROT((void)) {
    svalue_t *ret;
    
    ret = apply_master_ob("parse_command_prepos_list", 0);
    if (ret && ret->type == T_ARRAY) {
	num_literals = ret->u.arr->size;
	parse_copy_array(ret->u.arr, &literals);
    }
    the_word = make_shared_string("the");
    a_word = make_shared_string("a");
}

void f_parse_init PROT((void)) {
    parse_info_t *pi;

    if (num_literals == -1) {
	num_literals = 0;
	interrogate_master();
    }
    
    if (current_object->pinfo)
	return;

    pi = current_object->pinfo = ALLOCATE(parse_info_t, TAG_PARSER, "parse_init");
    pi->ob = current_object;
    pi->flags = 0;
}

void f_parse_refresh PROT((void)) {
    if (!(current_object->pinfo))
	error("Current object is not known by the parser.  Call parse_init() first.\n");
    current_object->pinfo->flags &= ~PI_SETUP;
}

/* called from free_object() */
void parse_free P1(parse_info_t *, pinfo) {
    int i;

    if (pinfo->flags & PI_VERB_HANDLER) {
	for (i = 0; i < VERB_HASH_SIZE; i++) {
	    verb_t *v = verbs[i];
	    while (v) {
		verb_node_t **vn = &(v->node), *old;
		while (*vn) {
		    if ((*vn)->handler == pinfo->ob) {
			old = *vn;
			*vn = (*vn)->next;
			FREE(old);
		    } else vn = &((*vn)->next);
		}
		v = v->next;
	    }
	}
    }
    if (pinfo->flags & PI_SETUP) {
	if (pinfo->num_ids) {
	    for (i = 0; i < pinfo->num_ids; i++)
		free_string(pinfo->ids[i]);
	    FREE(pinfo->ids);
	}
	if (pinfo->num_plurals) {
	    for (i = 0; i < pinfo->num_plurals; i++)
		free_string(pinfo->plurals[i]);
	    FREE(pinfo->plurals);
	}
	if (pinfo->num_adjs) {
	    for (i = 0; i < pinfo->num_adjs; i++)
		free_string(pinfo->adjs[i]);
	    FREE(pinfo->adjs);
	}
    }
    FREE(pinfo);
}

static void hash_clean PROT((void)) {
    int i;
    hash_entry_t **nodep, *next;

    for (i = 0; i < HASH_SIZE; i++) {
	for (nodep = &hash_table[i]; *nodep && ((*nodep)->flags & HV_PERM);
	     nodep = &((*nodep)->next))
	    ;
	while (*nodep) {
	    next = (*nodep)->next;
	    free_string((*nodep)->name);
	    FREE((*nodep));
	    *nodep = next;
	}
    }
}

static void free_parse_result P1(parse_result_t *, pr) {
    int i, j;

    if (pr->ob)
	free_object(pr->ob, "free_parse_result");
    
    for (i = 0; i < 4; i++) {
	if (pr->res[i].func) FREE_MSTR(pr->res[i].func);
	if (pr->res[i].args) {
    	    for (j = 0; j < pr->res[i].num; j++)
	        free_svalue(((svalue_t *)pr->res[i].args) + j, "free_parse_result");
	    FREE(pr->res[i].args);
	}
    }
    FREE(pr);
}

static void clear_result P1(parse_result_t *, pr) {
    int i;

    pr->ob = 0;
    
    for (i = 0; i < 4; i++) {
	pr->res[i].func = 0;
	pr->res[i].args = 0;
    }
}

static void free_parse_globals PROT((void)) {
    int i;

    if (best_result) {
	free_parse_result(best_result);
	best_result = 0;
    }
    pi = 0;
    hash_clean();
    if (objects_loaded) {
	for (i = 0; i < num_objects; i++)
	    free_object(loaded_objects[i], "free_parse_globals");
	objects_loaded = 0;
    }
}

token_def_t tokens0[] = { {0, 0} };
token_def_t tokens1[] = { {0, 0} };
token_def_t tokens2[] = { {0, 0} };
token_def_t tokens3[] = {
    { "OBJ", OBJ_TOKEN },
    { "STR", STR_TOKEN },
    { "LIV", LIV_TOKEN },
    { 0, 0 }
};

token_def_t *tokens[] = { tokens0, tokens1, tokens2, tokens3 };

static int tokenize P1(char **, rule) {
    char *start = *rule;
    int i, n;
    token_def_t *td;
    
    while (*start == ' ') start++;

    if (!*start)
	return 0; /* at the end */

    *rule = strchr(start, ' ');

    if (!*rule)
	*rule = start + strlen(start);
    
    n = *rule - start;

    if (n < 4) {
	td = tokens[n];
	while (td->name) {
	    if (strncmp(td->name, start, n) == 0)
		return td->token;
	    td++;
	}
    }

    /* it's not a standard token.  Check the literals */
    for (i = 0; i < num_literals; i++) {
	if ((literals[i][n] == 0) && (strncmp(literals[i], start, n) == 0))
	    return i + 1;
    }

    {
	char buf[256];

	if (n > 50) {
	    strncpy(buf, start, 50);
	    strcpy(buf + 50, "...");
	} else {
	    strncpy(buf, start, n);
	    buf[n] = 0;
	}

	error("Unknown token '%s'\n", buf);
    }
    return 0;
}

static void make_rule P2(char *, rule, int *, tokens) {
    int idx = 0;

    while (idx < 10) {
	if (!(tokens[idx++] = tokenize(&rule)))
	    return; /* we got to the end */
    }
    error("Only 10 tokens permitted per rule!\n");
}

static void free_words PROT((void)) {
    int i;

    for (i = 0; i < num_words; i++)
	if (words[i].type == WORD_ALLOCATED)
	    FREE_MSTR(words[i].string);
    num_words = 0;
}

static void interrogate_object P1(object_t *, ob) {
    svalue_t *ret;

    ret = apply("parse_command_id_list", ob, 0, ORIGIN_DRIVER);
    if (ret && ret->type == T_ARRAY) {
	ob->pinfo->num_ids = ret->u.arr->size;
	parse_copy_array(ret->u.arr, &ob->pinfo->ids);
    } else ob->pinfo->num_ids = 0;
    if (ob->flags & O_DESTRUCTED) return;
    ob->pinfo->flags |= PI_SETUP;

    ret = apply("parse_command_plural_id_list", ob, 0, ORIGIN_DRIVER);
    if (ret && ret->type == T_ARRAY) {
	ob->pinfo->num_plurals = ret->u.arr->size;
	parse_copy_array(ret->u.arr, &ob->pinfo->plurals);
    } else ob->pinfo->num_plurals = 0;
    if (ob->flags & O_DESTRUCTED) return;

    ret = apply("parse_command_adjectiv_id_list", ob, 0, ORIGIN_DRIVER);
    if (ret && ret->type == T_ARRAY) {
	ob->pinfo->num_adjs = ret->u.arr->size;
	parse_copy_array(ret->u.arr, &ob->pinfo->adjs);
    } else ob->pinfo->num_adjs = 0;
    if (ob->flags & O_DESTRUCTED) return;

    ret = apply("is_living", ob, 0, ORIGIN_DRIVER);
    if (!IS_ZERO(ret))
	ob->pinfo->flags |= PI_LIVING;
}

static void rec_add_object P1(object_t *, ob) {
    object_t *o;

    if (!ob) return;
    if (ob->flags & O_DESTRUCTED) return;
    if (ob->pinfo) {
	loaded_objects[num_objects++] = ob;
	add_ref(ob, "rec_add_object");
    }
    o = ob->contains;
    while (o) {
	rec_add_object(o);
	o = o->next_inv;
    }
}

static hash_entry_t *add_hash_entry P1(char *, str) {
    int h = DO_HASH(str, HASH_SIZE);
    hash_entry_t *he;

    he = hash_table[h];
    while (he) {
	if (he->name == str)
	    return he;
	he = he->next;
    }

    he = ALLOCATE(hash_entry_t, TAG_PARSER, "add_hash_entry");
    he->name = ref_string(str);
    bitvec_zero(he->pv.noun);
    bitvec_zero(he->pv.plural);
    bitvec_zero(he->pv.adj);
    he->next = hash_table[h];
    he->flags = 0;
    hash_table[h] = he;
    return he;
}

static void add_to_hash_table P2(object_t *, ob, int, index) {
    int i;
    parse_info_t *pi = ob->pinfo;
    hash_entry_t *he;
    
    for (i = 0; i < pi->num_ids; i++) {
	he = add_hash_entry(pi->ids[i]);
	he->flags |= HV_NOUN;
	he->pv.noun[BV_WHICH(index)] |= BV_BIT(index);
    }
    for (i = 0; i < pi->num_plurals; i++) {
	he = add_hash_entry(pi->plurals[i]);
	he->flags |= HV_PLURAL;
	he->pv.plural[BV_WHICH(index)] |= BV_BIT(index);
    }
    for (i = 0; i < pi->num_adjs; i++) {
	he = add_hash_entry(pi->adjs[i]);
	he->flags |= HV_ADJ;
	he->pv.adj[BV_WHICH(index)] |= BV_BIT(index);
    }

    if (pi->flags & PI_LIVING)
	cur_livings[BV_WHICH(index)] |= BV_BIT(index);
}

static void load_objects PROT((void)) {
    int i;
    svalue_t *ret;
    object_t *ob, *env;
    
    bitvec_zero(cur_livings);
    num_objects = 0;
    if (!parse_user || parse_user->flags & O_DESTRUCTED)
	error("No this_player()!\n");
    
    rec_add_object(parse_user->super);
    ret = apply_master_ob("parse_command_users", 0);
    num_people = 0;
    if (ret->type == T_ARRAY) {
	for (i = 0; i < ret->u.arr->size; i++) {
	    if (ret->u.arr->item[i].type != T_OBJECT) continue;
	    /* check if we got them already */
	    ob = ret->u.arr->item[i].u.ob;
	    env = ob->super;
	    while (env) {
		if (env == parse_user->super)
		    break;
		env = env->super;
	    }
	    if (env) continue;
	    loaded_objects[num_objects + num_people++] = ob;
	    add_ref(ob, "load_objects");
	}
    }
    num_objects += num_people;

    /* we loop through twice so we don't have to worry about things moving
       in the above call */
    for (i = 0; i < num_objects; i++) {
	if (loaded_objects[i]->flags & O_DESTRUCTED) continue;
	if (!(loaded_objects[i]->pinfo->flags & PI_SETUP))
	    interrogate_object(loaded_objects[i]);
	/* self destructing objects suck */
	if (loaded_objects[i]->flags & O_DESTRUCTED) continue;
	add_to_hash_table(loaded_objects[i], i);
    }
	    
    objects_loaded = 1;
}

static int get_single P1(unsigned int *, bv) {
    int i, j = -1;
    int res = -1;

    for (i = 0; i < NUM_BITVEC_INTS; i++) {
	if (bv[i]) {
	    if (j != -1) return -1;
	    j = i;
	}
    }
    if (j == -1) return -1;
    /* sigh.  This could be done better */
    for (i = 0; i < 32; i++) {
	if ((1 << i) & bv[j]) {
	    if (res != -1) return -1;
	    res = j * 32 + i;
	}
    }
    return res;
}

/* equivalent to strcpy(x, y); return x + strlen(y), but faster */
static char *strput P2(char *, x, char *, y) {
    while ((*x++ = *y++))
	;
    return x - 1;
}

static char *query_the_short P2(char *, start, object_t *, ob) {
    svalue_t *ret;
    
    if (ob->flags & O_DESTRUCTED || 
	!(ret = apply("the_short", ob, 0, ORIGIN_DRIVER))
	|| ret->type != T_STRING) {
	return strput(start, "the thing");
    }
    return strput(start, ret->u.string);
}

static void error_is_not P4(char *, buf, hash_entry_t *, adj, 
			    int, multiple, hash_entry_t *, noun) {
    unsigned int *objects = noun->pv.noun;
    int ob;

    /* first case.  If the multiple flag is set, there was more than one
     * adjective.  We don't bother to figure out which adjectives do and
     * do not apply.  For example: 'sharp red sword'
     * There could be sharp swords, and red swords, but no sharp red ones.
     * We have a adjective they specified at this point, however, so we
     * use it to make a decent error message
     * We don't count swords in this case, since we can't do any better for
     * one object, unless we want to go through all the adjectives figuring
     * out which one (or more) is the bad one.
     */
    if (multiple) {
	sprintf(buf, "There is no such %s %s here.\n", 
		adj->name, noun->name);
	return;
    }

    /* If adj is zero, the failure was due to refering to a remote living */
    if (!adj) {
	sprintf(buf, "%s isn't here.\n", noun->name);
	if (islower(*buf))
	    *buf = toupper(*buf);
	return;
    }
    /* check if there is more than one of that noun.  In this case, we've
       only been called with zero or one adjectives */
    if (get_single(objects) == -1) {
	/* 'the red sword' with many swords, none of
	   which are red */
	char *pl = pluralize(noun->name);
	
	sprintf(buf, "None of the %s are %s.\n", pl,
		adj->name);
	FREE_MSTR(pl);
	return;
    } else {
	/* Easy case.  'the red sword', one sword, which isn't red. */
	char *p;
	
	p = query_the_short(buf, loaded_objects[ob]);
	sprintf(p, " is not %s.\n", adj->name);
	if (islower(*buf))
	    *buf = toupper(*buf);
	return;
    }
}

static void error_not_living P4(char *, buf, unsigned int *, objects,
				hash_entry_t *, noun, hash_entry_t *, adj) {
    /* The X isn't alive. (if only 1 of noun)
       The X you refered to isn't alive.
       None of the X are alive.
       None of the X you refered to are alive */
    if (get_single(objects) != -1) {
	if (adj)
	    sprintf(buf, "The %s you refered to isn't alive.\n", noun->name);
	else
	    sprintf(buf, "The %s isn't alive.\n", noun->name);
    } else {
	char *pl = pluralize(noun->name);
	if (adj)
	    sprintf(buf, "None of the %s you refered to are alive.\n", pl);
	else
	    sprintf(buf, "None of the %s are alive.\n", pl);
	FREE_MSTR(pl);
    }
}

static void error_ambig P3(char *, buf, hash_entry_t *, noun,
			   unsigned int *, objects) {
    /* This should be more sophisticated */
    sprintf(buf, "Which %s do you mean?\n", noun->name);
}

static void parse_obj P2(int, tok, parse_state_t *, state) {
    parse_state_t local_state;
    unsigned int objects[NUM_BITVEC_INTS];
    unsigned int save_obs[NUM_BITVEC_INTS];
    int start = state->word_index;
    char buf[1024];
    char *str;
    hash_entry_t *hnode, *last_adj = 0;
    int multiple_adj = 0;
    int tmp;
    match_t *mp;
    int dont_bother;
    
    if (!objects_loaded)
	load_objects();
    
    all_objects(objects, parse_vn->handler->pinfo->flags & PI_REMOTE_LIVINGS);

    while (1) {
	str = words[state->word_index++].string;
	hnode = hash_table[DO_HASH(str, HASH_SIZE)];
	while (hnode) {
	    if (hnode->name == str) {
		if (hnode->flags & HV_NOUN) {
		    local_state = *state;
		    dont_bother = 0;
		    bitvec_copy(save_obs, objects);
		    if (!intersect(objects, hnode->pv.noun)) {
			if (last_adj && !best_match && 
			    state->num_errors < best_num_errors) {
			    error_is_not(buf, last_adj, multiple_adj,
					 hnode);
			    mp = add_match(&local_state, ERROR_TOKEN,
					   start, state->word_index - 1);
			    mp->val.string = buf;
			} else dont_bother = 1;
		    } else
		    if (tok == LIV_TOKEN && !intersect(objects, cur_livings)) {
			if (!best_match && state->num_errors < best_num_errors) {
			    error_not_living(buf, objects, hnode, last_adj);
			    mp = add_match(&local_state, ERROR_TOKEN,
					   start, state->word_index - 1);
			    mp->val.string = buf;
			} else dont_bother = 1;
		    } else
		    if ((tmp = get_single(objects)) != -1) {
			mp = add_match(&local_state, tok,
				       start, state->word_index - 1);
			mp->val.number = tmp;
		    } else {
			if (!best_match && state->num_errors < best_num_errors) {
			    error_ambig(buf, hnode, objects);
			    mp = add_match(&local_state, ERROR_TOKEN,
					   start, state->word_index - 1);
			    mp->val.string = buf;
			} else dont_bother = 1;
		    }
		    if (!dont_bother)
			parse_rule(&local_state);
		    bitvec_copy(objects, save_obs);
		}
		if (hnode->flags & HV_ADJ) {
		    intersect(objects, hnode->pv.adj);
		    if (last_adj) multiple_adj = 1;
		    last_adj = hnode;
		} else
		    return;
		break;
	    }
	    hnode = hnode->next;
	}
	if (!hnode) return;
    }
}

static verb_t *parse_verb PROT((void)) {
    verb_t *ret;

    ret = verbs[DO_HASH(words[0].string, VERB_HASH_SIZE)];
    while (ret) {
	if (ret->name == words[0].string)
	    return ret;
	ret = ret->next;
    }
    return 0;
}

static char *make_error_message P1(int, which) {
    char buf[1024];
    char *p;
    int cnt = 0;
    int tok;
    int index = 0;
    int i;
    
    p = strput(buf, "You can't ");
    p = strput(p, words[0].string);
    *p++ = ' ';
    while ((tok = parse_vn->token[index++])) {
	switch (tok) {
	case OBJ_TOKEN:
	case LIV_TOKEN:
	    if (cnt == which - 1) {
		p = strput(p, "that ");
	    } else {
		p = query_the_short(p, loaded_objects[matches[cnt].val.number]);
		*p++ = ' ';
	    }
	    cnt++;
	    break;
	case STR_TOKEN:
	    if (cnt == which - 1) {
		p = strput(p, "that ");
	    } else {
		for (i = matches[cnt].first; i <= matches[cnt].last; i++) {
		    p = strput(p, words[i].string);
		    *p++ = ' ';
		}
		*p = 0;
	    }
	    cnt++;
	    break;
	default:
	    p = strput(p, literals[tok - 1]);
	    *p++ = ' ';
	    break;
	}
    }
    p--;
    strcpy(p, ".\n");
    return string_copy(buf, "make_error_message");
}

static char *current_possible_error = 0;

static int process_answer P3(parse_state_t *, state, svalue_t *, sv,
			     int, which) {
    if (!sv) return 0;
    if (sv->type == T_NUMBER) {
	if (sv->u.number)
	    return 1;
	if (state->num_errors == best_num_errors) return -2;
	if (state->num_errors++ == 0) {
	    if (current_possible_error) FREE_MSTR(current_possible_error);
	    current_possible_error = make_error_message(which);
	}
	return -1;
    }
    if (sv->type != T_STRING)
	return 0;
    if (state->num_errors == best_num_errors) return -2;
    if (state->num_errors++ == 0) {
	if (current_possible_error) FREE_MSTR(current_possible_error);
	current_possible_error = string_copy(sv->u.string, "process_answer");
    }
    return -1;
}

static int push_real_names PROT((void)) {
    int index = 0, match = 0;
    int tok;

    while ((tok = parse_vn->token[index++])) {
	switch (tok) {
	case OBJ_TOKEN:
	case LIV_TOKEN:
	case STR_TOKEN:
	    {
		int i;
		char *p, *q;
		char tmp[1024];
		q = tmp;
		for (i = matches[match].first; i <= matches[match].last; i++) {
		    p = words[i].string;
		    while (*p) *q++ = *p++;
		    if (i < matches[match].last) *q++ = ' ';
		}
		*q = 0;
		push_malloced_string(string_copy(tmp, "push_real_names"));
		match++;
	    }
	    break;
	}
    }
    return match;
}

char *rule_string P1(verb_node_t *, vn) {
    int index = 0;
    int tok;
    static char buf[1024];
    char *p;

    p = buf;
    
    while (1) {
	switch (tok = vn->token[index++]) {
	case OBJ_TOKEN:
	    p = strput(p, "OBJ ");
	    break;
	case LIV_TOKEN:
	    p = strput(p, "LIV ");
	    break;
	case STR_TOKEN:
	    p = strput(p, "STR ");
	    break;
	case 0:
	    if (p == buf) {
		*buf = 0;
	    } else {
		*(p-1) = 0; /* nuke last space */
	    }
            return buf;
	default:
	    p = strput(p, literals[tok - 1]);
	    *p++ = ' ';
	    break;
	}
    }
}

static int make_function P4(char *, buf, char *, pre, 
			    parse_state_t *, state, int, try) {
    int index = 0, match = 0;
    int on_stack = 0;
    int tok;
    /* try = 0: "read_about_str_from_obj"
     * try = 1: "read_word_str_word_obj"
     * try = 2: "verb_word_str_word_obj"
     * try = 3: "verb_rule"
     */

    buf = strput(buf, pre);
    if (try < 2) {
	buf = strput(buf, words[0].string);
    } else {
	buf = strput(buf, "verb");
	push_string(words[0].string, STRING_SHARED);
	on_stack++;
    }

    if (try == 3) {
	buf = strput(buf, "_rule");
	/* leave the 0; this effectively truncates the string. */
	buf++;
	push_constant_string(rule_string(parse_vn));
	on_stack++;
    }
    while ((tok = parse_vn->token[index++])) {
	*buf++ = '_';
	switch (tok) {
	case OBJ_TOKEN:
	    buf = strput(buf, "obj");
	    if (matches[match].token == ERROR_TOKEN || 
		(loaded_objects[matches[match].val.number]->flags & O_DESTRUCTED)) {
		push_number(0);
	    } else {
		push_object(loaded_objects[matches[match].val.number]);
	    }
	    match++;
	    on_stack++;
	    break;
	case LIV_TOKEN:
	    buf = strput(buf, "liv");
	    if (matches[match].token == ERROR_TOKEN || 
		(loaded_objects[matches[match].val.number]->flags & O_DESTRUCTED)) {
		push_number(0);
	    } else {
		push_object(loaded_objects[matches[match].val.number]);
	    }
	    match++;
	    on_stack++;
	    break;
	case STR_TOKEN:
	    {
		int i;
		char *q;
		char tmp[1024];
		buf = strput(buf, "str");
		q = tmp;
		for (i = matches[match].first; i <= matches[match].last; i++) {
		    q = strput(q, words[i].string);
		    if (i < matches[match].last) *q++ = ' ';
		}
		*q = 0;
		push_malloced_string(string_copy(tmp, "push_real_names"));
		match++;
		on_stack++;
	    }
	    break;
	default:
	    if (!try) {
		buf = strput(buf, literals[tok - 1]);
	    } else if (try < 3) {
		buf = strput(buf, "word");
		push_string(literals[tok - 1], STRING_SHARED);
		on_stack++;
	    }
	}
    }
    return on_stack;
}

static char *prefixes[] = { "can_", "direct_", "indirect_" };

#define SET_OB(x) ob = (x); if (ob->flags & O_DESTRUCTED) return;

static void we_are_finished P1(parse_state_t *, state) {
    char func[256];
    char *p;
    int ret;
    int which, try, args, mtch = 0;
    object_t *ob;
    int local_error;
    
    /* ignore it if we already have somethign better */
    if (best_match >= state->match_level) return;
    if (state->num_errors) {
	local_error = 0;
	if (state->num_errors > best_num_errors) return;
	if (state->num_errors == best_num_errors
	    && state->match_level < best_error_match) return;
    } else local_error = 1; /* if we have an error, it was local */
    SET_OB(parse_user);

    for (which = 0; which < 3; which++) {
	if (ob)
	    for (try = 0, ret = 0; !ret && try < 8; try++) {
		if (try == 4) {
			    SET_OB(parse_vn->handler);
		}
		args = make_function(func, prefixes[which], state, try % 4);
		ret = process_answer(state,
				     apply(func, ob, args, ORIGIN_DRIVER),
				     which);
		if (ob->flags & O_DESTRUCTED) return;
		if (ret == -2) return;
		if (ret) break;
	    }
	if (try == 8) {
	    if (state->num_errors == best_num_errors) return;
	    if (state->num_errors++ == 0) {
		/* ### ERROR MESSAGE */
	    }
	}
	while (mtch != state->num_matches && matches[mtch].token < ERROR_TOKEN) {
	    mtch++;
	    if (mtch == state->num_matches) break;
	}
	if (mtch == state->num_matches) break;
	if (matches[mtch].token != ERROR_TOKEN) {
	    SET_OB(loaded_objects[matches[mtch].val.number]);
	} else 
	    ob = 0;
	mtch++;
    }
    if (state->num_errors) {
	if (state->num_errors == best_num_errors &&
	    state->match_level <= best_error_match) return;
	best_num_errors = state->num_errors;
	best_error_match = state->match_level;
	if (current_error) FREE_MSTR(current_error);
	if (local_error) {
	    current_error = current_possible_error;
	    current_possible_error = 0;
	} else {
	    for (mtch = 0; matches[mtch].token != ERROR_TOKEN; mtch++)
		;
	    current_error = string_copy(matches[mtch].val.string, 
					"we_are_finished");		
	}
    } else {
	best_match = state->match_level;
	if (best_result) free_parse_result(best_result);
	best_result = ALLOCATE(parse_result_t, TAG_PARSER, "we_are_finished");
	clear_result(best_result);
	SET_OB(parse_vn->handler);
	best_result->ob = ob;
	add_ref(ob, "best_result");
	for (try = 0; try < 4; try++) {
	    args = make_function(func, "do_", state, try);
	    args += push_real_names();
	    best_result->res[try].func = string_copy(func, "best_result");
	    best_result->res[try].num = args;
	    if (args) {
		p = (char *)(best_result->res[try].args = CALLOCATE(args,
				       svalue_t, TAG_PARSER, "best_result"));
		memcpy(p, (char *)(sp - args + 1), args * sizeof(svalue_t));
		sp -= args;
	    }
	}
    }
}

static void do_the_call PROT((void)) {
    int i, n;
    object_t *ob = best_result->ob;
    
    for (i = 0; i < 4; i++) {
	if (ob->flags & O_DESTRUCTED) return;
	n = best_result->res[i].num;
	if (n) {
	    memcpy((char *)(sp + 1), best_result->res[i].args, n*sizeof(svalue_t));
	    /*
	     * Make sure we haven't dumped any dested obs onto the stack;
	     * this also updates sp.
	     */
	    while (n--) {
		if ((++sp)->type == T_OBJECT && (sp->u.ob->flags & O_DESTRUCTED)) {
		    free_object(sp->u.ob, "do_the_call");
		    *sp = const0;
		}
	    }
	    FREE(best_result->res[i].args);
	}
	best_result->res[i].args = 0;
	if (apply(best_result->res[i].func, ob,
		  best_result->res[i].num, ORIGIN_DRIVER)) return;
    }
}

static void parse_rule P1(parse_state_t *, state) {
    int tok;
    parse_state_t local_state;
    match_t *mp;
    int start;

    while (1) {
	tok = parse_vn->token[state->tok_index++];
	if (state->word_index == num_words && tok)
	    return;
	switch (tok) {
	case 0:
	    if (state->word_index == num_words)
		we_are_finished(state);
	    return;
	case OBJ_TOKEN:
	case LIV_TOKEN:
	    local_state = *state;
	    parse_obj(tok, &local_state);
	    if (!best_match && state->num_errors < best_num_errors) {
		start = state->word_index++;
		while (state->word_index <= num_words) {
		    local_state = *state;
		    mp = add_match(&local_state, ERROR_TOKEN,
				   start, state->word_index - 1);
		    mp->val.string = "There is no ###X here.";
		    parse_rule(&local_state);
		    state->word_index++;
		}
	    }
	    return;
	case STR_TOKEN:
	    start = state->word_index++;
	    while (state->word_index <= num_words) {
		local_state = *state;
		add_match(&local_state, STR_TOKEN,
			  start, state->word_index - 1);
		parse_rule(&local_state);
		state->word_index++;
	    }
	    return;
	default:
	    if (literals[tok - 1] == words[state->word_index++].string) {
		state->match_level++;
	    } else {
		if (state->tok_index == 1) return;

		switch (parse_vn->token[state->tok_index - 2]) {
		case STR_TOKEN:
		    return;
		case OBJ_TOKEN:
		case LIV_TOKEN:
		    {
			match_t *last;
			
			while (literals[tok - 1] != words[state->word_index++].string) {
			    if (state->word_index == num_words)
				return;
			}
			last = &matches[state->num_matches];
			switch (last->token) {
			case LIV_TOKEN:
			    state->match_level -= 3;
			    break;
			case OBJ_TOKEN:
			    state->match_level -= 2;
			    break;
			case STR_TOKEN:
			    state->match_level--;
			}
			state->num_errors++;
			last->token = ERROR_TOKEN;
			last->last = state->word_index-1;
			last->val.string = "###RECOVER";
		    }
		    break;
		default:
		    return;
		}
	    }
	}
    }
}

static int check_literal P2(int, lit, int, start) {
    while (start < num_words) {
	if (literals[lit - 1] == words[start++].string)
	    return start;
    }
    return 0;
}

static void parse_rules PROT((void)) {
    int pos;
    parse_state_t local_state;

    parse_vn = parse_verb_entry->node;
    while (parse_vn) {
	if (!parse_restricted || parse_vn->handler == parse_restricted) {
	    pos = 0;
	    if ((!parse_vn->lit[0] ||
		 (pos = check_literal(parse_vn->lit[0], 1)))
		&& (!parse_vn->lit[1] ||
		    check_literal(parse_vn->lit[1], pos))) {
		local_state.tok_index = 0;
		local_state.word_index = 1;
		local_state.match_level = 1;
		local_state.num_matches = 0;
		local_state.num_errors = 0;
		parse_rule(&local_state);
	    }
	}
	parse_vn = parse_vn->next;
    }
}

static void reset_error PROT((void)) {
    if (current_error)
	FREE_MSTR(current_error);
    current_error = 0;
    best_match = 0;
    best_error_match = 0;
    best_num_errors = 5732; /* Yes.  Exactly 5,732 errors.  Don't ask. */
}

static void parse_recurse P1(char **, iwords) {
    char buf[1024];
    char *p, *q;
    char **iwp = iwords;
    int first = 1;
    int l;
    
    if (*iwords[0]) {
	*buf = 0;
	p = buf;
	do {
	    l = iwp[1] - iwp[0] - 1;
	    strcpy(p, *iwp++);
	    p += l;
	    if ((q = findstring(buf))) {
		if (q == a_word || q == the_word)
		    parse_recurse(iwp);
		else {
		    words[num_words].type = 0;
		    words[num_words++].string = q;
		    parse_recurse(iwp);
		    num_words--;
		}
	    } else if (first) {
		first = 0;
		words[num_words].type = WORD_ALLOCATED;
		words[num_words].string = new_string(l, "parse_recurse");
		memcpy(words[num_words].string, buf, l);
		words[num_words++].string[l] = 0;
		parse_recurse(iwp);
		num_words--;
		FREE_MSTR(words[num_words].string);
	    }
	    *p++ = ' ';
	} while (*iwp[0]);
    } else {
	if ((parse_verb_entry = parse_verb()))
	    parse_rules();
    }
}

static void parse_sentence P1(char *, input) {
    char *starts[256];
    char c, buf[1024], *p, *start;
    int n = 0;
    int i;

    free_words();
    p = start = buf;
    while ((c = *input++)) {
	if (isignore(c)) continue;
	if (isupper(c))
	    c = tolower(c);
	if (iskeep(c)) {
	    *p++ = c;
	} else {
	    /* whitespace, punctuation, or end of string */
	    if (p - start) {
		*p++ = 0;
		starts[n++] = start;
		start = p;
	    }
	}
    }
    if (p - start) {
	*p++ = 0;
	starts[n++] = start;
    }
    starts[n] = p;
    *p = 0;

    reset_error();
    /* find an interpretation, first word must be shared (verb) */
    for (i = 1; i <= n; i++) {
	if ((words[0].string = findstring(buf))) {
	    num_words = 1;
	    words[0].type = 0;
	    parse_recurse(&starts[i]);
	}
	starts[i][-1] = ' ';
    }
}

void f_parse_sentence PROT((void)) {
    if (!current_object->pinfo)
	error("Current object is not known by the parser.  Call parse_init() first.\n");

    if (pi)
	error("Illegal to call parse_sentence() recursively.\n");
    
    (++sp)->type = T_ERROR_HANDLER;
    sp->u.error_handler = free_parse_globals;

    parse_user = current_object;
    pi = current_object->pinfo;
    parse_restricted = 0;
    parse_sentence((sp-1)->u.string);
    
    if (best_match) {
	do_the_call();
	sp--; /* pop the error handler */
	free_string_svalue(sp);
	put_number(1);
    } else {
	sp--; /* pop the error handler */
	free_string_svalue(sp);
	if (current_error) {
	    sp->subtype = STRING_MALLOC;
	    sp->u.string = current_error;
	    current_error = 0;
	} else put_number(0);
    }
    free_parse_globals();
}

void f_parse_my_rules PROT((void)) {
    int flag = (st_num_arg == 3 ? (sp--)->u.number : 0);
    
    if (!(sp-1)->u.ob->pinfo)
	error("Object is not known by the parser.  Call parse_init() first.\n");
    if (!current_object->pinfo)
	error("Current object is not known by the parser.  Call parse_init() first.\n");

    if (pi)
	error("Illegal to call parse_sentence() recursively.\n");
    
    (++sp)->type = T_ERROR_HANDLER;
    sp->u.error_handler = free_parse_globals;

    parse_user = (sp-2)->u.ob;
    pi = parse_user->pinfo;
    parse_restricted = current_object;
    parse_sentence((sp-1)->u.string);
    
    if (best_match) {
	if (flag) {
	    do_the_call();
	    sp--; /* pop the error handler */
	    free_string_svalue(sp--);
	    free_svalue(sp, "parse_my_rules"); /* may have been destructed */
	    put_number(1);
	} else {
	    int n;
	    array_t *arr;
	    /* give them the info for the wildcard call */
	    n = best_result->res[3].num;
	    arr = allocate_empty_array(n);
	    if (n) {
		memcpy((char *)arr->item, best_result->res[3].args, n*sizeof(svalue_t));
		while (n--) {
		    if (arr->item[n].type == T_OBJECT && arr->item[n].u.ob->flags & O_DESTRUCTED) {
			free_object(arr->item[n].u.ob, "parse_my_rules");
			arr->item[n] = const0;
		    }
		}
		FREE(best_result->res[3].args);
	    }
	    best_result->res[3].args = 0;
	    sp--; /* pop the error handler */
	    free_string_svalue(sp--);
	    free_svalue(sp, "parse_my_rules"); /* may have been destructed */
	    put_array(arr);
	}
    } else {
	sp--; /* pop the error handler */
	free_string_svalue(sp--);
	free_svalue(sp, "parse_my_rules"); /* may have been destructed */
	if (current_error) {
	    sp->subtype = STRING_MALLOC;
	    sp->u.string = current_error;
	    current_error = 0;
	} else put_number(0);
    }
    free_parse_globals();
}

void f_parse_add_rule() {
    int tokens[10];
    int lit[2], i, j;
    svalue_t *ret;
    char *verb, *rule;
    object_t *handler;
    verb_t *verb_entry;
    verb_node_t *verb_node;
    int h;

    rule = (sp-1)->u.string;
    verb_entry = 0;
    verb = findstring((sp-2)->u.string);
    CHECK_TYPES(sp, T_OBJECT, 3, F_PARSE_ADD_RULE);
    handler = sp->u.ob;
    if (!(handler->pinfo))
	error("Verb handler is not known by the parser.  Call parse_init() first.\n");

    /* Create the rule */
    make_rule(rule, tokens);

    /* Now find a verb entry to put it in */
    if (verb) {
	verb_entry = verbs[DO_HASH(verb, VERB_HASH_SIZE)];
	while (verb_entry) {
	    if (verb_entry->name == verb)
		break;
	    verb_entry = verb_entry->next;
	}
    }
    
    if (!verb_entry) {
	if (!verb)
	    verb = make_shared_string((sp-2)->u.string);
	else
	    ref_string(verb);
	
	h = DO_HASH(verb, VERB_HASH_SIZE);
	verb_entry = ALLOCATE(verb_t, TAG_PARSER, "parse_add_rule");
	verb_entry->name = verb;
	verb_entry->node = 0;
	verb_entry->next = verbs[h];
	verbs[h] = verb_entry;
    }

    /* Add a new node */
    for (i = 0, j = 0; tokens[i]; i++) {
	if (tokens[i] > 0 && j < 2)
	    lit[j++] = tokens[i];
    }

    while (j < 2)
	lit[j++] = 0;

    verb_node = (verb_node_t *)DXALLOC(sizeof(verb_node_t) + sizeof(int)*i,
				       TAG_PARSER, "parse_add_rule");

    verb_node->lit[0] = lit[0];
    verb_node->lit[1] = lit[1];
    for (j = 0; j <= i; j++)
	verb_node->token[j] = tokens[j];
    verb_node->handler = handler;
    handler->pinfo->flags |= PI_VERB_HANDLER;
    verb_node->next = verb_entry->node;
    verb_entry->node = verb_node;

    ret = apply("livings_are_remote", handler, 0, ORIGIN_DRIVER);
    if (!IS_ZERO(ret))
        handler->pinfo->flags |=PI_REMOTE_LIVINGS;

    /* return */
    pop_stack();
    free_string_svalue(sp--);
    free_string_svalue(sp--);
}

void f_parse_dump PROT((void))
{
    int i;
    outbuffer_t ob;

    outbuf_zero(&ob);
    for (i = 0; i < VERB_HASH_SIZE; i++) {
	verb_t *v = verbs[i];
	while (v) {
	    verb_node_t *vn = v->node;
	    outbuf_addv(&ob, "Verb %s:\n", v->name);
	    while (vn) {
		outbuf_addv(&ob, "  (%s) %s\n", vn->handler->name, rule_string(vn));
		vn = vn->next;
	    }
	    v = v->next;
	}
    }
    outbuf_push(&ob);
}
