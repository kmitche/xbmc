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

using namespace XFILE;

MythXml::MythXml() {
	hostname_ = "";
	port_ = -1;
	pin_ = -1;
	timeout_ = -1;
}

MythXml::~MythXml() {
}

void MythXml::init(){
}

void MythXml::cleanup(){
}

bool MythXml::open(CStdString hostname, int port, CStdString user, CStdString pass, int pin, long timeout){
	hostname_ = hostname;
	port_ = port;
	timeout_ = timeout;
	pin_ = pin;
	CStdString strUrl;
	strUrl.Format("http://%s:%i/Myth/GetConnectionInfo?Pin=%i", hostname.c_str(), port, pin);
	CStdString strXML;

	CFileCurl http;

	http.SetTimeout(timeout);
	if(!http.Get(strUrl, strXML)){
	  XBMC->Log(LOG_DEBUG, "MythXml - Could not open connection to mythtv backend.");
		http.Cancel();
		return false;
	}
	http.Cancel();
	return true;
}

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
  return PVR_ERROR_UNKNOWN;
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
  return PVR_ERROR_UNKNOWN;
  /*
   return result.getVersion();
   return "";
   tm * localTimeinfo;
   time (localTime );

   localTimeinfo = localtime ( localTime );
   *gmtOffset = localTimeinfo->tm_gmtoff;
   */
}

PVR_ERROR MythXml::requestChannelList(PVR_HANDLE handle, int radio)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;

  GetChannelListCommand cmd;
  if (!ExecuteCommand(cmd))
    return PVR_ERROR_UNKNOWN;

  const vector<SChannel>& channels = cmd.GetChannels();
  vector<SChannel>::const_iterator it;

  for (it = channels.begin(); it != channels.end(); ++it)
  {
    PVR_CHANNEL pvrchannel;
    const SChannel& mythchannel = *it;

    pvrchannel.iUniqueId      = mythchannel.chanid;
    pvrchannel.iChannelNumber = mythchannel.channum;
    pvrchannel.strChannelName = mythchannel.channame;
    pvrchannel.bIsRadio       = false; // TODO: Don't hardcode this. Must be pulled out of Myth if possible.
    pvrchannel.strInputFormat = "";

    CStdString url            = GetLiveTvPath(mythchannel);
    pvrchannel.strStreamURL   = url;

    CStdString icon           = GetChannelIconPath(mythchannel);
    pvrchannel.strIconPath    = icon;

    /*
     * TODO: Determine how to hide channels in XBMC based on MythTV configuration. Some users have hundreds of channels.
     * Pretty sure the old myth:// code only showed channels with a number > 0.
     */
    pvrchannel.bIsHidden      = false;

    /*
     * Set the remaining parts that are not used by MythTV.
     */
    pvrchannel.iEncryptionSystem = 0;

    PVR->TransferChannelEntry(handle, &pvrchannel);
  }
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR MythXml::requestEPGForChannel(PVR_HANDLE handle, const PVR_CHANNEL &channel, time_t start, time_t end)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;

  GetProgramGuideCommand cmd(channel.iUniqueId, start, end);
  if (!ExecuteCommand(cmd))
    return PVR_ERROR_UNKNOWN;

  const vector<SProgram>& epg = cmd.GetEpg();
  vector<SProgram>::const_iterator it;

  int i = 0;
  for (it = epg.begin(); it != epg.end(); ++it)
  {
    EPG_TAG epgtag;
    const SProgram& program = *it;

    epgtag.iUniqueBroadcastId = i++; // GetRecordingId(program.chanid, program.start);
    epgtag.strTitle       = program.title;
    epgtag.strPlotOutline = program.subtitle;
    epgtag.strPlot        = program.description;
    epgtag.iGenreType     = program.genre_type;
    epgtag.iGenreSubType  = program.genre_subtype;

    epgtag.startTime      = program.start;
    epgtag.endTime        = program.end;

    epgtag.iChannelNumber = program.channum;

//    epgtag.firstAired;
//    epgtag.iEpisodeNumber;
//    epgtag.iEpisodePartNumber;
//    epgtag.iParentalRating;
//    epgtag.iSeriesNumber;
//    epgtag.iStarRating;
//    epgtag.iUniqueBroadcastId;
    epgtag.strEpisodeName  = ""; // HACK: To prevent Segmentation Fault.
    epgtag.strIconPath     = ""; // HACK: To prevent Segmentation Fault.

    PVR->TransferEpgEntry(handle, &epgtag);
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

PVR_ERROR MythXml::requestRecordingsList(PVR_HANDLE handle)
{
  if (!checkConnection())
    return PVR_ERROR_SERVER_ERROR;

  GetRecordedCommand cmd;
  if (!ExecuteCommand(cmd))
    return PVR_ERROR_UNKNOWN;

  const vector<SRecording> recordings = cmd.GetRecordings();
  vector<SRecording>::const_iterator it;

  int i = 0;
  for (it = recordings.begin(); it != recordings.end(); ++it)
  {
    PVR_RECORDING pvrrecording;
    const SRecording& mythrecording = *it;

    pvrrecording.iClientIndex    = i++;
    pvrrecording.strTitle        = mythrecording.title;
    pvrrecording.strPlotOutline  = mythrecording.subtitle;
    pvrrecording.strPlot         = mythrecording.description;
    pvrrecording.recordingTime   = mythrecording.recstart;
    pvrrecording.iDuration       = mythrecording.recend - mythrecording.recstart;

    pvrrecording.strChannelName  = mythrecording.channame;

    pvrrecording.strDirectory    = ""; // TODO: put in directory structure to support TV Shows and Movies ala myth://
    CStdString url               = GetUrlPrefix() + mythrecording.url;
    // CStdString url                = GetRecordingPath(recording);
    pvrrecording.strStreamURL    = url;

    pvrrecording.iPriority       = mythrecording.priority;
    pvrrecording.iLifetime       = 0; // TODO: Map the lifetime in MythTV to the lifetime supported in XBMC.

//    pvrrecording.iGenreSubType;
//    pvrrecording.iGenreType;

    PVR->TransferRecordingEntry(handle, &pvrrecording);
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

CStdString MythXml::GetChannelIconPath(const SChannel &channel)
{
  CStdString icon;
  icon.Format("%s/Myth/GetChannelIcon?ChanId=%i", GetUrlPrefix().c_str(), channel.chanid);
  return icon;
}

CStdString MythXml::GetRecordingId(const int chanid, const time_t start)
{
  char buffer[16];
  tm *local = localtime(&start);
  strftime(buffer, 16, "%Y%m%d%H%M%S", local); // YYYYmmddHHMMSS

  CStdString recording;
  recording.Format("%i_%s", chanid, buffer);  // e.g. 1244_20110526192800
  return recording;
}

