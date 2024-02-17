#pragma once
#include <filesystem>
#include <memory>
#include <unordered_map>

namespace model {

class Primitive {
   public:
    size_t id{};
};

//Implement here primitive hierarchy similar to that defined in a Viewer

class Core {
   public:
    void make_new_document();
    void export_document(std::filesystem::path&);
    void import_document(std::filesystem::path&);
    void add_primitive(size_t id, std::shared_ptr<Primitive> new_primitive);
    void remove_primitive(size_t id);
    std::unordered_map<size_t, std::shared_ptr<Primitive>>& get_primitives() {
        return primitives;
    }

   private:
    bool is_changed{};
    std::unordered_map<size_t, std::shared_ptr<Primitive>> primitives;
};
}  // namespace model