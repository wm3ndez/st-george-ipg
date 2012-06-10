#!/bin/sh
#
# Check the status of a transaction...
# statusCheck <gateway_address> <gateway_port> <client_id>
#    <certificate_file> <certificate_password> <transaction_reference_number>
#    [debug]
#
GATEWAY_ADDRESS=192.0.0.1
GATEWAY_PORT=3007
CLIENT_ID=10000000
CERT_FILE=/test.cer
CERT_PASS=
TXN_REF=0001000000005130
#
#
echo Doing status transaction to $GATEWAY_ADDRESS
./statusCheck "$GATEWAY_ADDRESS" "$GATEWAY_PORT" "$CLIENT_ID" "$CERT_FILE" "$CERT_PASS" "$TXN_REF" debug
