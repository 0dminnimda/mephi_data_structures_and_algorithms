#ifndef VECTOR_H_
#define VECTOR_H_

#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <utility>

template <class Item>
class Vector {
   private:
    Item *data_;
    std::size_t capacity_;
    std::size_t size_;

    void erase_from(std::size_t index) {
        if (index < size_) {
            for (std::size_t i = index; i < size_; i++) (data_ + i)->~Item();
            size_ = index;
        }
    }

   public:
    Vector() noexcept {
        data_ = nullptr;
        capacity_ = 0;
        size_ = 0;
    }

    Vector(std::size_t capacity) : Vector() { resize(capacity); }

    Vector(const Vector &other) : Vector(other.capacity_) {
        size_ = other.size_;
        std::copy(other.data_, other.data_ + size_, data_);
    }

    ~Vector() noexcept { delete[] data_; }

    Vector &operator=(const Vector &other) {
        if (this == &other) return *this;
        this->~Vector();
        Vector tmp(other);
        swap(tmp);
        return *this;
    }

    Item *data() const { return data_; }

    std::size_t capacity() const { return capacity_; }

    std::size_t size() const { return size_; }

    Item &operator[](std::size_t index) { return data_[index]; }

    void clear() { erase_from(0); }

    void reserve(std::size_t capacity) {
        if (capacity > capacity_) {
            Item *data = new Item[capacity];
            std::copy(data_, data_ + capacity_, data);
            delete[] data_;
            data_ = data;
            capacity_ = capacity;
        }
    }

    void resize(std::size_t capacity) {
        reserve(capacity);
        if (capacity < capacity_) erase_from(capacity);
        size_ = capacity;
    }

    void push_back(const Item &value) {
        if (capacity_ == size_) reserve(capacity_ * 2 + 1);
        data_[size_] = value;
        size_++;
    }

    void pop_back() { erase_from(size_ - 1); }

    void swap(Vector &b) {
        using std::swap;  // allow custom implementations
        swap(data_, b.data_);
        swap(capacity_, b.capacity_);
        swap(size_, b.size_);
    }
};

#endif  // VECTOR_H_
