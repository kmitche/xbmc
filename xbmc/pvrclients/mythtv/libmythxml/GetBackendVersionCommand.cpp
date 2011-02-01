#include "GetBackendVersionCommand.h"

#include "MythXmlResponse.h"

GetBackendVersionCommand::GetBackendVersionCommand()
{
  MythXmlParameters parameters;
  Init("GetStatus", parameters);
}

GetBackendVersionCommand::~GetBackendVersionCommand()
{
}

bool GetBackendVersionCommand::ParseResponse(const TiXmlHandle& handle)
{
  TiXmlElement* statusNode = handle.FirstChild("Status").ToElement();
  if (statusNode == NULL)
    return false;

  m_version = statusNode->Attribute("version");
  return true;
}

CStdString GetBackendVersionCommand::GetVersion()
{
  return m_version;
}
