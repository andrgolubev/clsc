#ifndef _RING_ARRAY_HPP_
#define _RING_ARRAY_HPP_

#include <comparable.hpp>

#include <array>
#include <stdexcept>
#include <algorithm>

namespace clsc
{
    //TODO: m_head - m_tail > 0 - always, except init
    template<typename T, std::size_t N>
    class ring_array : public adjustable_comparable<ring_array<T, N>>
    {
    public:
        ring_array() noexcept(true) : m_head(0), m_tail(0), m_capacity(N), m_array() {}

        ring_array(const std::initializer_list<T>& list) : m_head(0), m_tail(0), m_capacity(N), m_array(list) {}

        size_t size() const
        {
            return m_array.size();
        }

        size_t capacity() const
        {
            return m_capacity;
        }

        bool empty() const
        {
            return m_array.empty();
        }

        const T* data() const
        {
            return m_array.data();
        }

        void push_front(const T& val)
        {
            m_array[m_head] = val;
            m_head = (m_head - 1) >= 0 ? (m_head - 1) % m_capacity : (m_capacity - 1);
        }

        void push_back(const T& val)
        {
            if (m_tail == m_head && size() != 0)
            {
                m_head = (m_head + 1) % m_capacity;
            }
            m_array[m_tail] = val;
            m_tail = (m_tail + 1) % m_capacity;
        }

        const T& front() const
        {
            validate_emptiness();
            return m_array[m_head];
        }

        const T& rotate_front()
        {
            validate_emptiness();
            const auto& val = m_array[m_head];
            m_head = (m_head + 1) % m_capacity;
            m_tail = (m_tail + 1) % m_capacity;
            return val;
        }

        const T& back() const
        {
            validate_emptiness();
            const auto pos = (m_tail + m_capacity - 1) % m_capacity; // TODO: check pos calculation
            return m_array[pos];
        }

        const T& rotate_back()
        {
            validate_emptiness();
            const auto pos = (m_tail + m_capacity - 1) % m_capacity; // TODO: check pos calculation
            m_tail > 0 ? m_tail-- : m_tail = m_capacity - ((m_tail + 1) % m_capacity);
            m_head > 0 ? m_head-- : m_capacity - ((m_head + 1) % m_capacity);
            return m_array[pos];
        }

        T& operator[](int index)
        {
            validate_emptiness();
            if ((index >= size() && index >= m_array.size()) || index < 0)
                throw std::out_of_range("invalid index");
            return m_array[(m_head + index) % m_capacity];
        }

        T at(int index) const
        {
            validate_emptiness();
            if ((index >= size() && index >= m_array.size()) || index < 0)
                throw std::out_of_range("invalid index");
            return m_array[(m_head + index) % m_capacity];
        }

        //TODO: documentation
        std::enable_if_t<std::is_arithmetic<T>::value, int>
        operator()(const ring_array<T, N>& rhs)
        {
            //TODO: investigate - let the user define an implementation (e.g. via a callable)
            if (m_array.size() != rhs.m_array.size()) // check on size first
                return static_cast<int>(m_array.size() - rhs.m_array.size());

            // if arrays have the same size
            const auto mismatched_pair = std::mismatch(m_array.cbegin(), m_array.cend(), rhs.m_array.cbegin(), rhs.m_array.cend());
            if (mismatched_pair.first == m_array.cend() && mismatched_pair.second == rhs.m_array.cend())
                return 0; // arrays are considered equal

            return mismatched_pair.first - mismatched_pair.second;
        }

    private:
        void validate_emptiness() const noexcept(false) // TODO: cannot out_of_range because of std::array?
        {
            if (size() == 0 && m_array.size() == 0)
                throw std::out_of_range("array is empty");
        }

        int32_t m_head, m_tail;
        const size_t m_capacity;
        std::array<T, N> m_array;
    };
}

/** Experimental
template<typename Container>
using iterator_categoty_of_type = typename std::iterator_traits<typename Container::iterator>::iterator_category;
template<typename Container>
using has_input_iterator = std::is_base_of<std::input_iterator_tag, iterator_categoty_of_type<Container>>;
*/

#endif /* _RING_ARRAY_HPP_ */