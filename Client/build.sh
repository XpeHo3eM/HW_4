rm -r src/*
rm -r build/*
mkdir src
mkdir build

#asn1c s1ap.asn1 -D src -pdu=S1SetupRequest -no-gen-OER -fcompound-names -no-gen-example -fno-include-deps
sudo asn1c rrc-7.1.0.asn1 -D src -pdu=auto -fcompound-names -no-gen-OER

gcc -g -Isrc -I. -o build/attach main.c Attach_coder.c src/*.c -lsctp -DPDU=S1SetupRequest -DASN_DISABLE_OER_SUPPORT
./build/attach