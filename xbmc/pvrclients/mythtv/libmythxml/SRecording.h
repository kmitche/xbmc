#pragma once

#include "StdString.h"

struct SRecording
{
  CStdString  title;
  CStdString  subtitle;
  CStdString  description;
  CStdString  category;
  time_t      start;
  time_t      end;

  int         chanid;
  int         channum;
  CStdString  callsign;

  int         priority;

  time_t      recstart;
  time_t      recend;

  CStdString  recgroup; // e.g. LiveTV

  CStdString  url;

  SRecording() {
    start     = 0;
    end       = 0;

    chanid    = 0;
    channum   = 0;

    priority  = 0;

    recstart 	= 0;
    recend    = 0;
  }
};
