#include "RemoveSelected.hpp"
#include "ActionResponse.hpp"

#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/EditorUI.hpp>

bool RemoveSelectedObjects::isValid(const matjson::Object& j)
{
    return true;
}

ActionResponse RemoveSelectedObjects::run(LevelEditorLayer* editor, const matjson::Object& j)
{
    EditorUI* ui = editor->m_editorUI;
    for (GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(ui->getSelectedObjects()))
    {
        ui->deleteObject(obj, false);
    }
    return ActionResponse::make_success();
}