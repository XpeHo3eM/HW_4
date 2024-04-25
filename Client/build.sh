rm -r src/*
rm -r build/*
mkdir src
mkdir build

sudo asn1c lte-rrc-15.6.0.asn1 -D src -pdu=auto -fcompound-names -no-gen-OER -no-gen-example -fno-include-deps

gcc -g -Isrc -I. -o build/rrcConnectionRequest main.c RRCConnectionRequestCoder.c src/*.c -lsctp -DPDU=auto -DASN_DISABLE_OER_SUPPORT
./build/rrcConnectionRequest