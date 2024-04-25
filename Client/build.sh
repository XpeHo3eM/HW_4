rm -r src/*
rm -r build/*
mkdir src
mkdir build

#asn1c s1ap.asn1 -D src -pdu=S1SetupRequest -no-gen-OER -fcompound-names -no-gen-example -fno-include-deps
sudo asn1c EUTRA-RRC-Definitions.asn -D src -pdu=auto -fcompound-names -no-gen-OER -no-gen-example -fno-include-deps

gcc -g -Isrc -I. -o build/rrcConnectionRequest main.c RRCConnectionRequestCoder.c src/*.c -lsctp -DPDU=auto -DASN_DISABLE_OER_SUPPORT
./build/rrcConnectionRequest