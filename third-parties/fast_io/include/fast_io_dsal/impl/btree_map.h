#pragma once

namespace fast_io::containers
{
    template <typename key_type, typename value_type>
    struct btree_pair
    {
        key_type key;
        value_type value;
    };

    template <typename key_type, typename value_type>
    struct btree_node
    {
        using size_type = ::std::size_t;
        static constexpr size_type max_size{::std::max(sizeof(key_type), sizeof(value_type))};

        static constexpr size_type pairs_size{::std::max(static_cast<size_type>(16), static_cast<size_type>(4096) / max_size)};
        static constexpr size_type pairs_size_real = pairs_size - 1;

        btree_node* parent_node_ptr{};
        btree_node* pre_child_node_ptr[pairs_size + 1]{};

        size_type used{};
        key_type keys[pairs_size]{};
        value_type values[pairs_size]{};
    };

    template <::std::three_way_comparable key_type, typename value_type>
    struct btree_tuple
    {
        using pair_type = btree_pair<key_type, value_type>;
        using node_type = btree_node<key_type, value_type>;

        node_type* nt{};
        key_type kt{};
        value_type vt{};
    };

    template <::std::three_way_comparable key_type, typename value_type, typename allocator>
    class btree_map
    {
    public:
        using allocator_type = allocator;

    public:
        using pair_type = btree_pair<key_type, value_type>;
        using node_type = btree_node<key_type, value_type>;

        using pair_type_allocator_type = typed_generic_allocator_adapter<allocator_type, pair_type>;
        using node_type_allocator_type = typed_generic_allocator_adapter<allocator_type, node_type>;

        constexpr btree_map() noexcept = default;
        constexpr ~btree_map() noexcept = default;

        explicit constexpr btree_map(::std::initializer_list<pair_type> ilist) noexcept(::std::is_nothrow_move_constructible_v<pair_type>)
        {
            auto const end{ilist.end()};
            for(auto i{ilist.begin()}; i != end; ++i) { insert(::std::move(*i)); }
        }

        pair_type* insert(pair_type node) { return insert_impl(root, ::std::move(node)); }

        void check_and_alloc(node_type*& ptr)
        {
            if(ptr == nullptr) [[unlikely]]
            {
                ptr = node_type_allocator_type::allocate(1);
                ::std::construct_at(ptr);
            }
        }

        void insert_to_node(node_type* node_ptr, ::std::size_t index, pair_type&& pair)
        {
            ::std::move_backward(node_ptr->keys + index, node_ptr->keys + node_ptr->used, node_ptr->keys + node_ptr->used + 1u);
            ::std::move_backward(node_ptr->values + index, node_ptr->values + node_ptr->used, node_ptr->values + node_ptr->used + 1u);
            ::std::move_backward(node_ptr->pre_child_node_ptr + index,
                                 node_ptr->pre_child_node_ptr + node_ptr->used,
                                 node_ptr->pre_child_node_ptr + node_ptr->used + 1u);

            node_ptr->keys[index] = ::std::move(pair.key);
            node_ptr->values[index] = ::std::move(pair.value);
            ++node_ptr->used;
        }

        [[nodiscard]] auto node_split(node_type* node_ptr)
        {
            constexpr ::std::size_t mid{node_type::pairs_size / 2u};
            node_type* new_node_ptr = static_cast<node_type*>(calloc(1, sizeof(node_type)));
            ::std::move(node_ptr->keys + mid, node_ptr->keys + node_type::pairs_size, new_node_ptr->keys);
            ::std::move(node_ptr->values + mid, node_ptr->values + node_type::pairs_size, new_node_ptr->values);
            ::std::move(node_ptr->pre_child_node_ptr + mid, node_ptr->pre_child_node_ptr + node_type::pairs_size + 1u, new_node_ptr->pre_child_node_ptr);
            node_ptr->used = mid - 1;
            new_node_ptr->used = mid;

            for(::std::size_t i{}; i < mid + 1u; ++i)
            {
                if(new_node_ptr->pre_child_node_ptr[i]) { new_node_ptr->pre_child_node_ptr[i]->parent_node_ptr = new_node_ptr; }
            }

            return btree_tuple{new_node_ptr, ::std::move(node_ptr->keys[mid - 1]), ::std::move(node_ptr->values[mid - 1])};
        }

        auto find_insert_locale(node_type* node_ptr, pair_type const& pair)
        {
            ::std::size_t index{};
            while(index < node_ptr->used)
            {
                if(node_ptr->keys[index] < pair.key)
                {
                    ++index;
                    if(index == node_ptr->used && node_ptr->pre_child_node_ptr[index])
                    {
                        node_ptr = node_ptr->pre_child_node_ptr[index];
                        index = 0;
                    }

                    continue;
                }

                if(node_ptr->keys[index] == pair.key) [[unlikely]] { /*throw;*/ }

                if(node_ptr->keys[index] > pair.key)
                {
                    if(node_ptr->pre_child_node_ptr[index])
                    {
                        check_and_alloc(node_ptr->pre_child_node_ptr[index]);

                        node_ptr = node_ptr->pre_child_node_ptr[index];
                        index = 0;
                        break;
                    }
                    break;  // if (node_ptr->used < node_size)
                }
            }
            return ::std::pair{node_ptr, index};
        }

        pair_type* insert_impl(node_type*& root_node_ptr, pair_type&& pair)
        {
            check_and_alloc(root_node_ptr);

            auto [locale, index] = find_insert_locale(root_node_ptr, pair);

            insert_to_node(locale, index, ::std::move(pair));

            while(locale && locale->used == node_type::pairs_size)
            {
                if(locale->parent_node_ptr == nullptr)
                {
                    check_and_alloc(locale->parent_node_ptr);
                    root_node_ptr = locale->parent_node_ptr;
                }

                node_type* parent_node_ptr = locale->parent_node_ptr;

                ::std::size_t i{};
                auto [right_node_ptr, parent_key, parent_value] = node_split(locale);

                while(i < parent_node_ptr->used && parent_key > parent_node_ptr->keys[i]) { ++i; }

                insert_to_node(parent_node_ptr, i, {::std::move(parent_key), ::std::move(parent_value)});
                parent_node_ptr->pre_child_node_ptr[i] = locale;
                parent_node_ptr->pre_child_node_ptr[i + 1] = right_node_ptr;
                locale->parent_node_ptr = parent_node_ptr;
                right_node_ptr->parent_node_ptr = parent_node_ptr;
                locale = parent_node_ptr;
            }

            return {};
        }

        value_type* find(key_type const& key) { return find_impl(root, key); }

        value_type* find_impl(node_type* node, key_type const& key)
        {
            if(node == nullptr) { return nullptr; }

            ::std::size_t index{};

            while(index < node->used + 1)
            {
                if(index != node->used)
                {

                    if(node->keys[index] < key)
                    {
                        ++index;
                        continue;
                    }

                    if(node->keys[index] == key) { return &node->values[index]; }
                }
                auto const new_node{node->pre_child_node_ptr[index]};
                if(new_node == nullptr) [[unlikely]] { return {}; }

                node = new_node;
                index = 0;
            }

            return {};
        }

        void dfs(node_type* node_ptr, ::std::size_t layer = 0)
        {
            if(node_ptr == nullptr) { return; }

            for(::std::size_t i{}; i < node_ptr->used; ++i)
            {
                dfs(node_ptr->pre_child_node_ptr[i], layer + 1);
                ::fast_io::perrln(fast_io::pointervw(&node_ptr), " ", layer, " ", node_ptr->keys[i]);
            }

            dfs(node_ptr->pre_child_node_ptr[node_ptr->used], ++layer);
        }

        void print()
        {
            dfs(root);
            ::fast_io::perrln(" ***********************************************");
        }

        // auto& operator[](key_type const& key)
        //{
        //	return {};
        // }

        node_type* root{};
    };
}  // namespace fast_io::containers
