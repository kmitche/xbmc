#include "GetNumOfRecordingsCommand.h"

#include "MythXmlResponse.h"

GetNumOfRecordingsCommand::GetNumOfRecordingsCommand()
{
  MythXmlParameters parameters;
  Init("Myth/GetRecorded", parameters);
}

bool GetNumOfRecordingsCommand::ParseResponse(const TiXmlHandle& handle)
{
  TiXmlElement* countNode = handle.FirstChild("GetRecordedResponse").FirstChildElement("Count").ToElement();
  if (countNode == NULL)
    return false;

  m_number_of_recordings = MythXmlResponse::toInteger(countNode);
  return true;
}

int GetNumOfRecordingsCommand::GetNumberOfRecordings()
{
  return m_number_of_recordings;
}
