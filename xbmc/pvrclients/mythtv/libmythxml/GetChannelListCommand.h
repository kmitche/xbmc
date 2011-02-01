#pragma once

#include "MythXmlCommand.h"

#include <vector>

#include "SChannel.h"

class GetChannelListCommand: public MythXmlCommand
{
  public:
    GetChannelListCommand();
    ~GetChannelListCommand();

    const std::vector<SChannel>& GetChannels();

    bool ParseResponse(const TiXmlHandle& handle);

  private:
    std::vector<SChannel> m_channels;
};
