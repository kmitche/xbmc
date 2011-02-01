#include "GetBackendTimeCommand.h"

#include <time.h>

#include "MythXmlResponse.h"

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

bool GetBackendTimeCommand::ParseResponse(const TiXmlHandle& handle)
{
  TiXmlElement* statusNode = handle.FirstChild("Status").ToElement();
  if (statusNode == NULL)
    return false;

  m_datetime = MythXmlResponse::toDateTime(statusNode->Attribute("ISODate"));
  //the date comes without a gmt offset let's emulate that.
  time_t raw;
  time(&raw);
  struct tm* timeinfo;
  timeinfo = localtime(&raw);
  int localoffset = timeinfo->tm_gmtoff;
  timeinfo->tm_gmtoff = 0;
  time_t serverTime = mktime(timeinfo);
  int timediff = raw - serverTime;
  if (abs(timediff) < 3600)
  {
    // same zone
    m_gmtoffset = localoffset;
  }
  else
  {
    int off = timediff / 3600;
    m_gmtoffset = localoffset + off;
  }
  return true;
}
