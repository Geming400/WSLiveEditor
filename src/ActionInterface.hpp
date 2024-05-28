#pragma once
#include <matjson.hpp>
#include <string_view>
#include "ActionResponse.hpp"
#include "Geode/binding/LevelEditorLayer.hpp"

class LevelEditorLayer;

struct ActionInterface
{
    virtual bool isValid(const matjson::Object&) = 0;
    virtual std::string_view type() = 0;
    inline virtual bool isType(std::string_view actionname)
    {
        return actionname == type(); 
    }
    virtual ActionResponse run(LevelEditorLayer* editor, const matjson::Object&) = 0;

    inline bool isType(const matjson::Object& j)
    {
        if(auto&& act = checkTypeGetVal<std::string>(j, "action"); act.first)
        {
            return isType(act.second);
        }
        return false;
    }

protected:
    template<typename T>
    [[nodiscard]] static bool checkType(const matjson::Object& j, std::string_view key)
    {
        auto&& k = j.find(key);
        return k != j.end() && k->second.is<T>();
    }
    template<typename T>
    [[nodiscard]] static std::pair<bool, T> checkTypeGetVal(const matjson::Object& j, std::string_view key)
    {
        auto&& k = j.find(key);
        if(k != j.end() && k->second.is<T>())
        {
            return {true, k->second.as<T>()};
        }
        return {false, T{}};
    }
};