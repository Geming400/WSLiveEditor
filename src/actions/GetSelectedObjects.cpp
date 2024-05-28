#include "GetSelectedObjects.hpp"
#include "ActionResponse.hpp"

#include <Geode/binding/EditorUI.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/cocos.hpp>

bool GetSelectedObjects::isValid(const matjson::Object& j)
{
    return true;
}

ActionResponse GetSelectedObjects::run(LevelEditorLayer* editor, const matjson::Object& j)
{
    EditorUI* ui = editor->m_editorUI;
    auto array = ui->getSelectedObjects();

    std::stringstream str;
    bool first = true;
    for (GameObject* obj: geode::cocos::CCArrayExt<GameObject*>(array)) {
        if (first) first = false;
        else str << ';';

        str << obj->getSaveString(editor).operator std::string();
    }

    return ActionResponse::make_success(matjson::Value(str.str()));
}