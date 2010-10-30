#!/usr/bin/perl

$source = "/home/kyle/Desktop/openFrameworks";
$target = "/home/kyle/Desktop";

for(1..5) {
	$location = "3rdi$_.local";
	$cmd = "rsync -aqr $source $location:$target";
	print "$cmd\n"; 
	print `$cmd`;
}
