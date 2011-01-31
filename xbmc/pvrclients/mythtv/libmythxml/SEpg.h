#pragma once

#include "StdString.h"

// TODO: Rename to Program.
struct SEpg
{
  int         chanid;
  int         channum;
  CStdString  channame;
  CStdString  callsign;
  CStdString  title;
  CStdString  subtitle;
  CStdString  description;
  time_t      start;
  time_t      end;
  int         genre_type; // TODO: use category and do the mapping to genre in the client interface.
  int         genre_subtype;

  SEpg() {
    chanid          = 0;
    channum        = 0;
    genre_type      = 0;
    genre_subtype   = 0;
  }
};
