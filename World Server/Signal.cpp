/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://www.osrose.net
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    depeloped with Main erose/hrose source server + some change from the original eich source        
*/
#include "worldserver.h"
/*
/   from signal.h
 * NOTE: SIGINT is produced when the user presses Ctrl-C.
 *       SIGILL has not been tested.
 *       SIGFPE doesn't seem to work?
 *       SIGSEGV does not catch writing to a NULL pointer (that shuts down
 *               your app; can you say "segmentation violation core dump"?).
 *       SIGTERM comes from what kind of termination request exactly?
 *       SIGBREAK is indeed produced by pressing Ctrl-Break.
 *       SIGABRT is produced by calling abort.
*/

void StartSignal( )
{
    signal( SIGINT, HandleSignal );
    signal( SIGILL, HandleSignal );
    signal( SIGFPE, HandleSignal );        
    signal( SIGSEGV, HandleSignal );
    signal( SIGTERM, HandleSignal );        
    signal( SIGBREAK, HandleSignal );        
    signal( SIGABRT, HandleSignal );          
}

void StopSignal( )
{
    signal( SIGINT, 0 );
    signal( SIGILL, 0 );
    signal( SIGFPE, 0 );        
    signal( SIGSEGV, 0 );
    signal( SIGTERM, 0 );        
    signal( SIGBREAK, 0 );        
    signal( SIGABRT, 0 );          
}

void HandleSignal( int num )
{
    switch(num)
    {
        case SIGINT:/* Interactive attention */        
            Log( MSG_ERROR, "Signal received: SIGINT" );                      
    	    #ifdef _WIN32
            Sleep(1000);
    	    #else
    	    usleep(1000);
    	    #endif
    	    delete GServer;
            raise(num);
        break;        
        case SIGILL:/* Illegal instruction */
            Log( MSG_ERROR, "Signal received: SIGILL, Server will be closed, Trying to save..." );                          
            GServer->DisconnectAll();            
    	    #ifdef _WIN32
                Sleep(1000);
    	    #else
    	    usleep(1000);
    	    #endif
      	    delete GServer;
            raise(num);          
        break;        
        case SIGFPE:/* Floating point error */
            Log( MSG_ERROR, "Signal received: SIGFPE, Server will be closed, Trying to save..." );                          
            GServer->DisconnectAll();            
            delete GServer;
            raise(num);                     
        break;        
        case SIGSEGV:/* Segmentation violation */
            Log( MSG_ERROR, "Signal received: SIGSEGV, Server will be closed, Trying to save..." );                          
            GServer->DisconnectAll();
    	    #ifdef _WIN32
                Sleep(1000);
    	    #else
    	    usleep(1000);
    	    #endif            
    	    delete GServer;
            raise(num);                     
        break;                       
                                              
        case SIGTERM:/* Termination request */
            Log( MSG_INFO, "Signal received: SIGTERM, Server will be closed" );
            pthread_create( &GServer->WorldThread[SHUTDOWN_THREAD], NULL, (void *(__cdecl *)(void *))ShutdownServer, (PVOID)0);         
        break;   
    	#ifdef _WIN32
        case SIGBREAK:/* Control-break */
            Log( MSG_INFO, "Signal received: SIGBREAK, Server will be closed" );        
            pthread_create( &GServer->WorldThread[SHUTDOWN_THREAD], NULL, (void *(__cdecl *)(void *))ShutdownServer, (PVOID)0);    
        break;     
    	#endif          
        default:
            cout << "Unknown Signal Number: " << num << endl;
            raise(num);
    }
}