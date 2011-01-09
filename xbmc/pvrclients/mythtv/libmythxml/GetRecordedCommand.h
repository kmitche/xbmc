#pragma once

#include "MythXmlCommand.h"

#include <vector>

#include "SRecordingInfo.h"

class GetRecordedCommand: public MythXmlCommand
{
  public:
    GetRecordedCommand();

    const std::vector<SRecordingInfo>& GetRecordings();

    bool ParseResponse(CStdString response);

  private:
    std::vector<SRecordingInfo> m_recordings;
};
