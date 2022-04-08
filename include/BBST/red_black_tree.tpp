/**
 * @file red_black_tree.tpp
 * @author the-hyp0cr1t3
 * @brief Red Black Tree template class definition
 * @date 2022-03-25
 */
#pragma once

#include <iterator.tpp>

#include <memory>
#include <utility>
#include <iostream>
#include <algorithm>
#include <type_traits>


namespace BBST {

/// An enum for the colour of the rbtree nodes
enum color {
    RED,    ///< Denotes a red node
    BLACK   ///< Denotes a black node
};

/**
 * @brief A generic node struct
 * @tparam T The type of the key
 */
template <class T>
struct node_impl {
    const T key;        ///< The key value

    node_impl *l;       ///< A pointer to the left child, points to `node_impl::sentinel_ptr` by default
    node_impl *r;       ///< A pointer to the right child, points to `node_impl::sentinel_ptr` by default
    node_impl *p;       ///< A pointer to the parent, points to `node_impl::sentinel_ptr` by default

    color col { RED };  ///< The colour of the node, RED by default


    /**
     * @brief Construct a new node impl object
     * @param key The key value
     */
    node_impl(const T &key)
        : key(key), l(sentinel_ptr), r(sentinel_ptr), p(sentinel_ptr) {}

    /**
     * @brief Finds the predecessor of a node
     *
     * @param it A pointer to the node
     * @return `node_impl*` A pointer to the predecessor if it exists
     * @return `nullptr` otherwise
     */
    static node_impl *prev(node_impl *it);

    /**
     * @brief Finds the successor of a node
     *
     * @param it A pointer to the node
     * @return `node_impl*` A pointer to the successor if it exists
     * @return `nullptr` otherwise
     */
    static node_impl *next(node_impl *it);

    /// A pointer to the sentinel node
    static node_impl *sentinel_ptr;
};

/// Sentinel ptr definition
template <class T>
node_impl<T> *node_impl<T>::sentinel_ptr = nullptr;

/**
 * @brief Gets the sentinel node associated with `node_impl<T>` if it exists,
 * otherwise creates a new one and sets it as the designated sentinel node.
 *
 * @tparam T The type of the key
 * @return `node_impl<T>*` A pointer to the sentinel node.
 */
template <class T>
node_impl<T> *get_sentinel() {
    if(!node_impl<T>::sentinel_ptr) {
        node_impl<T>::sentinel_ptr = new node_impl<T>(T{});
        node_impl<T>::sentinel_ptr->col = BLACK;
    }
    return node_impl<T>::sentinel_ptr;
}

/**
 * @brief A templated red black tree class
 *
 * @code
 * #include <iostream>
 * #include <vector>
 * #include <rbtree.tpp>
 *
 * struct Box {
 *     int length, breadth;
 *     int area() const {
 *         return length * breadth;
 *     }
 * };
 *
 * int main() {
 *     std::vector<int> v{100, 200, 200, 300};
 *     BBST::red_black_tree<int> rbtree{3, 3, 4, -2, 5, -1, 5};
 *     rbtree.erase(4);
 *     rbtree.insert(20);
 *     rbtree.insert(20);
 *     rbtree.erase(10);
 *     rbtree.insert(v.begin(), v.end());
 *
 *     std::cout << rbtree.size() << std::endl;
 *     for (auto &x : rbtree) // supports range-based for loop
 *         std::cout << x << ' ';
 *     std::cout << std::endl;
 *
 *     auto it = rbtree.lower_bound(20);
 *     if (it != rbtree.end())
 *         std::cout << *it << std::endl;
 *     it = rbtree.upper_bound(20);
 *     if (it != rbtree.end())
 *         std::cout << *it << std::endl;
 *
 *
 *     // custom compare method 1:
 *     auto cmp = [](const Box &a, const Box &b) {
 *         return a.area() < b.area();
 *     };
 *     BBST::red_black_tree<Box, decltype(cmp)> custom_rbtree(cmp);
 *
 *
 *     // custom compare method 2:
 *     BBST::red_black_tree custom_rbtree(
 *         Box{},
 *         [](const Box &a, const Box &b) {
 *             return a.area() < b.area();
 *         }
 *     );
 *
 *
 *     custom_rbtree.insert(Box{2, 3});
 *     custom_rbtree.insert(Box{1, 10});
 *
 *     for (auto it = custom_rbtree.begin(); it != custom_rbtree.end(); it++)
 *         std::cout << '{' << it->length << ", " << it->breadth << '}' << std::endl;
 * }
 * @endcode
 *
 * @tparam T The type of the key
 * @tparam Compare The type of the Compare functor
 */
template <class T, class Compare = std::less<T>>
class red_black_tree {
    /// The compare functor
    Compare cmp {};

    /// A type alias for the nodes that will be used in the rbtree
    using node = node_impl<T>;
    node *sentinel_ptr { get_sentinel<T>() };

public:
    /// A type alias for the iterator type that will be used in the rbtree
    using iterator = raw_iterator<node, T>;

    /// A type alias for the const iterator type that will be used in the rbtree
    using const_iterator = raw_iterator<const node, const T>;

private:
    size_t sz { 0 };                        ///< The size of the tree, i.e. number of nodes

    node *root { sentinel_ptr };            ///< A pointer to the root

    iterator leftmost { sentinel_ptr };          ///< An iterator to the leftmost node
    iterator rightmost { sentinel_ptr };         ///< An iterator to the rightmost node
    const_iterator cleftmost { sentinel_ptr };   ///< A const iterator to the leftmost node
    const_iterator crightmost { sentinel_ptr };  ///< A const iterator to the rightmost node

    // ----------- helper methods ----------------

    /**
     * @brief Creates a new node and returns a pointer to it
     *
     * @param key The key value of the new node
     */
    node *create_node(const T &key);

    /**
     * @brief Transplants node y onto node x
     *
     * @param x A pointer to node x
     * @param y A pointer to node y
     */
    void transplant(node *x, node *y);

    /**
     * @brief Performs a left rotation
     *
     * @param x Pointer to the node to left rotate
     */
    void left_rotate(node *x);

    /**
     * @brief Performs a right rotation
     *
     * @param y Pointer to the node to right rotate
     */
    void right_rotate(node *y);

    /**
     * @brief Restores rbtree properties after an insertion through a series of rotations
     *
     * @param z Pointer to the node to fix
     */
    void fix_insert(node *z);

    /**
     * @brief Restores rbtree properties after an erase through a series of rotations and transplants
     *
     * @param it Successor of erased node
     */
    void fix_erase(node *x);

public:

    /**
     * @brief Construct a new red black tree object
     */
    red_black_tree() = default;

    /**
     * @brief Construct a new red black tree object
     *
     * @param T The type of the key
     * @param _cmp The compare functor
     */
    red_black_tree(T, Compare &&_cmp);

    /**
     * @brief Construct a new red black tree object
     *
     * @param cmp The compare functor
     */
    red_black_tree(Compare cmp);

    /**
     * @brief Construct a new red black tree object
     *
     * @tparam InputItThe type of the input iterator
     * @tparam isIter Compound enable_if to only allow iterators
     * @param first An iterator pointing to the first element to be inserted
     * @param last An iterator pointing to the position after the last element to be inserted
     */
    template <typename InputIt,
        typename = std::enable_if_t <
                        std::is_same < typename std::iterator_traits<InputIt>::iterator_category,
                                        std::random_access_iterator_tag >::value
                        || std::is_same < typename std::iterator_traits<InputIt>::iterator_category,
                                        std::bidirectional_iterator_tag >::value,
                        typename std::iterator_traits<InputIt>::difference_type >>
    red_black_tree(InputIt first, InputIt last);

    /**
     * @brief Construct a new red black tree object
     *
     * @param ilist An initializer list to intialize the rbtree
     */
    red_black_tree(std::initializer_list<T> ilist);

    /**
     * @brief Gets the begin iterator
     * @return iterator begin
     */
    iterator begin() const { return leftmost; }

    /**
     * @brief Gets the end iterator
     * @return iterator end
     */
    iterator end() const { return iterator { sentinel_ptr }; }

    /**
     * @brief Gets the begin const iterator
     * @return iterator begin
     */
    iterator cbegin() const { return cleftmost; }

    /**
     * @brief Gets the end const iterator
     * @return iterator end
     */
    iterator cend() const { return const_iterator { sentinel_ptr }; }

    // utility methods

    /**
     * @brief Gets the size of the tree, i.e. the number of nodes
     *
     * @return size_t The size of the tree
     */
    size_t size() const { return sz;  };

    /**
     * @brief Checks whether the rbtree is empty or not
     *
     * @return true if the rbtree has no nodes
     * @return false otherwise
     */
    bool  empty() const { return !sz; };

    /**
     * @brief Searches for an element with a given key
     *
     * @param key The key to be searched
     * @return iterator An iterator to the element in case it is found, else end()
     */
    iterator find(const T &key) const;

    /**
     * @brief Checks if a key exists
     *
     * @param key The key to check
     * @return true if the key exists in the tree
     * @return false otherwise
     */
    bool contains(const T &key) const;

    /**
     * @brief Returns an iterator pointing to the first element that is not less than (i.e. greater or equal to) key, or end if no such element is found.
     *
     * @param key The key value
     * @return iterator Iterator pointing to the first element that is not less than value, or end if no such element is found
     */
    iterator lower_bound(const T &key) const;

    /**
     * @brief Returns an iterator pointing to the first element that is greater than value, or end if no such element is found.
     *
     * @param key The key value
     * @return iterator Iterator pointing to the first element that is greater than value, or end if no such element is found.
     */
    iterator upper_bound(const T &key) const;

    // insert methods

    /**
     * @brief Inserts a range
     *
     * @tparam InputItThe type of the input iterator
     * @tparam isIter Compund enable_if to only allow iterators
     * @param first An iterator pointing to the first element to be inserted
     * @param last An iterator pointing to the position after the last element to be inserted
     */
    template <typename InputIt,
        typename = std::enable_if_t <
                        std::is_same < typename std::iterator_traits<InputIt>::iterator_category,
                                        std::random_access_iterator_tag >::value
                        || std::is_same < typename std::iterator_traits<InputIt>::iterator_category,
                                        std::bidirectional_iterator_tag >::value,
                        typename std::iterator_traits<InputIt>::difference_type >>
    void insert(InputIt first, InputIt last);

    /**
     * @brief Inserts elements in an initializer list
     *
     * @param ilist An initializer list of elements to be inserted
     */
    void insert(std::initializer_list<T> ilist);

    /**
     * @brief Inserts a key into the tree if it does not already exist
     *
     * @param key The key to be inserted
     * @return std::pair<iterator, bool> Returns a pair consisting of an iterator
     * to the inserted element (or to the element that prevented the insertion)
     * and a bool value set to true if the insertion took place.
     */
    std::pair<iterator, bool> insert(const T &key);

    // erase methods

    /**
     * @brief Erases a key from the tree if it exists
     *
     * @param key The key to be erased
     * @return true if the erase took place
     * @return false otherwise
     */
    bool erase(const T &key);

    /**
     * @brief Erases a node from the tree given an iterator pointing to it
     *
     * @param itr An iterator to the element to be erased
     * @return iterator An iterator to the next element in the rbtree
     */
    iterator erase(iterator itr);

    /**
     * @brief Erases nodes from a tree in the range [fisrt, last)
     *
     * @param first An iterator to the first node to be erased
     * @param last An iterator to the position after the last node to be erased
     */
    void erase(iterator first, iterator last);

};

/**
 * @brief Class template argument deduction for custom compare
 * @see [CTAD](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)
 * @tparam T The type of the key
 * @tparam Compare The type of the Compare functor
 */
template <class T, class Compare>
red_black_tree(T, Compare) -> red_black_tree<T, Compare>;

} // namespace BBST

#include <red_black_tree_impl.tpp>