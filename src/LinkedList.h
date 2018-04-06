#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi {

    template<typename Type>
    class LinkedList {
    private:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type *;
        using reference = Type &;
        using const_pointer = const Type *;
        using const_reference = const Type &;

        class ConstIterator;

        class Iterator;

        using const_iterator = ConstIterator;
        using iterator = Iterator;

        struct node {
            pointer value;
            struct node *next;
            struct node *prev;

            explicit node(pointer value) : value(value), next(nullptr), prev(nullptr) {}
            ~node() {
                delete value;
            }
        };

        using node_pointer = node *;

        node_pointer root;
        node_pointer tail;
        size_type size;

        void checkNotEmpty() {
            if (this->isEmpty()) {
                throw std::logic_error("Collection is empty.");
            }
        }

    public:

        LinkedList() : size(0) {
            this->root = this->tail = new node(nullptr);
        }

        LinkedList(std::initializer_list<Type> l) : LinkedList() {
            for (const auto &value : l) {
                append(value);
            }
        }

        LinkedList(const LinkedList &other) : LinkedList() {
            for (const auto &value : other) {
                append(value);
            }
        }

        LinkedList(LinkedList &&other) : LinkedList() {
            this->root = other.root;
            this->tail = other.tail;
            this->size = other.size;
            other.root = nullptr;
            other.tail = nullptr;
        }

        ~LinkedList() {
            node_pointer next = root;
            while (next != nullptr) {
                node_pointer to_delete = next;
                next = to_delete->next;
                delete to_delete;
            }
        }

        LinkedList &operator=(const LinkedList &other) {
            if (this == &other) {
                return *this;
            }

            erase(cbegin(), cend());
            append(other.begin(), other.end());
            return *this;
        }

        LinkedList &operator=(LinkedList &&other) {
            if (this == &other) {
                return *this;
            }

            erase(cbegin(), cend());
            root = other.root;
            tail = other.tail;
            size = other.size;

            other.root = other.tail = nullptr;
            other.size = 0;

            return *this;
        }

        bool isEmpty() const {
            return this->size == 0;
        }

        size_type getSize() const {
            return this->size;
        }

        void append(const Type &item) {
            insert(cend(), item);
        }

        void append(const const_iterator &start, const const_iterator &end) {
            for (auto it = start; it != end; ++it) {
                append(*it);
            }
        }

        void prepend(const Type &item) {
            insert(cbegin(), item);
        }

        void insert(const const_iterator &insertPosition, const Type &item) {
            const auto newNode = new node(new value_type(item));

            const auto insertPositionNode = insertPosition.current_node;
            newNode->next = insertPositionNode;
            newNode->prev = insertPositionNode->prev;
            newNode->next->prev = newNode;


            if (newNode->prev == nullptr) {
                this->root = newNode;
            } else {
                newNode->prev->next = newNode;
            }

            ++this->size;
        }

        Type popFirst() {
            this->checkNotEmpty();
            const auto first = *this->begin();
            erase(this->begin());
            return first;
        }

        Type popLast() {
            this->checkNotEmpty();
            const auto last = *(--this->end());
            erase(--this->end());
            return last;
        }

        void erase(const const_iterator &position) {
            if (position == end()) {
                throw std::out_of_range("Iterator is out of range");
            }
            const auto nodeToDelete = position.current_node;

            nodeToDelete->next->prev = nodeToDelete->prev;
            if (nodeToDelete == this->root) {
                this->root = nodeToDelete->next;
            } else {
                nodeToDelete->prev->next = nodeToDelete->next;
            }

            delete (nodeToDelete);
            --size;
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
            for (auto toDelete = firstIncluded; toDelete != lastExcluded; ++toDelete) {
                erase(toDelete);
            }
        }

        iterator begin() {
            return iterator(this->root, *this);
        }

        iterator end() {
            return iterator(this->tail, *this);
        }

        const_iterator cbegin() const {
            return const_iterator(this->root, *this);
        }

        const_iterator cend() const {
            return const_iterator(this->tail, *this);
        }

        const_iterator begin() const {
            return this->cbegin();
        }

        const_iterator end() const {
            return this->cend();
        }
    };

    template<typename Type>
    class LinkedList<Type>::ConstIterator {
        friend class LinkedList;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        explicit ConstIterator(node_pointer current_node, const LinkedList<value_type> &list) :
                current_node(current_node), list(list) {}

        reference operator*() const {
            checkIsNotEnd();
            return *(this->current_node->value);
        }

        ConstIterator &operator++() {
            checkIsNotEnd();
            this->current_node = this->current_node->next;
            return *this;
        }

        ConstIterator operator++(int) {
            checkIsNotEnd();
            const auto current = *this;
            this->current_node = this->current_node->next;
            return current;
        }

        ConstIterator &operator--() {
            checkIsNotBegin();
            this->current_node = this->current_node->prev;
            return *this;
        }

        ConstIterator operator--(int) {
            checkIsNotBegin();
            const auto current = *this;
            this->current_node = this->current_node->prev;
            return current;
        }

        ConstIterator operator+(difference_type d) const {
            auto copy = *this;
            for (int i = 0; i < d; ++i, ++copy);
            return copy;
        }

        ConstIterator operator-(difference_type d) const {
            auto copy = *this;
            for (int i = 0; i < d; ++i, --copy);
            return copy;
        }

        bool operator==(const ConstIterator &other) const {
            return this->current_node == other.current_node;
        }

        bool operator!=(const ConstIterator &other) const {
            return !(*this == other);
        }

    private:
        node_pointer current_node;
        const LinkedList<value_type> &list;

        void checkIsNotEnd() const {
            if (*this == list.end()) {
                throw std::out_of_range("Iterator is out of range");
            }
        }

        void checkIsNotBegin() const {
            if (*this == list.begin()) {
                throw std::out_of_range("Iterator is out of range");
            }
        };

    };

    template<typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator(node_pointer current_node, const LinkedList<value_type> &list)
                : ConstIterator(current_node, list) {}

        Iterator(const ConstIterator &other) : ConstIterator(other) {}

        Iterator &operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const {
            return ConstIterator::operator-(d);
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
