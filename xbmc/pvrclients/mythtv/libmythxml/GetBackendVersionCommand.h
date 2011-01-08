#pragma once

#include "MythXmlCommand.h"

class GetBackendVersionCommand: public MythXmlCommand
{
  public:
    GetBackendVersionCommand();

    CStdString GetVersion();
    bool ParseResponse(const CStdString& response);

  private:
    CStdString m_version;
};
