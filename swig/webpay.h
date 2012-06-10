/*****************************************************************************
 *								webpay.h
 *
 * This is the Webpay API header file for webpay.c
 * 
 *****************************************************************************/
#define BOOL size_t
#define true 1
#define false 0
#define LPCTSTR char *

#ifndef WEBPAY_H
#define WEBPAY_H

void * newBundle();
void   put( void * txn, LPCTSTR name, LPCTSTR value );
void   put_ClientID( void * txn, LPCTSTR newVal );
void   put_CertificatePath( void * txn, LPCTSTR newVal );
void   put_CertificatePassword( void * txn, LPCTSTR newVal );
void   setPort( void * txn, LPCTSTR Port );
void   setServers( void * txn, LPCTSTR ServerList );
char * executeTransaction( void * txn );
char * get( void * txn, LPCTSTR name );
void   cleanup( void * txn );
void * flushBundle( void * txn );
void   checkSymbolLoaded();
void   debug_logging(const char * format, ... );

typedef void * newBundle_t ( void );
typedef void   put_t ( void * txn, LPCTSTR name, LPCTSTR value );
typedef void   put_ClientID_t ( void * txn, LPCTSTR newVal );
typedef void   put_CertificatePath_t ( void * txn, LPCTSTR newVal );
typedef void   put_CertificatePassword_t ( void * txn, LPCTSTR newVal );
typedef void   setPort_t ( void * txn, LPCTSTR Port );
typedef void   setServers_t ( void * txn, LPCTSTR ServerList );
typedef BOOL   execute_t ( void * txn );
typedef char * get_t ( void * txn, LPCTSTR name );
typedef void * flushBundle_t ( void * txn );
typedef void   cleanup_t ( void * txn );

#endif

