#pragma once

#include "../StdString.h"

// TODO: Rename to just Recording or RecordedProgram. Move to the top level MythXML files when they are moved into this tree.
struct SRecordingInfo
{
  int          index;
  int          duration;
  int          priority;
  CStdString   title;
  CStdString   subtitle;
  CStdString   description;
  CStdString   channel_name;
  CStdString   stream_url;
  time_t       recording_time;
  
  SRecordingInfo() {
    index           = -1;
    duration        = 0;
    priority      	= 0;
    recording_time 	= 0; 
  }
};
