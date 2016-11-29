use Convert::Binary::C;

$directry="./";
opendir(DIR,$directry);
@dir=readdir(DIR);

my $c = new Convert::Binary::C ByteOrder => 'LittleEndian';
#$c->Include('/Users/motohisa/Documents/programing/WinSpec/');
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
		print "$filename\t$header->{polynom_coeff_x}[0]\n";
		print "$filename\t$header->{polynom_coeff_x}[1]\n";
		print "$filename\t$header->{polynom_coeff_x}[2]\n";
		print "$filename\t$header->{pixel_position_x}[1]\n";
		print "$filename\t$header->{calib_value_x}[1]\n";
	}
}
closedir(DIR);
