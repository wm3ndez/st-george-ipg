
#include "webpayclient.h"
#include "BTypes.h"

// prototypes
void checkTransaction( int argc, char *argv[] );
boolean approvedTransaction(char * responseCode);
void displayResults(void * bundle);

int main( int argc, char *argv[] )
{
	int status = 0;
	if( argc < 7 )
	{
		printf( "\r\nNative Webpay Status Check\r\n" );
		printf( "=========================\r\n" );
		printf( "usage: StatusCheck.exe <serverlist> <port> <clientid> <cert path> " );
		printf( "<cert pass> <txnref> {\"debug\" or \"no debug\"}>\r\n" );
		status = 1;
	}

	else
	{
		checkTransaction( argc, argv );
		// free the webpay client resources
		free_client( );
	}

	return 0;
}

// send a transaction given the specified parameters
void checkTransaction( int argc, char *argv[] )
{
	// Create a new context/bundle
	// A void pointer is used here as this application
	// only needs to hold a reference, and does not need
	// to know any of it's specifics.
	void *bundle = newBundle();
	
	//Add the client type and the version to the bundle
	put(bundle, "CLIENTTYPE", THE_CLIENT_TYPE);
	put(bundle, "VERSION", THE_VERSION);

	printf( "Arguments: %d\r\n", argc );

	if ( argc == 8 )
	{
		printf( "Debug set to: [%s]\r\n", argv[7] );
		if ( stricmp( argv[7], "debug" ) == 0 )
		{
			printf( "Debug ON\r\n" );
			put ( bundle, "DEBUG", "ON" );
			put ( bundle, "LOGFILE", "statusCheck.log" );
		}
		else
		{
			printf( "Debug OFF\r\n" );
			put ( bundle, "DEBUG", "OFF" );
		}
	}
	else
	{
		printf( "Debug OFF\r\n" );
		put ( bundle, "DEBUG", "OFF" );
	}

	// Set security related parameters
	put_CertificatePath( bundle, argv[4] );
	put_CertificatePassword( bundle, argv[5] );

	// Set the server address and port number
	setServers ( bundle, argv[1] );
	setPort ( bundle, argv[2] );

	// Set the transaction's parameters.
	// These vary between transaction types and are subject
	// to change with notice, as new types are added.
	put ( bundle, "TXNREFERENCE", argv[6] );
	put ( bundle, "CLIENTID", argv[3] );
	put ( bundle, "INTERFACE", "CREDITCARD" );
	
	//Set the transaction type to STATUS
	put ( bundle, "TRANSACTIONTYPE", "STATUS" );

	// Attempt to execute the transaction request...
	if ( execute ( bundle ) ) {
	   printf ( "Successfully communicated with the WTS.\r\n" );
	} else {
		printf ( "Unable to communicate with the WTS.\r\n" );
	}

	// Get the responses and display them...
	displayResults(bundle);


	cleanup ( bundle );
}

void displayResults(void * bundle)
{
	printf ( "************************************************\n" );

	if(approvedTransaction(get( bundle, "RESPONSECODE")))
	{
		printf("************* TRANSACTION APPROVED *************\n");
	}
	else
	{
		printf("********** TRANSACTION NOT APPROVED ************\n");
	}
	
	printf ( "************************************************\n" );
	printf ( "Transaction Reference\t : [%s]\r\n", get( bundle, "TXNREFERENCE") );
	printf ( "Result\t\t\t : [%s]\r\n", get( bundle, "RESULT") );
	printf ( "Auth Code\t\t : [%s]\r\n", get( bundle, "AUTHCODE") );
	printf ( "Response Text\t\t : [%s]\r\n", get( bundle, "RESPONSETEXT") );
	printf ( "Response Code\t\t : [%s]\r\n", get( bundle, "RESPONSECODE") );
	printf ( "Error Message\t\t : [%s]\r\n\n", get( bundle, "ERROR") );
	printf ( "OpenSSL Version\t\t : [%s]\r\n", get( bundle, "_OPENSSL_VERSION") );
	printf ( "************************************************\n" );
}

boolean approvedTransaction(char * responseCode)
{
	//
	// Check the returned response Code against the list of known Approved Response Codes
	//
	// Please check the documentation to ensure that you have the 
	// latest list of approved response codes.
	//

	if (responseCode)
	{
		#define ARRAY_SIZE 3
		char * listOfApprovedResponseCodes[ARRAY_SIZE];
		listOfApprovedResponseCodes[0] = "00";	// Transaction Approved
		listOfApprovedResponseCodes[1] = "08";	// Approved Signature
		//listOfApprovedResponseCodes[2] = "77";	// Approved VIP //Not currently used.
	
		for(int i=0; i < ARRAY_SIZE;i++)
		{
			if( strcmpi ( responseCode ,  listOfApprovedResponseCodes[i]) == 0)
			{
				//we have a match
				return true;
			}
		}
	}
	return false;
	
}
