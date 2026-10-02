#include "About_tab.hpp"

AboutTab::AboutTab()
{
    this->inflateFromXMLRes("xml/tabs/about.xml");
    version->setText("SaltyNX-Tool " APP_VERSION);
}

brls::View* AboutTab::create()
{
    return new AboutTab();
}
