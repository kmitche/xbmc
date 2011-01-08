#include "GetBackendVersionCommand.h"

#include "MythXmlResponse.h"

// TODO: As for all other files...
#include "../tinyxml/tinyxml.h"
#include "../client.h"

GetBackendVersionCommand::GetBackendVersionCommand()
{
  MythXmlParameters parameters;
  Init("GetStatus", parameters);
}

bool GetBackendVersionCommand::ParseResponse(const CStdString& response)
{
  TiXmlDocument xml;
  xml.Parse(response.c_str(), 0, TIXML_ENCODING_LEGACY);

  TiXmlHandle docHandle(&xml);

  // TODO: Error handling done upstream.
  TiXmlElement* child = docHandle.FirstChildElement("detail").ToElement();
  if (child != NULL)
  {
    // this is the error reponse, process it
    int errorCode;
    CStdString errorDesc;
    MythXmlResponse::parseErrorNode(child, errorCode, errorDesc);
    XBMC->Log(LOG_ERROR, "MythXML - GetBackendVersionResult - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  child = docHandle.FirstChild("Status").ToElement();
  if (child != NULL)
  {
    m_version = child->Attribute("version");
    return true;
  }
  XBMC->Log(LOG_ERROR, "MythXML - GetBackendVersionResult - xml data doesn't have the expected information - %s",
      response.c_str());
  return false;
}

CStdString GetBackendVersionCommand::GetVersion()
{
  return m_version;
}
