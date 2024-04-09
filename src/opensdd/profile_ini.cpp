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

#include "profile_ini.hpp"
#include "profile_template.hpp"
#include "../common/log.hpp"
#include "../common/input_event_names.hpp"
#include "../common/string_funcs.hpp"
// C++
#include <fstream>
#include <algorithm>

// Less messy
using namespace Drivers::Gamepad;


void ProfileIni::AddKeyEvent( BindType bindType, uint16_t code )
{
    bool    found = false;
    
    switch (bindType)
    {
        case BindType::GAME:
            for (auto& i : mProf.dev.gamepad.key_list)
                if (i == code)
                    found = true;
            if (!found)
                mProf.dev.gamepad.key_list.push_back( code );
        break;
        
        case BindType::MOTION:
            gLog.Write( Log::DEBUG, FUNC_NAME, "Key events are not supported on Motion device." );
            return;
        break;
        
        case BindType::MOUSE:
            for (auto& i : mProf.dev.mouse.key_list)
                if (i == code)
                    found = true;
            if (!found)
                mProf.dev.mouse.key_list.push_back( code );
        break;
        
        case BindType::NONE:
        case BindType::COMMAND:
        case BindType::PROFILE:
            gLog.Write( Log::DEBUG, FUNC_NAME, "Invalid binding type." );
            return;
        break;

        default:
            gLog.Write( Log::DEBUG, FUNC_NAME, "An unhandled binding type occurred." );
        break;
    }
}



void ProfileIni::AddAbsEvent( BindType bindType, uint16_t code, int32_t min, int32_t max, int32_t fuzz, int32_t res )
{
    bool    found = false;
    
    switch (bindType)
    {
        case BindType::GAME:
            for (auto& i : mProf.dev.gamepad.abs_list)
                if (i.code == code)
                    found = true;
                    
            if (!found)
            {
                Uinput::AbsAxisInfo     axis = {};
                axis.code = code;
                axis.min = min;
                axis.max = max;
                axis.fuzz = fuzz;
                axis.res = res;
                mProf.dev.gamepad.abs_list.push_back( axis );
            }
        break;
        
        case BindType::MOTION:
            for (auto& i : mProf.dev.motion.abs_list)
                if (i.code == code)
                    found = true;
                    
            if (!found)
            {
                Uinput::AbsAxisInfo     axis = {};
                axis.code = code;
                axis.min = min;
                axis.max = max;
                axis.fuzz = fuzz;
                axis.res = res;
                mProf.dev.motion.abs_list.push_back( axis );
            }
        break;
        
        case BindType::MOUSE:
            gLog.Write( Log::DEBUG, FUNC_NAME, "Absolute axis events are not supported on Mouse device." );
            return;
        break;
        
        case BindType::NONE:
        case BindType::COMMAND:
        case BindType::PROFILE:
            gLog.Write( Log::DEBUG, FUNC_NAME, "Invalid binding type." );
            return;
        break;
        
        default:
            gLog.Write( Log::DEBUG, FUNC_NAME, "An unhandled binding type occurred." );
        break;
    }
}



void ProfileIni::AddRelEvent( BindType bindType, uint16_t code )
{
    bool    found = false;
    
    switch (bindType)
    {
        case BindType::GAME:
            for (auto& i : mProf.dev.gamepad.rel_list)
                if (i == code)
                    found = true;
            if (!found)
                mProf.dev.gamepad.rel_list.push_back( code );
        break;
        
        case BindType::MOTION:
            for (auto& i : mProf.dev.motion.rel_list)
                if (i == code)
                    found = true;
            if (!found)
                mProf.dev.motion.rel_list.push_back( code );
        break;
        
        case BindType::MOUSE:
            for (auto& i : mProf.dev.mouse.rel_list)
                if (i == code)
                    found = true;
            if (!found)
                mProf.dev.mouse.rel_list.push_back( code );
        break;
        
        case BindType::NONE:
        case BindType::COMMAND:
        case BindType::PROFILE:
            gLog.Write( Log::DEBUG, FUNC_NAME, "Invalid binding type." );
            return;
        break;

        default:
            gLog.Write( Log::DEBUG, FUNC_NAME, "An unhandled binding type occurred." );
        break;
    }
}



void ProfileIni::GetFeatEnable( std::string key, bool& rValue )
{
    Ini::ValVec         val;
    
    // Value is unaltered if not found
    if (!mIni.DoesKeyExist( "Features", key ))
        return;

    // Key exists, get value
    val = mIni.GetVal( "Features", key );
    
    // Key expects 1 value
    if (!val.Count())
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects 1 true/false value.  Ignoring." );
        return;
    }
        
    if (Str::Lowercase(val.String()) == "true")
    {
        rValue = true;
        gLog.Write( Log::VERB, "    '" + key + "' is set to enabled" );
    }
    else
    {
        rValue = false;
        gLog.Write( Log::VERB, "    '" + key + "' is set to disabled" );
    }
}



void ProfileIni::GetDeviceInfo( std::string key, uint16_t& rVid, uint16_t& rPid, uint16_t& rVer, std::string& rName )
{
    Ini::ValVec         val;
    uint16_t            vid;
    uint16_t            pid;
    uint16_t            ver;
    bool                caught_exception=false;
    
    
    // Values are unaltered if not found
    if (mIni.DoesKeyExist( "DeviceInfo", key ))
    {
        // Key exists, continue
        val = mIni.GetVal( "DeviceInfo", key );
        
        // Key should have 4 values
        if (val.Count() == 4)
        {
            // Convert hex values to int
            try
            {
                vid = std::stoul( val.String(0), nullptr, 16 );
                pid = std::stoul( val.String(1), nullptr, 16 );
                ver = std::stoul( val.String(2), nullptr, 16 );
            }
            catch (...)
            {
                caught_exception=true;
                gLog.Write( Log::WARN, "Format error: 'DEVICEINFO." + key + "' contains an invalid hexidecimal value.  Ignoring key and using defaults." );
            }

            if (!caught_exception)
            {
                // No errors, so set reference values and return
                rVid = vid;
                rPid = pid;
                rVer = ver;
                rName = val.String(3);
            }
        }
        else
            gLog.Write( Log::WARN, "Format error: '" + key + "' expects 4 values.  Ignoring key and using defaults." );
    }
    else
        gLog.Write( Log::WARN, "No device info for '" + key + "'.  Using defaults." );

    gLog.Write( Log::VERB, "    '" + key + "' device info set:" );
    gLog.Write( Log::VERB, "        Name: '" + rName + "'" );
    gLog.Write( Log::VERB, "        VID:  0x" + Str::Uint16ToHex(rVid) );
    gLog.Write( Log::VERB, "        PID:  0x" + Str::Uint16ToHex(rPid) );
    gLog.Write( Log::VERB, "        Ver:  " + Str::Bcd16ToString(rVer) );
}



void ProfileIni::GetDeadzone( std::string key, double& rValue )
{
    Ini::ValVec         val;
    
    // Values are unaltered if not found
    if (!mIni.DoesKeyExist( "Deadzones", key ))
        return;
  
    // Key exists, get value
    val = mIni.GetVal( "Deadzones", key );

    // Key should have 1 value
    if (!val.Count())
    {
        // rValue is unaltered (i.e. uses default) if key is not found
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects 1 numeric value.  Ignoring." );
        return;
    }
    
    double v = val.Double();
    
    // Clamp range
    if (v > 0.9)
        v = 0.9;
    if (v < 0)
        v = 0;
        
    rValue = v;
    
    gLog.Write( Log::VERB, "    '" + key + "' deadzone set to " + std::to_string(v) );
}



void ProfileIni::GetAxisRange( std::string section, std::string key, int32_t& rMin, int32_t& rMax, int32_t& rFuzz, int32_t& rRes )
{
    Ini::ValVec         val;
    int                 min;
    int                 max;
    
    // Values are unaltered if not found
    if (!mIni.DoesKeyExist( section, key ))
        return;

    // Key exists, get value
    val = mIni.GetVal( section, key );
    
    // Key should have 2 values at minimum
    if (val.Count() < 2)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects at least 2 integer values.  Ignoring." );
        return;
    }
        
    min = val.Int(0);
    max = val.Int(1);
    
    // Get min / max range
    if (min == max)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' has an invalid range.  Ignoring." );
        return;
    }

    rMin = min;
    rMax = max;
    gLog.Write( Log::VERB, "    '" + key + "' axis set to:" );
    gLog.Write( Log::VERB, "        Min:  " + std::to_string(min) );
    gLog.Write( Log::VERB, "        Max:  " + std::to_string(max) );
    
    // Get fuzziness, if present
    if (val.Count() > 2)
    {
        rFuzz = val.Int(2);
        gLog.Write( Log::VERB, "        Fuzz: " + std::to_string(rFuzz) );
    }
    
    // Get resolution, if present
    if (val.Count() > 3)
    {
        rRes = val.Int(3);
        gLog.Write( Log::VERB, "        Res:  " + std::to_string(rRes) );
    }
}



void ProfileIni::GetPadAxisSensitivity( std::string key, double& rX, double& rY )
{
    Ini::ValVec         val;
    
    // Values are unaltered if not found
    if (!mIni.DoesKeyExist( "Touchpads", key ))
        return;
    
    // Key exists, get value
    val = mIni.GetVal( "Touchpads", key );

    // Key should have 1 value
    if (val.Count() < 2)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects 2 numeric values.  Ignoring." );
        return;
    }
    
    rX = val.Double(0);
    rY = val.Double(1);
    
    gLog.Write( Log::VERB, "    '" + key + "' sensitivity set to:" );
    gLog.Write( Log::VERB, "        X: " + std::to_string(rX) );
    gLog.Write( Log::VERB, "        Y: " + std::to_string(rY) );
}



void ProfileIni::GetPadDecay( std::string key, double& rX, double& rY )
{
    Ini::ValVec         val;
    
    // Values are unaltered if not found
    if (!mIni.DoesKeyExist( "Touchpads", key ))
        return;

    // Key exists, get value
    val = mIni.GetVal( "Touchpads", key );
        
        // Get decay rate if present, if not use default of 0.05
    if (val.Count() < 2)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects 2 numeric values.  Ignoring." );
        return;
    }

    // Clamp values from 0% to 100%
    rX = std::clamp( val.Double(0), 0.0, 1.0 );
    rY = std::clamp( val.Double(1), 0.0, 1.0 );

    gLog.Write( Log::VERB, "    '" + key + "' inertial decay set to:" );
    gLog.Write( Log::VERB, "        X: " + std::to_string(rX) );
    gLog.Write( Log::VERB, "        Y: " + std::to_string(rY) );
}



void ProfileIni::GetEventBinding( std::string key, Binding& rBind )
{
    Binding                 bind;
    std::string             dev_str;
    std::string             ev_str;
    Ini::ValVec             val;
    int                     ev_type = 0;
    int                     result;
    
    val = mIni.GetVal( "Bindings", key );

    // Need at least 2 params for event bindings
    if (val.Count() < 2)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expect at least 2 values.  Ignoring" );
        return;
    }

    // Get device type
    dev_str = Str::Uppercase(val.String(0));
    if (dev_str == "GAMEPAD")
    {
        bind.type = BindType::GAME;
        dev_str = "Gamepad";
    }
    else
        if (dev_str == "MOTION")
        {
            bind.type = BindType::MOTION;
            dev_str = "Motion";
        }
        else
            if (dev_str == "MOUSE")
            {
                bind.type = BindType::MOUSE;
                dev_str = "Mouse";
            }
            else
                bind.type = BindType::NONE;

    // Get 2nd param, which should be the event code, from which we extract the event type
    ev_str = Str::Uppercase( val.String(1) );
    result = EvName::GetEvType( ev_str );
    if (result < 0)
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Failed to get event type." );
        return;
    }
    ev_type = result;
    
    // Handle the rest based on the event type
    switch (ev_type)
    {
        // Button / key events
        case EV_KEY: 
            bind.ev_type  = EV_KEY;

            result = EvName::GetEvCode( ev_str );
            if (result < 0)
            {
                gLog.Write( Log::WARN, "Format error: '" + key + "' has an unknown KEY/BTN event code.  Ignoring." );
                return;
            }
            bind.ev_code  = (uint16_t)result;
            bind.dir   = false;
            
            // Enable key event to the specified device
            AddKeyEvent( bind.type, bind.ev_code );

            gLog.Write( Log::VERB, "    Added binding: '" + key + "' = " + dev_str + " " + ev_str );
        break;
        
        // Absolute axis events
        case EV_ABS: 
            // Need 3 params to continue
            if (val.Count() < 3)
            {
                gLog.Write( Log::WARN, "Format error: '" + key + "' expects 3 values.  Ignoring." );
                return;
            }
            bind.ev_type  = EV_ABS;
            
            // Get the event code + any offset
            result = EvName::GetEvCode( ev_str );
            if (result < 0)
            {
                gLog.Write( Log::WARN, "Format error: '" + key + "' has an unknown ABS event code.  Ignoring." );
                return;
            }
            bind.ev_code  = (uint16_t)result;

            // Get the direction of the axis even to bind
            if (val.String(2) == "+")
                bind.dir = true;
            else
                if (val.String(2) == "-")
                    bind.dir = false;
                else
                {
                    gLog.Write( Log::WARN, "Format error: " + key + " expects the 3rd value to be +/- direction indicator.  Ignoring." );
                    return;
                }
            
            // NOTE: The axis is not enabled enabled here, but from the GamepadAxes / MotionAxes section
            
            gLog.Write( Log::VERB, "    Added binding: '" + key + "' = " + dev_str + " " + ev_str + (bind.dir ? "+" : "-") );
        break;
        
        // Relative axis events
        case EV_REL:
            bind.ev_type  = EV_REL;

            result = EvName::GetEvCode( ev_str );
            if (result < 0)
            {
                gLog.Write( Log::WARN, "Format error: '" + key + "' has an unknown REL event code.  Ignoring." );
                return;
            }
            bind.ev_code  = (uint16_t)result;
            bind.dir   = false;
            
            // Check for optional scale/sensitivity parameter
            if (val.Count() > 2)
            {
                bind.scale = val.Double(2);
                if (bind.scale == 0)
                    gLog.Write( Log::WARN, "Binding for " + key + " has a scale value of 0.  This will make the axis unusable." );
            }

            // Enable relative axis event
            AddRelEvent( bind.type, bind.ev_code );
            
            gLog.Write( Log::VERB, "    Added binding: '" + key + "' = " + dev_str + " " + ev_str + " " + std::to_string( bind.scale ) );
        break;
        
        default:
            gLog.Write( Log::WARN, "Format error: '" + key + "' has an unknown event type.  Ignoring." );
            return;
        break;
    }

    // Assign new binding to referenced parameter
    rBind = bind;
}



void ProfileIni::GetCommandBinding( std::string key, Binding& rBind )
{
    Binding             bind;
    Ini::ValVec         val;
    std::string         temp_str;
    int                 result;
    static uint32_t     uid = 1;  // Unique ID for each command binding

    val = mIni.GetVal( "Bindings", key );

    // Need at least 4 params for Command bindings
    if (val.Count() < 4)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects 4 values for a command binding." );
        return;
    }
    
    bind.type = BindType::COMMAND;
    
    // wait_for_exit field
    if (val.Bool(1))
    {
        // wait_for_exit = true, so set a unique binding ID
        bind.id = uid;
        ++uid;
    }
    else
    {
        // wait_for_exit = false, so use an ID of zero to signal that we need to
        // ignore pid checking for this binding
        bind.id = 0;
    }
    
    // repeat_delay_ms field 
    result = val.Int(2);
    if (result < 0)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects a repeat_delay_ms of 0 or greater." );
        result = 0;
    }
        
    bind.delay = result;
    
    // Command string
    temp_str = val.FullString(3);
    if (temp_str.empty())
    {
        gLog.Write( Log::WARN, "Format error: " + key + " has no command to execute." );
        return;
    }
    bind.str = temp_str;
    
    gLog.Write( Log::VERB, "    Added binding: '" + key + "' = Command " + (bind.id ? "true" : "false") + " " + std::to_string(bind.delay) + " " + bind.str );

    // Assign new binding to referenced parameter
    rBind = bind;
}



void ProfileIni::GetProfileBinding( std::string key, Binding& rBind )
{
    Binding                 bind;
    Ini::ValVec             val;

    val = mIni.GetVal( "Bindings", key );

    // Need 2 params for Profile bindings
    if (val.Count() < 2)
    {
        gLog.Write( Log::WARN, "Format error: '" + key + "' expects 2 values for a profile binding." );
        return;
    }
    
    bind.type = BindType::PROFILE;
    
    bind.str = val.FullString(1);
    if (bind.str.empty())
        return;
    
    gLog.Write( Log::VERB, "    Added binding: '" + key + "' = Profile " + bind.str );
    
    // Assign new binding to referenced parameter
    rBind = bind;
}



void ProfileIni::GetBinding( std::string key, Binding& rBind )
{
    Ini::ValVec             val;
    std::string             temp_str;
    std::string             sub_str;
    
    // Find the key in the Bindings section
    val = mIni.GetVal( "Bindings", key );
    if (!val.Count())
    {
        gLog.Write( Log::WARN, "Format error: " + key + " has no values set.  Ignoring." );
        return;
    }

    // Get the the binding type
    temp_str = Str::Uppercase(val.String(0));
    if (temp_str == "NONE")
        return;
    else
    {
        if ((temp_str == "GAMEPAD") || (temp_str == "MOTION") || (temp_str == "MOUSE"))
            GetEventBinding( key, rBind );
        else
            if (temp_str == "COMMAND")
                GetCommandBinding( key, rBind );
            else
                if (temp_str == "PROFILE")
                    GetProfileBinding( key, rBind );
                else
                {                        
                    gLog.Write( Log::WARN, "Format error: '" + key + "' has an unknown binding type." );
                    return;
                }
    }
}



int ProfileIni::Load( std::filesystem::path filePath, Profile& rProf )
{
    Ini::ValVec             val;
    int                     result;
    
    namespace fs = std::filesystem;

    // Set to default values
    mProf = PROFILE_TEMPLATE;
    mIni.Clear();
    
    if (!fs::exists(filePath))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Profile file '" + filePath.string() + "' not found." );
        return Err::FILE_NOT_FOUND;
    }
    
    gLog.Write( Log::DEBUG, "Loading profile from '" + filePath.string() + "'." );
    result = mIni.LoadFile( filePath );
    if (result != Err::OK)
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Failed to parse profile file '" + filePath.string() + "'" );
        return result;
    }
    
    // Begin reading values from profile file
    // ----------------------------- [Profile] section -----------------------------
    if (mIni.DoesSectionExist( "Profile" ))
    {
        gLog.Write( Log::VERB, "Reading [Profile] section..." );
        
        if (mIni.DoesKeyExist( "Profile", "Name" ))
        {
            val = mIni.GetVal( "Profile", "Name" );
            if (val.Count())
                mProf.profile_name = val.FullString();
            
            gLog.Write( Log::VERB, "    'Name' is set to '" + mProf.profile_name + "'" );
        }

        
        if (mIni.DoesKeyExist( "Profile", "Description" ))
        {
            val = mIni.GetVal( "Profile", "Description" );
            if (val.Count())
                mProf.profile_desc = val.FullString();

            gLog.Write( Log::VERB, "    'Description' is set to '" + mProf.profile_desc + "'" );
        }
    }
    
    // ----------------------------- [Features] section -----------------------------
    if (mIni.DoesSectionExist( "Features" ))
    {
        gLog.Write( Log::VERB, "Reading [Features] section..." );
        GetFeatEnable( "ForceFeedback",     mProf.features.ff );
        GetFeatEnable( "MotionDevice",      mProf.features.motion );
        GetFeatEnable( "MouseDevice",       mProf.features.mouse );
        GetFeatEnable( "LizardMode",        mProf.features.lizard );
        GetFeatEnable( "StickFiltering",    mProf.features.filter_sticks );
        GetFeatEnable( "TrackpadFiltering", mProf.features.filter_pads );
    }

    // ----------------------------- [DeviceInfo] section -----------------------------
    if (mIni.DoesSectionExist( "DeviceInfo" ))
    {
        gLog.Write( Log::VERB, "Reading [DeviceInfo] section..." );
        GetDeviceInfo( "Gamepad",   mProf.dev.gamepad.vid, mProf.dev.gamepad.pid, mProf.dev.gamepad.ver, mProf.dev.gamepad.name );
        GetDeviceInfo( "Motion",    mProf.dev.motion.vid, mProf.dev.motion.pid, mProf.dev.motion.ver, mProf.dev.motion.name );
        GetDeviceInfo( "Mouse",     mProf.dev.mouse.vid, mProf.dev.mouse.pid, mProf.dev.mouse.ver, mProf.dev.mouse.name );
    }

    // ----------------------------- [Deadzone] section -----------------------------
    if (mIni.DoesSectionExist( "Deadzones" ))
    {
        gLog.Write( Log::VERB, "Reading [Deadzone] section..." );
        GetDeadzone( "LStick",  mProf.dz.stick.l );
        GetDeadzone( "RStick",  mProf.dz.stick.r );
        GetDeadzone( "LPad",    mProf.dz.pad.l );
        GetDeadzone( "RPad",    mProf.dz.pad.r );
        GetDeadzone( "LTrigg",  mProf.dz.trigg.l );
        GetDeadzone( "RTrigg",  mProf.dz.trigg.r );
    }
    
    // ----------------------------- [GamepadAxes] section -----------------------------
    if (mIni.DoesSectionExist( "GamepadAxes" ))
    {
        // Iterate through list of keys and define / enable axes
        gLog.Write( Log::VERB, "Reading [GamepadAxes] section..." );
        val = mIni.GetKeyList( "GamepadAxes" );
        for (auto& key : val.mData)
        {
            int32_t         min = 0;
            int32_t         max = 0;
            int32_t         fuzz = 0;
            int32_t         res = 0;
            uint16_t        code = 0;
            
            int             result;
            
            result = EvName::GetEvCode( key );
            if (result < 0)
            {
                gLog.Write( Log::DEBUG, FUNC_NAME, "Failed to enable gamepad axis '" + key + ": Event name not found." );
            }
            code = (uint16_t)result;
            
            GetAxisRange( "GamepadAxes", key, min, max, fuzz, res );
            AddAbsEvent( BindType::GAME, code, min, max, fuzz, res );
        }
    }

    // ----------------------------- [MotionAxes] section -----------------------------
    if (mIni.DoesSectionExist( "MotionAxes" ))
    {
        gLog.Write( Log::VERB, "Reading [MotionAxes] section..." );
        // Iterate through list of keys and define / enable axes
        val = mIni.GetKeyList( "MotionAxes" );
        for (auto& key : val.mData)
        {
            int32_t         min = 0;
            int32_t         max = 0;
            int32_t         fuzz = 0;
            int32_t         res = 0;
            uint16_t        code = 0;
            int             result;
            
            result = EvName::GetEvCode( key );
            if (result < 0)
            {
                gLog.Write( Log::DEBUG, FUNC_NAME, "Failed to enable gamepad axis '" + key + ": Event name not found." );
            }
            code = (uint16_t)result;
            
            GetAxisRange( "MotionAxes", key, min, max, fuzz, res );
            AddAbsEvent( BindType::MOTION, code, min, max, fuzz, res );
        }
    }
    
    // ----------------------------- [Touchpads] section ----------------------------
    if (mIni.DoesSectionExist( "Touchpads" ))
    {
        gLog.Write( Log::VERB, "Reading [Touchpads] section..." );
        // Pad senstivity
        GetPadAxisSensitivity( "LPadSensitivity", mProf.pad.l.sens.x, mProf.pad.l.sens.y );
        GetPadAxisSensitivity( "RPadSensitivity", mProf.pad.r.sens.x, mProf.pad.r.sens.y );
        // Pad inertial decay
        GetPadDecay( "LPadDecay", mProf.pad.l.decay.x, mProf.pad.l.decay.y );
        GetPadDecay( "RPadDecay", mProf.pad.r.decay.x, mProf.pad.r.decay.y );
    }
    
    // ----------------------------- [Bindings] section -----------------------------
    if (mIni.DoesSectionExist( "Bindings" ))
    {
        gLog.Write( Log::VERB, "Reading [Bindings] section..." );
        // Dpad
        GetBinding( "DpadUp",               mProf.map.dpad.up );
        GetBinding( "DpadDown",             mProf.map.dpad.down );
        GetBinding( "DpadLeft",             mProf.map.dpad.left );
        GetBinding( "DpadRight",            mProf.map.dpad.right );
        // Buttons
        GetBinding( "A",                    mProf.map.btn.a );
        GetBinding( "B",                    mProf.map.btn.b );
        GetBinding( "X",                    mProf.map.btn.x );
        GetBinding( "Y",                    mProf.map.btn.y );
        GetBinding( "L1",                   mProf.map.btn.l1 );
        GetBinding( "L2",                   mProf.map.btn.l2 );
        GetBinding( "L3",                   mProf.map.btn.l3 );
        GetBinding( "L4",                   mProf.map.btn.l4 );
        GetBinding( "L5",                   mProf.map.btn.l5 );
        GetBinding( "R1",                   mProf.map.btn.r1 );
        GetBinding( "R2",                   mProf.map.btn.r2 );
        GetBinding( "R3",                   mProf.map.btn.r3 );
        GetBinding( "R4",                   mProf.map.btn.r4 );
        GetBinding( "R5",                   mProf.map.btn.r5 );
        GetBinding( "Menu",                 mProf.map.btn.menu );
        GetBinding( "Options",              mProf.map.btn.options );
        GetBinding( "Steam",                mProf.map.btn.steam );
        GetBinding( "QuickAccess",          mProf.map.btn.quick_access );
        // Triggers
        GetBinding( "LTrigg",               mProf.map.trigg.l );
        GetBinding( "RTrigg",               mProf.map.trigg.r );
        // Left Stick
        GetBinding( "LStickUp",             mProf.map.stick.l.up );
        GetBinding( "LStickDown",           mProf.map.stick.l.down );
        GetBinding( "LStickLeft",           mProf.map.stick.l.left );
        GetBinding( "LStickRight",          mProf.map.stick.l.right );
        GetBinding( "LStickTouch",          mProf.map.stick.l.touch );
        GetBinding( "LStickForce",          mProf.map.stick.l.force );
        // Right Stick
        GetBinding( "RStickUp",             mProf.map.stick.r.up );
        GetBinding( "RStickDown",           mProf.map.stick.r.down );
        GetBinding( "RStickLeft",           mProf.map.stick.r.left );
        GetBinding( "RStickRight",          mProf.map.stick.r.right );
        GetBinding( "RStickTouch",          mProf.map.stick.r.touch );
        GetBinding( "RStickForce",          mProf.map.stick.r.force );
        // Left Touchpad
        GetBinding( "LPadUp",               mProf.map.pad.l.up );
        GetBinding( "LPadDown",             mProf.map.pad.l.down );
        GetBinding( "LPadLeft",             mProf.map.pad.l.left );
        GetBinding( "LPadRight",            mProf.map.pad.l.right );
        GetBinding( "LPadRelX",             mProf.map.pad.l.rel_x );
        GetBinding( "LPadRelY",             mProf.map.pad.l.rel_y );
        GetBinding( "LPadTouch",            mProf.map.pad.l.touch );
        GetBinding( "LPadPress",            mProf.map.pad.l.press );
        GetBinding( "LPadForce",            mProf.map.pad.l.force );
        GetBinding( "LPadPressQuadUp",      mProf.map.pad.l.btn_quad_up );
        GetBinding( "LPadPressQuadDown",    mProf.map.pad.l.btn_quad_down );
        GetBinding( "LPadPressQuadLeft",    mProf.map.pad.l.btn_quad_left );
        GetBinding( "LPadPressQuadRight",   mProf.map.pad.l.btn_quad_right );
        GetBinding( "LPadPressOrthUp",      mProf.map.pad.l.btn_orth_up );
        GetBinding( "LPadPressOrthDown",    mProf.map.pad.l.btn_orth_down );
        GetBinding( "LPadPressOrthLeft",    mProf.map.pad.l.btn_orth_left );
        GetBinding( "LPadPressOrthRight",   mProf.map.pad.l.btn_orth_right );
        GetBinding( "LPadPressGrid2x2_1",   mProf.map.pad.l.btn_2x2_1 );
        GetBinding( "LPadPressGrid2x2_2",   mProf.map.pad.l.btn_2x2_2 );
        GetBinding( "LPadPressGrid2x2_3",   mProf.map.pad.l.btn_2x2_3 );
        GetBinding( "LPadPressGrid2x2_4",   mProf.map.pad.l.btn_2x2_4 );
        GetBinding( "LPadPressGrid3x3_1",   mProf.map.pad.l.btn_3x3_1 );
        GetBinding( "LPadPressGrid3x3_2",   mProf.map.pad.l.btn_3x3_2 );
        GetBinding( "LPadPressGrid3x3_3",   mProf.map.pad.l.btn_3x3_3 );
        GetBinding( "LPadPressGrid3x3_4",   mProf.map.pad.l.btn_3x3_4 );
        GetBinding( "LPadPressGrid3x3_5",   mProf.map.pad.l.btn_3x3_5 );
        GetBinding( "LPadPressGrid3x3_6",   mProf.map.pad.l.btn_3x3_6 );
        GetBinding( "LPadPressGrid3x3_7",   mProf.map.pad.l.btn_3x3_7 );
        GetBinding( "LPadPressGrid3x3_8",   mProf.map.pad.l.btn_3x3_8 );
        GetBinding( "LPadPressGrid3x3_9",   mProf.map.pad.l.btn_3x3_9 );
        // Right Touchpad
        GetBinding( "RPadUp",               mProf.map.pad.r.up );
        GetBinding( "RPadDown",             mProf.map.pad.r.down );
        GetBinding( "RPadLeft",             mProf.map.pad.r.left );
        GetBinding( "RPadRight",            mProf.map.pad.r.right );
        GetBinding( "RPadRelX",             mProf.map.pad.r.rel_x );
        GetBinding( "RPadRelY",             mProf.map.pad.r.rel_y );
        GetBinding( "RPadTouch",            mProf.map.pad.r.touch );
        GetBinding( "RPadPress",            mProf.map.pad.r.press );
        GetBinding( "RPadForce",            mProf.map.pad.r.force );
        GetBinding( "RPadPressQuadUp",      mProf.map.pad.r.btn_quad_up );
        GetBinding( "RPadPressQuadDown",    mProf.map.pad.r.btn_quad_down );
        GetBinding( "RPadPressQuadLeft",    mProf.map.pad.r.btn_quad_left );
        GetBinding( "RPadPressQuadRight",   mProf.map.pad.r.btn_quad_right );
        GetBinding( "RPadPressOrthUp",      mProf.map.pad.r.btn_orth_up );
        GetBinding( "RPadPressOrthDown",    mProf.map.pad.r.btn_orth_down );
        GetBinding( "RPadPressOrthLeft",    mProf.map.pad.r.btn_orth_left );
        GetBinding( "RPadPressOrthRight",   mProf.map.pad.r.btn_orth_right );
        GetBinding( "RPadPressGrid2x2_1",   mProf.map.pad.r.btn_2x2_1 );
        GetBinding( "RPadPressGrid2x2_2",   mProf.map.pad.r.btn_2x2_2 );
        GetBinding( "RPadPressGrid2x2_3",   mProf.map.pad.r.btn_2x2_3 );
        GetBinding( "RPadPressGrid2x2_4",   mProf.map.pad.r.btn_2x2_4 );
        GetBinding( "RPadPressGrid3x3_1",   mProf.map.pad.r.btn_3x3_1 );
        GetBinding( "RPadPressGrid3x3_2",   mProf.map.pad.r.btn_3x3_2 );
        GetBinding( "RPadPressGrid3x3_3",   mProf.map.pad.r.btn_3x3_3 );
        GetBinding( "RPadPressGrid3x3_4",   mProf.map.pad.r.btn_3x3_4 );
        GetBinding( "RPadPressGrid3x3_5",   mProf.map.pad.r.btn_3x3_5 );
        GetBinding( "RPadPressGrid3x3_6",   mProf.map.pad.r.btn_3x3_6 );
        GetBinding( "RPadPressGrid3x3_7",   mProf.map.pad.r.btn_3x3_7 );
        GetBinding( "RPadPressGrid3x3_8",   mProf.map.pad.r.btn_3x3_8 );
        GetBinding( "RPadPressGrid3x3_9",   mProf.map.pad.r.btn_3x3_9 );
        // Acclerometers
        GetBinding( "AccelXPlus",           mProf.map.accel.x_plus );
        GetBinding( "AccelXMinus",          mProf.map.accel.x_minus );
        GetBinding( "AccelYPlus",           mProf.map.accel.y_plus );
        GetBinding( "AccelYMinus",          mProf.map.accel.y_minus );
        GetBinding( "AccelZPlus",           mProf.map.accel.z_plus );
        GetBinding( "AccelZMinus",          mProf.map.accel.z_minus );
        // Gyro / Attitude
        GetBinding( "RollPlus",             mProf.map.att.roll_plus );
        GetBinding( "RollMinus",            mProf.map.att.roll_minus );
        GetBinding( "PitchPlus",            mProf.map.att.pitch_plus );
        GetBinding( "PitchMinus",           mProf.map.att.pitch_minus );
        GetBinding( "YawPlus",              mProf.map.att.yaw_plus );
        GetBinding( "YawMinus",             mProf.map.att.yaw_minus );
    }

    // Assign loaded profile to reference parameter
    rProf = mProf;
    
    return Err::OK;
}



ProfileIni::ProfileIni()
{
    mProf = PROFILE_TEMPLATE;
}



ProfileIni::~ProfileIni()
{
    mIni.Clear();
}
