#pragma once

#include <borealis.hpp>

class OptionsTab : public brls::Box
{
  public:
    OptionsTab();

    static brls::View* create();

  private:
    BRLS_BIND(brls::BooleanCell, status, "status");
    BRLS_BIND(brls::BooleanCell, logs, "logs");
};
