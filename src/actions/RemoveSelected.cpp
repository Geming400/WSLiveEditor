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
    EditorUI* ui = editor->m_editorUI;
    auto array = ui->getSelectedObjects();
    auto toDelete = cocos2d::CCArray::create();

    for (GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(array)) {
        toDelete->addObject(obj);
    }
    for (GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(toDelete)) {
        ui->deleteObject(obj, false);
    }

    return ActionResponse::make_success();
}