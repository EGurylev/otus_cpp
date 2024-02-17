#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include <queue>
#include <unordered_map>
#include <variant>

namespace view {

class Primitive {
   public:
    Primitive(size_t id) : id{id} {}
    size_t id{};
    virtual ~Primitive() = default;
};

struct Position {
    int x{};
    int y{};
};

class Circle : public Primitive {
   public:
    Circle(size_t id, size_t radius, Position&& position)
        : Primitive{id}, radius{radius}, position{position} {}
    // Implement circle here
   private:
    size_t radius{};
    Position position;
};

class Square : public Primitive {
    // Implement square here
};

enum class ActionName {
    NewDocument,
    ExportDocument,
    ImportDocument,
    AddPrimitive,
    RemovePrimitive
};

class Action {
   public:
    ActionName name{};
    using DataType =
        std::variant<std::filesystem::path, std::shared_ptr<Primitive>>;
    DataType data;
};

class GUI {
   public:
    void update();
    void clear_primitives();
    std::optional<Action> next_action();
    void add_primitive(size_t id, std::shared_ptr<Primitive> primitive);

    void add_circle();
    void add_square();
    void new_document();
    void export_document();
    void import_document();
    void remove_primitive();

   private:
    std::queue<Action> action_queue;
    std::unordered_map<size_t, std::shared_ptr<Primitive>> primitives;
};

}  // namespace view