#ifndef COLLECTIONS_HASH_TABLE_HPP
#define COLLECTIONS_HASH_TABLE_HPP

// ISO C Includes
#include <cstddef>

// ISO C++ Includes
#include <concepts>
#include <ranges>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace collections {
    template<
        typename T,
        typename Hash = std::hash<T>,
        typename Equal = std::equal_to<T>,
        typename Allocator = std::allocator<T>
    >
    class hash_table {
    public:
        // ── Forward Declarations ────────────────────────────────────────────
        class iterator;

        class const_iterator;

        class local_iterator;

        class const_local_iterator;

        // ── Aliases ─────────────────────────────────────────────────────────
        using value_type = T;

        using allocator_type = Allocator;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using ator_traits = std::allocator_traits<allocator_type>;

        using hasher = Hash;

        using equal = Equal;

        using pointer = ator_traits::pointer;

        using const_pointer = ator_traits::const_pointer;

        using reference = value_type&;

        using const_reference = const value_type&;

        using reverse_iterator = std::reverse_iterator<iterator>;

        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        // ── node ────────────────────────────────────────────────────────────
        struct node {
            // ── Fields ──────────────────────────────────────────────────────
            value_type value;

            struct node* parent;

            struct node* left;

            struct node* right;
        };

        // ── Aliases ─────────────────────────────────────────────────────────
        using node_allocator_type = ator_traits::template rebind_alloc<
            struct node
        >;

        using node_ator_traits = std::allocator_traits<node_allocator_type>;

        // ── Fields ──────────────────────────────────────────────────────────

        size_type sz_;

        [[no_unique_address]]
        allocator_type alloc_;

    public:
        // ── node_type ───────────────────────────────────────────────────────
        class node_type {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using value_type = hash_table::value_type;

            using allocator_type = hash_table::node_allocator_type;

            using container_node_type = struct hash_table::node;

            using ator_traits = std::allocator_traits<allocator_type>;

            using reference = value_type&;

            using pointer = typename ator_traits::template rebind_traits<
                container_node_type
            >::pointer;
        
        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class btree;

            // ── Fields ──────────────────────────────────────────────────────
            pointer ptr_;

            std::optional<allocator_type> alloc_;
        
        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to test
            constexpr node_type() noexcept : ptr_(nullptr),
                                             alloc_(std::nullopt) {}

            constexpr node_type(const node_type&) noexcept = delete;

            // TODO: Need to test
            constexpr node_type(
                node_type&& other
            ) noexcept : ptr_(std::move(other.ptr_)), 
                         alloc_(std::move(other.alloc_)) {
                other.ptr_ = nullptr;
                other.alloc_ = std::nullopt;
            }

            // ── Destructor ──────────────────────────────────────────────────
            // TODO: Need to test
            constexpr ~node_type() noexcept {
                if (this->ptr_ == nullptr) [[unlikely]] {
                    return;
                }

                ator_traits::destroy(this->alloc_.value(), this->ptr_);
                ator_traits::deallocate(this->alloc_.value(), this->ptr_);
                this->ptr_ = nullptr;
            }

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(
                const node_type&
            ) noexcept -> node_type& = delete;
            
            // TODO: Need to test
            constexpr auto operator=(
                node_type&& rhs
            ) noexcept -> node_type& {
                this->~node_type();

                this->ptr_ = std::move(rhs.ptr_);
                rhs.ptr_ = nullptr;

                this->alloc_ = std::move(rhs.alloc_);
                rhs.alloc_ = std::nullopt;

                return *this;
            }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator==(
                const node_type& rhs
            ) const noexcept -> bool { return this->ptr_ == rhs.ptr_; }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator!=(
                const node_type& rhs
            ) const noexcept -> bool { return this->ptr_ != rhs.ptr_; }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator*() const noexcept -> reference {
                return *this->ptr_;
            }

            // TODO: Need to test
            [[nodiscard]]
            explicit constexpr operator bool() const noexcept {
                return this->ptr_ != nullptr;
            }

            // ── Methods ─────────────────────────────────────────────────────
            // TODO: Need to test
            [[nodiscard]]
            constexpr auto empty() const noexcept -> bool {
                return this->ptr_ == nullptr;
            }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto value() const -> std::optional<reference> {
                return this->ptr_ != nullptr ? *this->ptr_ : std::nullopt;
            }

            // TODO: Need to test
            constexpr void swap(node_type&& other) noexcept(
                ator_traits::propagate_on_container_swap::value ||
                ator_traits::is_always_equal::value
            ) {
                pointer tmp = this->ptr_;
                this->ptr_ = other.ptr_;
                other.ptr_ = tmp;
            }
        };

        // ── insert_return_type ──────────────────────────────────────────────
        template<typename Iterator = iterator, typename NodeType = node_type>
        struct insert_return_type {
            // ── Fields ──────────────────────────────────────────────────────
            Iterator position;

            bool inserted;

            NodeType node;
        };

        // ── iterator ────────────────────────────────────────────────────────
        class iterator {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using iterator_category = std::forward_iterator_tag;

            using iterator_concept = std::forward_iterator_tag;

            using value_type = hash_table::value_type;

            using size_type = hash_table::size_type;

            using difference_type = hash_table::difference_type;
            
            using reference = value_type&;

            using const_reference = const value_type&;

            using pointer = std::allocator_traits<
                hash_table::allocator_type
            >::pointer;

            using const_pointer = std::allocator_traits<
                hash_table::allocator_type
            >::const_pointer;

        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class hash_table;

            // ── Fields ──────────────────────────────────────────────────────

        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to implement
            constexpr iterator() noexcept;

            // TODO: Need to implement
            explicit constexpr iterator(const hash_table& table);

            constexpr iterator(const iterator&) = default;

            constexpr iterator(iterator&& other) noexcept = default;

            // ── Destructor ──────────────────────────────────────────────────
            constexpr ~iterator() noexcept = default;

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(const iterator&) -> iterator& = default;

            constexpr auto operator=(
                iterator&&
            ) noexcept -> iterator& = default;

            [[nodiscard]]
            constexpr auto operator==(const iterator&) const -> bool = default;

            [[nodiscard]]
            constexpr auto operator!=(const iterator&) const -> bool = default;

            // TODO: Need to implement
            [[nodiscard]]
            constexpr auto operator*() const noexcept -> reference;

            // TODO: Need to implement
            [[nodiscard]]
            constexpr auto operator->() const noexcept -> pointer;

            // TODO: Need to implement
            constexpr auto operator++() noexcept -> iterator&;

            // TODO: Need to test
            constexpr auto operator++(int) noexcept -> iterator& {
                iterator tmp = *this;
                ++(*this);
                return *this;
            }
        };

        // ── const_iterator ──────────────────────────────────────────────────
        class const_iterator {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using iterator_category = std::forward_iterator_tag;

            using iterator_concept = std::forward_iterator_tag;

            using value_type = hash_table::value_type;

            using size_type = hash_table::size_type;

            using difference_type = hash_table::difference_type;
            
            using reference = value_type&;

            using const_reference = const value_type&;

            using pointer = std::allocator_traits<
                hash_table::allocator_type
            >::pointer;

            using const_pointer = std::allocator_traits<
                hash_table::allocator_type
            >::const_pointer;

        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class hash_table;

            // ── Fields ──────────────────────────────────────────────────────

        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to implement
            constexpr const_iterator() noexcept;

            // TODO: Need to implement
            explicit constexpr const_iterator(
                const hash_table& table
            );

            constexpr const_iterator(const const_iterator&) = default;

            constexpr const_iterator(const_iterator&&) noexcept = default;

            // ── Destructor ──────────────────────────────────────────────────
            constexpr ~const_iterator() noexcept = default;

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(
                const const_iterator&
            ) -> const_iterator& = default;

            constexpr auto operator=(
                const_iterator&&
            ) noexcept -> const_iterator& = default;

            constexpr auto operator==(
                const const_iterator&
            ) const noexcept -> bool = default;

            constexpr auto operator!=(
                const const_iterator&
            ) const noexcept -> bool = default;

            // TODO: Need to implement
            constexpr auto operator*() const noexcept -> reference;

            // TODO: Need to implement
            constexpr auto operator->() const noexcept -> pointer;

            // TODO: Need to implement
            constexpr auto operator++() noexcept -> iterator&;

            // TODO: Need to test
            constexpr auto operator++(int) noexcept -> const_iterator& {
                const_iterator tmp = this;
                ++(*this);
                return tmp;
            }
        };

        // ── local_iterator ──────────────────────────────────────────────────
        class local_iterator {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using iterator_category = std::forward_iterator_tag;

            using iterator_concept = std::forward_iterator_tag;

            using value_type = hash_table::value_type;

            using size_type = hash_table::size_type;

            using difference_type = hash_table::difference_type;
            
            using reference = value_type&;

            using const_reference = const value_type&;

            using pointer = std::allocator_traits<
                hash_table::allocator_type
            >::pointer;

            using const_pointer = std::allocator_traits<
                hash_table::allocator_type
            >::const_pointer;

        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class hash_table;

            // ── Fields ──────────────────────────────────────────────────────

        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to implement
            constexpr local_iterator() noexcept;

            // TODO: Need to implement
            explicit constexpr local_iterator(const hash_table& table);

            constexpr local_iterator(const local_iterator&) = default;

            constexpr local_iterator(
                local_iterator&& other
            ) noexcept = default;

            // ── Destructor ──────────────────────────────────────────────────
            constexpr ~local_iterator() noexcept = default;

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(
                const local_iterator&
            ) -> local_iterator& = default;

            constexpr auto operator=(
                local_iterator&&
            ) noexcept -> local_iterator& = default;

            [[nodiscard]]
            constexpr auto operator==(
                const local_iterator&)
            const -> bool = default;

            [[nodiscard]]
            constexpr auto operator!=(
                const local_iterator&
            ) const -> bool = default;

            // TODO: Need to implement
            [[nodiscard]]
            constexpr auto operator*() const noexcept -> reference;

            // TODO: Need to implement
            [[nodiscard]]
            constexpr auto operator->() const noexcept -> pointer;

            // TODO: Need to implement
            constexpr auto operator++() noexcept -> local_iterator&;

            // TODO: Need to test
            constexpr auto operator++(int) noexcept -> local_iterator& {
                iterator tmp = *this;
                ++(*this);
                return *this;
            }
        };

        // ── const_local_iterator ─────────────────────────────────────────────
        class const_local_iterator {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using iterator_category = std::forward_iterator_tag;

            using iterator_concept = std::forward_iterator_tag;

            using value_type = hash_table::value_type;

            using size_type = hash_table::size_type;

            using difference_type = hash_table::difference_type;
            
            using reference = value_type&;

            using const_reference = const value_type&;

            using pointer = std::allocator_traits<
                hash_table::allocator_type
            >::pointer;

            using const_pointer = std::allocator_traits<
                hash_table::allocator_type
            >::const_pointer;

        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class hash_table;

            // ── Fields ──────────────────────────────────────────────────────

        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to implement
            constexpr const_local_iterator() noexcept;

            // TODO: Need to implement
            explicit constexpr const_local_iterator(
                const hash_table& table
            );

            constexpr const_local_iterator(
                const const_local_iterator&
            ) = default;

            constexpr const_local_iterator(
                const_local_iterator&&
            ) noexcept = default;

            // ── Destructor ──────────────────────────────────────────────────
            constexpr ~const_local_iterator() noexcept = default;

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(
                const const_local_iterator&
            ) -> const_local_iterator& = default;

            constexpr auto operator=(
                const_local_iterator&&
            ) noexcept -> const_local_iterator& = default;

            constexpr auto operator==(
                const const_local_iterator&
            ) const noexcept -> bool = default;

            constexpr auto operator!=(
                const const_local_iterator&
            ) const noexcept -> bool = default;

            // TODO: Need to implement
            constexpr auto operator*() const noexcept -> reference;

            // TODO: Need to implement
            constexpr auto operator->() const noexcept -> pointer;

            // TODO: Need to implement
            constexpr auto operator++() noexcept -> const_local_iterator&;

            // TODO: Need to test
            constexpr auto operator++(int) noexcept -> const_local_iterator& {
                const_local_iterator tmp = this;
                ++(*this);
                return tmp;
            }
        };


        // ── Constructors ────────────────────────────────────────────────────
        // TODO: Need to implement
        constexpr hash_table();

        // TODO: Need to implement
        explicit constexpr hash_table(
            const size_type bucket_count,
            const hasher& hash = Hash(),
            const equal& eq = equal(),
            const allocator_type& alloc = allocator_type()
        );

        // TODO: Need to implement
        constexpr hash_table(
            const size_type bucket_count,
            const allocator_type& alloc
        ) : hash_table(bucket_count, hasher(), equal(), alloc) {}

        // TODO: Need to implement
        constexpr hash_table(
            const size_type bucket_count,
            const hasher& hash,
            const allocator_type& alloc
        ) : hash_table(bucket_count, hash, equal(), alloc) {}

        // TODO: Need to implement
        explicit constexpr hash_table(
            const allocator_type& alloc
        );

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr hash_table(
            InputIt first,
            InputIt last,
            const size_type bucket_count = 0, // temporary
            const hasher& hash = Hash(),
            const equal& eq = equal(),
            const allocator_type& alloc = allocator_type()
        );

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr hash_table(
            InputIt first,
            InputIt last,
            const size_type bucket_count,
            const allocator_type& alloc
        ) : hash_table(
            first,
            last,
            bucket_count,
            hasher(),
            equal(),
            alloc
        ) {}

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr hash_table(
            InputIt first,
            InputIt last,
            const size_type bucket_count,
            const hasher& hash,
            const allocator_type& alloc
        ) : hash_table(
            first,
            last,
            bucket_count,
            hash,
            equal(),
            alloc
        ) {}

        // TODO: Need to implement
        constexpr hash_table(const hash_table& other);

        // TODO: Need to implement
        constexpr hash_table(
            const hash_table& other,
            const allocator_type& alloc
        );

        // TODO: Need to implement
        constexpr hash_table(hash_table&& other);

        // TODO: Need to implement
        constexpr hash_table(
            hash_table&& other,
            const allocator_type& alloc
        );

        // TODO: Need to implement
        constexpr hash_table(
            std::initializer_list<value_type> values,
            const size_type bucket_count = 0, // temporary
            const Hash& hash = Hash(),
            const equal& eq = equal(),
            const allocator_type& alloc = allocator_type()
        );

        // TODO: Need to implement
        constexpr hash_table(
            std::initializer_list<value_type> values,
            const size_type bucket_count,
            const allocator_type& alloc
        ) : hash_table(
            values,
            bucket_count,
            hasher(),
            equal(),
            alloc
        ) {}

        // TODO: Need to implement
        constexpr hash_table(
            std::initializer_list<value_type> values,
            const size_type bucket_count,
            const hasher& hash,
            const allocator_type& alloc
        ) : hash_table(
            values,
            bucket_count,
            hash,
            equal(),
            alloc
        ) {}

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr hash_table(
            std::from_range_t,
            R&& rg,
            const size_type bucket_count = 0, // temporary
            const hasher& hash = hasher(),
            const equal& eq = equal(),
            const allocator_type& alloc = allocator_type()
        );

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr hash_table(
            std::from_range_t,
            R&& rg,
            const size_type bucket_count,
            const Allocator& alloc
        ) : hash_table(
            std::from_range,
            std::forward<R>(rg),
            bucket_count,
            hasher(),
            equal(),
            alloc
        ) {}

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr hash_table(
            std::from_range_t,
            R&& rg,
            const size_type bucket_count,
            const hasher& hash,
            const allocator_type& alloc
        ) : hash_table(
            std::from_range,
            std::forward<R>(rg),
            bucket_count,
            hash,
            equal(),
            alloc
        ) {}
        
        // ── Destructor ──────────────────────────────────────────────────────
        // TODO: Need to implement
        constexpr ~hash_table() noexcept;
        
        // ── Overloaded Operators ────────────────────────────────────────────
        // TODO: Need to implement
        constexpr auto operator=(const hash_table& rhs) -> hash_table&;

        // TODO: Need to implement
        constexpr auto operator=(hash_table&& rhs) noexcept(
            ator_traits::is_always_equal::value &&
            std::is_nothrow_move_assignable<hasher>::value &&
            std::is_nothrow_move_assignable<equal>::value
        ) -> hash_table&;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator==(const hash_table& rhs) const -> bool;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator<=>(const hash_table& rhs) const;

        // ── Methods ───────────────────────────────────────────────────────── 
        // TODO: Need to implement
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto get_allocator() const noexcept -> allocator_type;
 
        // TODO: Need to implement
        constexpr auto begin() noexcept -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto begin() const noexcept -> const_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto cbegin() const noexcept -> const_iterator;

        // TODO: Need to implement
        constexpr auto rbegin() noexcept -> reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto rbegin() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto crbegin() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        constexpr auto end() noexcept -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto end() const noexcept -> const_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto cend() const noexcept -> const_iterator;

        // TODO: Need to implement
        constexpr auto rend() noexcept -> reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto rend() const -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto crend() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto empty() const noexcept -> bool;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto size() const noexcept -> size_type;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto max_size() const noexcept -> size_type;

        // TODO: Need to implement
        constexpr void clear() noexcept;

        // TODO: Need to implement
        constexpr auto insert(
            const_reference value
        ) -> std::pair<iterator, bool>;

        // TODO: Need to implement
        constexpr auto insert(value_type&& value) -> std::pair<iterator, bool>;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            const_reference value
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            value_type&& value
        ) -> iterator;

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr void insert(InputIt first, InputIt last);

        // TODO: Need to implement
        constexpr void insert(std::initializer_list<value_type> values);

        // TODO: Need to implement
        constexpr auto insert(
            node_type&& node_handle
        ) -> struct insert_return_type<iterator, node_type>;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            node_type&& node_handle
        ) -> iterator;

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void insert_range(R&& rg);

        template<typename... Args>
        constexpr auto emplace(Args&&... args) -> std::pair<iterator, bool>;
       
        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace(
            const_iterator pos,
            Args&&... args
        ) -> iterator;

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace_hint(
            const_iterator hint,
            Args&&... args
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto erase(iterator pos) -> iterator requires(
            !std::same_as<iterator, const_iterator>
        );

        // TODO: Need to implement
        constexpr auto erase(const_iterator pos) -> iterator;

        // TODO: Need to implement
        constexpr auto erase(
            const_iterator first,
            const_iterator last
        ) -> iterator;
        
        // TODO: Need to implement
        constexpr void swap(hash_table& other) noexcept(
            ator_traits::is_always_equal::value
        );

        // TODO: Need to implement
        constexpr auto extract(const_iterator position) -> node_type;

        // TODO: Need to implement
        constexpr void merge(hash_table& other);

        // TODO: Need to implement
        constexpr void merge(hash_table&& other);

        // TODO: Need to implement
        template<typename Comp> 
        constexpr void merge(hash_table& other, const Comp comp);

        // TODO: Need to implement
        template<typename Comp> 
        constexpr void merge(hash_table&& other, const Comp comp);

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto contains(const_reference value) const -> bool;

        // TODO: Need to implement
        constexpr auto lower_bound(const_reference value) -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto lower_bound(
            const_reference value
        ) const -> const_iterator;

        // TODO: Need to implement
        constexpr auto upper_bound(const_reference value) -> iterator;
        
        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto upper_bound(
            const_reference value
        ) const -> const_iterator;
        
        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto bucket_count() const -> size_type;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto max_bucket_count() const -> size_type;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto bucket_size(const size_type n) const -> size_type;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto bucket(const_reference value) const -> size_type;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto load_factor() const -> float;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto max_load_factor() const -> float;

        // TODO: Need to implement
        void max_load_factor(const float new_factor);

        // TODO: Need to implement
        void rehash(const size_type count);

        // TODO: Need to implement
        void reserve(const size_type count);

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto hash_function() const -> hasher;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto eq() const -> equal;
    };

    // ── Deduction Guides ────────────────────────────────────────────────────
    // TODO: Need to test
    template<
        std::input_iterator InputIt,
        typename Hash = std::hash<
            typename std::iterator_traits<InputIt>::value_type
        >,
        typename Pred = std::equal_to<
            typename std::iterator_traits<InputIt>::value_type
        >,
        typename Alloc = std::allocator<
            typename std::iterator_traits<InputIt>::value_type
        >
    >
    hash_table(
        InputIt,
        InputIt,
        typename std::allocator_traits<Alloc>::size_type = 0,
        Hash = Hash(),
        Pred = Pred(),
        Alloc = Alloc()
    ) -> hash_table<
        typename std::iterator_traits<InputIt>::value_type,
        Hash,
        Pred,
        Alloc
    >;

    // TODO: Need to test
    template<
        typename T,
        typename Hash = std::hash<T>,
        typename Pred = std::equal_to<T>,
        typename Alloc = std::allocator<T>
    >
    hash_table(
        std::initializer_list<T>,
        typename std::allocator_traits<Alloc>::size_type = 0,
        Hash = Hash(),
        Pred = Pred(),
        Alloc = Alloc()
    ) -> hash_table<T, Hash, Pred, Alloc>;

    // TODO: Need to test
    template<
        std::input_iterator InputIt,
        typename Alloc
    >
    hash_table(
        InputIt,
        InputIt,
        typename std::allocator_traits<Alloc>::size_type,
        Alloc
    ) -> hash_table<
        typename std::iterator_traits<InputIt>::value_type,
        std::hash<typename std::iterator_traits<InputIt>::value_type>,
        std::equal_to<typename std::iterator_traits<InputIt>::value_type>,
        Alloc
    >;

    // TODO: Need to implement
    template<
        std::input_iterator InputIt,
        typename Hash,
        typename Alloc
    >
    hash_table(
        InputIt,
        InputIt,
        typename std::allocator_traits<Alloc>::size_type,
        Hash,
        Alloc
    ) -> hash_table<
        typename std::iterator_traits<InputIt>::value_type, Hash,
        std::equal_to<typename std::iterator_traits<InputIt>::value_type>,
        Alloc
    >;

    // TODO: Need to test
    template<typename T, typename Alloc>
    hash_table(
        std::initializer_list<T>,
        typename std::allocator_traits<Alloc>::size_type,
        Alloc
    ) -> hash_table<T, std::hash<T>, std::equal_to<T>, Alloc>;

    // TODO: Need to test
    template<typename T, typename Hash, typename Alloc>
    hash_table(
        std::initializer_list<T>,
        typename std::allocator_traits<Alloc>::size_type,
        Hash,
        Alloc
    ) -> hash_table<T, Hash, std::equal_to<T>, Alloc>;

    // TODO: Need to test
    template<
        std::ranges::input_range R,
        typename Hash = std::hash<std::ranges::range_value_t<R>>,
        typename Pred = std::equal_to<std::ranges::range_value_t<R>>,
        typename Alloc = std::allocator<std::ranges::range_value_t<R>>
    >
    hash_table(
        std::from_range_t, R&&,
        typename std::allocator_traits<Alloc>::size_type = 0,
        Hash = Hash(),
        Pred = Pred(),
        Alloc = Alloc()
    ) -> hash_table<std::ranges::range_value_t<R>, Hash, Pred, Alloc>;

    // TODO: Need to test
    template<std::ranges::input_range R, typename Alloc>
    hash_table(
        std::from_range_t,
        R&&,
        typename std::allocator_traits<Alloc>::size_type,
        Alloc
    ) -> hash_table<
        std::ranges::range_value_t<R>,
        std::hash<std::ranges::range_value_t<R>>,
        std::equal_to<std::ranges::range_value_t<R>>,
        Alloc
    >;
    
    // TODO: Need to test
    template<std::ranges::input_range R, typename Alloc>
    hash_table(
        std::from_range_t,
        R&&, Alloc
    ) -> hash_table<
        std::ranges::range_value_t<R>,
        std::hash<std::ranges::range_value_t<R>>,
        std::equal_to<std::ranges::range_value_t<R>>,
        Alloc
    >;

    // TODO: Need to test
    template<std::ranges::input_range R, typename Hash, typename Alloc>
    hash_table(
        std::from_range_t, R&&,
        typename std::allocator_traits<Alloc>::size_type,
        Hash,
        Alloc
    ) -> hash_table<
        std::ranges::range_value_t<R>,
        Hash,
        std::equal_to<std::ranges::range_value_t<R>>,
        Alloc
    >;

    // ── Functions ───────────────────────────────────────────────────────────
    // TODO: Need to implement
    template<typename T, typename Hash, typename Equal, typename Alloc>
    constexpr void swap(
        hash_table<T, Hash, Equal, Alloc>& lhs,
        hash_table<T, Hash, Equal, Alloc>& rhs
    ) noexcept(noexcept(lhs.swap(rhs)));

    // TODO: Need to implement
    template<
        typename T,
        typename Hash,
        typename Equal,
        typename Alloc,
        typename Pred
    >
    hash_table<T, Hash, Equal, Alloc>::size_type erase_if(
        hash_table<T, Hash, Equal, Alloc>& table,
        Pred pred
    );
   
} // namespace collections

#endif // #ifndef COLLECTIONS_HASH_TABLE_HPP
