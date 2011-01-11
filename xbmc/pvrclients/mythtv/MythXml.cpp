/*
 * MythXml.cpp
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "MythXml.h"

#include <time.h>

#include "FileSystem/FileCurl.h"

#include "libmythxml/GetBackendTimeCommand.h"
#include "libmythxml/GetBackendVersionCommand.h"
#include "libmythxml/GetChannelListCommand.h"
#include "libmythxml/GetDriveSpaceCommand.h"
#include "libmythxml/GetNumOfChannelsCommand.h"
#include "libmythxml/GetNumOfRecordingsCommand.h"
#include "libmythxml/GetProgramGuideCommand.h"
#include "libmythxml/GetRecordedCommand.h"

using std::vector;
using XFILE::CFileCurl;

/*
 * TODO: Move MythXML to the libmythxml folder and take out the PVR struct information.
 *
 * client.cpp needs to do the conversion between the PVR structures and the structures defined in
 * libmythxml.
 */

int GetGMTOffset()
{
  time_t rawtime;
  tm * localTimeinfo;
  time(&rawtime);
  localTimeinfo = localtime(&rawtime);
  return localTimeinfo->tm_gmtoff;
}

time_t toLocalTime(time_t* gmtTime)
{
  return *gmtTime + GetGMTOffset();
}

MythXml::MythXml(CStdString& hostname, int port, CStdString& user, CStdString& pass, int pin, long timeout)
{
  hostname_ = hostname;
  port_ = port;
  username_ = user;
  password_ = pass;
  pin_ = pin;
  timeout_ = timeout;
}

MythXml::~MythXml()
{
}

void MythXml::Init()
{
}

void MythXml::Cleanup()
{
}

CStdString MythXml::GetBackendName()
{
  return hostname_;
}
CStdString MythXml::GetBackendVersion()
{
  if (!checkConnection())
    return "";

  GetBackendVersionCommand cmd;
  if (ExecuteCommand(cmd))
    return cmd.GetVersion();

  return "";
}

CStdString MythXml::GetConnectionString()
{
  CStdString res;
  res.Format("http://%s:%i", hostname_.c_str(), port_);
  return res;
}

int MythXml::getNumChannels()
{
  if (!checkConnection())
    return 0;
  GetNumOfChannelsCommand cmd;
  if (ExecuteCommand(cmd))
    return cmd.GetNumberOfChannels();
  return 0;
}

PVR_ERROR MythXml::GetDriveSpace(long long *total, long long *used)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;
  GetDriveSpaceCommand cmd;
  if (ExecuteCommand(cmd))
  {
    *total = cmd.GetTotal();
    *used = cmd.GetUsed();
    return PVR_ERROR_NO_ERROR;
  }
  return PVR_ERROR_UNKOWN;
}

PVR_ERROR MythXml::getBackendTime(time_t *localTime, int *gmtOffset)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;
  GetBackendTimeCommand cmd;
  if (ExecuteCommand(cmd))
  {
    *localTime = cmd.GetDateTime();
    *gmtOffset = cmd.GetGmtOffset();
    return PVR_ERROR_NO_ERROR;
  }
  return PVR_ERROR_UNKOWN;
  /*
   return result.getVersion();
   return "";
   tm * localTimeinfo;
   time (localTime );

   localTimeinfo = localtime ( localTime );
   *gmtOffset = localTimeinfo->tm_gmtoff;
   */
}

PVR_ERROR MythXml::requestChannelList(PVRHANDLE handle, int radio)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;
  GetChannelListCommand cmd;
  if (!ExecuteCommand(cmd))
    return PVR_ERROR_UNKOWN;

  const vector<SChannel>& channellist = cmd.GetChannels();
  vector<SChannel>::const_iterator it;
  PVR_CHANNEL tag;
  for (it = channellist.begin(); it != channellist.end(); ++it)
  {
    const SChannel& channel = *it;
    memset(&tag, 0, sizeof(tag));
    tag.uid = channel.id;
    tag.number = channel.id;
    tag.name = channel.name.c_str();
    tag.callsign = channel.callsign.c_str();
    tag.radio = false;
    tag.input_format = "";
    tag.stream_url = "";
    tag.bouquet = 0;
    PVR->TransferChannelEntry(handle, &tag);
  }
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR MythXml::requestEPGForChannel(PVRHANDLE handle, const PVR_CHANNEL &channel, time_t start, time_t end)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;

  int gmtoffset = GetGMTOffset();
  GetProgramGuideCommand cmd(channel.uid, start + gmtoffset, end + gmtoffset);
  if (!ExecuteCommand(cmd))
    return PVR_ERROR_UNKOWN;

  PVR_PROGINFO guideItem;
  const vector<SEpg>& epgInfo = cmd.GetEpg();
  vector<SEpg>::const_iterator it;

  for (it = epgInfo.begin(); it != epgInfo.end(); ++it)
  {
    const SEpg& epg = *it;
    guideItem.channum = epg.chan_num;
    guideItem.uid = epg.id;
    guideItem.title = epg.title;
    guideItem.subtitle = epg.subtitle;
    guideItem.description = epg.description;
    guideItem.genre_type = epg.genre_type;
    guideItem.genre_sub_type = epg.genre_subtype;
    guideItem.parental_rating = epg.parental_rating;
    guideItem.starttime = epg.start_time;
    guideItem.endtime = epg.end_time;
    PVR->TransferEpgEntry(handle, &guideItem);
  }
  return PVR_ERROR_NO_ERROR;
}

int MythXml::getNumRecordings()
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;

  GetNumOfRecordingsCommand cmd;
  if (!ExecuteCommand(cmd))
    return 0;
  return cmd.GetNumberOfRecordings();
}

PVR_ERROR MythXml::requestRecordingsList(PVRHANDLE handle)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;

  GetRecordedCommand cmd;
  if (!ExecuteCommand(cmd))
    return PVR_ERROR_UNKOWN;

  const vector<SRecordingInfo> recordings = cmd.GetRecordings();
  vector<SRecordingInfo>::const_iterator it;
  CStdString urlPrefix;
  urlPrefix.Format("http://%s:%i", hostname_, port_);
  int gmtoffset = GetGMTOffset();

  for (it = recordings.begin(); it != recordings.end(); ++it)
  {
    const SRecordingInfo& recording = *it;
    PVR_RECORDINGINFO tag;

    tag.index = recording.index;
    tag.channel_name = recording.channel_name;
    tag.description = recording.description;
    tag.duration = recording.duration;
    tag.priority = recording.priority;
    tag.recording_time = recording.recording_time + gmtoffset;
    tag.title = recording.title;
    tag.subtitle = recording.subtitle;
    tag.stream_url = urlPrefix + recording.stream_url;

    PVR->TransferRecordingEntry(handle, &tag);
  }
  return PVR_ERROR_NO_ERROR;
}

bool MythXml::ExecuteCommand(MythXmlCommand& command)
{
  CStdString url;
  url.Format("http://%s:%i/%s", hostname_.c_str(), port_, command.GetPath().c_str());

  CStdString xml;

  CFileCurl http;
  http.SetTimeout(timeout_);
  if (!http.Get(url, xml))
  {
    XBMC->Log(LOG_ERROR, "MythXml - Could not retrieve data for : %s", url.c_str());
    http.Cancel();
    return false;
  }
  /*
   * TODO: Parse the response and check for any error state. Then pass the TiXmlHandle through
   * to the ParseResponse method for the Command.
   */
  bool ret = command.ParseResponse(xml);
  http.Cancel();
  return ret;
}

bool MythXml::checkConnection()
{
  return true; // TODO: Can this ever do anything?
}
