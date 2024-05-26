#include "Add.hpp"
#include "ActionResponse.hpp"
#include <string>

#include <Geode/Geode.hpp>
#include <Geode/loader/Log.hpp>

bool AddObjectsAction::isValid(const matjson::Object& j)
{
    if(auto objects = checkTypeGetVal<std::string>(j, "objects"); objects.first)
    {
        //atleast an object id specifier (usually even the first 2 chars)
        return objects.second.find("1,") != std::string::npos;
    }
    return false;
}


ActionResponse AddObjectsAction::run(LevelEditorLayer* editor, const matjson::Object& j)
{
    editor->createObjectsFromString(j.find("objects")->second.as_string(), true, true);
    return ActionResponse::make_success(); 
}