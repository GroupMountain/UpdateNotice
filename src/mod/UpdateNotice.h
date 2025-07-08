#pragma once
#include "Config.h"
#include "gmlib/gm/i18n/LangI18n.h"
#include "ll/api/mod/NativeMod.h"

namespace gm {

class UpdateNotice {
public:
    static UpdateNotice& getInstance();

    UpdateNotice() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();

    bool enable();

    bool disable();

    bool unload();

    gmlib::i18n::LangI18n& getI18n();

private:
    ll::mod::NativeMod&                  mSelf;
    std::optional<Config>                mConfig;
    std::optional<gmlib::i18n::LangI18n> mI18n;
};

LANGI18N_LITERALS(UpdateNotice::getInstance().getI18n())

} // namespace gm