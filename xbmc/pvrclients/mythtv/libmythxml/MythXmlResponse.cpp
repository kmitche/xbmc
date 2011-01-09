#include "MythXmlResponse.h"

#include <stdlib.h>

/*
 * TODO: Move tinyxml underneath libmythxml since it's a dependent library? Or alter the include
 * path so the tinyxml directory is on the path and then just include tinyxml.h?
 */
#include "tinyxml/tinyxml.h"

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

time_t MythXmlResponse::fromMythDateTime(const CStdString& datetime)
{
  /*
   * YYYY-MM-DDTHH:MM:SS
   */
  if (datetime.size() != 19)
    return 0;

  struct tm time;
  time.tm_year = atoi(datetime.Mid(0, 4)) - 1900;
  time.tm_mon = atoi(datetime.Mid(5, 2)) - 1;
  time.tm_mday = atoi(datetime.Mid(8, 2));
  time.tm_hour = atoi(datetime.Mid(11, 2));
  time.tm_min = atoi(datetime.Mid(14, 2));
  time.tm_sec = atoi(datetime.Mid(17, 2));
  return mktime(&time);
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

