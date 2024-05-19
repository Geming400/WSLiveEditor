#pragma once
#include <ActionInterface.hpp>

struct GetLevelString : public ActionInterface
{
    bool isValid(const matjson::Object&) override;
    inline std::string_view type() override { return "GET_LEVEL_STRING"; }
    ActionResponse run(LevelEditorLayer* editor, const matjson::Object&) override;
};
