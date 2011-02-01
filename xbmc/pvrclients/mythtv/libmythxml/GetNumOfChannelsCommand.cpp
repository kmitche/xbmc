/*
 * GetNumChannelsResult.cpp
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "GetNumOfChannelsCommand.h"

#include "MythXmlResponse.h"

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

bool GetNumOfChannelsCommand::ParseResponse(const TiXmlHandle& handle)
{
  TiXmlElement* numOfChannelsNode = handle.FirstChild("GetProgramGuideResponse").FirstChildElement("NumOfChannels").ToElement();
  if (numOfChannelsNode == NULL)
    return false;

  m_number_of_channels = MythXmlResponse::toInteger(numOfChannelsNode);
  return true;
}

int GetNumOfChannelsCommand::GetNumberOfChannels()
{
  return m_number_of_channels;
}
