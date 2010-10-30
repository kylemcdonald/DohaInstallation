#!/usr/bin/perl

for(0..5) {
	$cmd = "scp ~/Desktop/openFrameworks/apps/DohaInstallation/$ARGV[0]/bin/data/unique/3rdi$_.local/unique.xml 3rdi$_.local:~/Desktop/openFrameworks/apps/DohaInstallation/$ARGV[0]/bin/data/";
	print "running \`$cmd\`\n";
	print `$cmd`;
}
