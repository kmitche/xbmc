#pragma once

/*
 * MythXml.h
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "libmythxml/MythXmlCommand.h"
#include "libmythxml/MythXmlResponse.h"
#include "MythTypes.h"
#include "client.h"

/*! \class MythXml
  \brief Acts as the glue between the PVR Addon world and the mythXML world.
 */
class MythXml {
public:
	MythXml();
	virtual ~MythXml();
	void init();
	void cleanup();
	bool open(CStdString hostname, int port, CStdString user, CStdString pass, int pin, long timeout);
  int getNumChannels();
  PVR_ERROR requestChannelList(PVR_HANDLE handle, int radio);
  PVR_ERROR requestEPGForChannel(PVR_HANDLE handle, const PVR_CHANNEL &channel, time_t start, time_t end);
  int getNumRecordings();
  PVR_ERROR requestRecordingsList(PVR_HANDLE handle);
  PVR_ERROR getBackendTime(time_t *localTime, int *gmtOffset);
  CStdString GetBackendName();
  CStdString GetBackendVersion();
  CStdString GetConnectionString();
  PVR_ERROR GetDriveSpace(long long *total, long long *used);

private:
  bool ExecuteCommand(MythXmlCommand& command);
  bool checkConnection();
  CStdString GetRecordingPath(const MythRecording &recording);
  CStdString GetLiveTvPath(const MythChannel &channel);
  CStdString GetChannelIconPath(const MythChannel &channel);
  CStdString GetUrlPrefix();
  CStdString GetRecordingId(const int chanid, const time_t start);

  CStdString hostname_;
  CStdString username_;
  CStdString password_;
  int port_;
  int timeout_;
  int pin_;
};
