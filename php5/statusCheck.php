<?php
Header("Cache-Control: no-cache");
$offset = 60 * 60 * 24 * 3;
$ExpireString = "Expires: " . gmdate("D, d M Y H:i:s", time() + $offset) . " GMT";
Header($ExpireString);
?>
<html>
<?php 
	//Define some constants
	define("THE_WEBPAY_PHP_LIB", "webpay_php.so");
	define("THE_INTERFACE", "CREDITCARD");

	//You may be required to change the following.
	define("THE_CLIENT_ID", "10000000");
	define("THE_CERT_PATH", "/test.cer");
	define("THE_CERT_PASSWORD", "webpay");
	define("THE_PORT", "3007");
	define("THE_SERVER", "192.0.0.1");

	//Transaction data
	define("THE_TRANSACTION_TYPE", "STATUS");
	define("THE_TXN_REF", "1000000000000097");

	//Use the dl function to load the Webpay PHP Library
	//so we can execute the functions within the library
	//Load library is a convenience method to load the webpay_php.so file
	//from the current directory. If it doesn't work due to some php 
	//restrictions change it back to dl(THE _WEBPAY_PHP_LIB)
 	load_library(THE_WEBPAY_PHP_LIB);

	print "<p> Perform Status check on Transaction using ";
	print THE_TXN_REF;
	print "</p>\n";

	//Create Webpay Transaction
 	$webpayRef = newBundle();
	if($webpayRef == NULL){
		exit("There has been an initialisation problem. Please check error log.<br></html>\n");
	}
	put($webpayRef, "DEBUG", "OFF");
	put($webpayRef, "LOGFILE", "statusCheck.log");
	put_ClientID ($webpayRef, THE_CLIENT_ID);
	put_CertificatePath ($webpayRef, THE_CERT_PATH);
	put_CertificatePassword ($webpayRef, THE_CERT_PASSWORD);
	setPort($webpayRef, THE_PORT);
	setServers($webpayRef, THE_SERVER);
	put($webpayRef, "INTERFACE", THE_INTERFACE);
	put($webpayRef, "TRANSACTIONTYPE", THE_TRANSACTION_TYPE);
	put($webpayRef, "TXNREFERENCE", THE_TXN_REF);
	put($webpayRef, "COMMENT", "Testing the webpayPHP api: statusCheck.php");
	
	$tranProcessed = executeTransaction( $webpayRef );
	if ($tranProcessed == "true") {
		print "Successfully communicated with the WTS<br>\n";
	} else {
		print "Unable to communicate with the WTS<br>\n";
		
	}
	//Check the result of the transaction.
	displayResults($webpayRef);
	print "<hr>";

?>
</html>

<?php

function displayResults($webpayRef){
	$txnRef = get( $webpayRef, "TXNREFERENCE");
	$result = get( $webpayRef, "RESULT");
	$authCode = get( $webpayRef, "AUTHCODE");
	$responseText = get( $webpayRef, "RESPONSETEXT");
	$responseCode = get( $webpayRef, "RESPONSECODE");
	$error = get( $webpayRef, "ERROR");

	print "--------------------------------------<br>\n";
	if ((approvedTransaction($responseCode))) {
		print ("-------- TRANSACTION APPROVED --------<br>\n");
	} else {
		print ("------ TRANSACTION NOT APPROVED ------<br>\n");
	}
	print "--------------------------------------<br>\n";
	print "Transaction Reference\t : [$txnRef]<br>\n";
	print "Result\t\t\t : [$result]<br>\n";
	print "Auth Code\t\t : [$authCode]<br>\n";
	print "Response Text\t\t : [$responseText]<br>\n";
	print "Response Code\t\t : [$responseCode]<br>\n";
	print "Error Message\t\t : [$error]<br>\n";
	print "--------------------------------------<br>";
	
}

function approvedTransaction($responseCode) {

	//
	// Check the returned response Code against the list of known Approved Response Codes
	//
	// Please check the documentation to ensure that you have the 
	// latest list of approved response codes.
	//

	if ($responseCode)
	{
		$ARRAY_SIZE = 3;
		$listOfApprovedResponseCodes[0] = "00"; //Transaction Approved
		$listOfApprovedResponseCodes[1] = "08"; //Approved Signature
		$listOfApprovedResponseCodes[2] = "77"; //Approved Signature
		for($i = 0; $i < $ARRAY_SIZE; $i++)
		{
			if( $responseCode == $listOfApprovedResponseCodes[$i]){
				//Found it. Return true to indicate approved transaction.
				return true;
			}
		}
	}
	return false;
}

function load_library( $extensionFile ) {
	//make sure that we are ABLE to load libraries
	if( !(bool)ini_get( "enable_dl" ) ) {
		exit( "enable_dl is set to false. Cannot dynamically load webpay_php.so.\n" );
	}

	if( (bool)ini_get( "safe_mode" ) ) {
		exit( "safe mode is set to true. Cannot dynamically load webpay_php.so.\n" );
	}

	//check to make sure the file exists
	if( !file_exists( $extensionFile ) ) {
		exit( "webpay_php does not exist: [$extensionFile].\n" );
	}

	//check the file permissions
	if( !is_executable( $extensionFile ) ) {
		exit( "Cannot execute webpay_php.so [$extensionFile}.\n" );
	}

	//we figure out the path
	$currentDir = getcwd() . "/";
	$currentExtPath = ini_get( "extension_dir" );
	$subDirs = preg_match_all( "/\//" , $currentExtPath , $matches );
	unset( $matches );

	//lets make sure we extracted a valid extension path
	if( !(bool)$subDirs ) {
		exit( "Could not determine a valid extension path [extension_dir].\n" );
	}

	$extPathLastChar = strlen( $currentExtPath ) - 1;
	if( $extPathLastChar == strrpos( $currentExtPath , "/" ) ) {
		$subDirs--;
	}

	$backDirStr = "";
	for( $i = 1; $i <= $subDirs; $i++ ) {
		$backDirStr .= "..";
		if( $i != $subDirs ) {
			$backDirStr .= "/";
		}
	}

	//construct the final path to load
	$finalExtPath = $backDirStr . $currentDir . $extensionFile;

	//now we execute dl() to actually load the module
	if( !dl( $finalExtPath ) ) {
		exit("Couldn't dynamically load webpay_php extention: [$finalExtPath]");
	}

	//if the module was loaded correctly, we must bow grab the module name
	$loadedExtensions = get_loaded_extensions();
	$thisExtName = $loadedExtensions[ sizeof( $loadedExtensions ) - 1 ];

	//lastly, we return the extension name
	return $thisExtName;
}
?>
