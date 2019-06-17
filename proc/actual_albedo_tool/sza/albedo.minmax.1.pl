#!/opt/ActivePerl-5.12/bin/perl

#this has check for latest proc_date for specific dates...]

if ( $#ARGV +1 ==0)
{die qq!

$0    file 
$0    /tmp/crap/alb/BRDF_Albedo_Parameters.Num_Land_Bands_Plus3_08.Num_Parameters_02.csv


reads file gets bands prints minmax
! ;}

$in_file=		$ARGV[0];

@bands_sort=('vis_white','vis_black','vis_actual','nir_white','nir_black','nir_actual','shortwave_white','shortwave_black','shortwave_actual');
$year_max1=$d_max1=$year_max=$d_max=-500;
$year_min1=$d_min1=$year_min=$d_min=5500;
$alb_min=50000;$alb_max=-50000; 
foreach $band_minmax(@bands_sort)
{
@lines=`cat $in_file |grep $band_minmax`; #so just pure data
foreach $line(@lines)
	{
	@s=split(/,/,$line);
	$s_now=$s[2];
	$s_now =~ s/^A//;
        push(@years,substr($s_now,0,4));
       	push(@days,substr($s_now,4,3));
	splice(@s,0,6);
	foreach $l(@s)
		{
		if($l > 1){next}
		 
		 
		if($l > $alb_max){$alb_max=$l}
		if($l < $alb_min){$alb_min=$l}
		} 
	
	}
	
#$alb_min=$alb_min-($min%10);$alb_max=10+($alb_max-($max%10));
	#minmax all pixels

	$year_max=$year_min=$years[0];
	foreach $i(@years){if($i>$year_max){$year_max=$i};if($i<$year_min){$year_min=$i};}
	$d_max=$d_min=$days[0];
	foreach $i(@days){if($i>$d_max){$d_max=$i};if($i<$d_min){$d_min=$i};}
	
}		
	
print "$alb_min $alb_max  $year_min $year_max $d_min $d_max  \n";
