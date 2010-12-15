#!/usr/bin/perl

`rsync -avz -e ssh thirdi\@thirdiproject.electrawn.com:/home/thirdi/3rdiStream/resized /home/kyle/Desktop/3rdiStream/`;

$source = "/home/kyle/Desktop/3rdiStream";
$target = "/home/kyle/Desktop";

for(1..5) {
	$location = "3rdi$_.local";
	$cmd = "rsync -aqr --delete $source $location:$target";
	print "$cmd\n"; 
	print `$cmd`;
}
