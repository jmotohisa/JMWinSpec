use Convert::Binary::C;

$directry="./";
opendir(DIR,$directry);
@dir=readdir(DIR);

my $c = new Convert::Binary::C ByteOrder => 'LittleEndian',Alignment => 1;
$c->Include('../');

#/Users/motohisa/Documents/programing/WinSpec/');
#$c->Include('/Users/motohisa/HD2/programing/WinSpec/');

$c->parse_file('WinSpecHeader25.h');
print "header size=",$c->sizeof('WINXHEAD'),"\n";
if($c->sizeof('WINXHEAD')!=4100)
{
	print "Error: header size does not much. (", $c->sizeof('WINXHEAD'),", should be 4100)\n";
	print "Check pudding\n";
#	exit;
}

foreach $filename (@dir) {
	if(-f $filename && ($filename=~/.SPE/ || $filename=~/.spe/)) {
		open(IN,$filename);
#		binmode(IN);
#		seek(IN,10,0);
		read(IN,$_,4100);
		$header = $c->unpack('WINXHEAD',$_);
		close(IN);
		$wlcen=$header->{SpecCenterWlNm};
		$wlcen2_calib=$header->{pixel_position_x}[1];
		$wlcen2=$header->{calib_value_x}[1];
		$wlcen3=pixcelToWL(512.5);
		$n_poly=$header->{polynom_order_x};
		print "$n_poly\n";
		for($i=0;$i<6;$i++) {
			print "$i: $header->{polynom_coeff_x}[$i]\n";
		}
		if(abs($wlcen-$wlcen2)>1) {
			print "$filename: wlcen=$wlcen, wlcen_calib=$wlcen2 at $wlcen2_calib, $wlcen3\n";
			for($i=0;$i<10;$i++) {
				print "$i: $header->{pixel_posision_x}[$i]:$header->{calib_value_x}[$i]\n";
			}
		}
	}
}
closedir(DIR);

sub pixcelToWL {
	$i=0;
	$wl=0;
	while($i<$header->{polynom_order_x})
	{
		$wl=$wl*$i+$header->{polynom_coeff_x}[$i];
		$i++;
	}
	$wl;
}
