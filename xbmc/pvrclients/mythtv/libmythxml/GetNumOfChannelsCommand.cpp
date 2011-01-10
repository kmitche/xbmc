/*
 * GetNumChannelsResult.cpp
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "GetNumOfChannelsCommand.h"

#include "MythXmlResponse.h"

// TODO: Same as for all classes...
#include "tinyXML/tinyxml.h"
#include "../client.h"

GetNumOfChannelsCommand::GetNumOfChannelsCommand()
{
  MythXmlParameters parameters;
  /*
   * Setting the start and end time to the current time lists all the programs that are on now.
   *
   * The number of channels is returned directly in an element in the response.
   */
  time_t now = time(NULL);
  parameters.SetParameter("StartTime", now);
  parameters.SetParameter("EndTime", now);
  parameters.SetParameter("NumOfChannels", -1); // All channels

  Init("Myth/GetProgramGuide", parameters);
}

GetNumOfChannelsCommand::~GetNumOfChannelsCommand()
{
}

bool GetNumOfChannelsCommand::ParseResponse(CStdString response)
{
  m_number_of_channels = 0;
  // TODO: I thought that all of the MythXML responses were in UTF8?
  TiXmlDocument xml;
  xml.Parse(response.c_str(), 0, TIXML_ENCODING_UTF8);

  TiXmlHandle docHandle(&xml);

  TiXmlElement* child = docHandle.FirstChildElement("detail").ToElement();
  if (child != NULL)
  {
    // this is the error reponse, process it
    int errorCode;
    CStdString errorDesc;
    MythXmlResponse::parseErrorNode(child, errorCode, errorDesc);
    //XBMC->Log(LOG_ERROR, "MythXML - GetNumChannelsResult - ErrorCode [%i] - %s", errorCode, errorDesc.c_str());
    return false;
  }

  child = docHandle.FirstChild("GetProgramGuideResponse").FirstChildElement("NumOfChannels").ToElement();
  if (child != NULL)
  {
    m_number_of_channels = MythXmlResponse::toInteger(child);
    return true;
  }

  //XBMC->Log(LOG_ERROR, "MythXML - GetNumChannelsResult - xml data doesn't have the expected information - %s",
  //    response.c_str());
  return false;
}

int GetNumOfChannelsCommand::GetNumberOfChannels()
{
  return m_number_of_channels;
}
