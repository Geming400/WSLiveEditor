#pragma once
#include <ActionInterface.hpp>

struct GetNextFreeLinks : public ActionInterface
{
    bool isValid(const matjson::Object&) override;
    inline std::string_view type() override { return "GET_NEXT_FREE_LINKS"; }
    ActionResponse run(LevelEditorLayer* editor, const matjson::Object&) override;
};