#include "MythXmlResponse.h"

#include <stdlib.h>
#include <time.h>

#include "tinyxml.h"

int MythXmlResponse::toInteger(TiXmlElement* element)
{
  if (element == NULL)
    return 0;
  return atoi(element->GetText());
}

int MythXmlResponse::toInteger(TiXmlAttribute* attr)
{
  if (attr == NULL)
    return 0;
  return attr->IntValue();
}

int MythXmlResponse::toInteger(const char* val)
{
  if (val == NULL)
    return 0;
  return atoi(val);
}

CStdString MythXmlResponse::toString(TiXmlElement* element)
{
  if (element == NULL)
    return "";
  return element->GetText();
}

CStdString MythXmlResponse::toString(TiXmlAttribute* attr)
{
  if (attr == NULL)
    return "";
  return attr->Value();
}

time_t MythXmlResponse::toDateTime(TiXmlElement* element)
{
  if (element == NULL)
    return 0;
  return fromMythDateTime(CStdString(element->GetText()));
}

time_t MythXmlResponse::toDateTime(TiXmlAttribute* attr)
{
  if (attr == NULL)
    return 0;
  return fromMythDateTime(CStdString(attr->Value()));
}

time_t MythXmlResponse::toDateTime(const char* val)
{
  return fromMythDateTime(val);
}

time_t MythXmlResponse::fromMythDateTime(const CStdString& iso)
{
  /*
   * YYYY-MM-DDTHH:MM:SS
   */
  if (iso.size() != 19)
    return 0;

  struct tm isotm;
  isotm.tm_year = atoi(iso.Mid(0,  4)) - 1900;
  isotm.tm_mon  = atoi(iso.Mid(5,  2)) - 1;
  isotm.tm_mday = atoi(iso.Mid(8,  2));
  isotm.tm_hour = atoi(iso.Mid(11, 2));
  isotm.tm_min  = atoi(iso.Mid(14, 2));
  isotm.tm_sec  = atoi(iso.Mid(17, 2));

  /*
   * TODO: Convert time to GMT since all the PVR interfaces seem to need GMT times?
   */
  time_t now;
  time(&now);
  tm * local;
  local = localtime(&now);

  return mktime(&isotm); // TODO: Convert to GMT? - local->tm_gmtoff;
}

void MythXmlResponse::parseErrorNode(TiXmlNode* node, int &errorCode, CStdString &desc)
{
  if (node == NULL)
  {
    errorCode = -1;
    desc = "";
  }

  errorCode = toInteger(node->FirstChildElement("errorCode"));
  desc = node->FirstChildElement("errorDescription")->GetText();
}

