#pragma once

#include "MythXmlCommand.h"

class GetBackendVersionCommand: public MythXmlCommand
{
  public:
    GetBackendVersionCommand();
    ~GetBackendVersionCommand();

    CStdString GetVersion();
    bool ParseResponse(const TiXmlHandle& handle);

  private:
    CStdString m_version;
};
