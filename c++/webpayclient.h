/***************************************************************************
                          webpayclient.h  -  description
                             -------------------
    begin                : Tue Jan 9 2001
    copyright            : (C) 2001-2008. FIS Australasia Pty Ltd. 
                                           All rights reserved.
 ***************************************************************************/
/***************************************************************************
                                                                       
   Contains declarations for "public" methods that user's 
   applications have access to.

 ***************************************************************************/


#ifndef __WEBPAYCLIENT_H__
#define __WEBPAYCLIENT_H__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "BPlatform.h"

// initialise the webpay client... this method can be called explicitly or it
// will be called automatically when the other methods are invoked
// returns true if the client was successfully initialised
export BOOL init_client( );

// free the resources of the webpay client... this method should be called when
// use of the webpay client is done to ensure that all system resources get freed
// returns true if nothing went wrong
export BOOL free_client( );

/* cleanup frees all memory used by constant structures and should be called
   before the application exits. */
export void cleanup ( void * txn );

// execute is used to send a transaction request to the servers after
// all parameters have been set.
export BOOL execute ( void * txn );

// flushBundle is used to re-create a context/bundle. All values, including
// server address, port, certificate password and path, and the licence key
// will need to be reset after calling this method.
export void * flushBundle ( void * txn );

// get is used to obtain values from the context/bundle by name.
export char * get ( void * txn, LPCTSTR name );

// newBundle is used to create a new context/bundle
export void * newBundle ( void );

// put sets values in the context/bundle by name
export void put ( void * txn, LPCTSTR name, LPCTSTR value );

// sets the Client ID in the context/bundle
export void put_ClientID ( void * txn, LPCTSTR newVal );

// sets the Certificate Password in the context/bundle
export void put_CertificatePassword ( void * txn, LPCTSTR newVal );

// sets the Certificate Path in the context/bundle
export void put_CertificatePath ( void * txn, LPCTSTR newVal );

// sets the Licence Key in the context/bundle
export void put_LicenceKey ( void * txn, LPCTSTR newVal );

// sets the server port in the context/bundle
export void setPort ( void * txn, LPCTSTR Port );

// sets the server address in the context/bundle
// currently only IP addresses are supported.
export void setServers ( void * txn, LPCTSTR ServerList );

// Declare the version information for this API
// These values are set during the make process
#define THE_CLIENT_TYPE   "__CLIENT_TYPE__"
#define THE_VERSION       "__VERSION__"

#endif
