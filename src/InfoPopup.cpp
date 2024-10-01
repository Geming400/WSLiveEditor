#include <Geode/Geode.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

auto savedValueKey = "has-readed-info-message";

bool hasReadedInfoMessage = Mod::get()->getSavedValue<bool>(savedValueKey, false);

class $modify(MenuLayer) {
    void userHasReadedMessage() {
        hasReadedInfoMessage = true;
        Mod::get()->setSavedValue(savedValueKey, true);
    }

    bool init() {
        if (!MenuLayer::init())
            return false;

        if (hasReadedInfoMessage)
            return false;

        geode::log::info("Showing WSLiveEditor's info message");

        auto alert = FLAlertLayer::create( // FLAlertLayer moment (real)
            "WSLiveEditor info",
            "Hi! Because of <cr>2.206</c>, when removing objects inside of the editor, you will <cr>need</c> to go back <cr>in it again</c> to see the <cl>changes made to the editor</c>!\n(this message won't show again)",
            "GOT IT"
        );
        alert->m_scene = this;
        alert->show();

        userHasReadedMessage();

        return true;
    }
};