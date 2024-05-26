#include "GetNextFreeLinks.hpp"
#include "ActionResponse.hpp"

#include <Geode/Geode.hpp>
#include <Geode/loader/Log.hpp>

bool GetNextFreeLinks::isValid(const matjson::Object& j)
{
    return true;
}

ActionResponse GetNextFreeLinks::run(LevelEditorLayer* editor, const matjson::Object& j)
{
    int count = 1;
    if (auto value = checkTypeGetVal<int>(j, "count"); value.first) {
        count = value.second;
    }

    std::set<int> used;

    auto array = editor->m_objects;
    for (GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(array)) {
        if (obj->m_linkedGroup > 0)
            used.insert(obj->m_linkedGroup);
    }
    int gotten = 0;
    std::vector<int> free;

    for (int i = 1; i <= 999999; i++) {
        if (!used.contains(i)) {
            free.push_back(i);
            gotten++;
            if (gotten >= count) {
                break;
            }
        }
    }

    std::stringstream str;
    bool first = true;
    for (short n : free) {
        if (first) {
            first = false;
        } else {
            str << ',';
        }
        str << n;
    }

    return ActionResponse::make_success(matjson::Value(str.str()));
}