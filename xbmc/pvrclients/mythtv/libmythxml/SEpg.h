#pragma once

#include "StdString.h"

struct SEpg
{
  int          id;
  int          chan_num;
  int          genre_type;
  int          genre_subtype;
  int          parental_rating;
  CStdString   title;
  CStdString   subtitle;
  CStdString   description;
  time_t       start_time;
  time_t       end_time;
  
  SEpg() {
    id              = 0;
    chan_num        = 0;
    genre_type      = 0;
    genre_subtype   = 0;
    parental_rating = 0;
  }
};
