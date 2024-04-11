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

#include "runner.hpp"
#include "../common/log.hpp"
// Linux
#include "sys/wait.h"

// Global instance
Runner gRunner;


void Runner::Daemon()
{
    int             result;
    int             status;
    
    // Loop this thread
    while (true)
    {
        // Don't need to check too frequently
        usleep( 100000 );

        std::lock_guard<std::mutex>     loop_lock( mLoopMutex );
        if (!mIsRunning)
            break;            

        // Lock access to ProcList since Exec will be called by other threads
        std::lock_guard<std::mutex>     cmd_lock( mCmdMutex );
        if (mProcList.size())
        {
            // Loop through list of PIDs and delete entries which have 
            for (auto i = mProcList.begin(); i != mProcList.end();)
            {
                result = waitpid( i->pid, &status, WNOHANG );
                if (result < 0)
                {
                    int e = errno;
                    gLog.Write( Log::DEBUG, FUNC_NAME, "Call to waitpid() failed: " + Err::GetErrnoString(e) );
                    gLog.Write( Log::WARN, "Failed to get status of child PID " + std::to_string(i->pid) );
                }
                else
                {
                    if (result > 0)
                    {
                        // Process has terminated so we can erase the element
                        gLog.Write( Log::VERB, FUNC_NAME, "Child process terminated (" + std::to_string(i->pid) + ")." );
                        i = mProcList.erase(i);
                    }
                    else
                    {
                        // If we don't delete the element, increment our iterator
                        ++i;
                    }
                }
            }  
        }
    }
    
    // Done
    gLog.Write( Log::VERB, FUNC_NAME, "Exiting wait() loop." );
}



int Runner::Exec( std::string command, uint32_t bindingId )
{
    int             result;
    
    if (!mIsRunning)
    {
        gLog.Write( Log::ERROR, "Runner thread has not been started." );
        return Err::NOT_INITIALIZED;
    }
    
    if (!command.size())
    {
        gLog.Write( Log::DEBUG, FUNC_NAME, "Command string is empty." );
        return Err::INVALID_PARAMETER;
    }
    
    // Lock ProcList
    std::lock_guard<std::mutex>     lock( mCmdMutex );
    
    // If binding ID is non-zero, prevent command from running if there's already and active pid
    if (bindingId)
        for (auto i : mProcList)
            if (i.bid == bindingId)
                return Err::ALREADY_OPEN;

    gLog.Write( Log::VERB, "Running command: '" + command + "'..." );
    
    result = fork();
    if (result == 0)
    {
        // Child process
        
        // Execute command and terminate child process
        execl("/bin/sh", "sh", "-c", command.c_str(), (char*)0);
        exit(0);
    }
    else if (result > 0)
    {
        // Parent process
        
        // Add bid/pid to list
        ProcInfo        pi = { .pid = result, .bid = bindingId };
        mProcList.push_back(pi);
        gLog.Write( Log::VERB, FUNC_NAME, "Child process created (" + std::to_string(result) + ")." );
    }
    else if (result < 0)
    {
        // fork() error
        int e = errno;
        gLog.Write( Log::DEBUG, FUNC_NAME, "Failed to create child process: " + Err::GetErrnoString(e) );
        gLog.Write( Log::WARN, "Failed to execute command binding because fork() did not succeed." );
        return Err::CANNOT_CREATE;
    }
    
    return Err::OK;
}



void Runner::Start()
{
    std::lock_guard<std::mutex>     lock( mLoopMutex ); 
    if (!mIsRunning)
    {
        gLog.Write( Log::VERB, FUNC_NAME, "Starting runner thread..." );
        mThread = std::thread( &Runner::Daemon, this );
    }
    else
        gLog.Write( Log::ERROR, "Cannot start runner daemon: Daemon is already running." );
}



void Runner::Stop()
{
    gLog.Write( Log::VERB, "Shutting down runner daemon..." );

    std::lock_guard<std::mutex>     lock( mLoopMutex );
    mIsRunning = false;

    mThread.join();

    if (!mProcList.empty())
    {
        std::string     str;
        int             status;
        
        for (auto& i : mProcList)
            str += std::to_string(i.pid) + " ";
        
        gLog.Write( Log::DEBUG, "Child PIDs: " + str + " are still running." );
        gLog.Write( Log::INFO, "Waiting for child process(es) to exit..." );
        
        // Wait for all child processes to exit
        for (auto& i : mProcList)
            waitpid( i.pid, &status, 0 );
    }
    
    gLog.Write( Log::VERB, "Runner daemon terminated." );
}



Runner::Runner()
{
    mIsRunning = false;
}



Runner::~Runner()
{
    Stop();
}
