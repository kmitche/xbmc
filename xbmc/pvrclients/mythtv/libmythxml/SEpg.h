#pragma once

#include "StdString.h"

// TODO: Rename to Program.
struct SEpg
{
  int         uid; // TODO: rename to chanid.
  int         chan_num; // TODO: rename to channum.
  CStdString  title;
  CStdString  subtitle;
  CStdString  description;
  time_t      start_time; // TODO: rename to start
  time_t      end_time; // TODO: rename to end
  int         genre_type; // TODO: use category and do the mapping to genre in the client interface.
  int         genre_subtype;
  int         parental_rating; // TODO: remove as not available through MythXML.

  SEpg() {
    chan_num        = 0;
    genre_type      = 0;
    genre_subtype   = 0;
    parental_rating = 0;
  }
};
