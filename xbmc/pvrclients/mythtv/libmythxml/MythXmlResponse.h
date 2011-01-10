#pragma once

#include "../StdString.h"

class TiXmlAttribute;
class TiXmlElement;
class TiXmlNode;

class MythXmlResponse
{
  public:
    static int toInteger(TiXmlElement* element);
    static int toInteger(TiXmlAttribute* attr);
    static int toInteger(const char* val);
    static time_t toDateTime(TiXmlElement* element);
    static time_t toDateTime(TiXmlAttribute* attr);
    static time_t toDateTime(const char* val);
    static CStdString toString(TiXmlElement* attr);
    static CStdString toString(TiXmlAttribute* attr);
    static void parseErrorNode(TiXmlNode* node, int &errorCode, CStdString &desc);

  private:
    static time_t fromMythDateTime(const CStdString& datetime);
};
