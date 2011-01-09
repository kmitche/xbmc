#pragma once

#include "MythXmlCommand.h"

class GetNumOfRecordingsCommand: public MythXmlCommand
{
  public:
    GetNumOfRecordingsCommand();

    int GetNumberOfRecordings();

    bool ParseResponse(CStdString response);

  private:
    int m_number_of_recordings;

};
