#pragma once

#include "MythTypes.h"

class MythTransformer {

public:
  MythTransformer();
  virtual ~MythTransformer();

  static bool IsMovie(const MythRecording &recording);

private:
  void SetSeasonAndEpisode(const MythRecording &recording, int *season, int *episode);

};
