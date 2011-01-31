#ifndef XBMC_PVRCLIENTS_MYTHTV_LIBMYTHXML_SCHANNEL_H_
#define XBMC_PVRCLIENTS_MYTHTV_LIBMYTHXML_SCHANNEL_H_

#include "../StdString.h"

struct SChannel
{
  int         chanid;
  int         channum;
  CStdString  channame;
  CStdString  callsign;

  SChannel() {
    chanid  = 0;
    channum = 0;
  }
};

#endif /* XBMC_PVRCLIENTS_MYTHTV_LIBMYTHXML_SCHANNEL_H_ */
