// This file is #included by red_black_tree.tpp

namespace BBST {

template <class T, class Compare>
node_impl<T> *red_black_tree<T, Compare>::create_node(const T &key) {
    return new node(key); // memory leak incoming...
}

template <class T>
node_impl<T> *node_impl<T>::prev(node_impl<T> *it) {

    if(it == sentinel_ptr)
        throw std::runtime_error("Attempt to decrement nullptr");

    if(it->l != sentinel_ptr) {
        it = it->l;

        while(it->r != sentinel_ptr)
            it = it->r;

    } else {

        while(it->p != sentinel_ptr and it == it->p->l)
            it = it->p;

        it = it->p;
    }

    return it;
}

template <class T>
node_impl<T> *node_impl<T>::next(node_impl<T> *it) {

    if(it == sentinel_ptr)
        throw std::runtime_error("Attempt to increment nullptr");

    if(it->r != sentinel_ptr) {
        it = it->r;

        while(it->l != sentinel_ptr)
            it = it->l;

    } else {

        while(it->p != sentinel_ptr and it == it->p->r)
            it = it->p;

        it = it->p;
    }

    return it;
}

template <class T, class Compare>
void red_black_tree<T, Compare>::transplant(node *x, node *y) {
    if(x->p == sentinel_ptr)
        root = y;
    else if(x == x->p->l)
        x->p->l = y;
    else
        x->p->r = y;
    y->p = x->p;
}

template <class T, class Compare>
void red_black_tree<T, Compare>::left_rotate(node *x) {
    node *y = x->r;
    x->r = y->l;
    if(y->l != sentinel_ptr)
        y->l->p = x;

    y->p = x->p;
    if(y->p == sentinel_ptr)
        root = y;
    else if(x == x->p->l)
        x->p->l = y;
    else
        x->p->r = y;

    y->l = x;
    x->p = y;
}

template <class T, class Compare>
void red_black_tree<T, Compare>::right_rotate(node *y) {
    node *x = y->l;
    y->l = x->r;
    if(x->r != sentinel_ptr)
        x->r->p = y;

    x->p = y->p;
    if(y->p == sentinel_ptr)
        root = x;
    else if(y == y->p->l)
        y->p->l = x;
    else
        y->p->r = x;

    x->r = y;
    y->p = x;
}

template <class T, class Compare>
void red_black_tree<T, Compare>::fix_insert(node *z) {

    while(z->p->col == RED) {

        if(z->p == z->p->p->l) {
            node *y = z->p->p->r;

            if(y->col == RED) {
                z->p->col = y->col = BLACK;
                z->p->p->col = RED;
                z = z->p->p;

            } else {
                if(z == z->p->r)
                    left_rotate(z = z->p);

                z->p->col = BLACK;
                z->p->p->col = RED;
                right_rotate(z->p->p);
            }

        } else {
            node *y = z->p->p->l;

            if(y->col == RED) {
                z->p->col = y->col = BLACK;
                z->p->p->col = RED;
                z = z->p->p;

            } else {
                if(z == z->p->l)
                    right_rotate(z = z->p);

                z->p->col = BLACK;
                z->p->p->col = RED;
                left_rotate(z->p->p);
            }

        }
    }

    root->col = BLACK;
}

template <class T, class Compare>
void red_black_tree<T, Compare>::fix_erase(node *x) {

    while(x != root and x->col == BLACK) {

        if(x == x->p->l) {
            node *w = x->p->r;

            if(w->col == RED) {
                w->col = BLACK;
                x->p->col = RED;
                left_rotate(x->p);
                w = x->p->r;
            }

            if(w->l->col == BLACK
                and w->r->col == BLACK) {
                    w->col = RED;
                    x = x->p;

            } else {
                if(w->r->col == BLACK) {
                    w->l->col = BLACK;
                    w->col = RED;
                    right_rotate(w);
                    w = x->p->r;
                }

                w->col = x->p->col;
                x->p->col = BLACK;
                w->r->col = BLACK;
                left_rotate(x->p);

                x = root;
            }

        } else {
            node *w = x->p->l;

            if(w->col == RED) {
                w->col = BLACK;
                x->p->col = RED;
                right_rotate(x->p);
                w = x->p->l;
            }

            if(w->r->col == BLACK
                and w->l->col == BLACK) {
                    w->col = RED;
                    x = x->p;

            } else {
                if(w->l->col == BLACK) {
                    w->r->col = BLACK;
                    w->col = RED;
                    left_rotate(w);
                    w = x->p->l;
                }

                w->col = x->p->col;
                x->p->col = BLACK;
                w->l->col = BLACK;
                right_rotate(x->p);

                x = root;
            }

        }

    }

    x->col = BLACK;
}

// ctors
template <class T, class Compare>
red_black_tree<T, Compare>::red_black_tree(T, Compare &&_cmp)
    : cmp(std::forward<Compare>(_cmp)) {}

template <class T, class Compare>
red_black_tree<T, Compare>::red_black_tree(Compare _cmp): cmp(_cmp) {}

template <class T, class Compare>
template <typename InputIt, typename isIter>
red_black_tree<T, Compare>::red_black_tree(InputIt first, InputIt last) {
    insert(first, last);
}

template <class T, class Compare>
red_black_tree<T, Compare>::red_black_tree(std::initializer_list<T> ilist) {
    insert(ilist.begin(), ilist.end());
}

// utility
template <class T, class Compare>
typename red_black_tree<T, Compare>::iterator red_black_tree<T, Compare>::find(const T &key) const {
    node *it = root;

    while(it != sentinel_ptr) {
        bool is_less = cmp(key, it->key);
        bool is_greater = cmp(it->key, key);

        if(!is_less and !is_greater)
            return iterator { it };

        it = is_less? it->l : it->r;
    }

    return iterator { sentinel_ptr };
}

template <class T, class Compare>
bool red_black_tree<T, Compare>::contains(const T &key) const {
    return find(key) != end();
}

template <class T, class Compare>
typename red_black_tree<T, Compare>::iterator red_black_tree<T, Compare>::lower_bound(const T &key) const {
    node *it = root, *lb = sentinel_ptr;

    while(it != sentinel_ptr) {
        if(!cmp(it->key, key))      // it->key >= key
            lb = it, it = it->l;
        else it = it->r;
    }

    return iterator { lb };
}

template <class T, class Compare>
typename red_black_tree<T, Compare>::iterator red_black_tree<T, Compare>::upper_bound(const T &key) const {
    node *it = root, *ub = sentinel_ptr;

    while(it != sentinel_ptr) {
        if(cmp(key, it->key))      // it->key > key
            ub = it, it = it->l;
        else it = it->r;
    }

    return iterator { ub };
}

template <class T, class Compare>
template <typename InputIt, typename isIter>
void red_black_tree<T, Compare>::insert(InputIt first, InputIt last) {
    while(first != last) {
        insert(*first);
        ++first;
    }
}

template <class T, class Compare>
void red_black_tree<T, Compare>::insert(std::initializer_list<T> ilist) {
    insert(ilist.begin(), ilist.end());
}

template <class T, class Compare>
std::pair<typename red_black_tree<T, Compare>::iterator, bool> red_black_tree<T, Compare>::insert(const T &key) {
    node *it = root, *par = sentinel_ptr;

    while(it != sentinel_ptr) {
        bool is_less = cmp(key, it->key);
        bool is_greater = cmp(it->key, key);

        if(!is_less and !is_greater)
            return { iterator { it }, false };      // already present

        par = it;
        it = is_less? it->l : it->r;
    }

    ++sz;
    node *new_node = create_node(key);
    new_node->p = par;

    if(par == sentinel_ptr)
        root = new_node;
    else if(cmp(key, par->key))
        par->l = new_node;
    else
        par->r = new_node;

    fix_insert(new_node);

    if(leftmost == end() or cmp(key, *leftmost))
        leftmost = new_node;

    if(rightmost == end() or cmp(*rightmost, key))
        rightmost = new_node;

    return { iterator { new_node }, true };
}

template <class T, class Compare>
bool red_black_tree<T, Compare>::erase(const T &key) {
    iterator itr = find(key);
    if(itr == end())
        return false;
    return erase(itr), true;
}

template <class T, class Compare>
void red_black_tree<T, Compare>::erase(iterator first, iterator last) {
    while(first != last) {
        erase(first);
        ++first;
    }
}

template <class T, class Compare>
typename red_black_tree<T, Compare>::iterator red_black_tree<T, Compare>::erase(iterator itr) {
    if(itr == end())
        throw std::runtime_error("Attempt to erase past the end iterator");

    node *it = itr.get_ptr();

    iterator nxt { node::next(it) };

    if(itr == leftmost)
        leftmost = nxt;

    if(itr == rightmost)
        rightmost = iterator { node::prev(it) };

    if(!--sz) {
        root = sentinel_ptr;
        return nxt;
    }

    color orig_col = it->col;

    node *x, *y = it;
    if(it->l == sentinel_ptr) {
        x = it->r;
        transplant(it, it->r);

    } else if(it->r == sentinel_ptr) {
        x = it->l;
        transplant(it, it->l);

    } else {
        y = it->r;
        while(y->l != sentinel_ptr)
            y = y->l;

        orig_col = y->col;
        x = y->r;

        if(y->p == it)
            x->p = y;
        else {
            transplant(y, y->r);
            y->r = it->r;
            y->r->p = y;
        }

        transplant(it, y);
        y->l = it->l;
        y->l->p = y;
        y->col = it->col;
    }

    if(orig_col == BLACK)
        fix_erase(x);

    return nxt;
}

} // namespace BBST