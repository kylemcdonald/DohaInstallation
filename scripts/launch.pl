#!/usr/bin/perl

print `./mirror.pl "export DISPLAY=:0.0; $ARGV[0] 0>/dev/null 1>&0 2>&0 &\"`;
