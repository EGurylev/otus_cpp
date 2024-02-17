#include <model.h>

namespace model {

void Core::make_new_document() { primitives.clear(); }

void Core::export_document(std::filesystem::path&) {
    // Serialize primitives and write them into file
}

void Core::import_document(std::filesystem::path&) {
    primitives.clear();
    // Read binary file here, deserialize it and fill
    // primitives container
}

void Core::add_primitive(size_t id,
                         std::shared_ptr<Primitive> new_primitive) {
    primitives[id] = new_primitive;
}

void Core::remove_primitive(size_t id) { primitives.erase(id); }

}  // namespace model