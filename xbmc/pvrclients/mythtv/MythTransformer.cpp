#include "MythTransformer.h"

MythTransformer::MythTransformer() {
}

MythTransformer::~MythTransformer() {
}

bool MythTransformer::IsMovie(const MythRecording &recording)
{
  /*
   * The programid (if it exists) is a combination key where the first 2 characters maps to the
   * category_type and the rest is the key. From MythTV/release-0-21-fixes/mythtv/libs/libmythtv/programinfo.cpp
   * "MV" = movie
   * "EP" = series
   * "SP" = sports
   * "SH" = tvshow
   *
   * The reliability of the programid being filled in depends on the quality of the EPG content.
   */

  CStdString programid = recording.programid;

  /*
   * If we have no useful information, return true if the duration is longer than 90 minutes.
   */
  if (programid.IsEmpty())
  {
    int duration = recording.end - recording.start; // seconds
    return (duration > 90 * 60); // 90 minutes. TODO: should this be configurable?
  }

  if (programid.Left(2) == "MV")
    return true;

  if (programid.Left(2) == "EP"
  ||  programid.Left(2) == "SP"
  ||  programid.Left(2) == "SH")
    return false;

  return false;
}

void MythTransformer::SetSeasonAndEpisode(const MythRecording &program, int *season, int *episode) {
  /*
   * A valid programid generated from an XMLTV source should look like:
   * [EP|MV|SH|SP][seriesid][episode][season]([partnumber][parttotal])
   * mythtv/trunk/programs/mytfilldatabaseline/xmltvparser.cpp - Line 522 onwards.
   *
   * Season changed to a base36 character for XMLTV in Myth 0.24. http://svn.mythtv.org/trac/changeset/24724
   *
   * A valid SchedulesDirect programid appears to have a similar format to the XMLTV programid but
   * doesn't have any obvious way to parse out the season and episode information. The number at the
   * end of the programid could possibly be the completely sequential number for the episode, but
   * even that doesn't seem to match up with TVDB. SchedulesDirect data does seem to have a valid
   * original air date though, so if we identify a SchedulesDirect programid, leave the season and
   * episode as 0.
   */
  CStdString seriesid = program.seriesid;
  CStdString programid = program.programid;

  /*
   * Default the season and episode to 0 so XBMC treats the content as an episode and displays tag
   * information. If the season and episode can be parsed from the programid these will be
   * overwritten.
   */
  *season  = 0;
  *episode = 0;

  if (programid.IsEmpty() // Can't do anything if the program ID is empty
  ||  seriesid.IsEmpty()) // Can't figure out the end parsing if the series ID is empty  {
    return;

  CStdString category = programid.Left(2); // Valid for both XMLTV and SchedulesDirect sources
  if (category != "MV"  // Movie
  &&  category != "EP"  // Series
  &&  category != "SH"  // TV Show
  &&  category != "SP") // Sports
    return;

  if (programid.Mid(category.length(), seriesid.length()) != seriesid) // Series ID does not follow the category
    return;

  CStdString remainder = programid.Mid(category.length() + seriesid.length()); // Whatever is after series ID

  /*
   * All SchedulesDirect remainders appear to be 4 characters and start with a 0. If the assumption
   * is correct that the number somehow relates to the sequential episode number across all seasons
   * then we can ignore remainders that start with 0. It will be very unlikely for a sequential
   * episode number for a series to be > 999.
   */
  if (remainder.length() == 4     // All SchedulesDirect codes seem to be 4 characters
  &&  remainder.Left(0)  == "0")  // Padded with 0's for low number. No valid XMLTV remainder will start with 0.
    return;

  /*
   * If the remainder is more than 5 characters, it must include the optional part number and total
   * number of parts. Strip off the last 2 characters assuming that there are ridiculously few
   * cases where the number of parts for a single episode is > 9.
   */
  if (remainder.length() >= 5) // Must include optional part number and total number of parts
    remainder = remainder.Left(remainder.length() - 2); // Assumes part number and total are both < 10

  /*
   * Now for some heuristic black magic.
   */
  if (remainder.length() == 2)  // Single character season and episode.
  {
    *season = atoi(remainder.Right(1)); // TODO: Fix for base 36 in Myth 0.24. Assume season < 10
    *episode = atoi(remainder.Left(1));
  }
  else if (remainder.length() == 3) // Ambiguous in Myth 0.23. Single character season in Myth 0.24
  {
    /*
     * Following heuristics are intended to work with largest possible number of cases. It won't be
     * perfect, but way better than just assuming the season is < 10.
     */
    if (remainder.Right(1) == "0") // e.g. 610. Unlikely to have a season of 0 (specials) with more than 9 special episodes.
    {
      *season = atoi(remainder.Right(2));
      *episode = atoi(remainder.Left(1));
    }
    else if (remainder.Mid(2, 1) == "0") // e.g. 203. Can't have a season start with 0. Must be end of episode.
    {
      *season = atoi(remainder.Right(1)); // TODO: Fix for base 36 in Myth 0.24. Assume season < 10
      *episode = atoi(remainder.Left(2));
    }
    else if (atoi(remainder.Left(1)) > 3) // e.g. 412. Very unlikely to have more than 39 episodes per season if season > 9.
    {
      /*
       * TODO: See if a check for > 2 is better, e.g. is it still unlikely to have more than 29 episodes
       * per season if season > 9?
       */
      *season = atoi(remainder.Right(2));
      *episode = atoi(remainder.Left(1));
    }
    else // e.g. 129. Assume season is < 10 or Myth 0.24 Base 36 season.
    {
      *season = atoi(remainder.Right(1)); // TODO: Fix for base 36 in Myth 0.24. Assume season < 10
      *episode = atoi(remainder.Left(2));
    }
  }
  else if (remainder.length() == 4) // Double digit season and episode in Myth 0.23 OR TODO: has part number and total number of parts
  {
    *season = atoi(remainder.Right(2));
    *episode = atoi(remainder.Left(2));
  }
  return;
}
