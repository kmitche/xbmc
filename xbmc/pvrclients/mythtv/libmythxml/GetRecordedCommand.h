#pragma once

#include "MythXmlCommand.h"

#include <vector>

#include "../MythTypes.h"

class GetRecordedCommand: public MythXmlCommand
{
  public:
    GetRecordedCommand();
    ~GetRecordedCommand();

    const std::vector<MythRecording>& GetRecordings();

    bool ParseResponse(const TiXmlHandle& handle);

  private:
    std::vector<MythRecording> m_recordings;
};
