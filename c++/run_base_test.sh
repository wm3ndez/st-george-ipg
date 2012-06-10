#!/bin/sh
#
# base test of webpay client library....
# BaseTest.exe <gateway_address> <gateway_port> <client_id>
#    <certificate_file> <certificate_password> <trusted_certificates_file>
#    <number_of_transactions>
#
GATEWAY_ADDRESS=192.0.0.1
GATEWAY_PORT=3007
CLIENT_ID=10000000
CERT_FILE=/test.p12
CERT_PASS=
TRUSTED_CERT_FILE=
NUM_TRANS=2
DEBUG_FLAG=debug
#
echo Doing test transaction to $GATEWAY_ADDRESS
./baseTest "$GATEWAY_ADDRESS" "$GATEWAY_PORT" "$CLIENT_ID" "$CERT_FILE" "$CERT_PASS" "$TRUSTED_CERT_FILE" "$NUM_TRANS" "$DEBUG_FLAG"
