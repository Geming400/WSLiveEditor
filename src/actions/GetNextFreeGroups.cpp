#include "GetNextFreeGroups.hpp"
#include "ActionResponse.hpp"

#include <Geode/Geode.hpp>
#include <Geode/loader/Log.hpp>

bool GetNextFreeGroups::isValid(const matjson::Object& j)
{
    return true;
}

ActionResponse GetNextFreeGroups::run(LevelEditorLayer* editor, const matjson::Object& j)
{
    int count = 1;
    if (auto value = checkTypeGetVal<int>(j, "count"); value.first) {
        count = value.second;
    }

    std::set<short> used;

    auto array = editor->m_objects;
    for (GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(array)) {
        if (obj->m_groups == nullptr) continue;
        for (short group: *obj->m_groups) {
            used.insert(group);
        }
    }
    int gotten = 0;
    std::vector<short> free;

    for (short i = 1; i <= 9999; i++) {
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