#include "Options_tab.hpp"

#include <cstdio>

#define DISABLE_FLAG "sdmc:/SaltySD/flags/disable.flag"
#define LOG_FLAG     "sdmc:/SaltySD/flags/log.flag"

static bool flagExists(const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file)
        return false;
    fclose(file);
    return true;
}

static void setFlag(const char* path, bool present)
{
    if (present)
    {
        FILE* file = fopen(path, "w");
        if (file)
            fclose(file);
    }
    else
        remove(path);
}

OptionsTab::OptionsTab()
{
    this->inflateFromXMLRes("xml/tabs/options.xml");

    status->setOn(!flagExists(DISABLE_FLAG), false);
    status->getEvent()->subscribe([](bool enabled) {
        setFlag(DISABLE_FLAG, !enabled);
    });

    logs->setOn(flagExists(LOG_FLAG), false);
    logs->getEvent()->subscribe([](bool enabled) {
        setFlag(LOG_FLAG, enabled);
    });
}

brls::View* OptionsTab::create()
{
    return new OptionsTab();
}
