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

#ifndef __DAEMON_HPP__
#define __DAEMON_HPP__

#include "filemgr.hpp"
#include "config.hpp"
#include "drivers/gamepad/driver.hpp"


class Daemon
{
private:
    FileMgr                         mFileMgr;
    Config                          mConfig;
    Drivers::Gamepad::Driver*       mpGpDrv;

    bool                            DoesProfileExist( std::string profileName );
    int                             LoadProfile( std::string fileName );
    int                             Startup();
    void                            Shutdown();
   
public:
    // Daemon config overrides
    void                            SetStartupProfile( std::string profileName );

    int                             Run();
    void                            Stop();
    
    Daemon();
    ~Daemon();
};


#endif // __DAEMON_HPP__
