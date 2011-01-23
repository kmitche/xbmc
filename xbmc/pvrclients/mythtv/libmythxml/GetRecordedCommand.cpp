#include "GetRecordedCommand.h"

#include "MythXmlResponse.h"

// TODO: Update includes so these don't need to reference up the folder hierarchy. Assume in path.
#include "tinyXML/tinyxml.h"
#include "../client.h"

GetRecordedCommand::GetRecordedCommand()
{
  MythXmlParameters parameters;
  Init("Myth/GetRecorded", parameters);
}

GetRecordedCommand::~GetRecordedCommand()
{
}

bool GetRecordedCommand::ParseResponse(CStdString response)
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
    //XBMC->Log(LOG_ERROR, "MythXML - GetRecordingListResult - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  child = docHandle.FirstChild("GetRecordedResponse").FirstChild("Recorded").FirstChildElement("Programs").ToElement();
  if (child != NULL)
  {
    TiXmlElement* channelNode = NULL;
    TiXmlElement* programNode = NULL;
    TiXmlElement* recordingNode = NULL;

    int idx = 0;
    for (programNode = child->FirstChildElement("Program"); programNode; programNode = programNode->NextSiblingElement(
        "Program"))
    {
      channelNode = programNode->FirstChildElement("Channel");
      recordingNode = programNode->FirstChildElement("Recording");
      if (channelNode == NULL || recordingNode == NULL)
        continue;

      time_t starttime = MythXmlResponse::toDateTime(recordingNode->Attribute("recStartTs"));
      time_t endtime = MythXmlResponse::toDateTime(recordingNode->Attribute("recEndTs"));
      int priority = MythXmlResponse::toInteger(recordingNode->Attribute("recPriority")); // TODO: , min, max API added to toInteger.
      priority = priority < 0 ? 0 : priority;
      priority = priority > 100 ? 100 : priority;

      SRecording recordingInfo;
      // TODO: populate all the other common attributes from the XML information.
      recordingInfo.index = idx;
      recordingInfo.duration = endtime - starttime;
      recordingInfo.priority = priority;
      recordingInfo.recording_time = starttime;
      recordingInfo.title = programNode->Attribute("title");
      recordingInfo.subtitle = programNode->Attribute("subTitle");
      recordingInfo.description = programNode->GetText();
      recordingInfo.channel_name = channelNode->Attribute("channelName");
      recordingInfo.stream_url.Format("/Myth/GetRecording?ChanId=%s&StartTime=%s",
                                      channelNode->Attribute("chanId"), recordingNode->Attribute("recStartTs"));
      m_recordings.push_back(recordingInfo);
      idx++;
    }
    return true;
  }
  XBMC->Log(LOG_ERROR, "MythXML - GetRecordingListResult - xml data doesn't have the expected information - %s",
            response.c_str());
  return false;
}

const std::vector<SRecording>& GetRecordedCommand::GetRecordings()
{
  return m_recordings;
}
