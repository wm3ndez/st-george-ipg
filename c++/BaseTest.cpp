/***************************************************************************
                          BaseTest.cpp  -  description
                             -------------------
    begin                : Tue Jan 9 2001
    copyright            : (C) 2001-2008. FIS Australasia Pty Ltd
 ***************************************************************************/

/***************************************************************************

	This sample application has been provided to illustrate how to 
	communicate with the Payment Gateway. Please see the developers guide
	for more details.

 ***************************************************************************/

#include "webpayclient.h"
#include "BTypes.h"

// prototypes
void sendTransaction( int argc, char *argv[] );
boolean doStatusCheck(void * bundle);
boolean approvedTransaction(char * responseCode);
void displayResults(void * bundle);

int main( int argc, char *argv[] )
{
	int status = 0;
	if( argc < 8 )
	{
		printf( "\r\nNative Webpay Client Test\r\n" );
		printf( "=========================\r\n" );
		printf( "usage: BaseTest.exe <serverlist> <port> <clientid> <cert path> " );
		printf( "<cert pass> <ca_file> <num_iters> {\"debug\" or \"no debug\"}>\r\n" );
		status = 1;
	}
	else
	{
		// send a repeated number of transactions
		int i, numIters = atoi( argv[7] );
		for ( i = 0; i < numIters; i++ )
		{
			sendTransaction( argc, argv );
		}

		// free the webpay client resources
		free_client( );
	}

	return 0;
}

// send a transaction given the specified parameters
void sendTransaction( int argc, char *argv[] )
{
	// Create a new context/bundle
	// A void pointer is used here as this application only needs to hold a reference, 
	// and does not need to know any of it's specifics.
	void *bundle = newBundle();
	
	//Add the client type and the version to the bundle
	put(bundle, "CLIENTTYPE", THE_CLIENT_TYPE);
	put(bundle, "VERSION", THE_VERSION);

	printf( "Arguments: %d\r\n", argc );

	if ( argc == 9 )
	{
		printf( "Debug set to: [%s]\r\n", argv[8] );
		if ( stricmp( argv[8], "debug" ) == 0 )
		{
			printf( "Debug ON\r\n" );
			put ( bundle, "DEBUG", "ON" );
			put ( bundle, "LOGFILE", "webpay.log" );
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

	put_ClientID( bundle, argv[3]);

	// Set security related parameters
	put_CertificatePath( bundle, argv[4] );
	put_CertificatePassword( bundle, argv[5] );
	put( bundle, "_CAFILE", argv[6] );

	// Set the server address and port number
	setServers ( bundle, argv[1] );
	setPort ( bundle, argv[2] );

	// Set the transaction's parameters.
	// These vary between transaction types and are subject
	// to change with notice, as new types are added.
	put ( bundle, "CARDDATA", "4564456445644564" );
	put ( bundle, "CARDEXPIRYDATE", "0215" );
	put ( bundle, "DATA", "Example Transaction" );

	put ( bundle, "INTERFACE", "CREDITCARD" );
	put ( bundle, "TRANSACTIONTYPE", "PURCHASE" );

	put ( bundle, "TOTALAMOUNT", "10.00" );
	put ( bundle, "TAXAMOUNT", "1.00" );

	// Attempt to execute the transaction request...
	if ( execute ( bundle ) ) {
		// If the execute method returns successfully this indicates 
		// that communication with the Payment Gateway has been successful.
		// A further test of the Response Code and Response Text will be 
		// required to determine if the Payment has been successfully 
		// Authorised. Please see the developers guide for more details.
		
		printf ( "Successfully communicated with the WTS.\r\n" );
			
	} else {
		// There has been a problem during the execute call.

		// Log message.
		printf ( "Unable to communicate with the WTS.\r\n" );

		//Try transaction recovery
		char *transactionRef = get( bundle, "TXNREFERENCE" );
		if (transactionRef)
		{
			//We have a transaction reference so attempt a status transaction.
			printf("Performing status check with Transaction Ref = [%s]\n", transactionRef);
			if(doStatusCheck(bundle))
			{
			   printf ( "Status Check Successful - Details are displayed below.\r\n" );
			} else {
				printf ( "Status check failed: Unknown transaction status.\nPlease wait a short while and try status check again using Transaction Ref [%s].\r\n", transactionRef );
			}
		}
		else
		{
			// There is no transaction reference number so the transaction has failed completely.
			// It can be safely reprocessed.
			printf("The transaction can be safely reprocessed as no Transaction Reference Number exists.\n");
		}
		
	}

	// Get the responses and display them...
	displayResults(bundle);

	cleanup ( bundle );
}

boolean doStatusCheck(void * bundle)
{
	char *transactionRef = get( bundle, "TXNREFERENCE" );
	if (transactionRef)
	{
		//We have a transaction reference so attempt a status transaction.
		put ( bundle, "TRANSACTIONTYPE", "STATUS" );
		return execute ( bundle );
	}
	else
	{
		//No txnref number so we can not do a status check.
		return false;
	}

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
		listOfApprovedResponseCodes[2] = "77";	// Approved 
	
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
