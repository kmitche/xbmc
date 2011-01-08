#include "GetChannelListCommand.h"

#include "MythXmlResponse.h"

// TODO: Same as all files
#include "../client.h"
#include "../tinyxml/tinyxml.h"

GetChannelListCommand::GetChannelListCommand()
{
  MythXmlParameters parameters;
  /*
   * Setting the start and end time to the current time lists all the programs that are on now, from
   * which just the channels can be parsed.
   */
  time_t now = time(NULL);
  parameters.SetParameter("StartTime", now);
  parameters.SetParameter("EndTime", now);
  parameters.SetParameter("NumOfChannels", -1); // All channels

  Init("Myth/GetProgramGuide", parameters);
}

const std::vector<SChannel>& GetChannelListCommand::GetChannels()
{
  return m_channels;
}

bool GetChannelListCommand::ParseResponse(const CStdString& response)
{
  TiXmlDocument xml;
  xml.Parse(response.c_str(), 0, TIXML_ENCODING_LEGACY); // TODO: UTF8?

  TiXmlHandle docHandle(&xml);

  // TODO: error handling upstream.
  TiXmlElement* child = docHandle.FirstChildElement("detail").ToElement();
  if (child != NULL)
  {
    // this is the error reponse, process it
    int errorCode;
    CStdString errorDesc;
    MythXmlResponse::parseErrorNode(child, errorCode, errorDesc);
    XBMC->Log(LOG_ERROR, "MythXML - GetChannelListResult - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  child = docHandle.FirstChild("GetProgramGuideResponse").FirstChild("ProgramGuide").FirstChild("Channels").ToElement();
  if (child != NULL)
  {
    TiXmlElement* channelNode = NULL;
    for (channelNode = child->FirstChildElement("Channel"); channelNode; channelNode = channelNode->NextSiblingElement(
        "Channel"))
    {
      SChannel channel;
      channel.id = MythXmlResponse::toInteger(channelNode->Attribute("chanId"));
      channel.name = channelNode->Attribute("channelName");
      channel.callsign = channelNode->Attribute("callSign");
      channel.number = channelNode->Attribute("chanNum");
      m_channels.push_back(channel);
    }
    return true;
  }

  XBMC->Log(LOG_ERROR, "MythXML - GetChannelListResult - xml data doesn't have the expected information - %s",
      response.c_str());
  return false;
}
