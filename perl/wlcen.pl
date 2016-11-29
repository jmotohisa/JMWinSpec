use Convert::Binary::C;

$directry="./";
opendir(DIR,$directry);
@dir=readdir(DIR);

my $c = new Convert::Binary::C ByteOrder => 'LittleEndian',Alignment=>1;
#$c->Include('/Users/motohisa/Documents/programing/WinSpec/');
#1$c->Include('/Volumes/Macintosh\ HD\ 2/programiing/WinSpec/');
$c->Include('../');
$c->parse_file('WinSpecHeader25.h');

foreach $filename (@dir) {
	if(-f $filename && ($filename=~/.SPE/ || $filename=~/.spe/)) {
		open(IN,$filename);
#		binmode(IN);
#		seek(IN,10,0);
		read(IN,$_,4100);
		$header = $c->unpack('WINXHEAD',$_);
		close(IN);
		print "$filename\t$header->{SpecCenterWlNm}\n";
	}
}
closedir(DIR);
