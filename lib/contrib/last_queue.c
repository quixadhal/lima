/* Do not remove the headers from this file! see /USAGE for more info. */

// Quick system for efficiently managing queues of last X items

class lqueue {
    int size;
    int index;
    array elems;
}

class lqueue lqueue_new(int size) {
    class lqueue q = new(class lqueue);
    q->size = size;
    q->elems = allocate(size);
    q->index = 0;
    return q;
}

void lqueue_add(class lqueue q, mixed elem) {
    q->elems[q->index++] = elem;
    if (q->index == q->size) q->index = 0;
}

// 1 = last, 2 = before that, etc
void lqueue_member(class lqueue q, int ind) {
    int which = q->index - ind;
    if (which < 0) which += q->size;
    return q->elems[which];
}

array lqueue_list(class lqueue q) {
    return q->elems[q->index..] + q->elems[0..q->index-1];
}
