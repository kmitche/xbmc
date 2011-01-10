#include "GetBackendTimeCommand.h"

#include <time.h>

#include "MythXmlResponse.h"

// TODO: Assume tinyXML on path?
#include "tinyXML/tinyxml.h"

// TODO: Logging without XBMC handle?
#include "../client.h"

GetBackendTimeCommand::GetBackendTimeCommand()
{
  MythXmlParameters parameters;
  Init("GetStatus", parameters);
}

GetBackendTimeCommand::~GetBackendTimeCommand()
{
}

int GetBackendTimeCommand::GetGmtOffset()
{
  return m_gmtoffset;
}

time_t GetBackendTimeCommand::GetDateTime()
{
  return m_datetime;
}

bool GetBackendTimeCommand::ParseResponse(CStdString response)
{
  TiXmlDocument xml;
  xml.Parse(response.c_str(), 0, TIXML_ENCODING_LEGACY);

  TiXmlHandle docHandle(&xml);

  // TODO: Move same error handling to MythXml.
  TiXmlElement* child = docHandle.FirstChildElement("detail").ToElement();
  if (child != NULL)
  {
    // this is the error response, process it
    int errorCode;
    CStdString errorDesc;
    MythXmlResponse::parseErrorNode(child, errorCode, errorDesc);
    //XBMC->Log(LOG_ERROR, "MythXML - GetBackendVersionResult - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  child = docHandle.FirstChild("Status").ToElement();
  if (child != NULL)
  {
    m_datetime = MythXmlResponse::toDateTime(child->Attribute("ISODate"));
//    //the date comes without a gmt offset let's emulate that.
//    time_t raw;
//    time(&raw);
//    struct tm* timeinfo;
//    timeinfo = localtime(&raw);
//    int localoffset = timeinfo->tm_gmtoff;
//    timeinfo->tm_gmtoff = 0;
//    time_t serverTime = mktime(timeinfo);
//    int timediff = raw - serverTime;
//    if (abs(timediff) < 3600)
//    {
//      // same zone
//      m_gmtoffset = localoffset;
//    }
//    else
//    {
//      int off = timediff / 3600;
//      m_gmtoffset = localoffset + off;
//    }
    return true;
  }
  //XBMC->Log(LOG_ERROR, "MythXML - GetBackendVersionResult - xml data doesn't have the expected information - %s",
  //          response.c_str());
  return false;
}
