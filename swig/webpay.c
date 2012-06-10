/******************************************************************************
                          webpay.c - description
                             -------------------
    begin                : Wed 30th Jul 2003
    copyright            : (C) 2003-2008. FIS Australasia Pty Ltd
 *****************************************************************************/
// webpay.c
//
// Revision History
// ================
// Date	        Developer       Revision Description
//-----------------------------------------------------------------------------
// 31/07/2003   Tod Lavis       File Cleanup
// 04/08/2003   Tod Lavis       executeTransaction now returns Char * rather
//                              than BOOL.
// 11/08/2003   Tod Lavis       Add ClientType and Version Data to TxnBundle
//                              during bundle creation (newBundle()).
// 14/08/2003   Tod Lavis       Created constructor and destructor methods.
// 27/04/2004   Tod Lavis       Now using RTLD_NOW. Open webpay_client_lib every
//                              time.
// 28/10/2008   Tim Deboer      Changed version and copyright. 
// 11/11/2008   Tim Deboer		Copied this file from PHP project and moved it into the SWIG project

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <dlfcn.h>
#include "webpay.h"

#define WEBPAY_CLIENT_LIB "libwebpayclient.so"
#define THE_CLIENT_TYPE   "__CLIENT_TYPE__"
#define THE_VERSION       "__VERSION__"

// Handle to the Webpay Client Library
void * handle;

//
// Wrapper Methods for Webpay Functions.
//
newBundle_t                 *wrap_newBundle;
put_t                       *wrap_put;
put_ClientID_t              *wrap_put_ClientID;
put_CertificatePath_t       *wrap_put_CertificatePath;
put_CertificatePassword_t   *wrap_put_CertificatePassword;
setPort_t                   *wrap_setPort;
setServers_t                *wrap_setServers;
execute_t                   *wrap_execute;
get_t                       *wrap_get;
cleanup_t                   *wrap_cleanup;
flushBundle_t               *wrap_flushBundle;


//
// Constructor : runs before dlopen returns.
//
//
// Dynamically open the Webpay client library and retrieve a handle to all the
// Webpay relevant symbols.
//

__attribute__ ((constructor)) void init()
{
	debug_logging("Starting init().\n");
		
	// Dynamically load the Webpay client library.
	handle = dlopen(WEBPAY_CLIENT_LIB, RTLD_NOW );
	if(!handle) {
		fprintf(stderr, "\r\n--------error while opening file [%s]--------\r\n", WEBPAY_CLIENT_LIB);
		debug_logging("\r\n--------error while opening file [%s]--------\r\n", WEBPAY_CLIENT_LIB);
	    debug_logging(dlerror());
		debug_logging("\r\nreturning [%s]\r\n\r\n","false");
	    return;
	}
	// Take a "handle" from the dynamic library and the symbol name.
	wrap_cleanup = (cleanup_t*)dlsym(handle, "cleanup");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_execute = (execute_t*)dlsym(handle, "execute");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_flushBundle = (flushBundle_t*)dlsym(handle, "flushBundle");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_get = (get_t*)dlsym(handle, "get");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_newBundle = (newBundle_t*)dlsym(handle, "newBundle");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_put = (put_t*)dlsym(handle, "put");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_put_ClientID = (put_ClientID_t*)dlsym(handle, "put_ClientID");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_put_CertificatePassword = (put_CertificatePassword_t*)dlsym(handle, "put_CertificatePassword");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_put_CertificatePath = (put_CertificatePath_t*)dlsym(handle, "put_CertificatePath");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_setPort = (setPort_t*)dlsym(handle, "setPort");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	wrap_setServers = (setServers_t*)dlsym(handle, "setServers");
	checkSymbolLoaded();	//Log a msg if symbol is not loaded correctly.

	debug_logging("init returning [%s]\n","true");
	return;
	
}

//
// Destructor : runs before dlclose returns.
//
// Release the "handle" on the Webpay client library file.
//
__attribute__ ((destructor)) void fini()
{
	size_t ret;
//	fprintf(stderr, "\r\n-------- in fini --------\r\n");
	if(handle){ 
		ret = dlclose(handle);
		if(ret == true){
			debug_logging( "dlcose returned: [%d] lib closed\n", ret);
			handle = 0;
		}
	} else {
		debug_logging( "Nothing to close.\r\n");
	}
}


//
// Listed below are all the wrapper functions for the symbols contained in the
// Webpay client library.
// Please refer to the webpay documentation for more details.
//

void * newBundle()
{
	void * ret;
	if(wrap_newBundle == NULL){
		// Method not available
		fprintf(stderr, " wrap_newBundle not available \r\n");
		return NULL;
	}
	ret = wrap_newBundle();
	// Set the Client Type and the Version number in the Bundle
	put(ret, "CLIENTTYPE", THE_CLIENT_TYPE);
	put(ret, "VERSION", THE_VERSION);
	
	debug_logging("Calling newBundle [%d]\n",ret);
	return ret;
}

void put(void * webpayRef, LPCTSTR name, LPCTSTR value)
{
	debug_logging("put: [%d] %s=%s\n",webpayRef,name,value);
	if(wrap_put == NULL){
		// Method not available
		fprintf(stderr, " wrap_put not available \r\n");
		return;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during Put: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return;
	}
	wrap_put(webpayRef,name,value);
}

void put_ClientID(void * webpayRef, LPCTSTR newVal)
{
	debug_logging("put_ClientID: [%d] %s\n",webpayRef,newVal);
	if(wrap_put_ClientID == NULL){
		// Method not available
		fprintf(stderr, "wrap_put_ClientID not available \r\n");
		return;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during put_ClientID: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return;
	}
	wrap_put_ClientID(webpayRef,newVal);
}

void put_CertificatePath(void * webpayRef, LPCTSTR newVal)
{
	debug_logging("put_CertificatePath: [%d] %s\n",webpayRef,newVal);
	if(wrap_put_CertificatePath == NULL){
		// Method not available
		fprintf(stderr, "wrap_put_CertificatePath not available \r\n");
		return;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during put_CertificatePath: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return;
	}
	wrap_put_CertificatePath(webpayRef,newVal);
}

void put_CertificatePassword(void * webpayRef, LPCTSTR newVal)
{
	debug_logging("put_CertificatePassword: [%d] %s\n",webpayRef,newVal);
	if(wrap_put_CertificatePassword == NULL){
		// Method not available
		fprintf(stderr, "wrap_put_CertificatePassword not available \r\n");
		return;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during put_CertificatePassword: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return;
	}
	wrap_put_CertificatePassword(webpayRef,newVal);
}

void setPort(void * webpayRef, LPCTSTR Port)
{
	debug_logging("setPort: [%d] %s\n",webpayRef,Port);
	if(wrap_setPort == NULL){
		// Method not available
		fprintf(stderr, "wrap_setPort not available \r\n");
		return;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during setPort: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return;
	}
	wrap_setPort(webpayRef,Port);
}

void setServers(void * webpayRef, LPCTSTR ServerList)
{
	debug_logging("setServers: [%d] %s\n",webpayRef,ServerList);
	if(wrap_setServers == NULL){
		// Method not available
		fprintf(stderr, "wrap_setServers not available \r\n");
		return;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during setServers: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return;
	}
	wrap_setServers(webpayRef,ServerList);
}

// NB this function has been renamed executeTransaction to prevent a 
// namespace clash with execute in the Zend module.
char * executeTransaction( void * webpayRef )
{
	BOOL ret;
	debug_logging("Execute: [%d]\n",webpayRef);
	if(wrap_execute == NULL){
		// Method not available
		fprintf(stderr, "wrap_execute not available \r\n");
		return "false";
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during executeTransaction: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return "false";
	}
	ret = wrap_execute(webpayRef);
	debug_logging("Execute returned: [%d]\n",ret);
	if (ret == true){
		return "true";		
	} else {
		return "false";
	}
}

char * get(void * webpayRef, LPCTSTR name)
{
	char * value;
	if(wrap_get == NULL){
		// Method not available
		fprintf(stderr, "wrap_get not available \r\n");
		fprintf(stderr, "wrap_get not available \r\n");
		fprintf(stderr, "Error: While trying get(). Check init() and the location of the Webpay client .so file : [%s]\n", WEBPAY_CLIENT_LIB);
		return "Error: While trying get(). Check init() and the location of the Webpay client .so file";
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during get: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return "Error: while trying get(): webpayRef is Null, check value returned from NewBundle is valid.";
	}
	
	debug_logging("get: [%d] %s\n",webpayRef,name);
	value = wrap_get(webpayRef,name);
	if(value == NULL){
		value = "";
	}
	debug_logging("Get returning: [%d]: %s\n",webpayRef,value);
	return value;
}

void cleanup( void * webpayRef )
{
	debug_logging("Cleanup: [%d]\n",webpayRef);
	if(wrap_cleanup == NULL){
		// Method not available
		fprintf(stderr, "wrap_cleanup not available \r\n");
		return;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during cleanup: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return;
	}
	wrap_cleanup(webpayRef);
}

void * flushBundle(void * webpayRef)
{
	debug_logging("flushBundle: [%d]\n",webpayRef);
	if(wrap_flushBundle == NULL){
		// Method not available
		fprintf(stderr, "wrap_flushBundle not available \r\n");
		return NULL;
	}
	if(webpayRef == NULL){
		fprintf(stderr, "Error during flushBundle: webpayRef is null. Ensure that newBundle returns valid webpayRef. \r\n");
		return NULL;
	}
	return wrap_flushBundle(webpayRef);
}

//
// This method should be called after attempting to load a symbol via dlsym
// In the event that the symbol can not be loaded an error message will be 
// logged.
//
void checkSymbolLoaded(){
	char * dlsymError = dlerror();
	if(dlsymError){
		fprintf(stderr, "Error : [%s]\r\n", dlsymError);
	}
}

//
// To disable logging place a return statement at the top of this function.
// To enable logging ensure that the first line of this function is not a 
// return statement.
//
void debug_logging(const char * message, ... )
{
	return;
	va_list args;
	va_start(args, message);
		vfprintf( stderr, message, args );
	va_end(args);
}

