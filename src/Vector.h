#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi {

    template<typename Type>
    class Vector {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type *;
        using reference = Type &;
        using const_pointer = const Type *;
        using const_reference = const Type &;

        class ConstIterator;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        Vector() {
            this->reserved_size = 4;
            this->size = 0;
            this->storage = new value_type[this->reserved_size];
        }

        Vector(std::initializer_list<Type> l) {
            this->size = 0;
            this->reserved_size = l.size();
            this->storage = new value_type[this->reserved_size];
            for (const auto &value: l) {
                this->storage[this->size++] = value;
            }
        }

        Vector(const Vector &other) {
            this->size = other.size;
            this->reserved_size = other.size;
            this->storage = new value_type[this->reserved_size];
            std::copy(other.begin(), other.end(), this->storage);
        }

        Vector(Vector &&other) {
            this->storage = other.storage;
            other.storage = nullptr;
            this->size = other.size;
            this->reserved_size = other.reserved_size;
        }

        ~Vector() {
            if (this->storage) {
                delete[] this->storage;
            }
        }

        Vector &operator=(const Vector &other) {
            if (this == &other) {
                return *this;
            }

            delete[] this->storage;
            this->storage = new Type[other.reserved_size];
            this->size = other.size;
            this->reserved_size = other.reserved_size;
            std::copy(other.cbegin(), other.cend(), begin());

            return *this;
        }

        Vector &operator=(Vector &&other) {
            if (this == &other) {
                return *this;
            }

            delete[] this->storage;
            this->storage = other.storage;
            other.storage = nullptr;
            this->reserved_size = other.reserved_size;
            this->size = other.size;
            return *this;
        }

        bool isEmpty() const {
            return this->getSize() == 0;
        }

        size_type getSize() const {
            return this->size;
        }

        void append(const Type &item) {
            this->insert(this->cend(), item);
        }

        void prepend(const Type &item) {
            this->insert(this->cbegin(), item);
        }

        void insert(const const_iterator &insertPosition, const Type &item) {
            difference_type dst =  insertPosition - cbegin();
            if (this->size == this->reserved_size) {
                reallocate();
            }
            for (pointer it = this->storage + this->size - 1; it >= this->storage + dst; --it) {
                *(it + 1) = *it;
            }
            ++this->size;
            *(begin() + dst) = item;
        }

        Type popFirst() {
            this->checkNotEmpty();
            const auto first = this->storage[0];
            std::copy(++this->begin(), this->end(), this->begin());
            --this->size;
            return first;
        }

        Type popLast() {
            this->checkNotEmpty();
            --this->size;
            const auto last = this->storage[this->size];
            return last;
        }

        void erase(const const_iterator &position) {
            std::copy(position + 1, cend(), iterator(position));
            --this->size;
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
            std::copy(lastExcluded, cend(), iterator(firstIncluded));
            this->size -= lastExcluded - firstIncluded;
        }

        iterator begin() {
            return iterator(this->storage, *this);
        }

        iterator end() {
            return iterator(this->storage + this->size, *this);
        }

        const_iterator cbegin() const {
            return const_iterator(this->storage, *this);
        }

        const_iterator cend() const {
            return const_iterator(this->storage + this->size, *this);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        pointer storage;
        size_type size;
        size_type reserved_size;

        void reallocate() {
            this->reserved_size = this->reserved_size + this->reserved_size / 2 + 1;
            const auto newStorage = new value_type[this->reserved_size];
            std::copy(this->cbegin(), this->cend(), newStorage);
            delete[] storage;
            this->storage = newStorage;
        }

        void checkNotEmpty() {
            if (this->isEmpty()) {
                throw std::logic_error("Collection is empty.");
            }
        }
    };

    template<typename Type>
    class Vector<Type>::ConstIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;

        explicit ConstIterator(pointer current, const Vector<value_type> &vector) : current(current), vector(vector) {}

        reference operator*() const {
            this->checkIsNotEnd();
            return *(current);
        }

        ConstIterator &operator++() {
            this->checkIsNotEnd();
            ++current;
            return *this;
        }

        ConstIterator operator++(int) {
            this->checkIsNotEnd();
            const auto result = *this;
            ++current;
            return result;
        }

        ConstIterator &operator--() {
            this->checkIsNotBegin();
            --current;
            return *this;
        }

        ConstIterator operator--(int) {
            this->checkIsNotBegin();
            const auto result = *this;
            --current;
            return result;
        }

        ConstIterator &operator+=(difference_type d) {
            current += d;
            return *this;
        }

        ConstIterator &operator-=(difference_type d) {
            current -= d;
            return *this;
        }

        ConstIterator operator+(difference_type d) const {
            auto result = *this;
            result += d;
            return result;
        }

        difference_type operator-(const ConstIterator &other) const {
            return current - other.current;
        }

        ConstIterator operator-(difference_type d) const {
            auto result = *this;
            result -= d;
            return result;
        }

        bool operator==(const ConstIterator &other) const {
            return current == other.current;
        }

        bool operator!=(const ConstIterator &other) const {
            return !(*this == other);
        }

    private:
        pointer current;
        const Vector<value_type> &vector;

        void checkIsNotEnd() const {
            if (*this == vector.end()) {
                throw std::out_of_range("Iterator is out of range");
            }
        }

        void checkIsNotBegin() const {
            if (*this == vector.begin()) {
                throw std::out_of_range("Iterator is out of range");
            }
        };
    };

    template<typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;

        explicit Iterator(pointer current, const Vector<value_type> &vector) : ConstIterator(current, vector) {}

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

#endif // AISDI_LINEAR_VECTOR_H
