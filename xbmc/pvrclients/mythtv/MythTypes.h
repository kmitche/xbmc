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

struct MythSchedule
{
  int             recordid;
  bool            inactive;
  CStdString      title;
  CStdString      storagegroup;
  int             chanid;
  int             channum;
  time_t          start;
  time_t          end;
  time_t          m_firstday;
  int             m_recording;
  int             m_priority;
  int             m_lifetime;
  int             m_repeat;
  MythRecordingTypes  type;

  MythSchedule()
  {
    recordid    = -1;
    inactive    = false;
    chanid      = 0;
    channum     = 0;
    start       = 0;
    end         = 0;
    m_firstday    = 0;
    m_recording   = 0;
    m_priority    = 0;
    m_lifetime    = 0;
    m_repeat      = 0;
    type        = kNotRecording;
  }
};
