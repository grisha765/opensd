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

#include "filemgr.hpp"
#include "../common/xdg.hpp"
#include "../common/log.hpp"
// Generated by cmake
#include "cmake_vars.hpp"
  

bool FileMgr::IsInstalled()
{
    namespace           fs = std::filesystem;
    fs::path            path;

    path = CMakeVar::INSTALL_DATA_DIR;
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }

    path = CMakeVar::INSTALL_DATA_CONFIG_DIR;
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }
    
    path = CMakeVar::INSTALL_DATA_CONFIG_DIR + CMakeVar::DEFAULT_CONFIG_FILENAME;
    if ((!fs::exists( path )) || (!fs::is_regular_file( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "File '" + path.string() + "' does not exist." );
        return false;
    }

    path = CMakeVar::INSTALL_DATA_PROFILE_DIR;
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }
    
    path = CMakeVar::INSTALL_DATA_PROFILE_DIR + CMakeVar::DEFAULT_PROFILE_FILENAME;
    if ((!fs::exists( path )) || (!fs::is_regular_file( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "File '" + path.string() + "' does not exist." );
        return false;
    }
    
    // Return success if nothing missing
    return true;
}



bool FileMgr::IsLocalBuild()
{
    namespace           fs = std::filesystem;
    fs::path            path;

    path = "data/";
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }

    path = "data/config/";
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }
    
    path = "data/config/" + CMakeVar::DEFAULT_CONFIG_FILENAME;
    if ((!fs::exists( path )) || (!fs::is_regular_file( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "File '" + path.string() + "' does not exist." );
        return false;
    }

    path = "data/profiles/";
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }
    
    path = "data/profiles/" + CMakeVar::DEFAULT_PROFILE_FILENAME;
    if ((!fs::exists( path )) || (!fs::is_regular_file( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "File '" + path.string() + "' does not exist." );
        return false;
    }
    
    // Return success if nothing missing
    return true;
    
}



bool FileMgr::HasUserHome()
{
    namespace           fs = std::filesystem;
    fs::path            path;
    
    path = Xdg::UserHome();
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }
    
    return true;
}



bool FileMgr::HasSystemConfig()
{
    namespace           fs = std::filesystem;
    fs::path            path;
    
    path = CMakeVar::SYSTEM_CONFIG_DIR;
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }

    path = CMakeVar::SYSTEM_CONFIG_DIR + CMakeVar::DEFAULT_CONFIG_FILENAME;
    if ((!fs::exists( path )) || (!fs::is_regular_file( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "File '" + path.string() + "' does not exist." );
        return false;
    }

    path = CMakeVar::SYSTEM_PROFILE_DIR;
    if ((!fs::exists( path )) || (!fs::is_directory( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Directory '" + path.string() + "' does not exist." );
        return false;
    }
    
    return true;
}



int FileMgr::CreateUserConfigDir()
{
    namespace       fs = std::filesystem;
    
    if (mConfigDir.empty())
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Config directory member variable not set." );
        return Err::NOT_INITIALIZED;
    }

    gLog.Write( Log::DEBUG, FUNC_NAME, "Creating user config directory '" + mConfigDir.string() + "'..." );
    if(!fs::create_directory( mConfigDir ))
    {
        gLog.Write( Log::DEBUG, "Create directory failed." );
        return Err::CANNOT_CREATE;
    }
    gLog.Write( Log::DEBUG, FUNC_NAME, "Create directory OK." );
    
    return Err::OK;
}



int FileMgr::CopyUserConfigFile()
{
    namespace       fs = std::filesystem;
    fs::path        src;
    fs::path        dst;

    if (mConfigDir.empty())
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Config directory member variable not set." );
        return Err::NOT_INITIALIZED;
    }

    src = mDataDir.string() + "config/" + CMakeVar::DEFAULT_CONFIG_FILENAME;
    dst = mConfigDir.string() + CMakeVar::DEFAULT_CONFIG_FILENAME;
    
    gLog.Write( Log::DEBUG, FUNC_NAME, "Copying '" + src.string() + "' to '" + dst.string() + "'..." );
    fs::copy( src, dst );
    if ((!fs::exists(dst)) || (!fs::is_regular_file(dst)))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Copy failed." );
        return Err::COPY_FAILED;
    }
    gLog.Write( Log::DEBUG, FUNC_NAME, "Copy OK." );

    return Err::OK;
}



int FileMgr::CreateUserProfileDir()
{
    namespace       fs = std::filesystem;
    
    if (mProfileDir.empty())
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Profile directory member variable not set." );
        return Err::NOT_INITIALIZED;
    }

    gLog.Write( Log::DEBUG, FUNC_NAME, "Creating user profile directory '" + mProfileDir.string() + "'..." );
    if(!fs::create_directory( mProfileDir ))
    {
        gLog.Write( Log::DEBUG, "Create directory failed." );
        return Err::CANNOT_CREATE;
    }
    gLog.Write( Log::DEBUG, FUNC_NAME, "Create directory OK." );
    
    return Err::OK;
}



int FileMgr::CopyUserProfileFiles()
{
    namespace       fs = std::filesystem;
    fs::path        src;
    fs::path        dst;

    if (mProfileDir.empty())
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Profile directory member variable not set." );
        return Err::NOT_INITIALIZED;
    }

    src = mDataDir.string() + "profiles/";
    dst = mProfileDir;
    
    for (auto const& p : std::filesystem::directory_iterator( src ))
    {
        if (fs::is_regular_file( p ))
        {
            // Copy if file doesnt exist
            if (!fs::exists( dst / p.path().filename() ))
            {
                dst = mProfileDir.string() + p.path().filename().string();
                gLog.Write( Log::DEBUG, FUNC_NAME, "Copying '" + p.path().string() + "' to '" + dst.string() + "'..." );
                fs::copy_file( p, dst );
            }
        }
    }
    gLog.Write( Log::DEBUG, FUNC_NAME, "Copy OK." );

    return Err::OK;
}



int FileMgr::Init()
{
    namespace       fs = std::filesystem;
    fs::path        path;
    int             result;
    
    // Find data files
    if (IsInstalled())
    {
        gLog.Write( Log::INFO, "Detected OpenSD installation in '" + CMakeVar::INSTALL_DATA_DIR + "'" );
        mDataDir = CMakeVar::INSTALL_DATA_DIR;
    }
    else
    {
        gLog.Write( Log::INFO, "OpenSD installation not detected.  Checking for local build data..." );
        if (IsLocalBuild())
        {
            gLog.Write( Log::INFO, "Using data files from build directory instead." );
            mDataDir = "data/";
        }
        else
        {
            gLog.Write( Log::ERROR, "Failed to find OpenSD data files.  Please either install OpenSD, or run from build directory." );
            mDataDir.clear();
        }
    }
    
    if (mDataDir.empty())
        return Err::DIR_NOT_FOUND;


    // Find configuration directories
    if (HasUserHome())
    {
        mConfigDir = Xdg::ConfigHome().string() + "opensd/";
        mProfileDir = mConfigDir.string() + "profiles/";
        mIsConfigDirWritable = true;
        mIsProfileDirWritable = true;
        gLog.Write( Log::DEBUG, FUNC_NAME, "Setting config directory to '" + mConfigDir.string() + "'" );
        gLog.Write( Log::DEBUG, FUNC_NAME, "Setting profile directory to '" + mProfileDir.string() + "'" );
        
        // Check if user config dirs exist
        if (!fs::exists(mConfigDir))
        {
            gLog.Write( Log::DEBUG, FUNC_NAME, "User config directory '" + mConfigDir.string() + "' does not exist." );
            result = CreateUserConfigDir();
            if (result != Err::OK)
            {
                gLog.Write( Log::ERROR, "Failed to create user configuration directory.  Check your file permissions." );
                return result;
            }
            
            result = CopyUserConfigFile();
            if (result != Err::OK)
            {
                gLog.Write( Log::ERROR, "Failed to create user configuration file.  Check your file permissions." );
                return result;
            }
            
        }
        else
        {
            // Make sure it's a directory
            if (!fs::is_directory( mConfigDir ))
            {
                gLog.Write( Log::ERROR, "OpenSD user configuration directory '" + mConfigDir.string() + 
                                        "' is blocked by a file of the same name.  Please delete or rename this file and re-run." );
                return Err::CANNOT_CREATE;
            }
            
            // Copy master config file here if missing
            path = mConfigDir.string() + CMakeVar::DEFAULT_CONFIG_FILENAME;
            if (!fs::exists( path ))
            {
                result = CopyUserConfigFile();
                if (result != Err::OK)
                {
                    gLog.Write( Log::ERROR, "Failed to create user configuration file.  Check your file permissions." );
                    return result;
                }
            }
            else
            {
                // Check if config file is actually a file
                if (!fs::is_regular_file( path ))
                {
                    gLog.Write( Log::ERROR, "User configuration file is not a file." );
                    return Err::FILE_NOT_FOUND;
                }
            }
        }

        // Check if user profile dir exists
        if (!fs::exists(mProfileDir))
        {
            // Create user profile directory if it does not exist
            gLog.Write( Log::DEBUG, FUNC_NAME, "User profile directory '" + mProfileDir.string() + "' does not exist." );
            result = CreateUserProfileDir();
            if (result != Err::OK)
            {
                gLog.Write( Log::ERROR, "Failed to create user profile directory.  Check your file permissions." );
                return result;
            }
        }
        else
        {
            // Check that profile dir is actually a directory
            if (!fs::is_directory(mProfileDir))
            {
                gLog.Write( Log::ERROR, "User profile directory is not a directory." );
                return Err::DIR_NOT_FOUND;
            }
        }

        // Copy default profiles to user profile directory if they do not
        // already exist.  Does not replace old / modified files.
        result = CopyUserProfileFiles();
        if (result != Err::OK)
        {
            gLog.Write( Log::ERROR, "Failed to create user profile files.  Check your file permissions." );
            return result;
        }
        
    }
    else
    {
        // No user home, try to use system-wide dir in /etc/opensd/
        gLog.Write( Log::WARN, "User has no $HOME directory, falling back to system-wide configuration." );
        if (!HasSystemConfig())
        {
            // No user config or system config. Cannot continue
            gLog.Write( Log::ERROR, "No system-wide configuration found.  Unable to load configuration." );
            mConfigDir.clear();
            mProfileDir.clear();
        }
        else
        {
            // Use system config settings instead of per-user config
            mConfigDir  = CMakeVar::SYSTEM_CONFIG_DIR;
            mProfileDir = CMakeVar::SYSTEM_PROFILE_DIR;
            mIsConfigDirWritable = false;
            mIsProfileDirWritable = false;
            gLog.Write( Log::DEBUG, FUNC_NAME, "Setting config directory to '" + mConfigDir.string() + "'" );
            gLog.Write( Log::DEBUG, FUNC_NAME, "Setting profile directory to '" + mProfileDir.string() + "'" );
        }
    }
    
    if (mConfigDir.empty() || mProfileDir.empty())
        return Err::ENVIRONMENT_ERROR;
    
    // TODO: get initial list of profiles
    
    return Err::OK;
}



std::filesystem::path FileMgr::GetConfigFilePath()
{
    return mConfigDir.string() + CMakeVar::DEFAULT_CONFIG_FILENAME;
}



std::vector<std::string> FileMgr::GetProfileList()
{
    namespace                   fs = std::filesystem;
    std::vector<std::string>    list;
    
    
    if (mProfileDir.empty())
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Profile directory is not set." );
        return {};
    }
    
    if ((!fs::exists( mProfileDir ) || (!fs::is_directory( mProfileDir ))))
    {
        gLog.Write( Log::ERROR, "Profile directory does not exist." );
        return {};
    }
    
    gLog.Write( Log::DEBUG, FUNC_NAME, "Scanning for profiles..." );
    for (auto const& p : fs::directory_iterator( mProfileDir ))
    {
        list.push_back( p.path().filename().string() );
        gLog.Write( Log::VERB, "Found profile: " + list.back() );
    }
    
    return list;
}



std::filesystem::path FileMgr::GetProfileFilePath( std::string fileName )
{
    namespace           fs = std::filesystem;
    fs::path            path;
    
    if (fileName.empty())
    {
        gLog.Write( Log::DEBUG, "filename parameter is empty." );
        return "";
    }
    
    // Turn filename into a full path
    path = mProfileDir.string() + fileName;
    if (!path.has_extension())
        path += ".profile";
    
    // Check if profile exists
    if ((!fs::exists( path )) || (!fs::is_regular_file( path )))
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Profile '" + path.string() + "' does not exist." );
        return "";
    }
    
    // return path to profile if found
    return path;
}
