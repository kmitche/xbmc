#pragma once

#include "utils/StdString.h"

struct MythChannel
{
  int         chanid;
  int         channum;
  CStdString  channame;
  CStdString  callsign;

  MythChannel() {
    chanid  = 0;
    channum = 0;
  }
};

struct MythProgram
{
  CStdString  title;
  CStdString  subtitle;
  CStdString  description;
  time_t      start;
  time_t      end;

  int         genre_type; // TODO: use category and do the mapping to genre in the client interface.
  int         genre_subtype;

  int         chanid;
  int         channum;
  CStdString  channame;
  CStdString  callsign;

  MythProgram() {
    start         = 0;
    end           = 0;
    genre_type    = 0;
    genre_subtype = 0;

    chanid        = 0;
    channum       = 0;
  }
};

struct MythRecording
{
  CStdString  title;
  CStdString  subtitle;
  CStdString  description;
  CStdString  category;
  time_t      start;
  time_t      end;

  int         chanid;
  int         channum;
  CStdString  channame;
  CStdString  callsign;

  int         priority;

  time_t      recstart;
  time_t      recend;

  CStdString  recgroup; // e.g. LiveTV

  CStdString  url;

  MythRecording() {
    start     = 0;
    end       = 0;

    chanid    = 0;
    channum   = 0;

    priority  = 0;

    recstart  = 0;
    recend    = 0;
  }
};

enum MythRecordingTypes
{
  kNotRecording = 0,
  kSingleRecord = 1,
  kTimeslotRecord,
  kChannelRecord,
  kAllRecord,
  kWeekslotRecord,
  kFindOneRecord,
  kOverrideRecord,
  kDontRecord,
  kFindDailyRecord,
  kFindWeeklyRecord
};

enum MythRepeatFlags
{
  kMonday = 0,
  kTueday = 1,
  kWednesday,
  kThursday,
  kFriday,
  kSaturday,
  kSunday,
  kWeekDays,
  kAllButSunday,
  kAll,
  kWeekend
};

struct MythSchedule
{
  MythSchedule()
  {
    m_recordid    = -1;
    m_inactive    = false;
    m_chanid      = 0;
    m_channum     = 0;
    m_starttime   = 0;
    m_endtime     = 0;
    m_firstday    = 0;
    m_recording   = 0;
    m_priority    = 0;
    m_lifetime    = 0;
    m_repeat      = 0;
    m_repeatflags = kAll;
    m_type        = kNotRecording;
  }

  int             m_recordid;
  bool            m_inactive;
  CStdString      m_title;
  CStdString      m_storagegroup;
  int             m_chanid;
  int             m_channum;
  time_t          m_starttime;
  time_t          m_endtime;
  time_t          m_firstday;
  int             m_recording;
  int             m_priority;
  int             m_lifetime;
  int             m_repeat;
  MythRepeatFlags     m_repeatflags;
  MythRecordingTypes  m_type;
};
