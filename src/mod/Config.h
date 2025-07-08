#pragma once
#include <string>

struct Config {
    int         version     = 0;
    std::string language    = "en_US";
    bool        send_notice = true;
    bool        send_toast  = true;
    bool        send_form   = true;
};