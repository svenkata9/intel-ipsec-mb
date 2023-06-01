/* POLY1305, RFC7539 */
#include "mac_test.h"

const struct mac_test poly1305_test_json[] = {
        /* Vectors from https://tools.ietf.org/html/rfc7539 */
        {256, 128, 1,
         "\x85\xd6\xbe\x78\x57\x55\x6d\x33\x7f\x44\x52\xfe\x42\xd5\x06\xa8\x01\x03"
         "\x80\x8a\xfb\x0d\xb2\xfd\x4a\xbf\xf6\xaf\x41\x49\xf5\x1b",
         "\x43\x72\x79\x70\x74\x6f\x67\x72\x61\x70\x68\x69\x63\x20\x46\x6f\x72\x75"
         "\x6d\x20\x52\x65\x73\x65\x61\x72\x63\x68\x20\x47\x72\x6f\x75\x70",
         "\xa8\x06\x1d\xc1\x30\x51\x36\xc6\xc2\x2b\x8b\xaf\x0c\x01\x27\xa9", 1, 272,
         NULL, 0},
        {256, 128, 2,
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1, 512,
         NULL, 0},
        {256, 128, 3,
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x36\xe5"
         "\xf6\xb5\xc5\xe0\x60\x70\xf0\xef\xca\x96\x22\x7a\x86\x3e",
         "\x41\x6e\x79\x20\x73\x75\x62\x6d\x69\x73\x73\x69\x6f\x6e\x20\x74\x6f\x20"
         "\x74\x68\x65\x20\x49\x45\x54\x46\x20\x69\x6e\x74\x65\x6e\x64\x65\x64\x20"
         "\x62\x79\x20\x74\x68\x65\x20\x43\x6f\x6e\x74\x72\x69\x62\x75\x74\x6f\x72"
         "\x20\x66\x6f\x72\x20\x70\x75\x62\x6c\x69\x63\x61\x74\x69\x6f\x6e\x20\x61"
         "\x73\x20\x61\x6c\x6c\x20\x6f\x72\x20\x70\x61\x72\x74\x20\x6f\x66\x20\x61"
         "\x6e\x20\x49\x45\x54\x46\x20\x49\x6e\x74\x65\x72\x6e\x65\x74\x2d\x44\x72"
         "\x61\x66\x74\x20\x6f\x72\x20\x52\x46\x43\x20\x61\x6e\x64\x20\x61\x6e\x79"
         "\x20\x73\x74\x61\x74\x65\x6d\x65\x6e\x74\x20\x6d\x61\x64\x65\x20\x77\x69"
         "\x74\x68\x69\x6e\x20\x74\x68\x65\x20\x63\x6f\x6e\x74\x65\x78\x74\x20\x6f"
         "\x66\x20\x61\x6e\x20\x49\x45\x54\x46\x20\x61\x63\x74\x69\x76\x69\x74\x79"
         "\x20\x69\x73\x20\x63\x6f\x6e\x73\x69\x64\x65\x72\x65\x64\x20\x61\x6e\x20"
         "\x22\x49\x45\x54\x46\x20\x43\x6f\x6e\x74\x72\x69\x62\x75\x74\x69\x6f\x6e"
         "\x22\x2e\x20\x53\x75\x63\x68\x20\x73\x74\x61\x74\x65\x6d\x65\x6e\x74\x73"
         "\x20\x69\x6e\x63\x6c\x75\x64\x65\x20\x6f\x72\x61\x6c\x20\x73\x74\x61\x74"
         "\x65\x6d\x65\x6e\x74\x73\x20\x69\x6e\x20\x49\x45\x54\x46\x20\x73\x65\x73"
         "\x73\x69\x6f\x6e\x73\x2c\x20\x61\x73\x20\x77\x65\x6c\x6c\x20\x61\x73\x20"
         "\x77\x72\x69\x74\x74\x65\x6e\x20\x61\x6e\x64\x20\x65\x6c\x65\x63\x74\x72"
         "\x6f\x6e\x69\x63\x20\x63\x6f\x6d\x6d\x75\x6e\x69\x63\x61\x74\x69\x6f\x6e"
         "\x73\x20\x6d\x61\x64\x65\x20\x61\x74\x20\x61\x6e\x79\x20\x74\x69\x6d\x65"
         "\x20\x6f\x72\x20\x70\x6c\x61\x63\x65\x2c\x20\x77\x68\x69\x63\x68\x20\x61"
         "\x72\x65\x20\x61\x64\x64\x72\x65\x73\x73\x65\x64\x20\x74\x6f",
         "\x36\xe5\xf6\xb5\xc5\xe0\x60\x70\xf0\xef\xca\x96\x22\x7a\x86\x3e", 1,
         3000, NULL, 0},
        {256, 128, 4,
         "\x36\xe5\xf6\xb5\xc5\xe0\x60\x70\xf0\xef\xca\x96\x22\x7a\x86\x3e\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\x41\x6e\x79\x20\x73\x75\x62\x6d\x69\x73\x73\x69\x6f\x6e\x20\x74\x6f\x20"
         "\x74\x68\x65\x20\x49\x45\x54\x46\x20\x69\x6e\x74\x65\x6e\x64\x65\x64\x20"
         "\x62\x79\x20\x74\x68\x65\x20\x43\x6f\x6e\x74\x72\x69\x62\x75\x74\x6f\x72"
         "\x20\x66\x6f\x72\x20\x70\x75\x62\x6c\x69\x63\x61\x74\x69\x6f\x6e\x20\x61"
         "\x73\x20\x61\x6c\x6c\x20\x6f\x72\x20\x70\x61\x72\x74\x20\x6f\x66\x20\x61"
         "\x6e\x20\x49\x45\x54\x46\x20\x49\x6e\x74\x65\x72\x6e\x65\x74\x2d\x44\x72"
         "\x61\x66\x74\x20\x6f\x72\x20\x52\x46\x43\x20\x61\x6e\x64\x20\x61\x6e\x79"
         "\x20\x73\x74\x61\x74\x65\x6d\x65\x6e\x74\x20\x6d\x61\x64\x65\x20\x77\x69"
         "\x74\x68\x69\x6e\x20\x74\x68\x65\x20\x63\x6f\x6e\x74\x65\x78\x74\x20\x6f"
         "\x66\x20\x61\x6e\x20\x49\x45\x54\x46\x20\x61\x63\x74\x69\x76\x69\x74\x79"
         "\x20\x69\x73\x20\x63\x6f\x6e\x73\x69\x64\x65\x72\x65\x64\x20\x61\x6e\x20"
         "\x22\x49\x45\x54\x46\x20\x43\x6f\x6e\x74\x72\x69\x62\x75\x74\x69\x6f\x6e"
         "\x22\x2e\x20\x53\x75\x63\x68\x20\x73\x74\x61\x74\x65\x6d\x65\x6e\x74\x73"
         "\x20\x69\x6e\x63\x6c\x75\x64\x65\x20\x6f\x72\x61\x6c\x20\x73\x74\x61\x74"
         "\x65\x6d\x65\x6e\x74\x73\x20\x69\x6e\x20\x49\x45\x54\x46\x20\x73\x65\x73"
         "\x73\x69\x6f\x6e\x73\x2c\x20\x61\x73\x20\x77\x65\x6c\x6c\x20\x61\x73\x20"
         "\x77\x72\x69\x74\x74\x65\x6e\x20\x61\x6e\x64\x20\x65\x6c\x65\x63\x74\x72"
         "\x6f\x6e\x69\x63\x20\x63\x6f\x6d\x6d\x75\x6e\x69\x63\x61\x74\x69\x6f\x6e"
         "\x73\x20\x6d\x61\x64\x65\x20\x61\x74\x20\x61\x6e\x79\x20\x74\x69\x6d\x65"
         "\x20\x6f\x72\x20\x70\x6c\x61\x63\x65\x2c\x20\x77\x68\x69\x63\x68\x20\x61"
         "\x72\x65\x20\x61\x64\x64\x72\x65\x73\x73\x65\x64\x20\x74\x6f",
         "\xf3\x47\x7e\x7c\xd9\x54\x17\xaf\x89\xa6\xb8\x79\x4c\x31\x0c\xf0", 1,
         3000, NULL, 0},
        {256, 128, 5,
         "\x1c\x92\x40\xa5\xeb\x55\xd3\x8a\xf3\x33\x88\x86\x04\xf6\xb5\xf0\x47\x39"
         "\x17\xc1\x40\x2b\x80\x09\x9d\xca\x5c\xbc\x20\x70\x75\xc0",
         "\x27\x54\x77\x61\x73\x20\x62\x72\x69\x6c\x6c\x69\x67\x2c\x20\x61\x6e\x64"
         "\x20\x74\x68\x65\x20\x73\x6c\x69\x74\x68\x79\x20\x74\x6f\x76\x65\x73\x0a"
         "\x44\x69\x64\x20\x67\x79\x72\x65\x20\x61\x6e\x64\x20\x67\x69\x6d\x62\x6c"
         "\x65\x20\x69\x6e\x20\x74\x68\x65\x20\x77\x61\x62\x65\x3a\x0a\x41\x6c\x6c"
         "\x20\x6d\x69\x6d\x73\x79\x20\x77\x65\x72\x65\x20\x74\x68\x65\x20\x62\x6f"
         "\x72\x6f\x67\x6f\x76\x65\x73\x2c\x0a\x41\x6e\x64\x20\x74\x68\x65\x20\x6d"
         "\x6f\x6d\x65\x20\x72\x61\x74\x68\x73\x20\x6f\x75\x74\x67\x72\x61\x62\x65"
         "\x2e",
         "\x45\x41\x66\x9a\x7e\xaa\xee\x61\xe7\x08\xdc\x7c\xbc\xc5\xeb\x62", 1,
         1016, NULL, 0},
        {256, 128, 6,
         "\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",
         "\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1, 128,
         NULL, 0},
        {256, 128, 7,
         "\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff"
         "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",
         "\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1, 128,
         NULL, 0},
        {256, 128, 8,
         "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xf0\xff"
         "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x11\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1, 384,
         NULL, 0},
        {256, 128, 9,
         "\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfb\xfe"
         "\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\xfe\x01\x01\x01\x01"
         "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01",
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1, 384,
         NULL, 0},
        {256, 128, 10,
         "\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\xfd\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",
         "\xfa\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", 1, 128,
         NULL, 0},
        {256, 128, 11,
         "\x01\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\xe3\x35\x94\xd7\x50\x5e\x43\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x33\x94"
         "\xd7\x50\x5e\x43\x79\xcd\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\x14\x00\x00\x00\x00\x00\x00\x00\x55\x00\x00\x00\x00\x00\x00\x00", 1, 512,
         NULL, 0},
        {256, 128, 12,
         "\x01\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\xe3\x35\x94\xd7\x50\x5e\x43\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x33\x94"
         "\xd7\x50\x5e\x43\x79\xcd\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
         "\x13\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1, 384,
         NULL, 0},
        {0, 0, 0, NULL, NULL, NULL, 0, 0, NULL, 0}
};
