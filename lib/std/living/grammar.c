/* Do not remove the headers from this file! see /USAGE for more info. */

/* An attempt to put all the grammar/gender related stuff in one
 * file.  -Beek
 */

/* 0 = neuter, 1 = male, 2 = female.  Anything else, you're wierd. */

int gender;

void set_gender(int x) { gender = x; }

int query_gender() { return gender; }

string query_pronoun() { switch (gender) {
    case 0: return "it";
    case 1: return "he";
    case 2: return "she";
    }}


string query_objective() {switch (gender) {
    case 0: return "it";
    case 1: return "him";
    case 2: return "her";
    }
}


string query_subjective() { return query_pronoun(); }

string query_possessive() { switch (gender) {
    case 0: return "its";
    case 1: return "his";
    case 2: return "her";
    }}

string query_named_possessive() {
    string res;
    res = this_object()->query_name();
    if (res[<1]=='s') return res+"'";
    else return res+"'s";
}

string query_reflexive(){
    switch(gender){
    case 0: return "itself";
    case 1: return "himself";
    case 2: return "herself";
    }
}

