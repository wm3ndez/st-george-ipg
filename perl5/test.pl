#!/usr/bin/perl

use webpay_perl;
use strict;

#
# Declare constants here...
#    modify these for your own use.
#
use constant THE_CLIENT_ID => "10000000";
use constant THE_CERT_PATH => "test.p12";
use constant THE_CERT_PASSWORD => "webpay";
use constant THE_PORT => "3007";
use constant THE_SERVER => "192.0.0.1";
#
use constant TRUE  => "1";
use constant FALSE => "0";
use constant NUM_TRANS => "1";

doWebpayTransaction();

sub doWebpayTransaction { 
	
	for (my $loop_index = 1; $loop_index <= NUM_TRANS; $loop_index++) {
		print "-------------------------------------------------\n";
		print "Doing transaction number $loop_index\n\n";
		my $webpayRef = webpay_perl::newBundle();
		webpay_perl::put($webpayRef, "DEBUG", "ON");
		webpay_perl::put($webpayRef, "LOGFILE", "webpay.log");
		webpay_perl::put_ClientID ($webpayRef, THE_CLIENT_ID);
		webpay_perl::put_CertificatePath ($webpayRef, THE_CERT_PATH);
		webpay_perl::put_CertificatePassword ($webpayRef, THE_CERT_PASSWORD);
		webpay_perl::setPort ($webpayRef, THE_PORT);
		webpay_perl::setServers ($webpayRef, THE_SERVER);
		webpay_perl::put($webpayRef, "TOTALAMOUNT", "10.00");
		webpay_perl::put($webpayRef, "CARDDATA", "4564456445644564");
		webpay_perl::put($webpayRef, "CARDEXPIRYDATE", "1515");
		webpay_perl::put($webpayRef, "COMMENT", "Testing the webpayPerl api: test.pl");
		webpay_perl::put($webpayRef, "INTERFACE", "CREDITCARD");
		webpay_perl::put($webpayRef, "TRANSACTIONTYPE", "PURCHASE");
	
		my $tranProcessed = webpay_perl::executeTransaction( $webpayRef );
		if ($tranProcessed eq "true") {
			# If the execute method returns successfully this indicates 
			# that communication with the Payment Gateway has been successful.
			# A further test of the Response Code and Response Text will be 
			# required to determine if the Payment has been successfully 
			# Authorised. Please see the developers guide for more details.
			print "Successfully communicated with the WTS.\n";


	      
	   } else {
			# There has been a problem during the execute call.
			
			# Log message.
			print "Unable to communicate with the WTS.\n";
			
			# Try transaction recovery

			my $txnRef = webpay_perl::get( $webpayRef, "TXNREFERENCE");
			
			if($txnRef){
				# We have a transaction reference so attempt a status transaction.
				print "Performing status check with Transaction Ref = [$txnRef]\n";

				if(doStatusCheck($webpayRef)){
				   print "Status Check Successful - Details are displayed below.\r\n";
				} else {
					printf "Status check failed: Unknown transaction status.\nPlease wait a short while and try status check again using Transaction Ref [$txnRef].\r\n" ;
				}
			} else {
				# There is no transaction reference number so the transaction has failed completely.
				# It can be safely reprocessed.
				print ("The transaction can be safely reprocessed as no Transaction Reference Number exists.\n");
			}
		
			
	   }
	   displayResults($webpayRef);
	   webpay_perl::cleanup( $webpayRef );
	}

	# Free client
	#webpay_perl::free_client();
	print "\nGoodbye\n\n\n";
}


sub doStatusCheck {
	
	my $webpayRef = $_[0]; 

	my $txnRef = webpay_perl::get( $webpayRef, "TXNREFERENCE");
	
	if ($txnRef) {
		#We have a transaction reference so attempt a status transaction.
		webpay_perl::put( $webpayRef, "TRANSACTIONTYPE", "STATUS" );
		return webpay_perl::executeTransaction( $webpayRef );
	} else {
		# No txnref number so we can not do a status check.
		return FALSE;
	}
}

sub displayResults {

	my $webpayRef = $_[0]; 
	my $txnRef = webpay_perl::get( $webpayRef, "TXNREFERENCE");
	my $result = webpay_perl::get( $webpayRef, "RESULT");
	my $authCode = webpay_perl::get( $webpayRef, "AUTHCODE");
	my $responseText = webpay_perl::get( $webpayRef, "RESPONSETEXT");
	my $responseCode = webpay_perl::get( $webpayRef, "RESPONSECODE");
	my $error = webpay_perl::get( $webpayRef, "ERROR");

	print "--------------------------------------\n";
	if ((approvedTransaction($responseCode)) eq TRUE) {
		print ("-------- TRANSACTION APPROVED --------\n");
	} else {
		print ("------ TRANSACTION NOT APPROVED ------\n");
	}
	print "--------------------------------------\n";
	print ( "Transaction Reference\t : [$txnRef]\r\n");
	print ( "Result\t\t\t : [$result]\r\n");
	print ( "Auth Code\t\t : [$authCode]\r\n");
	print ( "Response Text\t\t : [$responseText]\r\n");
	print ( "Response Code\t\t : [$responseCode]\r\n");
	print ( "Error Message\t\t : [$error]\r\n\n");
	print "--------------------------------------\n";
}


sub approvedTransaction {

	my $responseCode = $_[0]; 
	
	#
	# Check the returned response Code against the list of known Approved Response Codes
	#
	# Please check the documentation to ensure that you have the 
	# latest list of approved response codes.
	#

	if ($responseCode)
	{
		my @listOfApprovedResponseCodes;
		my $count = 0;
		my $testThisCode;
		$count = push (@listOfApprovedResponseCodes, "00"); # Transaction Approved
		$count = push (@listOfApprovedResponseCodes, "08"); # Approved Signature
		for(my $i = 0; $i < $count; $i++)
		{
			$testThisCode = pop(@listOfApprovedResponseCodes);
			if( $responseCode eq $testThisCode){
				# we have a match
				return TRUE;
			}
		}
	}
	return FALSE;
}
