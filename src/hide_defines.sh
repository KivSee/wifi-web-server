#!/bin/sh
awk '{ if (/#define USER_SSID/) print "#define USER_SSID ..."; else print $0; }'
awk '{ if (/#define USER_PASS/) print "#define USER_PASS ..."; else print $0; }'
exit 0