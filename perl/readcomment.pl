use Convert::Binary::C;

# $directry="./";
# opendir(DIR,$directry);
# @dir=readdir(DIR);

my $c = new Convert::Binary::C ByteOrder => 'LittleEndian';
#$c->Include('/Users/motohisa/Documents/programing/WinSpec/');
$c->Include('../');
$c->parse_file('WinSpecHeader25.h');

foreach $filename (@ARGV) {
	if(-f $filename && ($filename=~/.SPE/ || $filename=~/.spe/)) {
		open(IN,$filename);
#		binmode(IN);
		seek(IN,200,0);
		read(IN,$comment1,80);
		read(IN,$comment2,80);
		read(IN,$comment3,80);
		read(IN,$comment4,80);
		read(IN,$comment5,80);
		# $header = $c->unpack('WINXHEAD',$_);
		close(IN);
		print "$filename:$comment1\n";
		print "$filename:$comment2\n";
		print "$filename:$comment3\n";
		print "$filename:$comment4\n";
		print "$filename:$comment5\n";
	}
}
# closedir(DIR);
