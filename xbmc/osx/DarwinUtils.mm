/*
 *      Copyright (C) 2010 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#define BOOL XBMC_BOOL 
#include "system.h"
#include "DllPaths.h"
#include "utils/log.h"
#undef BOOL

#if defined(__APPLE__)
#if defined(__arm__)
  #import <Foundation/Foundation.h>
  #import <UIKit/UIKit.h>
  #import <mach/mach_host.h>
  #import <sys/sysctl.h>
#else
  #import <Cocoa/Cocoa.h>
  #import <IOKit/ps/IOPowerSources.h>
  #import <IOKit/ps/IOPSKeys.h>
#endif

#import "AutoPool.h"
#import "DarwinUtils.h"


bool DarwinIsAppleTV2(void)
{
  static int result = -1;
#if defined(__APPLE__) && defined(__arm__)
  if( result == -1 )
  {
    char        buffer[512];
    size_t      len = 512;
    result = 0;    
    std::string hw_machine = "unknown";

    if (sysctlbyname("hw.machine", &buffer, &len, NULL, 0) == 0)
      hw_machine = buffer;

    if (hw_machine.find("AppleTV2,1") != std::string::npos)
      result = 1;   
  }
#endif
  return (result == 1);
}

float GetIOSVersion(void)
{
  CCocoaAutoPool pool;
  float version;
#if defined(__arm__)
  version = [[[UIDevice currentDevice] systemVersion] floatValue];
#else
  version = 0.0f;
#endif

  return(version);
}

int  GetDarwinFrameworkPath(bool forPython, char* path, uint32_t *pathsize)
{
  CCocoaAutoPool pool;
  // see if we can figure out who we are
  NSString *pathname;

  path[0] = 0;
  *pathsize = 0;

  // a) XBMC frappliance running under ATV2
  Class XBMCfrapp = NSClassFromString(@"XBMCAppliance");
  if (XBMCfrapp != NULL)
  {
    pathname = [[NSBundle bundleForClass:XBMCfrapp] pathForResource:@"Frameworks" ofType:@""];
    strcpy(path, [pathname UTF8String]);
    *pathsize = strlen(path);
    //CLog::Log(LOGDEBUG, "DarwinFrameworkPath(a) -> %s", path);
    return 0;
  }

  // b) XBMC application running under IOS
  pathname = [[NSBundle mainBundle] executablePath];
  if (pathname && strstr([pathname UTF8String], "XBMC.app/XBMC"))
  {
    strcpy(path, [pathname UTF8String]);
    // Move backwards to last "/"
    for (int n=strlen(path)-1; path[n] != '/'; n--)
      path[n] = '\0';
    strcat(path, "Frameworks");
    *pathsize = strlen(path);
    //CLog::Log(LOGDEBUG, "DarwinFrameworkPath(c) -> %s", path);
    return 0;
  }

  // d) XBMC application running under OSX
  pathname = [[NSBundle mainBundle] privateFrameworksPath];
  if (pathname && strstr([pathname UTF8String], "Contents"))
  {
    // check for 'Contents' if we are running as real xbmc.app
    strcpy(path, [pathname UTF8String]);
    *pathsize = strlen(path);
    //CLog::Log(LOGDEBUG, "DarwinFrameworkPath(d) -> %s", path);
    return 0;
  }

  // e) XBMC OSX binary running under xcode or command-line
  // but only if it's not for python. In this case, let python
  // use it's internal compiled paths.
  if (!forPython)
  {
    strcpy(path, PREFIX_USR_PATH);
    strcat(path, "/lib");
    *pathsize = strlen(path);
    //CLog::Log(LOGDEBUG, "DarwinFrameworkPath(e) -> %s", path);
    return 0;
  }

  return -1;
}

int  GetDarwinExecutablePath(char* path, uint32_t *pathsize)
{
  CCocoaAutoPool pool;
  // see if we can figure out who we are
  NSString *pathname;

  // a) XBMC frappliance running under ATV2
  Class XBMCfrapp = NSClassFromString(@"XBMCAppliance");
  if (XBMCfrapp != NULL)
  {
    pathname = [[NSBundle bundleForClass:XBMCfrapp] pathForResource:@"XBMC" ofType:@""];
    strcpy(path, [pathname UTF8String]);
    *pathsize = strlen(path);
    //CLog::Log(LOGDEBUG, "DarwinExecutablePath(a) -> %s", path);
    return 0;
  }

  // b) XBMC application running under IOS
  // c) XBMC application running under OSX
  pathname = [[NSBundle mainBundle] executablePath];
  strcpy(path, [pathname UTF8String]);
  *pathsize = strlen(path);
  //CLog::Log(LOGDEBUG, "DarwinExecutablePath(b/c) -> %s", path);

  return 0;
}

bool DarwinHasVideoToolboxDecoder(void)
{
  static int DecoderAvailable = -1;

  if (DecoderAvailable == -1)
  {
    Class XBMCfrapp = NSClassFromString(@"XBMCAppliance");
    if (XBMCfrapp != NULL)
    {
      // atv2 has seatbelt profile key removed so nothing to do here
      DecoderAvailable = 1;
    }
    else
    {
      /* Get Application directory */
      uint32_t path_size = 2*MAXPATHLEN;
      char     given_path[2*MAXPATHLEN];
      int      result = -1;
      
      memset(given_path, 0x0, path_size);
      result = GetDarwinExecutablePath(given_path, &path_size);
      if (result == 0) 
      {
        /* When XBMC is started from a sandbox directory we have to check the sysctl values */
        if (strlen("/var/mobile/Applications/") < path_size &&
           strncmp(given_path, "/var/mobile/Applications/", strlen("/var/mobile/Applications/")) == 0)
        {

          uint64_t proc_enforce = 0;
          uint64_t vnode_enforce = 0; 
          size_t size = sizeof(vnode_enforce);
          
          sysctlbyname("security.mac.proc_enforce",  &proc_enforce,  &size, NULL, 0);  
          sysctlbyname("security.mac.vnode_enforce", &vnode_enforce, &size, NULL, 0);
          
          if (vnode_enforce && proc_enforce)
          {
            DecoderAvailable = 0;
            CLog::Log(LOGINFO, "VideoToolBox decoder not available. Use : sysctl -w security.mac.proc_enforce=0; sysctl -w security.mac.vnode_enforce=0\n");
            //NSLog(@"%s VideoToolBox decoder not available. Use : sysctl -w security.mac.proc_enforce=0; sysctl -w security.mac.vnode_enforce=0", __PRETTY_FUNCTION__);
          }
          else
          {
            DecoderAvailable = 1;
            CLog::Log(LOGINFO, "VideoToolBox decoder available\n");
            //NSLog(@"%s VideoToolBox decoder available", __PRETTY_FUNCTION__);
          }  
        }
        else
        {
          DecoderAvailable = 1;
        }
        //NSLog(@"%s Executable path %s", __PRETTY_FUNCTION__, given_path);
      }
      else
      {
        // In theory this case can never happen. But who knows.
        DecoderAvailable = 1;
      }
    }
  }

  return (DecoderAvailable == 1);
}

int DarwinBatteryLevel(void)
{
  float batteryLevel = 0;
#if defined(TARGET_DARWIN_IOS)
  if(!DarwinIsAppleTV2())
    batteryLevel = [[UIDevice currentDevice] batteryLevel];
#else
  CFTypeRef powerSourceInfo = IOPSCopyPowerSourcesInfo();
  CFArrayRef powerSources = IOPSCopyPowerSourcesList(powerSourceInfo);

  CFDictionaryRef powerSource = NULL;
  const void *powerSourceVal;

  for (int i = 0 ; i < CFArrayGetCount(powerSources) ; i++)
  {
    powerSource = IOPSGetPowerSourceDescription(powerSourceInfo, CFArrayGetValueAtIndex(powerSources, i));
    if (!powerSource) break;

    powerSourceVal = (CFStringRef)CFDictionaryGetValue(powerSource, CFSTR(kIOPSNameKey));

    int curLevel = 0;
    int maxLevel = 0;

    powerSourceVal = CFDictionaryGetValue(powerSource, CFSTR(kIOPSCurrentCapacityKey));
    CFNumberGetValue((CFNumberRef)powerSourceVal, kCFNumberSInt32Type, &curLevel);

    powerSourceVal = CFDictionaryGetValue(powerSource, CFSTR(kIOPSMaxCapacityKey));
    CFNumberGetValue((CFNumberRef)powerSourceVal, kCFNumberSInt32Type, &maxLevel);

    batteryLevel = (int)((double)curLevel/(double)maxLevel);
  }
#endif
  return batteryLevel * 100;  
}

#endif
