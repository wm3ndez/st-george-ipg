/*****************************************************************************
 *								webpay.i
 *
 * This is the webpay SWIG interface file for webpay.
 * This file lists those methods that will be available to SWIG supported 
 * languages.
 * 
 *****************************************************************************/

%module webpay
%{
#define DWORD unsigned long
#define BOOL char
#define true 1
#define false 0
#define LPCTSTR char *

extern void * newBundle ( void );
extern void   put ( void * txn, char * name, char * value );
extern void   setPort ( void * txn, char * Port );
extern void   setServers ( void * txn, char * ServerList );
extern void   put_ClientID ( void * txn, char * newVal );
extern void   put_CertificatePath ( void * txn, char * newVal );
extern void   put_CertificatePassword ( void * txn, char * newVal );
extern char * executeTransaction( void * txn );
extern char * get ( void * txn, char * value );
extern void   cleanup ( void * txn );
extern void * flushBundle ( void * txn );

%}

/* Information in the following section is updated automatically during the Make process. This information will then be displayed in phpinfo() if the extension is loaded via the php.ini file */
%pragma(php) phpinfo="
  php_info_print_table_start();
  php_info_print_table_header(2, \"Comment\", \"Value\");
  php_info_print_table_row(2, \"Client Type\", \"__CLIENT_TYPE__\");
  php_info_print_table_row(2, \"Webpay PHP Library\", \"__WEBPAY_PHP_LIB_NAME__\");
  php_info_print_table_row(2, \"Webpay PHP Library Version\", \"__VERSION__\");
  php_info_print_table_row(2, \"Webpay Core Library\", \"__CORE_LIB_NAME__\");
  php_info_print_table_row(2, \"Webpay Core Library Version\", \"__CORE_LIB_VERSION__\");
  php_info_print_table_row(2, \"Operating Environment Detected\", \"__OPERATING_ENVIRONMENT__\");
  php_info_print_table_end();
"


extern void * newBundle ( void );
extern void   put ( void * txn, char * name, char * value );
extern void   setPort ( void * txn, char * Port );
extern void   setServers ( void * txn, char * ServerList );
extern void   put_ClientID ( void * txn, char * newVal );
extern void   put_CertificatePath ( void * txn, char * newVal );
extern void   put_CertificatePassword ( void * txn, char * newVal );
extern char * executeTransaction( void * txn );
extern char * get ( void * txn, char * value );
extern void   cleanup ( void * txn );
extern void * flushBundle ( void * txn );

