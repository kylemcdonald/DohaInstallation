#!/usr/bin/perl

for(1..5) {
	$cmd = "ssh 3rdi$_.local \"$ARGV[0]\"";
	print "running \`$cmd\`\n";
	print `$cmd`;
}
