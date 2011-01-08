#include "GetNumOfRecordingsCommand.h"

#include "MythXmlResponse.h"

// TODO: Tidy like all other files.
#include "../tinyxml/tinyxml.h"
#include "../client.h"

GetNumOfRecordingsCommand::GetNumOfRecordingsCommand()
{
  MythXmlParameters parameters;
  Init("Myth/GetRecorded", parameters);
}

bool GetNumOfRecordingsCommand::ParseResponse(const CStdString& response)
{
  TiXmlDocument xml;
  xml.Parse(response.c_str(), 0, TIXML_ENCODING_LEGACY); // TODO: UTF8

  TiXmlHandle docHandle(&xml);

  TiXmlElement* child = docHandle.FirstChildElement("detail").ToElement();
  if (child != NULL)
  {
    // this is the error reponse, process it
    int errorCode;
    CStdString errorDesc;
    MythXmlResponse::parseErrorNode(child, errorCode, errorDesc);
    XBMC->Log(LOG_ERROR, "MythXML - GetNumRecordingsResult - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  child = docHandle.FirstChild("GetRecordedResponse").FirstChildElement("Count").ToElement();
  if (child != NULL)
  {
    m_number_of_recordings = MythXmlResponse::toInteger(child);
    return true;
  }
  XBMC->Log(LOG_ERROR, "MythXML - GetNumRecordingsResult - xml data doesn't have the expected information - %s",
      response.c_str());
  return false;
}

int GetNumOfRecordingsCommand::GetNumberOfRecordings()
{
  return m_number_of_recordings;
}
