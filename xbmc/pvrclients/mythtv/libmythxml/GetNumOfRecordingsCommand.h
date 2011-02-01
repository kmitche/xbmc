#pragma once

#include "MythXmlCommand.h"

class GetNumOfRecordingsCommand: public MythXmlCommand
{
  public:
    GetNumOfRecordingsCommand();

    int GetNumberOfRecordings();

    bool ParseResponse(const TiXmlHandle& handle);

  private:
    int m_number_of_recordings;

};
