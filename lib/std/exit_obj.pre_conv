/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_EXIT_OBJ;

void remove() {
    m_exit_obj::remove();
    object::remove();
}

varargs void on_clone() {
    object::on_clone();
    m_exit_obj::on_clone();
}
