#include "GetChannelListCommand.h"

#include "MythXmlResponse.h"

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

GetChannelListCommand::~GetChannelListCommand()
{
}

const std::vector<SChannel>& GetChannelListCommand::GetChannels()
{
  return m_channels;
}

bool GetChannelListCommand::ParseResponse(const TiXmlHandle& handle)
{
  TiXmlElement* channelsNode = handle.FirstChild("GetProgramGuideResponse").FirstChild("ProgramGuide").FirstChild("Channels").ToElement();
  if (channelsNode == NULL)
    return false;

  TiXmlElement* channelNode = NULL;
  for (channelNode = channelsNode->FirstChildElement("Channel"); channelNode;
       channelNode = channelNode->NextSiblingElement("Channel"))
  {
    SChannel channel;
    channel.chanid    = MythXmlResponse::toInteger(channelNode->Attribute("chanId"));
    channel.channum   = MythXmlResponse::toInteger(channelNode->Attribute("chanNum"));
    channel.channame  = channelNode->Attribute("channelName");
    channel.callsign  = channelNode->Attribute("callSign");
    /*
     * If the channel number is 0 then it hasn't been set in the MythTV backend. These channels
     * are also ignored within XBMC.
     */
    if (channel.channum != 0)
      m_channels.push_back(channel);
  }
  return true;
}
