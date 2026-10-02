#pragma once

#include <borealis.hpp>

class AboutTab : public brls::Box
{
  public:
    AboutTab();

    static brls::View* create();

  private:
    BRLS_BIND(brls::Label, version, "version");
};
