#include "GetLevelString.hpp"
#include "ActionResponse.hpp"

#include <Geode/loader/Log.hpp>

bool GetLevelString::isValid(const matjson::Object& j)
{
    return true;
}

ActionResponse GetLevelString::run(LevelEditorLayer* editor, const matjson::Object& j)
{
    return ActionResponse::make_success(matjson::Value(editor->getLevelString()));
}