/* -*- C++ -*- */
#ifndef LQUEUE_CPP
#define LQUEUE_CPP

#include "LQueue.h"
#include <algorithm>
#include <memory>

/**
 * @class LQueue_Node
 * @brief Defines a node in the @a LQueue that's implemented as a circular
 * linked list.
 */
template <typename T> class LQueue_Node {
    friend class LQueue<T>;

    friend class LQueue_Iterator<T>;

    friend class LQueue_Const_Iterator<T>;

public:
    /// Constructor.
    LQueue_Node(const T& item, LQueue_Node<T>* next = 0);

    /// Constructor.
    LQueue_Node(LQueue_Node<T>* next);

    /// Default constructor that doesn't initialize <item_>.
    LQueue_Node();

    /// Allocate a new <LQueue_Node>, trying first from the @a free_list_
    /// and if that's empty try from the global @a ::operator new.
    void* operator new(size_t bytes);

    /// Return @a ptr to the @a free_list_.
    void operator delete(void* ptr);

    /// Return the next node to which this node points.
    LQueue_Node* next();

    /// Preallocate n @a LQueue_Nodes and store them on the @a
    /// free_list_.
    static void free_list_allocate(size_t n);

    /// Returns all dynamic memory on the free list to the free store.
    static void free_list_release();

private:
    /// Head of the free list, which is a stack of @a LQueue_Nodes
    /// used to speed up allocation.
    static LQueue_Node<T>* free_list;

    /// Item in this node.
    T item;

    /// Pointer to the next node.
    LQueue_Node<T>* nextPtr;
};

/* static */
template <typename T> LQueue_Node<T>* LQueue_Node<T>::free_list = 0;

// Allocate a new <LQueue_Node>, trying first from the
// <free_list_> and if that's empty try from the global <::operator
// new>.

template <typename T> void* LQueue_Node<T>::operator new(size_t)
{
    // extract element from the free_list_ if there is one left
    if (LQueue_Node<T>::free_list != 0) {
        // get the top element of the list
        LQueue_Node<T>* new_node = LQueue_Node<T>::free_list;

        // "remove" the element from the list and pass it to the caller
        LQueue_Node<T>::free_list = new_node->nextPtr;

        return new_node;
    }

    return ::operator new(sizeof(LQueue_Node<T>));
}

// Return <ptr> to the <free_list_>.
template <typename T> void LQueue_Node<T>::operator delete(void* ptr)
{
    // do nothing on a null pointer
    if (ptr != 0) {
        // cast to a node pointer
        LQueue_Node<T>* node = static_cast<LQueue_Node<T>*>(ptr);

        // put the node back into the list
        node->nextPtr = LQueue_Node<T>::free_list;

        LQueue_Node<T>::free_list = node;
    }
}

// Returns the next node to which this node points.
template <typename T> LQueue_Node<T>* LQueue_Node<T>::next()
{
    return next;
}

// Returns all dynamic memory on the free list to the free store.

template <typename T> void LQueue_Node<T>::free_list_release()
{
    // delete free list element by element
    while (LQueue_Node<T>::free_list != 0) {
        LQueue_Node<T>* node = LQueue_Node<T>::free_list;
        LQueue_Node<T>::free_list = node->nextPtr;
        ::operator delete(node);
    }
}

// Preallocate <n> <LQueue_Nodes> and store them on the
// <free_list_>.

template <typename T> void LQueue_Node<T>::free_list_allocate(size_t n)
{
    // add a new element to the stack n times
    for (size_t node_number = 0; node_number < n; ++node_number) {
        // create a new element avoiding the overwritten new operator
        LQueue_Node<T>* new_node
            = reinterpret_cast<LQueue_Node<T>*>(::operator new(sizeof(LQueue_Node<T>)));

        new_node->nextPtr = LQueue_Node<T>::free_list;

        // make the new element the top of the list
        LQueue_Node<T>::free_list = new_node;
    }
}

template <typename T>
LQueue_Node<T>::LQueue_Node(const T& item, LQueue_Node<T>* next)
    : item(item)
    , nextPtr(next)
{
}

template <typename T>
LQueue_Node<T>::LQueue_Node(LQueue_Node<T>* next)
    : nextPtr(next)
{
}

// This method is helpful to implement the dummy node in a concise
// way.
template <typename T>
LQueue_Node<T>::LQueue_Node()
    : nextPtr(this)
{
}

// Returns the current size.
template <typename T> size_t LQueue<T>::size() const
{
    return count;
}

// Constructor.

template <typename T>
LQueue<T>::LQueue(size_t size_hint)
    // Initialize fields here.
    : tail(0)
    , count(0)
{
    // use the size_hint to preallocate  memory for nodes
    LQueue_Node<T>::free_list_allocate(size_hint);

    // create the dummy node
    tail = new LQueue_Node<T>();
}

// Copy constructor.

template <typename T>
LQueue<T>::LQueue(const LQueue<T>& rhs)
    // Initialize fields here.
    : tail(nullptr)
    , count(0) // count will be set correctly by copy_list
{
    // insert a dummy node first and keep it as an unique_ptr for exception
    // safety issues
    std::unique_ptr<LQueue_Node<T>> tempTail(new LQueue_Node<T>());
    tail = tempTail.get();

    // copy_list has strong exception safety, so no try catch block
    // is necessary here
    copy_list(rhs);

    // from here on, the unique_ptr should not try to delete the
    // tail pointer anymore.
    tempTail.release();
}

// Copy a linked list of nodes
template <typename T> void LQueue<T>::copy_list(const LQueue<T>& rhs)
{
    LQueue<T> temp;

    // enqueue the elements into the temporary list
    for (typename LQueue<T>::const_iterator it = rhs.begin(); it != rhs.end(); ++it) {
        temp.enqueue(*it);
    }

    // we only swap the lists if the temporary list has been successfully
    // created. This ensures strong exception guarantees.
    std::swap(tail, temp.tail);

    // swap the counts too
    std::swap(count, temp.count);
}

// Delete a linked list of nodes
template <typename T> void LQueue<T>::delete_list()
{
    // we do not delete the dummy node here. This will be done in the destructor
    // we dequeue all elements until the queue is empty again
    while (!is_empty()) {
        dequeue_i();
    }
}

// Assignment operator.
template <typename T> LQueue<T>& LQueue<T>::operator=(const LQueue<T>& rhs)
{
    // test for self assignment first
    if (this != &rhs) {
        // delete old data of the rhs
        delete_list();

        // copy new data
        copy_list(rhs);
    }

    return *this;
}

// Perform actions needed when queue goes out of scope.

template <typename T> LQueue<T>::~LQueue()
{
    // delete all elements of the list
    delete_list();

    // delete the last dummy node here
    delete tail;
}

// Compare this queue with <rhs> for equality.  Returns true if the
// size()'s of the two queues are equal and all the elements from 0
// .. size() are equal, else false.
template <typename T> bool LQueue<T>::operator==(const LQueue<T>& rhs) const
{
    return (size() == rhs.size()) && std::equal(begin(), end(), rhs.begin());
}

// Compare this queue with <rhs> for inequality such that <*this> !=
// <s> is always the complement of the boolean return value of
// <*this> == <s>.

template <typename T> bool LQueue<T>::operator!=(const LQueue<T>& rhs) const
{
    return !(*this == rhs);
}

// Place a <new_item> at the tail of the queue.  Throws
// the <Overflow> exception if the queue is full.

template <typename T> void LQueue<T>::enqueue(const T& new_item)
{
    try {
        // assign the value to the tail element before the new allocation
        // to make sure that exceptions thrown in the assignment operator
        // of T does leave the queue structure altered
        tail->item = new_item;

        // integrate the new node into the list
        tail->nextPtr = new LQueue_Node<T>(tail->nextPtr);
        tail = tail->nextPtr;

        // increment the element count
        ++count;
    } catch (const std::bad_alloc&) {
        // we transform a bad_alloc exception into an overflow exception,
        // because it basically means, that it is no longer possible
        // to enqueue new elements
        throw Overflow();
    }
}

// Remove and return the front item on the queue.
// Throws the <Underflow> exception if the queue is empty.

template <typename T> T LQueue<T>::dequeue()
{
    // check for empty queue first
    if (is_empty()) {
        throw Underflow();
    }

    // extract the value of the head node. This is done before we actually
    // remove the element for exceptions could be thrown in the assignment
    // operator.
    T item = tail->nextPtr->item;

    // call actual dequeue implementation
    dequeue_i();

    return item;
}

template <typename T> void LQueue<T>::dequeue_i()
{
    // remember the current queue head
    LQueue_Node<T>* head = tail->nextPtr;
    // remove the head from the queue
    tail->nextPtr = head->nextPtr;
    // decrement the element count
    --count;
    // delete the old head node
    delete head;
}

// Returns the front queue item without removing it.
// Throws the <Underflow> exception if the queue is empty.

template <typename T> T LQueue<T>::front() const
{
    // check for empty queue first
    if (is_empty())
        throw Underflow();

    // return the item in head
    return tail->nextPtr->item;
}

// Returns true if the queue is empty, otherwise returns false.

template <typename T> bool LQueue<T>::is_empty() const
{
    return count == 0;
}

// Returns true if the queue is full, otherwise returns false.

template <typename T> bool LQueue<T>::is_full() const
{
    // there is no upper limit for the queue
    return false;
}

// Get an iterator to the begining of the queue
template <typename T> typename LQueue<T>::iterator LQueue<T>::begin()
{
    // iterator starts at the head element
    return typename LQueue<T>::iterator(*this, tail->next);
}

// Get an iterator pointing past the end of the queue
template <typename T> typename LQueue<T>::iterator LQueue<T>::end()
{
    // iterator starts at the tail element
    return typename LQueue<T>::iterator(*this, tail);
}

// Get an iterator to the begining of the queue
template <typename T> typename LQueue<T>::const_iterator LQueue<T>::begin() const
{
    // iterator starts at the head element
    return typename LQueue<T>::const_iterator(*this, tail->nextPtr);
}

// Get an iterator pointing past the end of the queue
template <typename T> typename LQueue<T>::const_iterator LQueue<T>::end() const
{
    // iterator starts at the tail element
    return typename LQueue<T>::const_iterator(*this, tail);
}

template <typename T> T& LQueue_Iterator<T>::operator*()
{
    return pos->item;
}

template <typename T> const T& LQueue_Iterator<T>::operator*() const
{
    return pos->item;
}

template <typename T> LQueue_Iterator<T>& LQueue_Iterator<T>::operator++()
{
    // advance to the next position
    pos = pos->next;

    return *this;
}

// Post-increment.
template <typename T> LQueue_Iterator<T> LQueue_Iterator<T>::operator++(int)
{
    // keep copy of the original iterator
    LQueue_Iterator<T> copy = *this;

    // advance to the next position
    pos = pos->next;

    // return original iterator
    return copy;
}

template <typename T> bool LQueue_Iterator<T>::operator==(const LQueue_Iterator<T>& rhs) const
{
    // check if the iterator points to the same position in the same queue
    // (we could even omit the check for queue equality, because it is
    //  very unlikely that two queues share the same node pointer)
    return (pos == rhs.pos);
}

template <typename T> bool LQueue_Iterator<T>::operator!=(const LQueue_Iterator<T>& rhs) const
{
    // implement != in terms of ==
    return !(*this == rhs);
}

template <typename T>
LQueue_Iterator<T>::LQueue_Iterator(LQueue<T>& queue, size_t position)
    : queue(queue)
    , pos(queue.tail->next)
{
    // iterator over the queue unto the right position
    // we save iterations for values > count_ by doing modulo calculations
    for (position = position % (queue.count - 1); position > 0; --position) {
        // advance one position each time
        pos = pos->next;
    }
}

template <typename T>
LQueue_Iterator<T>::LQueue_Iterator(LQueue<T>& queue, LQueue_Node<T>* pos)
    : queue(queue)
    , pos(pos)
{
}

template <typename T> const T& LQueue_Const_Iterator<T>::operator*() const
{
    return pos->item;
}

template <typename T> const LQueue_Const_Iterator<T>& LQueue_Const_Iterator<T>::operator++() const
{
    // advance to the next position
    pos = pos->nextPtr;
    return *this;
}

template <typename T> LQueue_Const_Iterator<T> LQueue_Const_Iterator<T>::operator++(int) const
{
    // keep copy of the original iterator
    LQueue_Const_Iterator<T> copy = *this;
    // advance to the next position
    pos = pos->next;
    // return original iterator
    return copy;
}

template <typename T>
bool LQueue_Const_Iterator<T>::operator==(const LQueue_Const_Iterator<T>& rhs) const
{
    // check if the iterator points to the same position in the same queue
    return (pos == rhs.pos);
}

template <typename T>
bool LQueue_Const_Iterator<T>::operator!=(const LQueue_Const_Iterator<T>& rhs) const
{
    return !(*this == rhs);
}

template <typename T>
LQueue_Const_Iterator<T>::LQueue_Const_Iterator(const LQueue<T>& queue, size_t position)
    : queue(queue)
    , pos(queue.tail->next)
{
    // iterator over the queue unto the right position
    // we save iterations for values > count_ by doing modulo calculations
    for (position = position % (queue.count - 1); position > 0; --position) {
        // advance one position each time
        pos = pos->next;
    }
}

template <typename T>
LQueue_Const_Iterator<T>::LQueue_Const_Iterator(const LQueue<T>& queue, LQueue_Node<T>* pos)
    : queue(queue)
    , pos(pos)
{
}

#endif // LQUEUE_CPP
