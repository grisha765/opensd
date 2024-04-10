////////////////////////////////////////////////////////////////////////////////////////////////////
//  OpenSD
//  An open-source userspace driver for Valve's Steam Deck hardware
//
//  Copyright (C) 2022-2024 Daniel Nguyen
//  https://codeberg.org/opensd/opensd
//  Licensed under the GNU GPLv3+
//
//  This program is free software: you can redistribute it and/or modify it under the terms of the 
//  GNU General Public License as published by the Free Software Foundation, either version 3 of 
//  the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with this program. 
//  If not, see <https://www.gnu.org/licenses/>.             
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "prog_args.hpp"
#include "../common/log.hpp"


int ProgArgs::GetArgCount()
{
    return mArgList.size();
}



int ProgArgs::GetErrorCount()
{
    return mErrorCount;
}



int ProgArgs::FindOption( std::string shortOpt, std::string longOpt )
{
    unsigned int    pos = -1;
    int             count = 0;
    bool            terminated = false;

    // Return 0 if there are no program arguments
    if (mArgList.size() < 1)
        return -1;

    // Return 0 if no short and long options specified
    if ((shortOpt.empty()) && (longOpt.empty()))
    {
        gLog.Write( Log::ERROR, FUNC_NAME, "No options specified." );
        ++mErrorCount;
        return -1;
    }

    // Add preceeding "-" to short option
    if (!shortOpt.empty())
        shortOpt = "-" + shortOpt;

    // Add preceeding "--" to long option
    if (!longOpt.empty())
        longOpt = "--" + longOpt;

    // Loop through arg list and look for option
    for (unsigned int i = 0; i < mArgList.size(); ++i)
    {
        if (!terminated)
        {
            if (mArgList.at(i) == "--")
                terminated = true;
            else
            {
                if (!shortOpt.empty())
                    if (mArgList.at(i) == shortOpt)
                    {
                        if (count == 0)
                            pos = i;
                        ++count;
                    }

                if (!longOpt.empty())
                    if (mArgList.at(i) == longOpt)
                    {
                        if (count == 0)
                            pos = i;
                        ++count;
                    }
            }
        }
    }

    // No matches
    if (count == 0)
        return -1;

    // Multiple matches returns error
    if (count > 1)
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Multiple options definitions." );
        ++mErrorCount;
        return -1;
    }

    // If theres only 1 match, return the position
    return pos;
}



std::string ProgArgs::GetParam( int pos )
{
    // Return empty string if option was not found
    if (pos < 0)
        return "";

    // Next arg
    ++pos;

    // No option after argument
    if ((unsigned int)pos >= mArgList.size())
        return "";

    // Just to be safe
    if (mArgList.at(pos).empty())
        return "";

    // Check to see if next argument is an option
    if (mArgList.at(pos).at(0) == '-')
        return "";

    // Return non-option argument following specified option
    return mArgList.at(pos);
}



bool ProgArgs::HasOpt( std::string shortOpt, std::string longOpt, std::string& rParam )
{
    int     pos;

    pos = FindOption( shortOpt, longOpt );

    // Return if option not found
    if (pos < 0)
    {
        rParam = "";
        return false;
    }
    
    // Get option parameter if present
    rParam = GetParam( pos );

    return true;
}



bool ProgArgs::HasOpt( std::string shortOpt, std::string longOpt )
{
    std::string     s;

    return HasOpt( shortOpt, longOpt, s );
}



bool ProgArgs::PopOpt( std::string shortOpt, std::string longOpt, std::string& rParam )
{
    int     pos;

    pos = FindOption( shortOpt, longOpt );

    // Return if option not found
    if (pos < 0)
    {
        rParam = "";
        return false;
    }

    // Get option parameter if present
    rParam = GetParam( pos );

    // Erase param argument if present
    if (!rParam.empty())
    {
        try 
        {
            mArgList.erase( mArgList.begin() + pos + 1 );
        }
        catch (...)
        {
            int e = errno;
            gLog.Write( Log::ERROR, FUNC_NAME, "Caught exception while erasing vector element: " + Err::GetErrnoString(e) );
        }
    }

    // Erase opt argument
    try
    {
        mArgList.erase( mArgList.begin() + pos );
    }
    catch (...)
    {
        int e = errno;
        gLog.Write( Log::ERROR, FUNC_NAME, "Caught exception while erasing vector element: " + Err::GetErrnoString(e) );
    }

    return true;
}



bool ProgArgs::PopOpt( std::string shortOpt, std::string longOpt )
{
    std::string     s;

    return PopOpt( shortOpt, longOpt, s );
}



ProgArgs::ProgArgs( std::vector<std::string> argList )
{
    if (argList.size() > 1)
    {
        argList.erase( argList.begin() );
        mArgList = argList;
    }
    
    mErrorCount = 0;
}



ProgArgs::~ProgArgs()
{
    mArgList.clear();
}
