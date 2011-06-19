#include "GetRecordedCommand.h"

#include "MythXmlResponse.h"

GetRecordedCommand::GetRecordedCommand()
{
  MythXmlParameters parameters;
  Init("Myth/GetRecorded", parameters);
}

GetRecordedCommand::~GetRecordedCommand()
{
}

bool GetRecordedCommand::ParseResponse(const TiXmlHandle& handle)
{
  TiXmlElement* programsNode = handle.FirstChild("GetRecordedResponse").FirstChild("Recorded").FirstChildElement("Programs").ToElement();
  if (programsNode == NULL)
    return false;

  TiXmlElement* programNode = NULL;
  for (programNode = programsNode->FirstChildElement("Program"); programNode;
       programNode = programNode->NextSiblingElement("Program"))
  {
    TiXmlElement* recordingNode = programNode->FirstChildElement("Recording");
    TiXmlElement* channelNode   = programNode->FirstChildElement("Channel");
    if (channelNode == NULL || recordingNode == NULL)
      continue;

    MythRecording recording;
    recording.title       = programNode->Attribute("title");
    recording.subtitle    = programNode->Attribute("subTitle");
    recording.description = programNode->GetText();
    recording.category    = programNode->Attribute("category");
    recording.start       = MythXmlResponse::toDateTime(programNode->Attribute("startTime"));
    recording.end         = MythXmlResponse::toDateTime(programNode->Attribute("endTime"));

    recording.chanid      = atoi(channelNode->Attribute("chanId"));
    recording.channum     = atoi(channelNode->Attribute("chanNum"));
    recording.channame    = channelNode->Attribute("channelName");
    recording.callsign    = channelNode->Attribute("callSign");

    int priority          = atoi(recordingNode->Attribute("recPriority")); // TODO: move priority shuffle to client
    priority              = priority < 0 ? 0 : priority;
    priority              = priority > 100 ? 100 : priority;
    recording.priority    = priority;

    recording.recstart    = MythXmlResponse::toDateTime(recordingNode->Attribute("recStartTs"));
    recording.recend      = MythXmlResponse::toDateTime(recordingNode->Attribute("recEndTs"));

    recording.recgroup    = recordingNode->Attribute("recGroup");

    recording.url.Format("/Myth/GetRecording?ChanId=%i&StartTime=%s", recording.chanid,
                         recordingNode->Attribute("recStartTs")); // Needs to stay in ISO format

    m_recordings.push_back(recording);
  }
  return true;
}

const std::vector<MythRecording>& GetRecordedCommand::GetRecordings()
{
  return m_recordings;
}
