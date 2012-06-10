<?php
Header("Cache-Control: no-cache");

$offset = 60 * 60 * 24 * 3;
$ExpireString = "Expires: " . gmdate("D, d M Y H:i:s", time() + $offset) . " GMT";
Header($ExpireString);
?>
<?php 

// Simple helper function for displaying table rows
function outTableRow($name, $value){
	print "<tr><td>$name</td><td>$value</td></tr>";
}

// Simple helper function for displaying Data
function displayData($cardNumber, $expiryDate, $amount){
	print "The following Data will be used in this transaction<br>";
	print "<table border='1'>";
	outTableRow("THE_CLIENT_ID", THE_CLIENT_ID);
	outTableRow("THE_CERT_PATH", THE_CERT_PATH);
	outTableRow("THE_CERT_PASSWORD", THE_CERT_PASSWORD);
	outTableRow("THE_PORT", THE_PORT);
	outTableRow("THE_SERVER", THE_SERVER);
	outTableRow("CardNumber", $cardNumber);
	outTableRow("expiryDate", $expiryDate);
	outTableRow("Amount", $amount);
	print "</table>";
}
?>
<?php 

	//Define some constants
	define("THE_INTERFACE", "CREDITCARD");
	define("THE_TRANSACTION_TYPE", "PURCHASE");
	define("THE_WEBPAY_PHP_LIB", "webpay_php.so");

	//These constants may need to be modified for your system
	define("THE_CLIENT_ID", "10000000");
	define("THE_CERT_PATH", "/test.cer");
	define("THE_CERT_PASSWORD", "webpay");
	define("THE_PORT", "3007");
	define("THE_SERVER", "192.0.0.1");
	
	//Retrieve Form Data
	$cardNumber = $_POST['cardNumber'];
	$expiryDate = $_POST['expiryDate'];
	$amount		= $_POST['amount'];

?>
<html>
<body>
<h2> WebpayPHP formHandler </h2>
<p>Return to <a href="webpayForm.php">form</a></p>
<?php 

	//Display input data to the screen.
	displayData($cardNumber, $expiryDate, $amount);
	
	//Attempt to load the WebpayPHP library - display message on error.
	if (!extension_loaded("webpay")) {
		//Load library is a convenience method to load the webpay_php.so file
		//from the current directory. If it doesn't work due to some php restrictions
		//please change it back to dl(THE _WEBPAY_PHP_LIB)
	    if (!load_library(THE_WEBPAY_PHP_LIB)) {
	    	print "<p>Unable to load the library <b>";
	    	print THE_WEBPAY_PHP_LIB;
	    	print "</b></p>";
	        exit;
	    } else {
	    	print "<p>Successfully loaded the library <b>";
	    	print THE_WEBPAY_PHP_LIB;
	    	print "</b></p>";
	    }
	}
	
	print "<p> Starting webpay Transaction </p>";

	//Create Webpay Transaction
 	$webpayRef = newBundle();
	if($webpayRef == NULL){
		exit("There has been an initialisation problem. Please check error log.<br></body></html>\n");
	}
	put($webpayRef, "DEBUG", "OFF");
	put($webpayRef, "LOGFILE", "webpay.log");
	put_ClientID ($webpayRef, THE_CLIENT_ID);
	put_CertificatePath ($webpayRef, THE_CERT_PATH);
	put_CertificatePassword ($webpayRef, THE_CERT_PASSWORD);
	setPort($webpayRef, THE_PORT);
	setServers($webpayRef, THE_SERVER);
	put($webpayRef, "TOTALAMOUNT", $amount);
	put($webpayRef, "CARDDATA", $cardNumber);
	put($webpayRef, "CARDEXPIRYDATE", $expiryDate);
	put($webpayRef, "INTERFACE", THE_INTERFACE);
	put($webpayRef, "TRANSACTIONTYPE", THE_TRANSACTION_TYPE);
	put($webpayRef, "COMMENT", "Testing the PHP api : webpayFormHandler.php");
	
	//Send the transaction to the Webpay Transaction Server
	$tranProcessed = executeTransaction( $webpayRef );
	if ($tranProcessed == "true") {
		print "Successfully communicated with the WTS<br>\n";
	} else {
		print "Unable to communicate with the WTS<br>\n";
		//Try transaction recovery
		$transactionRef = get( $webpayRef, "TXNREFERENCE" );
		if ($transactionRef)
		{
			//We have a transaction reference so attempt a status transaction.
			print "Performing status check with Transaction Ref = [$transactionRef]<br>\n";
			if(doStatusCheck($webpayRef)) {
			   print "Status Check Successful - Details are displayed below.<br>\n";
			} else {
				print "Status check failed: Unknown transaction status.\nPlease wait a short while and try status check again using Transaction Ref [$transactionRef].<br>\n";
			}
		}
		else
		{
			// There is no transaction reference number so the transaction has failed completely.
			// It can be safely reprocessed.
			print "The transaction can be safely reprocessed as no Transaction Reference Number exists.<br>\n";
		}
	}

	// Check the error field
	$error = get( $webpayRef, "ERROR" );

	if($error){
		print "Error during transaction<br>";
		print "<table border='1'>";
		outTableRow("Error text.", $error);	
		print "</table>";
	}
	displayResults( $webpayRef );
	cleanup( $webpayRef );
?>


</body>
</html>

<?php

function doStatusCheck ($webpayRef) {
	
	$txnRef = get( $webpayRef, "TXNREFERENCE");
	
	if ($txnRef) {
		//We have a transaction reference so attempt a status transaction.
		put( $webpayRef, "TRANSACTIONTYPE", "STATUS" );
		if (executeTransaction( $webpayRef ) == "true"){
			return true;
		}else{
			return false;			
		}
		
	} else {
		// No txnref number so we can not do a status check.
		return false;
	}
}


function displayResults($webpayRef){
	$txnRef = get( $webpayRef, "TXNREFERENCE");
	$result = get( $webpayRef, "RESULT");
	$authCode = get( $webpayRef, "AUTHCODE");
	$responseText = get( $webpayRef, "RESPONSETEXT");
	$responseCode = get( $webpayRef, "RESPONSECODE");

	if ((approvedTransaction($responseCode))) {
		print ("<b>TRANSACTION APPROVED</b>");
	} else {
		print ("<b>TRANSACTION NOT APPROVED</b>");
	}
	print "<table border='1'>";
	outTableRow("Transaction Reference", $txnRef);
	outTableRow("Result", $result);
	outTableRow("Auth Code", $authCode);
	outTableRow("Response Text", $responseText);
	outTableRow("Response Code", $responseCode);
	print "</table>";
	
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
