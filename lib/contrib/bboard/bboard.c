/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Bulletin Board interface to NEWS_D
** Note: To use set_board_name(), have an admin create
** a newsgroup of that name if one doesn't already exist.
** Hiccups@RoE, 4/6/97
** (c)1997 The Realms of Exile.      (Heh, aren't we cool?)
** RoE: exile.imaginary.com 6666
*/
 
#include <edit.h>
 
inherit OBJ;
inherit CLASS_NEWSMSG;
 
private string boarddesc;
private static string new_header;
private string boardname;
 
static mapping idtable=([]);
 
void remap_ids() {
        int *ids, *tmp;
        int so, j=0;
	idtable=([]);
        ids=NEWS_D->get_messages(boardname);
        ids=sort_array(ids, 1);
        so=sizeof(ids);
        if(!so) return 0;
        for(int i=0; i<so; i++) {
                class news_msg msg;
                msg=NEWS_D->get_message(boardname, ids[i]);
                if(!msg->body) continue;
                j++;
                idtable[j]=ids[i];
        }
}
 
int query_actual_id(int i) { return idtable[i]; }
 
void set_board_name(string what) {
        if(member_array(what, NEWS_D->get_groups())==-1) {
                destruct(this_object());
                error("bulletin board error: that newsgroup does not exist");
        } else
                boardname=what;
}
 
string query_in_room_desc() {
        int x=sizeof(keys(idtable));
        string oird=::query_in_room_desc();
        oird += (" ["+(string)x);
        x==1 ? oird+=" note" : oird+=" notes";
        oird += "]";
        return oird;
}
 
string format_subj(int id) {
        class news_msg msg;
        string subject;
        msg = NEWS_D->get_message(boardname, query_actual_id(id));
        if(!msg || !msg->body)
                return sprintf("%d  *** REMOVED ***", id);
        else
                subject = msg->subject;
        return sprintf("%d  %s (%s, %s)",
                id,
                subject[0..34],
                msg->poster,
                intp(msg->time) ? ctime(msg->time)[4..9] : msg->time);
}
 
string show_msg(int id) {
        class news_msg msg;
        string body, subject;
        string ret, poster;
        msg = NEWS_D->get_message(boardname, query_actual_id(id));
/*
        if(!msg || !msg->body)
                return "*** REMOVED ***\n";
*/
        subject = msg->subject;
        body = msg->body;
        poster = msg->poster;
        ret = "This note is titled '"+subject+"' ("+poster+", "+(intp(msg->time) ? ctime(msg->time)[4..9] : msg->time)+")\n";
        ret += body;
        return ret;
}
 
void read_entry(string id) {
        int note;
        int *nom=NEWS_D->get_messages(boardname);
        if (!sscanf(id, "%d", note))
                if (!sscanf(id, "note %d", note))
                        return;
        if(note<1 || note>sizeof(keys(idtable))) {
                if(note<1) {
                        write("Not that few notes.\n");
                        return;
                }
                else {
                       write("Not that many notes.\n");
                       return;
                }
        }
        more(show_msg(note)+"\n");
        return;
}
 
string display_list() {
        int *ids;
        string lines="";
        ids=keys(idtable);
        if(!sizeof(ids)) return 0;
        ids=sort_array(ids, 1);
        foreach(int i in ids) {
                lines+=(format_subj(i)+"\n");
        }
        return lines;
}
 
string long()
{
        string ret;
        if(!boarddesc || boarddesc=="") 
                ret="A bulletin board.\nType 'note <header> on board' to post a note\nand 'read <note #> on board' to read a note.\n";
        else 
                ret=boarddesc;
        if(!display_list())
                ret += "There are no messages on the board.\n";
        else {
                ret+="\tThe following notes are on the board:\n";
                ret+=display_list();
        }
        return ret;
}
 
void set_board_desc(string what) { boarddesc=what; }
 
void done_wri(string *text) {
        if(!sizeof(text)) {
                this_body()->simple_action("$N $vabort writing $p note.");
                return;
        }
        NEWS_D->post(boardname, new_header, implode(text, "\n"));
        this_body()->simple_action("$N $vfinish writing a note.");
        remap_ids();
	remap_ids();
        return;
}
 
int note(string what) {
        if(!what || what=="") {
                write("Note <header> on board\n");
                return 1;
        }
        new_header=what;
        new(EDIT_OB, EDIT_TEXT, 0, (: done_wri :));
}
 
// Strictly for historical purposes
void reset() {
   if(!random(4)) {
        tell_environment(this_object(),"A small gnome appears and secures some " +
                "notes on the board that were loose.\n");
        tell_environment(this_object(),"The gnome leaves again.\n");
   }
}
 
int do_remove(string id) {
        int note;
        int rs;
        int *nom=keys(idtable);
        if (!sscanf(id, "%d", note))
                if (!sscanf(id, "note %d", note)) {
                                write("Remove which note from the board?\n");
                                return 1;
                }
        if(note<1 || note>sizeof(nom)) {
                write("Not that many notes.\n");
                return 1;
        }
        rs=NEWS_D->remove_post(boardname, query_actual_id(note));
        if(!rs)
                write("You can't remove that post.\n");
        else {
                this_body()->simple_action("$N $vremove post "+id+".");
                remap_ids();
        }
        return 1;
}
 
void init_board() { if(boardname) remap_ids(); }
 
mixed direct_note_str_on_obj(string str, object ob) { return 1; }
 
mixed direct_read_str_on_obj(string str, object ob) { return 1; }
 
mixed direct_remove_str_from_obj(string str, object ob) { return 1; }
