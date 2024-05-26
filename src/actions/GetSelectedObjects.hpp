#pragma once
#include <ActionInterface.hpp>

struct GetSelectedObjects : public ActionInterface
{
    bool isValid(const matjson::Object&) override;
    inline std::string_view type() override { return "GET_SELECTED_OBJECTS"; }
    ActionResponse run(LevelEditorLayer* editor, const matjson::Object&) override;
};