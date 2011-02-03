/*
 * MythXml.cpp
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "MythXml.h"

#include <time.h>

#include "tinyxml.h"
#include "filesystem/FileCurl.h"

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
    pvrchannel.uid           = mythchannel.chanid;
    pvrchannel.number        = mythchannel.channum;
    pvrchannel.name          = mythchannel.channame.c_str();
    pvrchannel.callsign      = mythchannel.callsign.c_str();
    pvrchannel.radio         = false; // TODO: Don't hardcode this. Must be pulled out of Myth if possible.
    pvrchannel.input_format  = "";
    pvrchannel.stream_url    = GetLiveTvPath(mythchannel).c_str();

    CStdString icon;
    icon.Format("%s/Myth/GetChannelIcon?ChanId=%i", GetUrlPrefix().c_str(), mythchannel.chanid);
    pvrchannel.iconpath = icon;

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

  const vector<SProgram>& epg = cmd.GetEpg();
  vector<SProgram>::const_iterator it;

  for (it = epg.begin(); it != epg.end(); ++it)
  {
    PVR_PROGINFO proginfo;
    const SProgram& program = *it;

    proginfo.title          = program.title;
    proginfo.subtitle       = program.subtitle;
    proginfo.description    = program.description;
    proginfo.genre_type     = program.genre_type;
    proginfo.genre_sub_type = program.genre_subtype;

    proginfo.starttime      = program.start;
    proginfo.endtime        = program.end;

    proginfo.channum        = program.channum;

    PVR->TransferEpgEntry(handle, &proginfo);
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

  int i= 0;
  for (it = recordings.begin(); it != recordings.end(); ++it)
  {
    PVR_RECORDINGINFO recordinginfo;
    const SRecording& recording = *it;

    recordinginfo.index           = i++; // TODO: Hopefully this can be removed from the API.

    recordinginfo.title           = recording.title;
    recordinginfo.subtitle        = recording.subtitle;
    recordinginfo.description     = recording.description;
    recordinginfo.recording_time  = recording.recstart;
    recordinginfo.duration        = recording.recend - recording.recstart;

    recordinginfo.channel_name    = recording.channame;

    recordinginfo.directory       = ""; // TODO: put in directory structure to support TV Shows and Movies ala myth://
    // CStdString url                = GetUrlPrefix() + recording.url;
    CStdString url                = GetRecordingPath(recording);
    recordinginfo.stream_url      = url;

    recordinginfo.priority        = recording.priority;
    recordinginfo.lifetime        = 0; // TODO: Map the lifetime in MythTV to the lifetime supported in XBMC.

    PVR->TransferRecordingEntry(handle, &recordinginfo);
  }
  return PVR_ERROR_NO_ERROR;
}

bool MythXml::ExecuteCommand(MythXmlCommand& command)
{
  CStdString url;
  url.Format("%s/%s", GetUrlPrefix().c_str(), command.GetPath().c_str());

  CFileCurl http;
  http.SetTimeout(timeout_);

  CStdString response;
  if (!http.Get(url, response))
  {
    XBMC->Log(LOG_ERROR, "MythXml - Could not retrieve data for : %s", url.c_str());
    http.Cancel();
    return false;
  }
  TiXmlDocument xml;
  xml.Parse(response.c_str());

  TiXmlHandle handle(&xml);

  /*
   * Check for the existence of a <detail> node. If it exists it means there was an error. It will
   * contain information about the error.
   */
  TiXmlElement* detailnode = handle.FirstChildElement("detail").ToElement();
  if (detailnode != NULL)
  {
    int code = atoi(detailnode->FirstChildElement("errorCode")->GetText());
    CStdString description = detailnode->FirstChildElement("errorDescription")->GetText();
    XBMC->Log(LOG_ERROR, "%s - Error response received from MythXML command: [%i] - %s",
              __FUNCTION__, code, description.c_str());
    return false;
  }

  bool ret = command.ParseResponse(handle);
  http.Cancel();
  return ret;
}

bool MythXml::checkConnection()
{
  return true; // TODO: Can this ever do anything?
}

CStdString MythXml::GetUrlPrefix()
{
    CStdString urlPrefix;
    urlPrefix.Format("http://%s:%i", hostname_, port_);
    return urlPrefix;
}

CStdString MythXml::GetLiveTvPath(const SChannel &channel)
{
  /*
   * HACK: Use the existing LiveTV functionality within XBMC until libcmyth has been moved to the PVR addon.
   */
  CStdString path;
  path.Format("myth://%s/channels/%i.ts", hostname_.c_str(), channel.channum);
  return path;
}

CStdString MythXml::GetRecordingPath(const SRecording &recording)
{
  /*
   * HACK: The MythXML GetRecorded interface doesn't support seeking so use the existing myth://
   * functionality within XBMC until libcmyth has been moved to the PVR addon.
   */
  struct tm * recstart;
  recstart = localtime(&recording.recstart);

  /*
   * The format for the pathinfo for a mythbackend recording is:
   * <chanid>_<recstart - yyyymmddHHMMSS>.mpg
   */
  char pathinfo[16];
  strftime(pathinfo, 16, "%Y%m%d%H%M%S", recstart);

  CStdString path;
  path.Format("myth://%s/recordings/%i_%s.mpg", hostname_.c_str(), recording.chanid, pathinfo);
  return path;
}
