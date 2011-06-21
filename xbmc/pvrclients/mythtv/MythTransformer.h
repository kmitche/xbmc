#pragma once

#include "MythTypes.h"

class MythTransformer {

public:
  MythTransformer();
  virtual ~MythTransformer();

private:
  void SetSeasonAndEpisode(const MythRecording &recording, int *season, int *episode);

};
