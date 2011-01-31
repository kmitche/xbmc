#pragma once

#include "StdString.h"

// TODO: Rename to Program.
struct SEpg
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

  SEpg() {
    start         = 0;
    end           = 0;
    genre_type    = 0;
    genre_subtype = 0;

    chanid        = 0;
    channum       = 0;
  }
};
