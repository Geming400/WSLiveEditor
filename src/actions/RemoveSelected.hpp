#pragma once
#include <ActionInterface.hpp>

struct RemoveSelectedObjects : public ActionInterface
{
    bool isValid(const matjson::Object&) override;
    inline std::string_view type() override { return "REMOVE_SELECTED_OBJECTS"; }

    inline bool isType(std::string_view actionname) override {
        return actionname == type();
    }
    ActionResponse run(LevelEditorLayer* editor, const matjson::Object&) override;
};
