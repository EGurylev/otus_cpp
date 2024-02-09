#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class simple_forward_list {
    struct Node {
        Node(const T& value) : value{value} {}
        T value{};
        Node* next{};
    };

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        iterator(Node* ptr) : ptr{ptr} {}

        const reference operator*() const noexcept { return ptr->value; }
        reference operator*() noexcept { return ptr->value; }
        const iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        friend bool operator==(iterator a, iterator b) {
            return a.ptr == b.ptr;
        }
        friend bool operator!=(iterator a, iterator b) { return !(a == b); }

       private:
        Node* ptr;
    };

    using NodeAllocator = typename Allocator::rebind<Node>::other;

   public:
    simple_forward_list() {
        tail = std::allocator_traits<NodeAllocator>::allocate(alloc, 1);
        std::allocator_traits<NodeAllocator>::construct(alloc, tail, T{});
    }

    simple_forward_list(const simple_forward_list& other) = delete;

    simple_forward_list(simple_forward_list&& other) = delete;

    void push_front(const T& value) {
        Node* new_head{
            std::allocator_traits<NodeAllocator>::allocate(alloc, 1)};
        std::allocator_traits<NodeAllocator>::construct(alloc, new_head, value);
        if (!head) {
            new_head->next = tail;
        } else {
            new_head->next = head;
        }
        head = new_head;
        ++sz;
    }

    size_t size() const noexcept { return sz; }
    bool empty() const noexcept { return sz == 0; }
    iterator begin() noexcept { return iterator(head); }
    iterator end() noexcept { return iterator(tail); }

   private:
    NodeAllocator alloc;
    Node* head{};
    Node* tail{};
    size_t sz{};
};