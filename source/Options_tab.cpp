#include "Options_tab.hpp"

#include <cstdio>

#define DISABLE_FLAG "sdmc:/SaltySD/flags/disable.flag"
#define LOG_FLAG     "sdmc:/SaltySD/flags/log.flag"
#define BLOCK_FILE_STATS_FLAG "sdmc:/SaltySD/flags/blockfilestats.flag"
#define NVN_COUNTERS_FLAG     "sdmc:/SaltySD/flags/nvncounters.flag"
#define NO_LOGO_FLAG          "sdmc:/SaltySD/flags/nologo.flag"

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

    blockFileStats->setOn(flagExists(BLOCK_FILE_STATS_FLAG), false);
    blockFileStats->getEvent()->subscribe([](bool enabled) {
        setFlag(BLOCK_FILE_STATS_FLAG, enabled);
    });

    nvnCounters->setOn(flagExists(NVN_COUNTERS_FLAG), false);
    nvnCounters->getEvent()->subscribe([](bool enabled) {
        setFlag(NVN_COUNTERS_FLAG, enabled);
    });

    noLogo->setOn(flagExists(NO_LOGO_FLAG), false);
    noLogo->getEvent()->subscribe([](bool enabled) {
        setFlag(NO_LOGO_FLAG, enabled);
    });
}

brls::View* OptionsTab::create()
{
    return new OptionsTab();
}
