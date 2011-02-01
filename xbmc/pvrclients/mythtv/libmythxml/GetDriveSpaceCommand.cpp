#include "GetDriveSpaceCommand.h"

#include "MythXmlResponse.h"

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

GetDriveSpaceCommand::~GetDriveSpaceCommand()
{
}

bool GetDriveSpaceCommand::ParseResponse(const TiXmlHandle& handle)
{
  TiXmlElement* storageNode = handle.FirstChild("Status").FirstChildElement("MachineInfo").FirstChildElement("Storage").ToElement();
  if (storageNode == NULL)
    return false;

  TiXmlElement* groupNode = NULL;
  for (groupNode = storageNode->FirstChildElement("Group"); groupNode;
       groupNode = groupNode->NextSiblingElement("Group"))
  {
    CStdString id = groupNode->Attribute("id");
    if (id.Find("total") != 0)
      continue;

    CStdString total = groupNode->Attribute("total");
    CStdString used = groupNode->Attribute("used");

    // TODO: Are these suppose to be int64_t or int32_t?
    m_total = my_atoll(total.c_str());
    m_used = my_atoll(used.c_str());
  }
  return true;
}

long long GetDriveSpaceCommand::GetTotal()
{
  return m_total;
}

long long GetDriveSpaceCommand::GetUsed()
{
  return m_used;
}
