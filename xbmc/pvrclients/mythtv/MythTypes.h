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

/*
 * The enumeration for the types is defined at http://www.mythtv.org/wiki/Record_table
 */
enum MythScheduleType
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
  MythScheduleType type;
  int             recordid;
  bool            inactive;
  CStdString      title;
  CStdString      subtitle;
  CStdString      description;
  CStdString      category;
  CStdString      storagegroup;
  int             chanid;
  int             channum;
  time_t          start;
  time_t          end;

  int             startoffset;    /* minutes */
  int             endoffset;      /* minutes */

  int             priority;

  MythSchedule()
  {
    recordid    = -1;
    inactive    = false;
    chanid      = 0;
    channum     = 0;
    start       = 0;
    end         = 0;

    startoffset = 0;
    endoffset   = 0;

    priority    = 0;
    type        = kNotRecording;
  }
};
