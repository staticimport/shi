#pragma once

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
    void push_back(T const &);

    // iterate

private:
    struct segment
    {
        uint32_t m_head;
        uint32_t m_tail;
        T m_data[];
    };

    uint32_t m_size;
    uint32_t m_read_mask;
    uint32_t m_write_mask;
    segment* m_read_segment;
    segment* m_write_segment;
};


//
// IMPLEMENTATION
//
template <typename T>
circular_queue<T>::circular_queue()
    : m_size{}
    , m_read_mask{}
    , m_write_mask{}
    , m_read_segment{}
    , m_write_segment{}
{
}

template <typename T>
circular_queue<T>::~circular_queue()
{
    while (!empty()) { pop_front(); }
}

template <typename T>
T const & circular_queue<T>::back() const
{
    return m_write_segment->m_data[(m_write_segment->m_tail - 1) & m_write_mask];
}

template <typename T>
bool circular_queue<T>::empty() const
{
    return m_size == 0;
}

template <typename T>
T const & circular_queue<T>::front() const
{
    return m_read_segment->m_data[m_read_segment->m_head & m_read_mask];
}

template <typename T>
uint32_t circular_queue<T>::size() const
{
    return m_size;
}
}
