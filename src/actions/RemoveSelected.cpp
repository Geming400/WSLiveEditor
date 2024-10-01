#include "RemoveSelected.hpp"
#include "ActionResponse.hpp"

#include <Geode/Geode.hpp>
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
    auto m_objects = editor->m_objects;

    EditorUI* ui = editor->m_editorUI;
    for(GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(ui->getSelectedObjects()))
    {
        m_objects->removeObject(obj);
        editor->removeChild(obj, true);
        obj->cleanup();
    }

    return ActionResponse::make_success();
}