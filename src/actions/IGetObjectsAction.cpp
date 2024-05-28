#include "IGetObjectsAction.hpp"
#include "ActionResponse.hpp"
#include "Geode/binding/LevelEditorLayer.hpp"
#include "Geode/utils/cocos.hpp"
#include <matjson.hpp>
#include <matjson/stl_serialize.hpp>
#include <fmt/format.h>

using geode::cocos::CCArrayExt;

//TODO
bool IGetObjectsAction::Filters::isFilterJsonValid(const matjson::Object& actionJson)
{
    return true;
}

//TODO
bool IGetObjectsAction::Filters::shouldKeepObjectString(std::string_view)
{
    return true;
}

//TODO
std::optional<IGetObjectsAction::Filters> IGetObjectsAction::Filters::getFromActionJson(const matjson::Object& j)
{
    return {};
}

//TODO
bool IGetObjectsAction::isValid(const matjson::Object&)
{
    return true;
}



std::vector<std::string> IGetObjectsAction::getFilteredObjectStrings(LevelEditorLayer* editor, const matjson::Object& j)
{
    std::vector<std::string> ret;
    std::optional<Filters> filters = Filters::getFromActionJson(j);

    if(!filters)
    {
        CCArrayExt<GameObject*> objects = getObjects(editor);
        ret.reserve(objects.size());
        for(const auto& obj : objects)
        {
            ret.emplace_back(obj->getSaveString(editor));
        }
        return ret;
    }

    return ret;
}


ActionResponse IGetObjectsAction::run(LevelEditorLayer* editor, const matjson::Object& j)
{
    //if we have a separator...
    if(auto separator = getOpt<std::string>(j, "separator"))
    {
        std::string& sep = *separator;
        std::vector<std::string> objstrings = getFilteredObjectStrings(editor, j);
        std::string ret;
        if(sep.empty())
        {
            for(const auto& str : objstrings)
            {
                ret += str;
            }
            return ActionResponse::make_success(matjson::Value(ret));
        }

        //atleast this size
        ret.reserve(sep.size() * objstrings.size());
        for(const auto& str : objstrings)
        {
            ret += fmt::format("{}{}", sep, str);
        }
        return ActionResponse::make_success(matjson::Value(ret));
    }

    //if no separator return array like [str, str1]


    std::vector<std::string> strs = getFilteredObjectStrings(editor, j);
    matjson::Value jsonarray = matjson::Serialize<decltype(strs)>::to_json(strs);
    return ActionResponse::make_success(jsonarray);
}
