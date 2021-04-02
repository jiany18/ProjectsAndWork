/* -*- C++ -*- */
#ifndef LSTACK_CPP
#define LSTACK_CPP

#include "LStack.h"
#include <algorithm>
#include <memory>

/**
 * @class LStack_Node
 * @brief Defines a node in the @a LStack that's implemented as a linked list.
 */
template <typename T> class LStack_Node {
    friend class LStack<T>;

    friend class LStack_Iterator<T>;

    friend class LStack_Const_Iterator<T>;

public:
    // = Initialization methods
    LStack_Node(const T& item, LStack_Node<T>* next = nullptr);

    LStack_Node(LStack_Node<T>* next);

    LStack_Node();
    // Default constructor that doesn't initialize <item_>.

    void* operator new(size_t bytes);
    // Allocate a new <LStack_Node>, trying first from the
    // <free_list_> and if that's empty try from the global <::operator
    // new>.

    void operator delete(void* ptr);
    // Return <ptr> to the <free_list_>.

    LStack_Node* next();
    // Return the next node to which this node points.
    // This method is added to support the Scoped_List class below.

    static void free_list_allocate(size_t n);
    // Preallocate <n> <LStack_Nodes> and store them on the
    // <free_list_>.

    static void free_list_release();
    // Returns all dynamic memory on the free list to the free store.

private:
    static LStack_Node<T>* free_list;
    // Head of the "free list", which is a stack of
    // <LStack_Nodes> used to speed up allocation.

    T item;
    // Item in this node.

    LStack_Node<T>* nextPtr;
    // Pointer to the next node.
};

/* static */
template <typename T> LStack_Node<T>* LStack_Node<T>::free_list = nullptr;

// Allocate a new <LStack_Node>, trying first from the
// <free_list_> and if that's empty try from the global <::operator
// new>.

template <typename T> void* LStack_Node<T>::operator new(size_t)
{
    // extract element from the free_list_ if there is one left
    if (LStack_Node<T>::free_list != nullptr) {
        // get the top element of the list
        LStack_Node<T>* new_node = LStack_Node<T>::free_list;

        // "remove" the element from the list and pass it to the caller
        LStack_Node<T>::free_list = new_node->nextPtr;

        return new_node;
    }

    return ::operator new(sizeof(LStack_Node<T>));
}

// Return <ptr> to the <free_list_>.

template <typename T> void LStack_Node<T>::operator delete(void* ptr)
{
    // do nothing on a null pointer
    if (ptr != nullptr) {
        // cast to a node pointer
        LStack_Node<T>* node = static_cast<LStack_Node<T>*>(ptr);

        // put the node back into the list
        node->nextPtr = LStack_Node<T>::free_list;

        LStack_Node<T>::free_list = node;
    }
}

// Returns the next node to which this node points.
template <typename T> LStack_Node<T>* LStack_Node<T>::next()
{
    return nextPtr;
}

// Returns all dynamic memory on the free list to the free store.
template <typename T> void LStack_Node<T>::free_list_release()
{
    // delete free list element by element
    while (LStack_Node<T>::free_list != nullptr) {
        LStack_Node<T>* node = LStack_Node<T>::free_list;
        LStack_Node<T>::free_list = node->nextPtr;
        ::operator delete(node);
    }
}

// Preallocate <n> <LStack_Nodes> and store them on the
// <free_list>.
template <typename T> void LStack_Node<T>::free_list_allocate(size_t n)
{
    // add a new element to the stack n times
    for (size_t node_number = 0; node_number < n; ++node_number) {
        // create a new element avoiding the overwritten new operator
        LStack_Node<T>* new_node
            = reinterpret_cast<LStack_Node<T>*>(::operator new(sizeof(LStack_Node<T>)));

        new_node->nextPtr = LStack_Node<T>::free_list;

        // make the new element the top of the list
        LStack_Node<T>::free_list = new_node;
    }
}

template <typename T>
LStack_Node<T>::LStack_Node(const T& item, LStack_Node<T>* next)
    : item(item)
    , nextPtr(next)
{
}

template <typename T>
LStack_Node<T>::LStack_Node(LStack_Node<T>* next)
    : nextPtr(next)
{
}

// This method is helpful to implement the dummy node in a concise
// way.

template <typename T>
LStack_Node<T>::LStack_Node()
    : nextPtr(this)
{
}

// Returns the current size.
template <typename T> size_t LStack<T>::size() const
{
    return count;
}

// Constructor.
template <typename T>
LStack<T>::LStack(size_t size_hint)
    // Initialize fields here.
    : head(nullptr)
    , count(0)
{
    // use the size_hint to preallocate  memory for nodes
    LStack_Node<T>::free_list_allocate(size_hint);
}

// Copy constructor.
template <typename T>
LStack<T>::LStack(const LStack<T>& rhs)
    // Initialize fields here.
    : head(nullptr)
    , count(0) // count will be set correctly by copy_list
{
    // insert a dummy node first and keep it as an unique_prt for exception
    // safety issues
    std::unique_ptr<LStack_Node<T>> tail(new LStack_Node<T>());
    head = tail.get();

    // copy_list has strong exception safety, so no try catch block
    // is necessary here
    copy_list(rhs);

    // from here on, the unique_ptr should not try to delete the
    // tail pointer anymore.
    tail.release();
}

// Copy a linked list of nodes
template <typename T> void LStack<T>::copy_list(const LStack<T>& rhs)
{
    LStack<T> temp;
    LStack_Node<T>* prev = nullptr;

    // Iterate along the list of stack nodes in <s>, creating a new
    // corresponding node and chaining them along. Note that we
    // can't use push() to insert at the head since it will reverse
    // the stack.

    std::unique_ptr<LStack_Node<T>> new_node;

    for (typename LStack<T>::const_iterator it = rhs.begin(); it != rhs.end(); ++it) {
        new_node.reset(new LStack_Node<T>(*it));

        if (it == rhs.begin()) {
            // special case for the first iteration: set the head element of
            // temporary stack
            temp.head = new_node.release();
            prev = temp.head;
        } else {
            // standard case: add one element to prev
            prev->nextPtr = new_node.release();
            prev = prev->nextPtr;
        }

        // make sure that the element count of temp stays correct
        ++temp.count;
    }

    // we only swap the lists if the temporary list has been successfully
    // created. This ensures strong exception guarantees.
    std::swap(head, temp.head);

    // set the counts correctly
    std::swap(count, temp.count);
}

// Delete a linked list of nodes
template <typename T> void LStack<T>::delete_list()
{
    // we do not delete the dummy node here. This will be done in the destructor
    // we pop all elements until the queue is empty again
    while (!is_empty()) {
        pop_i();
    }
}

// Delete a linked list of nodes
template <typename T> void LStack<T>::erase()
{
    delete_list();
}

// Assignment operator.
template <typename T> LStack<T>& LStack<T>::operator=(const LStack<T>& rhs)
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

template <typename T> LStack<T>::~LStack()
{
    // delete all elements of the list
    delete_list();
}

// Compare this queue with <rhs> for equality.  Returns true if the
// size()'s of the two queues are equal and all the elements from 0
// .. size() are equal, else false.
template <typename T> bool LStack<T>::operator==(const LStack<T>& rhs) const
{
    return (size() == rhs.size()) && std::equal(begin(), end(), rhs.begin());
}

// Compare this queue with <rhs> for inequality such that <*this> !=
// <s> is always the complement of the boolean return value of
// <*this> == <s>.

template <typename T> bool LStack<T>::operator!=(const LStack<T>& rhs) const
{
    return !(*this == rhs);
}

// Place a <new_item> at the tail of the queue.  Throws
// the <Overflow> exception if the queue is full.

template <typename T> void LStack<T>::push(const T& new_item)
{
    try {
        // create a temporary new node for exception safety reasons
        std::unique_ptr<LStack_Node<T>> temp(new LStack_Node<T>(new_item, head));

        // integrate the new node into the list
        head = temp.release();

        // increment the element count
        ++count;
    } catch (const std::bad_alloc&) {
        // we transform a bad_alloc exception into an overflow exception,
        // because it basically means, that it is no longer possible
        // to push new elements
        throw Overflow();
    }
}

// Remove and return the front item on the queue.
// Throws the <Underflow> exception if the queue is empty.

template <typename T> T LStack<T>::pop()
{
    // check for empty queue first
    if (is_empty()) {
        throw Underflow();
    }

    // extract the value of the head node. This is done before we actually
    // remove the element for exceptions could be thrown in the assignment
    // operator.
    T item = head->item;

    // call actual pop implementation
    pop_i();

    return item;
}

template <typename T> void LStack<T>::pop_i()
{
    // remember the current queue head
    LStack_Node<T>* tmpHead = head;
    // remove the head from the queue
    head = tmpHead->nextPtr;
    // decrement the element count
    --count;
    // delete the old head node
    delete tmpHead;
}

// Returns the front queue item without removing it.
// Throws the <Underflow> exception if the queue is empty.

template <typename T> T LStack<T>::top() const
{
    // check for empty queue first
    if (is_empty())
        throw Underflow();

    // return the item in head
    return head->item;
}

// Returns true if the queue is empty, otherwise returns false.

template <typename T> bool LStack<T>::is_empty() const
{
    return count == 0;
}

// Returns true if the queue is full, otherwise returns false.

template <typename T> bool LStack<T>::is_full() const
{
    // there is no upper limit for the queue
    return false;
}

// Get an iterator to the beginning of the queue
template <typename T> typename LStack<T>::iterator LStack<T>::begin()
{
    // iterator starts at the head element
    return typename LStack<T>::iterator(*this, head);
}

// Get an iterator pointing past the end of the queue
template <typename T> typename LStack<T>::iterator LStack<T>::end()
{
    // iterator starts at the tail element
    return typename LStack<T>::iterator(*this, (LStack_Node<T>*)nullptr);
}

// Get an iterator to the begining of the queue
template <typename T> typename LStack<T>::const_iterator LStack<T>::begin() const
{
    // iterator starts at the head element
    return typename LStack<T>::const_iterator(*this, head);
}

// Get an iterator pointing past the end of the queue
template <typename T> typename LStack<T>::const_iterator LStack<T>::end() const
{
    // iterator starts at the tail element
    return typename LStack<T>::const_iterator(*this, (LStack_Node<T>*)nullptr);
}

template <typename T> T& LStack_Iterator<T>::operator*()
{
    return pos->item;
}

template <typename T> const T& LStack_Iterator<T>::operator*() const
{
    return pos->item;
}

template <typename T> LStack_Iterator<T>& LStack_Iterator<T>::operator++()
{
    // advance to the next position
    pos = pos->next;

    return *this;
}

// Post-increment.
template <typename T> LStack_Iterator<T> LStack_Iterator<T>::operator++(int)
{
    // keep copy of the original iterator
    LStack_Iterator<T> copy = *this;

    // advance to the next position
    pos = pos->next;

    // return original iterator
    return copy;
}

template <typename T> bool LStack_Iterator<T>::operator==(const LStack_Iterator<T>& rhs) const
{
    // check if the iterator points to the same position in the same queue
    // (we could even omit the check for queue equality, because it is
    //  very unlikely that two queues share the same node pointer)
    return (pos == rhs.pos);
}

template <typename T> bool LStack_Iterator<T>::operator!=(const LStack_Iterator<T>& rhs) const
{
    // implement != in terms of ==
    return !(*this == rhs);
}

template <typename T>
LStack_Iterator<T>::LStack_Iterator(LStack<T>& stack, size_t position)
    : stack(stack)
    , pos(stack.head)
{
    // iterator over the stack unto the right position
    // we save iterations for values > count by doing modulo calculations
    for (position = position % (stack.count - 1); position > 0; --position) {
        // advance one position each time
        pos = pos->nextPtr;
    }
}

template <typename T>
LStack_Iterator<T>::LStack_Iterator(LStack<T>& stack, LStack_Node<T>* pos)
    : stack(stack)
    , pos(pos)
{
}

template <typename T> const T& LStack_Const_Iterator<T>::operator*() const
{
    return pos->item;
}

template <typename T> const LStack_Const_Iterator<T>& LStack_Const_Iterator<T>::operator++() const
{
    // advance to the next position
    pos = pos->nextPtr;
    return *this;
}

template <typename T> LStack_Const_Iterator<T> LStack_Const_Iterator<T>::operator++(int) const
{
    // keep copy of the original iterator
    LStack_Const_Iterator<T> copy = *this;
    // advance to the next position
    pos = pos->nextPtr;
    // return original iterator
    return copy;
}

template <typename T>
bool LStack_Const_Iterator<T>::operator==(const LStack_Const_Iterator<T>& rhs) const
{
    // check if the iterator points to the same position in the same stack
    return (pos == rhs.pos);
}

template <typename T>
bool LStack_Const_Iterator<T>::operator!=(const LStack_Const_Iterator<T>& rhs) const
{
    return !(*this == rhs);
}

template <typename T>
LStack_Const_Iterator<T>::LStack_Const_Iterator(const LStack<T>& stack, size_t position)
    : stack(stack)
    , pos(stack.head)
{
    // iterator over the stack unto the right position
    // we save iterations for values > count by doing modulo calculations
    for (position = position % (stack.count - 1); position > 0; --position) {
        // advance one position each time
        pos = pos->nextPtr;
    }
}

template <typename T>
LStack_Const_Iterator<T>::LStack_Const_Iterator(const LStack<T>& stack, LStack_Node<T>* pos)
    : stack(stack)
    , pos(pos)
{
}

#endif // LSTACK_CPP
