#pragma once

#include "MythXmlCommand.h"

#include <vector>

#include "../MythTypes.h"

class GetChannelListCommand: public MythXmlCommand
{
  public:
    GetChannelListCommand();
    ~GetChannelListCommand();

    const std::vector<MythChannel>& GetChannels();

    bool ParseResponse(const TiXmlHandle& handle);

  private:
    std::vector<MythChannel> m_channels;
};
