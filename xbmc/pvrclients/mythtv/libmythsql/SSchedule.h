#pragma once
#include "../StdString.h"

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
    m_id          = -1;
    m_active      = 0;
    m_channel     = 0;
    m_starttime   = 0;
    m_endtime     = 0;
    m_firstday    = 0;
    m_recording   = 0;
    m_priority    = 0;
    m_lifetime    = 0;
    m_repeat      = 0;
    m_repeatflags = kAll;
    m_recType     = kNotRecording;
  }
  
  int             m_id;
  int             m_active;
  CStdString      m_title;
  CStdString      m_directory;
  int             m_channel;
  time_t          m_starttime;
  time_t          m_endtime;
  time_t          m_firstday;
  int             m_recording;
  int             m_priority;
  int             m_lifetime;
  int             m_repeat;
  RepeatFlags     m_repeatflags;
  RecordingTypes  m_recType;
};
