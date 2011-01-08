#pragma once

#include "MythXmlCommand.h"

#include <vector>

#include "SChannel.h"

class GetChannelListCommand: public MythXmlCommand
{
  public:
    GetChannelListCommand();
    const std::vector<SChannel>& GetChannels();

    bool ParseResponse(const CStdString& reponse);

  private:
    std::vector<SChannel> m_channels;
};
