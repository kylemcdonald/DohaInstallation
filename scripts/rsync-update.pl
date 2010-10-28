#!/usr/bin/perl

$source = "/home/kyle/Desktop/openFrameworks";
$target = "/home/kyle/Desktop";

print "from $source\n";
print "to $target\n";

for(1..5) {
	$location = "3rdi$_.local";
	$cmd = "rsync -aq $sourceDirectory $location:$target";
	print "\t$location\n"; 
	print `$cmd`;
}
