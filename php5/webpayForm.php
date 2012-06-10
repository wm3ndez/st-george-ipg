<?php
Header("Cache-Control: no-cache");

$offset = 60 * 60 * 24 * 3;
$ExpireString = "Expires: " . gmdate("D, d M Y H:i:s", time() + $offset) . " GMT";
Header($ExpireString);
?>
<html>
<body>
<h2> WebpayPHP test form </h2>
<form method="POST" action="webpayFormHandler.php">  

<table>
<tr>
  <td>Credit Card Number:</td><td><INPUT type="text" name="cardNumber" size="20" value="4564456445644564"><td>
</tr>
<tr>
  <td>Expiry Date:</td><td><INPUT type="text" name="expiryDate" size="4" value="0515"><td>
</tr>
<tr>
  <td>Amount:</td><td><INPUT type="text" name="amount" size="6" value="10.00"><td>
</tr>
<tr>
  <td colspan="2"><INPUT type="submit"><td>
</tr>
</table>

</form>

</body>
</html>

