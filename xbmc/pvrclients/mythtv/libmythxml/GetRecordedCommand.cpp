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

    for (programNode = child->FirstChildElement("Program"); programNode; programNode = programNode->NextSiblingElement(
        "Program"))
    {
      channelNode = programNode->FirstChildElement("Channel");
      recordingNode = programNode->FirstChildElement("Recording");
      if (channelNode == NULL || recordingNode == NULL)
        continue;

      SRecording recording;
      recording.title       = programNode->Attribute("title");
      recording.subtitle    = programNode->Attribute("subTitle");
      recording.description = programNode->GetText();
      recording.category    = programNode->Attribute("category");
      recording.start       = MythXmlResponse::toDateTime(programNode->Attribute("startTime"));
      recording.end         = MythXmlResponse::toDateTime(programNode->Attribute("endTime"));

      recording.chanid      = atoi(channelNode->Attribute("chanId"));
      recording.channum     = atoi(channelNode->Attribute("chanNum"));
      recording.callsign    = channelNode->Attribute("callSign");

      int priority          = atoi(recordingNode->Attribute("recPriority")); // TODO: move priority shuffle to client
      priority              = priority < 0 ? 0 : priority;
      priority              = priority > 100 ? 100 : priority;
      recording.priority    = priority;

      recording.recstart    = MythXmlResponse::toDateTime(recordingNode->Attribute("recStartTs"));
      recording.recend      = MythXmlResponse::toDateTime(recordingNode->Attribute("recEndTs"));

      recording.recgroup    = recordingNode->Attribute("recGroup");

      recording.url.Format("/Myth/GetRecording?ChanId=%s&StartTime=%s", recording.chanid,
                           recordingNode->Attribute("recStartTs")); // Needs to stay in ISO format

      m_recordings.push_back(recording);
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
