#include "GetProgramGuideCommand.h"

#include "MythXmlResponse.h"

#include <stdlib.h>
#include <map>

#include "tinyxml.h"

// TODO: understand if there is a better way to log from what should be "binary" libraries that don't know anything about XBMC.
#include "../client.h"

GetProgramGuideCommand::GetProgramGuideCommand(const int &chanid, const time_t &gmt_start, const time_t &gmt_end)
{
  MythXmlParameters parameters;
  if (chanid < 0)
    parameters.SetParameter("NumOfChannels", -1); // All
  else
  {
    parameters.SetParameter("NumOfChannels", 1);
    parameters.SetParameter("StartChanId", chanid);
  }
  parameters.SetParameter("StartTime", gmt_start);
  parameters.SetParameter("EndTime", gmt_end);

  Init("Myth/GetProgramGuide", parameters);
}

bool GetProgramGuideCommand::ParseResponse(CStdString response)
{
  TiXmlDocument document;
  document.Parse(response.c_str(), 0, TIXML_ENCODING_LEGACY);

  TiXmlHandle handle(&document);

  // TODO: Put the error checking directly into MythXML for the response. Pass the TiXmlHandle into this method.
  TiXmlElement* channels = handle.FirstChildElement("detail").ToElement();
  if (channels != NULL)
  {
    // this is the error reponse, process it
    int errorCode;
    CStdString errorDesc;
    MythXmlResponse::parseErrorNode(channels, errorCode, errorDesc);
    //XBMC->Log(LOG_ERROR, "MythXML - GetProgramGuideResult - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  channels = handle.FirstChild("GetProgramGuideResponse").FirstChild("ProgramGuide").FirstChildElement("Channels").ToElement();
  if (channels != NULL)
  {
    TiXmlElement* channelNode;
    for (channelNode = channels->FirstChildElement("Channel"); channelNode;
         channelNode = channelNode->NextSiblingElement("Channel"))
    {
      int chanid = MythXmlResponse::toInteger(channelNode->Attribute("chanId"));
      int channum = MythXmlResponse::toInteger(channelNode->Attribute("chanNum"));
      TiXmlElement* programNode;
      for (programNode = channelNode->FirstChildElement("Program"); programNode;
           programNode = programNode->NextSiblingElement("Program"))
      {
        SEpg epg;
        epg.chanid            = chanid;
        epg.channum           = channum;
        epg.description       = programNode->GetText();
        epg.title             = programNode->Attribute("title");
        epg.subtitle          = programNode->Attribute("subTitle");
        epg.start_time        = MythXmlResponse::toDateTime(programNode->Attribute("startTime"));
        epg.end_time          = MythXmlResponse::toDateTime(programNode->Attribute("endTime"));

        CStdString category   = programNode->Attribute("category");
        GenrePair genre       = m_genre_mapper.getGenreTypeId(category);
        epg.genre_type        = genre.type;
        epg.genre_subtype     = genre.subtype;

        // TODO: Parse out the "Recording" child element to determine if the program is scheduled
        // to record.
        m_epg.push_back(epg);
      }
    }
    return true;
  }

  //XBMC->Log(LOG_ERROR, "MythXML - GetProgramGuideResult - xml data doesn't have the expected information - %s",
  //    response.c_str());
  return false;
}

const std::vector<SEpg>& GetProgramGuideCommand::GetEpg()
{
  return m_epg;
}

/*
 * TODO: Where did this list come from?
 *
 static const SContentType g_content_group[] =
 { { 0x10, "Movie/Drama" }
 , { 0x20, "News/Current Affairs" }
 , { 0x30, "Show/Game show" }
 , { 0x40, "Sports" }
 , { 0x50, "Children's/Youth" }
 , { 0x60, "Music/Ballet/Dance" }
 , { 0x70, "Arts/Culture (without music)" }
 , { 0x80, "Social/Political issues/Economics" }
 , { 0x90, "Childrens/Youth Education/Science/Factual" }
 , { 0xa0, "Leisure hobbies" }
 , { 0xb0, "Misc" }
 , { 0xf0, "Unknown" }
 };

 static const SContentType g_content_type[] =
 {
 // movie/drama
 { 0x11, "Detective/Thriller" }
 , { 0x12, "Adventure/Western/War" }
 , { 0x13, "Science Fiction/Fantasy/Horror" }
 , { 0x14, "Comedy" }
 , { 0x15, "Soap/Melodrama/Folkloric" }
 , { 0x16, "Romance" }
 , { 0x17, "Serious/ClassicalReligion/Historical" }
 , { 0x18, "Adult Movie/Drama" }

 // news/current affairs
 , { 0x21, "News/Weather Report" }
 , { 0x22, "Magazine" }
 , { 0x23, "Documentary" }
 , { 0x24, "Discussion/Interview/Debate" }

 // show/game show
 , { 0x31, "Game show/Quiz/Contest" }
 , { 0x32, "Variety" }
 , { 0x33, "Talk" }

 // sports
 , { 0x41, "Special Event (Olympics/World cup/...)" }
 , { 0x42, "Magazine" }
 , { 0x43, "Football/Soccer" }
 , { 0x44, "Tennis/Squash" }
 , { 0x45, "Team sports (excluding football)" }
 , { 0x46, "Athletics" }
 , { 0x47, "Motor Sport" }
 , { 0x48, "Water Sport" }
 , { 0x49, "Winter Sports" }
 , { 0x4a, "Equestrian" }
 , { 0x4b, "Martial sports" }

 // childrens/youth
 , { 0x51, "Pre-school" }
 , { 0x52, "Entertainment (6 to 14 year-olds)" }
 , { 0x53, "Entertainment (10 to 16 year-olds)" }
 , { 0x54, "Informational/Educational/Schools" }
 , { 0x55, "Cartoons/Puppets" }

 // music/ballet/dance
 , { 0x61, "Rock/Pop" }
 , { 0x62, "Serious music/Classical Music" }
 , { 0x63, "Folk/Traditional music" }
 , { 0x64, "Jazz" }
 , { 0x65, "Musical/Opera" }
 , { 0x66, "Ballet" }

 // arts/culture
 , { 0x71, "Performing Arts" }
 , { 0x72, "Fine Arts" }
 , { 0x73, "Religion" }
 , { 0x74, "Popular Culture/Tradital Arts" }
 , { 0x75, "Literature" }
 , { 0x76, "Film/Cinema" }
 , { 0x77, "Experimental Film/Video" }
 , { 0x78, "Broadcasting/Press" }
 , { 0x79, "New Media" }
 , { 0x7a, "Magazine" }
 , { 0x7b, "Fashion" }

 // social/political/economic
 , { 0x81, "Magazine/Report/Documentary" }
 , { 0x82, "Economics/Social Advisory" }
 , { 0x83, "Remarkable People" }

 // children's youth: educational/science/factual
 , { 0x91, "Nature/Animals/Environment" }
 , { 0x92, "Technology/Natural sciences" }
 , { 0x93, "Medicine/Physiology/Psychology" }
 , { 0x94, "Foreign Countries/Expeditions" }
 , { 0x95, "Social/Spiritual Sciences" }
 , { 0x96, "Further Education" }
 , { 0x97, "Languages" }

 // leisure hobbies
 , { 0xa1, "Tourism/Travel" }
 , { 0xa2, "Handicraft" }
 , { 0xa3, "Motoring" }
 , { 0xa4, "Fitness & Health" }
 , { 0xa5, "Cooking" }
 , { 0xa6, "Advertisement/Shopping" }
 , { 0xa7, "Gardening" }

 // misc
 , { 0xb0, "Original Language" }
 , { 0xb1, "Black and White" }
 , { 0xb2, "Unpublished" }
 , { 0xb3, "Live Broadcast" }
 };
 */

