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

  /*
   * Dates supplied by MythXML don't have any timezone information set. The MythXML interface
   * doesn't supply information about the GMT offset being used in dates so assume that they all
   * have the same timezone as the machine that XBMC is running on.
   */
  time_t now;
  time(&now);
  tm* local;
  local = localtime(&now);

  int localoffset = local->tm_gmtoff; // seconds
  /*
   * Take off the GMT offset and see what the difference in time is (seconds). This is the GMT offset.
   */
  local->tm_gmtoff = 0;

  int difference = now - mktime(local); // seconds
  if (abs(difference) < 3600) // Less than an hours difference. Same timezone.
    m_gmtoffset = localoffset;
  else // More than an hours difference. Different timezone.
    m_gmtoffset = localoffset + difference;

  return true;
}
