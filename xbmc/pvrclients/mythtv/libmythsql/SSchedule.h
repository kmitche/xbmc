#pragma once

#include "utils/StdString.h"

enum RecordingTypes
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

enum RepeatFlags
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

struct SSchedule
{
  SSchedule()
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
  RepeatFlags     m_repeatflags;
  RecordingTypes  m_type;
};
