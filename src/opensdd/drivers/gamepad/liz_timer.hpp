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

#ifndef __GAMEPAD__LIZ_TIMER_HPP__
#define __GAMEPAD__LIZ_TIMER_HPP__

#include <chrono>


namespace Drivers::Gamepad
{
    // Lengh of time for the thread to sleep before keyboard emulation 
    // has to be disabled again with a CLEAR_MAPPINGS report.
    const double    LIZARD_SLEEP_MSEC   = 2000;

    class LizTimer
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock>  mStartTime;

    public:
        void        Reset()
        {
            mStartTime = std::chrono::steady_clock::now();
        }
        
        bool        Ready()
        {
            using namespace std::literals;
            
            //return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - mStartTime) >= LIZARD_SLEEP_MSEC );
            return (std::chrono::steady_clock::now() - mStartTime) / 1ms;
        }

        LizTimer()
        {
            mStartTime = std::chrono::steady_clock::now();
        };

        ~LizTimer()
        {
            //
        };
    };
}

#endif  // __GAMEPAD__LIZ_TIMER_HPP__
