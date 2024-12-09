#pragma once

#include <shi/common.hpp>

namespace shi
{
template <typename T>
class circular_queue
{
public:
    circular_queue();
    ~circular_queue();

    // access
    T const& back()  const;
    bool     empty() const;
    T const& front() const;
    uint32_t size()  const;

    // mutate
    T&   back();
    T&   front();
    void pop_front();
    void push_back(T const &);

    // iterate

private:
    struct segment
    {
        uint32_t front_idx;
        uint32_t back_idx;
        segment* next;
        T data[];
    };

    static bool _is_segment_empty(segment const&);
    void _pop_front_segment();  // TODO mark noinline
    segment& _push_back_new_segment();  // TODO mark noinline

private:
    uint32_t num_elements;
    uint32_t front_mask;
    uint32_t back_mask;
    segment* front_segment;
    segment* back_segment;
};


//
// IMPLEMENTATION
//
template <typename T>
circular_queue<T>::circular_queue()
    : num_elements{}
    , front_mask{}
    , back_mask{}
    , front_segment{}
    , back_segment{}
{
}

template <typename T>
circular_queue<T>::~circular_queue()
{
    while (!empty()) { pop_front(); }
    delete this->back_segment;
}

template <typename T>
T const & circular_queue<T>::back() const
{
    segment& b = *this->back_segment;
    return b.data[(b.back_idx - 1) & this->back_mask];
}

template <typename T>
bool circular_queue<T>::empty() const
{
    return this->num_elements == 0;
}

template <typename T>
T const & circular_queue<T>::front() const
{
    segment& f = *this->front_segment;
    return f.data[f.front_idx & this->front_mask];
}

template <typename T>
void circular_queue<T>::pop_front()
{
    segment& f = *this->front_segment;
    f.data[f.front_idx & this->front_mask].~T();
    ++f.front_idx;
    --this->num_elements;
    if (shi_unlikely((&f != this->back_segment) & _is_segment_empty(f)))
    {
        _pop_front_segment();
    }
}

template <typename T>
void circular_queue<T>::push_back(T const & t)
{
    segment& b = shi_unlikely(_is_segment_full(*this->back_segment)) ? _push_back_new_segment() : *this->back_segment;
    new(&b.data[b.back_idx & this->back_mask]) T(t);
    ++b.back_idx;
    ++this->num_elements;
}

template <typename T>
uint32_t circular_queue<T>::size() const
{
    return this->num_elements;
}

template <typename T>
bool circular_queue<T>::_is_segment_empty(circular_queue<T>::segment const & s)
{
    return s.front_idx == s.back_idx;
}

template <typename T>
void circular_queue<T>::_pop_front_segment()
{
    segment* const tmp  = this->front_segment;
    this->front_segment = this->front_segment->next;
    this->front_mask    = 2 * (this->front_mask) - 1;
    delete tmp;
}

template <typename T>
typename circular_queue<T>::segment& circular_queue<T>::_push_back_new_segment()
{
    uint32_t const new_capacity = shi_likely(this->back_segment) ? 2 * (this->back_mask + 1) : 16U;
    auto space         = new char[sizeof(segment) + new_capacity * sizeof(T)];  // TODO need to align
    auto new_segment   = reinterpret_cast<segment*>(space);
    new_segment->front_idx = 0;
    new_segment->back_idx  = 0;
    new_segment->next      = nullptr;
    if (this->back_segment)
    {
        this->back_segment->next = new_segment;
        this->back_segment       = new_segment;
    }
    else
    {
        this->front_segment = this->back_segment = new_segment;
    }
}
}
