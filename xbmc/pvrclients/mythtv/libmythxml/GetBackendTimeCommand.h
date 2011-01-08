#pragma once

#include "MythXmlCommand.h"

class GetBackendTimeCommand: public MythXmlCommand
{
  public:
    GetBackendTimeCommand();
    time_t  GetDateTime();
    int     GetGmtOffset();

    bool ParseResponse(const CStdString& response);

  private:
    time_t  m_datetime;
    int     m_gmtoffset;
};
