#!/usr/bin/perl

$appName = "OscTest";

print "Closing $appName.\n";
print `./mirror.pl "killall -9 $appName"`;

print "Copying latest files.\n";
print `./rsync-update.pl`;

print "Copying unique files.\n";
print `./copy-unique.pl "$appName"`;

print "Launching $appName.\n";
print `./launch.pl "cd ~/Desktop/openFrameworks/apps/DohaInstallation/$appName/bin; ./$appName"`;

#print "Launching master.\n";
#print `cd ../OscTest/bin/; ./OscTest`;
