/**
 * @file iterator.tpp
 * @author the-hyp0cr1t3
 * @brief Describes an STL-like raw iterator template for the rbtree
 * @date 2022-03-25
 */
#pragma once

// template <typename T>
// concept has_key = requires(T x) { x.key; };      // C++20 only

/**
 * @class raw_iterator
 * @brief STL-like raw iterator template for the rbtree
 *
 * @tparam node_t The node type
 * @tparam key_t The key type
 */
template <class node_t, class key_t>
struct raw_iterator {
    using value_type        = key_t;
    using pointer           = const key_t *;    // const so as to not discard const-ness of node->key
    using reference         = const key_t &;    // const so as to not discard const-ness of node->key
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    /**
     * @brief Construct a new raw iterator object
     *
     * @param ptr Pointer which the iterator will use under the hood
     */
    raw_iterator(node_t *ptr = nullptr): m_ptr(ptr) {}

    /**
     * @brief Copy constructor
     *
     * @param other Another iterator to copy
     */
    raw_iterator(const raw_iterator &other) = default;

    /**
     * @brief Overloading the `=` operator (assignment)
     *
     * @param other Another iterator to copy
     * @return `raw_iterator&` A reference to this iterator after making the copy
     */
    raw_iterator &operator=(const raw_iterator &other) = default;

    /**
     * @brief Overloading the `=` operator (assignment)
     *
     * @param ptr Pointer which the iterator will use under the hood
     * @return `raw_iterator&` A reference to this iterator after making the copy
     */
    raw_iterator &operator=(pointer ptr) {
        m_ptr = ptr;
        return *this;
    }

    /**
     * @brief Checks if this iterator is valid
     *
     * @return `true` if it points to a non null object
     * @return `false` otherwise
     */
    operator bool() const { return m_ptr; }

    /**
     * @brief Overloading the `==` operator
     *
     * @param itr The other iterator to compare to
     * @return `true` if the pointers of the iterators match
     * @return `false` otherwise
     */
    bool operator==(const raw_iterator &itr) const {
        return m_ptr == itr.get_const_ptr();
    }

    /**
     * @brief Overloading the `!=` operator
     *
     * @param itr The other iterator to compare to
     * @return `true` if the pointers of the iterators do not match
     * @return `false` otherwise
     */
    bool operator!=(const raw_iterator &itr) const {
        return m_ptr != itr.get_const_ptr();
    }

    /**
     * @brief Overloading the `++` operator (prefix increment)
     *
     * Calls node_t::next under the hood to get the next pointer. <br>
     * Returns nullptr if there is no successor node.
     *
     * @return `raw_iterator&` An iterator to the successor node
     */
    raw_iterator &operator++() {
        m_ptr = node_t::next(m_ptr);
        return *this;
    }

    /**
     * @brief Overloading the `--` operator (prefix decrement)
     *
     * Calls node_t::prev under the hood to get the previous pointer. <br>
     * Returns nullptr if there is no predecessor node.
     *
     * @return `raw_iterator&` An iterator to the predecessor node
     */
    raw_iterator &operator--() {
        m_ptr = node_t::prev(m_ptr);
        return *this;
    }

    /**
     * @brief Overloading the `++` operator (postfix increment)
     *
     * Calls node_t::next under the hood to get the next pointer. <br>
     * Returns nullptr if there is no successor node.
     *
     * @return `raw_iterator` An iterator to the successor node
     */
    raw_iterator operator++(int) {
        raw_iterator res{*this};
        m_ptr = node_t::next(m_ptr);
        return res;
    }

    /**
     * @brief Overloading the `--` operator (postfix decrement)
     *
     * Calls node_t::prev under the hood to get the previous pointer. <br>
     * Returns nullptr if there is no predecessor node.
     *
     * @return `raw_iterator` An iterator to the predecessor node
     */
    raw_iterator operator--(int) {
        raw_iterator res{*this};
        m_ptr = node_t::prev(m_ptr);
        return res;
    }

    /**
     * @brief Overloading the `*` operator (dereference)
     *
     * Mimics bare pointers, can be dereferenced like one, to access the node's key.
     *
     * @return `reference` A reference to the node's key
     */
    reference operator*() { return m_ptr->key; }

    /**
     * @brief Overloading the `*` operator (dereference)
     *
     * Mimics bare pointers, can be dereferenced like one, to access the node's key.
     *
     * @return `const reference` A const reference to the node's key
     */
    const reference operator*() const { return m_ptr->key; }

    /**
     * @brief Overloading the `->` operator
     *
     * @return `pointer` A pointer to the underlying node ptr
     */
    pointer operator->() { return &m_ptr->key; }

    /**
     * @brief Get the const ptr object
     *
     * @return `const node_t*` A const pointer to the underlying node ptr
     */
    const node_t *get_const_ptr() const { return m_ptr; }

    /**
     * @brief Get the ptr object
     *
     * @return `node_t*` A pointer to the underlying node ptr
     */
    node_t *get_ptr() const { return m_ptr; }

protected:
    /// The underlying node pointer
    node_t *m_ptr;
};