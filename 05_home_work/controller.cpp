#include "controller.h"

namespace controller {

Logic::Logic() {
    core = std::make_unique<model::Core>();
    gui = std::make_unique<view::GUI>();

    register_callbacks();

    while (true) {
        // Add here some sleep
        gui->update();
        process_gui_action();
    }
}

void Logic::register_callbacks() {
    action_callbacks[view::ActionName::NewDocument] =
        [=](view::Action::DataType) {
            core->make_new_document();
            gui->clear_primitives();
        };

    action_callbacks[view::ActionName::ImportDocument] =
        [=](view::Action::DataType data) {
            core->import_document(std::get<std::filesystem::path>(data));
        };

    action_callbacks[view::ActionName::ExportDocument] =
        [=](view::Action::DataType data) {
            core->export_document(std::get<std::filesystem::path>(data));
            for (auto& primitive : core->get_primitives()) {
                gui->add_primitive(
                    primitive.second->id,
                    PrimitiveConvertor::convert(primitive.second));
            }
        };

    action_callbacks[view::ActionName::AddPrimitive] =
        [=](view::Action::DataType data) {
            auto primitive{std::get<std::shared_ptr<view::Primitive>>(data)};
            core->add_primitive(primitive->id,
                                PrimitiveConvertor::convert(primitive));
        };

    action_callbacks[view::ActionName::RemovePrimitive] =
        [=](view::Action::DataType data) {
            auto primitive{std::get<std::shared_ptr<view::Primitive>>(data)};
            core->remove_primitive(primitive->id);
        };
}

void Logic::process_gui_action() {
    if (auto action{gui->next_action()}; action) {
        auto it{action_callbacks.find(action->name)};
        if (it != action_callbacks.end()) {
            it->second(action->data);
        }
    }
}

}  // namespace controller