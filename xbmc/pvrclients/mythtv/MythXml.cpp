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
  time_t now;
  time(&now);

  tm * local;
  local = localtime(&now);
  return local->tm_gmtoff;
}

time_t toLocalTime(time_t* gmt)
{
  return *gmt + GetGMTOffset();
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

  const vector<SChannel>& channels = cmd.GetChannels();
  vector<SChannel>::const_iterator it;
  PVR_CHANNEL pvrchannel;
  for (it = channels.begin(); it != channels.end(); ++it)
  {
    const SChannel& mythchannel = *it;
    memset(&pvrchannel, 0, sizeof(pvrchannel));
    pvrchannel.uid           = mythchannel.id;
    pvrchannel.number        = mythchannel.number;
    pvrchannel.name          = mythchannel.name.c_str();
    pvrchannel.callsign      = mythchannel.callsign.c_str();
    pvrchannel.radio         = false; // TODO: Don't hardcode this. Must be pulled out of Myth if possible.
    pvrchannel.input_format  = "";
    pvrchannel.stream_url    = GetLiveTvPath(mythchannel).c_str();
    /*
     * TODO: Determine how to hide channels in XBMC based on MythTV configuration. Some users have hundreds of channels.
     * Pretty sure the old myth:// code only showed channels with a number > 0.
     */
    pvrchannel.hide = false;
    pvrchannel.recording = false; // TODO: pull out of XML somehow. Perhaps using the ProgramGuideResponse.
    PVR->TransferChannelEntry(handle, &pvrchannel);
  }
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR MythXml::requestEPGForChannel(PVRHANDLE handle, const PVR_CHANNEL &channel, time_t start, time_t end)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;

  GetProgramGuideCommand cmd(channel.uid, start, end);
  if (!ExecuteCommand(cmd))
    return PVR_ERROR_UNKOWN;

  PVR_PROGINFO guideItem;
  const vector<SEpg>& epgInfo = cmd.GetEpg();
  vector<SEpg>::const_iterator it;

  int i = 0;
  for (it = epgInfo.begin(); it != epgInfo.end(); ++it)
  {
    const SEpg& epg = *it;
    guideItem.channum = epg.chan_num;
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

  const vector<SRecording> recordings = cmd.GetRecordings();
  vector<SRecording>::const_iterator it;
  CStdString urlPrefix;
  urlPrefix.Format("http://%s:%i", hostname_, port_);
  int gmtoffset = GetGMTOffset();

  int i= 0;
  for (it = recordings.begin(); it != recordings.end(); ++it)
  {
    const SRecording& recording = *it;
    PVR_RECORDINGINFO pvrinfo;
    pvrinfo.index = i++; // TODO: Hopefully this can be removed from the API.
    pvrinfo.channel_name = recording.callsign;
    pvrinfo.description = recording.description;
    pvrinfo.duration = recording.duration;
    pvrinfo.priority = recording.priority;
    pvrinfo.recording_time = recording.recstart;
    pvrinfo.title = recording.title;
    pvrinfo.subtitle = recording.subtitle;

    CStdString url = urlPrefix + recording.stream_url;
    pvrinfo.stream_url = url;
    pvrinfo.directory = "";
    pvrinfo.lifetime = 0;
    // tag.lifetime = 0; No lifetime for recordings.

    PVR->TransferRecordingEntry(handle, &pvrinfo);
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

CStdString MythXml::GetLiveTvPath(const SChannel &channel)
{
  /*
   * Use the existing LiveTV functionality within XBMC until libcmyth has been moved to the PVR addon.
   */
  CStdString path;
  path.Format("myth://%s/channels/%i.ts", hostname_.c_str(), channel.number);
  return path;
}
