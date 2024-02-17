#include "controller.h"

namespace view {

std::optional<Action> GUI::next_action() {
    if (!action_queue.empty()) {
        auto action{std::move(action_queue.front())};
        action_queue.pop();
        return action;
    }
    return {};
}

void GUI::update() {
    // Implement here logic for updating the screen
}

void GUI::add_primitive(size_t id, std::shared_ptr<Primitive> primitive) {
    primitives[id] = primitive;
}

void GUI::add_circle() {
    // Add circle with defined parameters
    action_queue.push(Action{ActionName::AddPrimitive,
                             std::make_shared<Circle>(Circle(0, 1, {2, 3}))});

    // Add here this circle to primitives map as well
}

// Implement here other gui actions: add_square, new_document, export_document,
// import_rocument, remove_primitive

void GUI::clear_primitives() { primitives.clear(); }

}  // namespace view

int main() { controller::Logic app; }