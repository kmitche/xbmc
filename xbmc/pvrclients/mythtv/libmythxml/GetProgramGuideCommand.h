#pragma once

#include <vector>

#include "MythXmlCommand.h"
#include "SProgram.h"


struct GenrePair
{
  int type;
  int subtype;

  GenrePair()
  {
    type = 0xf;
    subtype = 0xb2;
  };

  GenrePair(int type, int subtype)
  {
    type = type;
    subtype = subtype;
  };
};

class GenreIdMapper
{
  public:
    GenreIdMapper()
    {
      // TODO: Where did this list of strings come from?
      // TODO: Community review of the mappings.
      // TODO: Use the EVCONTENTMASK_* constants and order by those.
      genreTypeIdMap_["Auction"]          = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Awards"]           = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Biography"]        = GenrePair(0x70, 0x74);
      genreTypeIdMap_["Educational"]      = GenrePair(0x90, 0x96);
      genreTypeIdMap_["Entertainment"]    = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Holiday"]          = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Holiday special"]  = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Home improvement"] = GenrePair(0xA0, 0xa7);
      genreTypeIdMap_["How-to"]           = GenrePair(0x90, 0x96);
      genreTypeIdMap_["Music"]            = GenrePair(0x60, 0x61);
      genreTypeIdMap_["Music special"]    = GenrePair(0x60, 0x61);
      genreTypeIdMap_["Music talk"]       = GenrePair(0x60, 0x61);
      genreTypeIdMap_["Shopping"]         = GenrePair(0xA0, 0xa6);
      genreTypeIdMap_["Sitcom"]           = GenrePair(0x10, 0x14);
      genreTypeIdMap_["Soap"]             = GenrePair(0x10, 0x15);
      genreTypeIdMap_["Soap talk"]        = GenrePair(0x10, 0x15);
      genreTypeIdMap_["Golf"]             = GenrePair(0x40, 0x41);
      genreTypeIdMap_["Lacrosse"]         = GenrePair(0x40, 0x45);
      genreTypeIdMap_["Law"]              = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Card games"]       = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Collectibles"]     = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Community"]        = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Computers"]        = GenrePair(0x90, 0x92);
      genreTypeIdMap_["Consumer"]         = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Fundraiser"]       = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Gaming"]           = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Gay/lesbian"]      = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Military"]         = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Miniseries"]       = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Opera"]            = GenrePair(0x60, 0x65);
      genreTypeIdMap_["Parade"]           = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Paranormal"]       = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Parenting"]        = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Poker"]            = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Reality"]          = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Self improvement"] = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Special"]          = GenrePair(0xB0, 0xb2);
      genreTypeIdMap_["Standup"]          = GenrePair(0x10, 0x14);
      genreTypeIdMap_["Politics"]         = GenrePair(0x80, 0x82);
      genreTypeIdMap_["Public affairs"]   = GenrePair(0x80, 0x82);
      genreTypeIdMap_["Historical drama"] = GenrePair(0x10, 0x17);
      genreTypeIdMap_["History"]          = GenrePair(0x10, 0x17);
      genreTypeIdMap_["Boat"]             = GenrePair(0xA0, 0xa3);
      genreTypeIdMap_["Bus./financial"]   = GenrePair(0x80, 0x82);
      genreTypeIdMap_["Auto"]             = GenrePair(0xA0, 0xa3);
      genreTypeIdMap_["Aviation"]         = GenrePair(0xA0, 0xa3);
      genreTypeIdMap_["Nature"]           = GenrePair(0x90, 0x91);
      genreTypeIdMap_["Agriculture"]      = GenrePair(0x90, 0x91);
      genreTypeIdMap_["Animals"]          = GenrePair(0x90, 0x91);
      genreTypeIdMap_["Environment"]      = GenrePair(0x90, 0x91);
      genreTypeIdMap_["French"]           = GenrePair(0x90, 0x97);
      genreTypeIdMap_["Horse"]            = GenrePair(0x90, 0x91);
      genreTypeIdMap_["Outdoors"]         = GenrePair(0x90, 0x91);
      genreTypeIdMap_["Science"]          = GenrePair(0x90, 0x92);
      genreTypeIdMap_["Technology"]       = GenrePair(0x90, 0x92);
      genreTypeIdMap_["Medical"]          = GenrePair(0x90, 0x93);
      genreTypeIdMap_["Hunting"]          = GenrePair(0x90, 0x91);
      genreTypeIdMap_["Fishing"]          = GenrePair(0x90, 0x91);
      genreTypeIdMap_["Health"]           = GenrePair(0xA0, 0xa4);
      genreTypeIdMap_["Cooking"]          = GenrePair(0xA0, 0xa5);
      genreTypeIdMap_["House/garden"]     = GenrePair(0xA0, 0xa7);
      genreTypeIdMap_["Motorcycle"]       = GenrePair(0xA0, 0xa3);
      genreTypeIdMap_["Travel"]           = GenrePair(0xA0, 0xa1);
      genreTypeIdMap_["Aerobics"]         = GenrePair(0xA0, 0xa4);
      genreTypeIdMap_["Exercise"]         = GenrePair(0xA0, 0xa4);
      genreTypeIdMap_["Anthology"]        = GenrePair(0x70, 0x74);
      genreTypeIdMap_["Art"]              = GenrePair(0x70, 0x72);
      genreTypeIdMap_["Arts/crafts"]      = GenrePair(0x70, 0x74);
      genreTypeIdMap_["Fashion"]          = GenrePair(0x70, 0x7b);
      genreTypeIdMap_["Performing arts"]  = GenrePair(0x70, 0x71);
      genreTypeIdMap_["Spanish"]          = GenrePair(0x90, 0x97);
      genreTypeIdMap_["Religious"]        = GenrePair(0x70, 0x73);
      genreTypeIdMap_["Dance"]            = GenrePair(0x60, 0x66);
      genreTypeIdMap_["Animated"]         = GenrePair(0x50, 0x55);
      genreTypeIdMap_["Anime"]            = GenrePair(0x50, 0x55);
      genreTypeIdMap_["Children"]         = GenrePair(0x50, 0x52);
      genreTypeIdMap_["Children-music"]   = GenrePair(0x50, 0x52);
      genreTypeIdMap_["Children-special"] = GenrePair(0x50, 0x53);
      genreTypeIdMap_["Holiday-children"] = GenrePair(0x50, 0x52);
      genreTypeIdMap_["Holiday-children special"] = GenrePair(0x50, 0x52);
      genreTypeIdMap_["Game show"]        = GenrePair(0x30, 0x31);
      genreTypeIdMap_["Talk"]             = GenrePair(0x30, 0x33);
      genreTypeIdMap_["Variety"]          = GenrePair(0x30, 0x32);
      genreTypeIdMap_["Debate"]           = GenrePair(0x20, 0x24);
      genreTypeIdMap_["Docudrama"]        = GenrePair(0x20, 0x23);
      genreTypeIdMap_["Documentary"]      = GenrePair(0x20, 0x23);
      genreTypeIdMap_["Interview"]        = GenrePair(0x20, 0x24);
      genreTypeIdMap_["News"]             = GenrePair(0x20, 0x21);
      genreTypeIdMap_["Newsmagazine"]     = GenrePair(0x20, 0x21);
      genreTypeIdMap_["Weather"]          = GenrePair(0x20, 0x21);
      genreTypeIdMap_["Action"]           = GenrePair(0x10, 0x12);
      genreTypeIdMap_["Adults only"]      = GenrePair(0x10, 0x18);
      genreTypeIdMap_["Adventure"]        = GenrePair(0x10, 0x12);
      genreTypeIdMap_["Comedy"]           = GenrePair(0x10, 0x14);
      genreTypeIdMap_["Comedy-drama"]     = GenrePair(0x10, 0x14);
      genreTypeIdMap_["Crime"]            = GenrePair(0x10, 0x11);
      genreTypeIdMap_["Crime drama"]      = GenrePair(0x10, 0x11);
      genreTypeIdMap_["Drama"]            = GenrePair(0x10, 0x18);
      genreTypeIdMap_["Fantasy"]          = GenrePair(0x10, 0x13);
      genreTypeIdMap_["Horror"]           = GenrePair(0x10, 0x13);
      genreTypeIdMap_["Musical"]          = GenrePair(0x60, 0x65);
      genreTypeIdMap_["Musical comedy"]   = GenrePair(0x60, 0x65);
      genreTypeIdMap_["Mystery"]          = GenrePair(0x10, 0x12);
      genreTypeIdMap_["Romance"]          = GenrePair(0x10, 0x16);
      genreTypeIdMap_["Romance-comedy"]   = GenrePair(0x10, 0x16);
      genreTypeIdMap_["Science fiction"]  = GenrePair(0x10, 0x13);
      genreTypeIdMap_["Suspense"]         = GenrePair(0x10, 0x11);
      genreTypeIdMap_["War"]              = GenrePair(0x10, 0x12);
      genreTypeIdMap_["Western"]          = GenrePair(0x10, 0x12);
      genreTypeIdMap_["Action sports"]    = GenrePair(0x40, 0x4b);
    };

    const GenrePair& getGenreTypeId(CStdString& genre)
    {
      std::map<CStdString, GenrePair>::iterator it;
      it = genreTypeIdMap_.find(genre);
      if (it != genreTypeIdMap_.end())
        return it->second;
      return c_unknown_;
    };

  private:
    std::map<CStdString, GenrePair> genreTypeIdMap_;
    const GenrePair c_unknown_;
};

class GetProgramGuideCommand: public MythXmlCommand
{
  public:
    GetProgramGuideCommand(const int &channel, const time_t &gmt_start, const time_t &gmt_end);

    bool ParseResponse(const TiXmlHandle& handle);

    const std::vector<SProgram>& GetEpg();

  private:
    // TODO: have the genre mapper as a global variable, not referenced internally in this class.
    GenreIdMapper m_genre_mapper;

    // TODO: Rename the SEpg class since each instance isn't an EPG. Perhaps SProgram?
    std::vector<SProgram> m_epg;
};
