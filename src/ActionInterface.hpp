#pragma once
#include <matjson.hpp>
#include <string_view>
#include <optional>
#include "ActionResponse.hpp"
#include "Geode/binding/LevelEditorLayer.hpp"

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
        return isType(getOpt<std::string>(j, "action").value_or(""));
    }

protected:

    template<typename T>
    [[nodiscard]] static std::optional<T> getOpt(const matjson::Object& j, std::string_view key)
    {
        if(auto&& k = j.find(key); k != j.end() && k->second.is<T>())
        {
            return std::optional<T>{std::in_place_t{}, k->second.as<T>()};
        }
        return {};
    }

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