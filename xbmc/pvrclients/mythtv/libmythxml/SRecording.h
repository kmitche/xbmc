#pragma once

#include "StdString.h"

struct SRecording
{
  int          duration;
  int          priority;
  CStdString   title;
  CStdString   subtitle;
  CStdString   description;
  CStdString   callsign;
  CStdString   stream_url;
  time_t       recstart;
  // TODO: Add chanid.
  // TODO: Add recend.

  SRecording() {
    duration        = 0;
    priority      	= 0;
    recstart 	= 0; 
  }
};
