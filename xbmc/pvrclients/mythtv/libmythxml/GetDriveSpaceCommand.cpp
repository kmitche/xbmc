#include "GetDriveSpaceCommand.h"

#include "MythXmlResponse.h"

#include "../tinyxml/tinyxml.h"
#include "../client.h"

/*
 * ascii-to-longlong conversion
 *
 * no error checking; assumes decimal digits
 *
 * efficient conversion:
 *   start with value = 0
 *   then, starting at first character, repeat the following
 *   until the end of the string:
 *
 *     new value = (10 * (old value)) + decimal value of next character
 *
 */

// TODO: Change usages to atof and then cast to int_64t if it's supposed to be 64 bit.
long long my_atoll(const char *instr)
{
  long long retval;

  retval = 0;
  for (; *instr; instr++)
  {
    retval = 10 * retval + (*instr - '0');
  }
  return retval;
}

GetDriveSpaceCommand::GetDriveSpaceCommand()
{
  MythXmlParameters parameters;
  Init("GetStatus", parameters);
}

bool GetDriveSpaceCommand::ParseResponse(const CStdString& response)
{
  TiXmlDocument xml;
  xml.Parse(response.c_str(), 0, TIXML_ENCODING_LEGACY);

  TiXmlHandle docHandle(&xml);

  TiXmlElement* child = docHandle.FirstChildElement("detail").ToElement();
  if (child != NULL)
  {
    // this is the error reponse, process it
    int errorCode;
    CStdString errorDesc;
    MythXmlResponse::parseErrorNode(child, errorCode, errorDesc);
    XBMC->Log(LOG_ERROR, "MythXML - GetDriveSpaceCommand - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  child = docHandle.FirstChild("Status").FirstChildElement("MachineInfo").FirstChildElement("Storage").ToElement();
  if (child != NULL)
  {
    TiXmlElement* groupNode = NULL;
    for (groupNode = child->FirstChildElement("Group"); groupNode; groupNode = groupNode->NextSiblingElement("Group"))
    {
      CStdString idAttr = groupNode->Attribute("id");
      if (idAttr.Find("total") != 0)
        continue;
      CStdString total = groupNode->Attribute("total");
      CStdString used = groupNode->Attribute("used");

      // TODO: Are these suppose to be int64_t or int32_t?
      m_total = my_atoll(total.c_str());
      m_used = my_atoll(used.c_str());
    }
    return true;
  }
  XBMC->Log(LOG_ERROR, "MythXML - GetDriveSpaceCommand - xml data doesn't have the expected information - %s",
      response.c_str());
  return false;
}

long long GetDriveSpaceCommand::GetTotal()
{
  return m_total;
}

long long GetDriveSpaceCommand::GetUsed()
{
  return m_used;
}
