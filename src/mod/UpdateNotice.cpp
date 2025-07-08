#include "mod/UpdateNotice.h"
#include "glacie/GlacieAPI.h"
#include "gmlib/gm/papi/PlaceholderAPI.h"
#include "ll/api/Config.h"
#include "ll/api/Versions.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/player/PlayerJoinEvent.h"
#include "ll/api/form/SimpleForm.h"
#include "ll/api/mod/RegisterHelper.h"
#include "resource.h"

namespace gm {

UpdateNotice& UpdateNotice::getInstance() {
    static UpdateNotice instance;
    return instance;
}

bool UpdateNotice::load() { return true; }

bool UpdateNotice::enable() {
    mConfig.emplace();
    if (!ll::config::loadConfig(*mConfig, getSelf().getConfigDir() / u8"config.json")) {
        ll::config::saveConfig(*mConfig, getSelf().getConfigDir() / u8"config.json");
    }
    mI18n.emplace(getSelf().getLangDir());
    mI18n->updateOrCreateLanguage("en_US", en_US);
    mI18n->updateOrCreateLanguage("zh_CN", zh_CN);
    mI18n->loadAllLanguages();
    mI18n->chooseLanguage(mConfig->language);
    // 没有这个 PAPI 就自动注册一个 (GMLIB v1.3.1 开始自带)
    if (!gmlib::PlaceholderAPI::getPlaceholderData("player_client_version")) {
        gmlib::PlaceholderAPI::registerPlaceholder(
            "player_client_version",
            [&](optional_ref<gmlib::GMPlayer> player) -> std::optional<std::string> {
                if (player.has_value()) {
                    if (auto version = glacie::getClientVersion(player->getNetworkIdentifier())) {
                        return version->to_string();
                    }
                }
                return {};
            }
        );
    }
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.emplaceListener<ll::event::PlayerJoinEvent>([&](const ll::event::PlayerJoinEvent& ev) {
        auto& pl = static_cast<gmlib::GMPlayer&>(ev.self());
        if (pl.getNetworkProtocolVersion() < ll::getNetworkProtocolVersion()) {
            if (mConfig->send_form) {
                ll::form::SimpleForm fm("form.title"_tr(), gmlib::PlaceholderAPI::translate("form.content"_tr(), pl));
                fm.appendButton("form.confirm"_tr());
                fm.sendTo(pl);
            }
            if (mConfig->send_notice) {
                pl.sendMessage(gmlib::PlaceholderAPI::translate("notice.message"_tr(), pl));
            }
            if (mConfig->send_toast) {
                pl.sendToast("toast.title"_tr(), gmlib::PlaceholderAPI::translate("toast.message"_tr(), pl));
            }
        }
    });
    return true;
}

bool UpdateNotice::disable() {
    mI18n.reset();
    mConfig.reset();
    return true;
}

bool UpdateNotice::unload() { return true; }

gmlib::i18n::LangI18n& UpdateNotice::getI18n() { return *mI18n; }

} // namespace gm

LL_REGISTER_MOD(gm::UpdateNotice, gm::UpdateNotice::getInstance());
