#!/usr/bin/perl

use Convert::Binary::C;
use Data::Dumper;
use Data::Hexdumper;

my $c = new Convert::Binary::C ByteOrder => 'LittleEndian', Alignment => 1;
#$c->Include('/Users/motohisa/Documents/programing/WinSpec/');
$c->Include('../');

$c->parse_file('WinSpecHeader25.h');

print Dumper($c->configure);
	
foreach $filename (@ARGV) {
	if(-f $filename && ($filename=~/.SPE/ || $filename=~/.spe/)) {
		open(IN,$filename);
		read(IN,$_,4100);
		$header = $c->unpack('WINXHEAD',$_);
		close(IN);
		print Dumper($header);
	}
}
