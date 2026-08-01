#ifndef COLLECTIONS_HASH_TABLE_HPP
#define COLLECTIONS_HASH_TABLE_HPP

// ISO C Includes
#include <cstddef>

// ISO C++ Includes
#include <functional>
#include <memory>

namespace collections {
    template<typename T, typename Hash = std::hash<T>, typename Equal = std::equal_to<T>, typename Allocator = std::allocator<T>>
    class hash_table {
    };
} // namespace collections

#endif // #ifndef COLLECTIONS_HASH_TABLE_HPP
