#ifndef VECTOR_H_
#define VECTOR_H_

#include <cstddef>

template <class Item>
class Vector {
   private:
    Item *data_;
    size_t capacity_;
    size_t size_;

    void clear_starting_from(size_t index) {
        for (size_t i = index; i < size_; i++) delete &data_[i];
        if (size_ > index) size_ = index;
    }

   public:
    void swap(Vector &b) {
        swap(data_, b.data_);
        swap(capacity_, b.capacity_);
        swap(size_, b.size_);
    }

    Item &operator[](size_t index) { return data_[index]; }

    Item *data() { return data_; }

    size_t capacity() { return capacity_; }

    size_t size() { return size_; }

    Vector() {
        data_ = nullptr;
        capacity_ = 0;
        size_ = 0;
    }

    Vector(size_t capacity) : Vector() {
        resize(capacity);
    }

    void clear() { clear_starting_from(0); }

    ~Vector() {
        clear();
        delete[] data_;
    }

    void resize(size_t capacity) {
        if (capacity == capacity_) return;

        clear_starting_from(capacity);
        capacity_ = capacity;

        Item *data = new Item[capacity];
        std::copy(data_, data_ + capacity, data);
        delete[] data_;
        data_ = data;
    }

    void reserve(size_t capacity) {
        if (capacity <= capacity_) return;
        resize(capacity);
    }

    void push_back(Item value) {
        if (capacity_ == size_) reserve(capacity_ * 2 + 1);
        data_[size_] = value;
        size_++;
    }

    void pop_back() { clear_starting_from(size_ - 1); }
};

#endif  // VECTOR_H_
