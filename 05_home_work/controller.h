#pragma once
#include <functional>
#include <memory>

#include "model.h"
#include "view.h"

namespace controller {
class Logic {
   public:
    Logic();

   private:
    void register_callbacks();
    void process_gui_action();

    std::unique_ptr<model::Core> core;
    std::unique_ptr<view::GUI> gui;

    std::unordered_map<view::ActionName,
                       std::function<void(view::Action::DataType)>>
        action_callbacks;
};

// Implement logic for primitives conversion from viewer to model and vise versa
class PrimitiveConvertor {
   public:
    static std::shared_ptr<model::Primitive> convert(
        std::shared_ptr<view::Primitive>) {
        return {};
    };

    static std::shared_ptr<view::Primitive> convert(
        std::shared_ptr<model::Primitive>) {
        return {};
    };
};
}  // namespace controller