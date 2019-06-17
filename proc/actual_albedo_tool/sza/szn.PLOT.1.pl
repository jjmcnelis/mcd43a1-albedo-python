#!/opt/ActivePerl-5.12/bin/perl

#this has check for latest proc_date for specific dates...]

if ( $#ARGV +1 ==0)
{die qq!

$0    lat  year_min  year_max 
$0    35.958767  2000  2005    > /tmp/crap/szn.png

 spits out local.solar.noon plot, pipe to a file
! ;}

$lat=	 	$ARGV[0];
$year_min =  	$ARGV[1];
$year_max = 	$ARGV[2];

foreach $y($year_min..$year_max){push(@years,$y)}
foreach $i(@years){if($i>$year_max){$year_max=$i};if($i<$year_min){$year_min=$i};}
$d_max=$d_min=$days[0];
foreach $i(@days){if($i>$d_max){$d_max=$i};if($i<$d_min){$d_min=$i};}
plot();



sub plot
{
$max=100;
$min=55;
 
 
use GD;
$extra_y=100;  #for grid at bottom      % bad  ;a klugde to get 2 plots in one img
 #.618034 magic ratio, golden ratio
$xsize=700;
$ysize=100;
$marg=50;

$im = new GD::Image($xsize+1,$ysize+1+$marg+$marg);
    # allocate some colors
$white = $im->colorAllocate(255,255,255);
$black = $im->colorAllocate(0,0,0);       
$red = $im->colorAllocate(255,0,0);      
$blue = $im->colorAllocate(0,0,255);
#$wheat2 = $im->colorAllocate(238,216,174);
$wheat2 = $im->colorAllocate(255,250,214);
$green= $im->colorAllocate(0,216,0);
$dot1= $im->colorAllocate(255,0,255);
$magenta=$im->colorAllocate(255,199 ,218); #FFCOCB
$magenta=$im->colorAllocate(255,209 ,228); #FFCOCB
$gray= $im->colorAllocate(220,220,220); 
# make the background transparent and interlaced
$im->transparent($white);
$im->interlaced('true');
         
# Put a black frame around the picture
$bottom_box=$im->colorAllocate(240,240,240);  
#$im->rectangle(1,1,$xsize,$ysize,$black);
$im->filledRectangle($marg,$marg,$xsize-$marg,$ysize-$marg,$bottom_box);
$im->rectangle($marg,$marg,$xsize-$marg,$ysize-$marg,$black);

#get pixel steps both axis
$xstep=($xsize-($marg*2))/(1+$year_max-$year_min); #year_max is year max, pixels per year ; add one to yar so all will fit
$ystep=($ysize-($marg*2))/($max-$min);
$dstep=$xstep/365; #pixel per day

 
#calc szn form 365 days
$gray= $im->colorAllocate(220,220,220);
for $i(1..365)
	{
	$joe_szn=`./plug_1/local_szn.1.exe $lat $i`;
	@joe=split(/ /,$joe_szn);
	#if($joe[5] > 100.){$joe[5] = 100.} #some > 100 where sun below howizon
	push(@annual_szn,$joe[5]);
	}
$max_annual_szn=0;
foreach $a(@annual_szn){if($a>$max_annual_szn){$max_annual_szn=$a}}
$max_annual_szn=int($max_annual_szn);
$max_annual_szn=10+($max_annual_szn-($max_annual_szn%10)); #up to a 10 thingie
#plot on bottom annual szn in tan 
foreach $i (0 .. ($year_max-$year_min))
        {
	foreach $j (1..364) #day 365 is 100%
		{
		$xx=$marg+$xstep*($i);
        	$xx=$xx+($dstep*$j);
		#if($i == 0 and $j < 50){next}
		#$yy=$ysize+$extra_y-$marg-($annual_szn[$j]) ;
		#scale to max, not 100
		$yy=$ysize+$extra_y-$marg-($annual_szn[$j]/($max_annual_szn*.01)) ;
		$im->line($xx,$ysize+$extra_y-$marg,$xx,$yy,$gray);
		}
	} 


$im->rectangle($marg,$ysize-$marg,$xsize-$marg,$ysize+$extra_y-$marg,$black);

#along side 0, %, 100
#$im->string(gdGiantFont,$marg/2-5,$ysize-$marg+5,"100",$black);
$im->string(gdGiantFont,$marg/2-5,$ysize-$marg-7,$max_annual_szn,$black);
$im->string(gdGiantFont,$marg/2-5,$ysize+$extra_y-$marg-7,"0",$black);
#$im->string(gdGiantFont,$marg/2-5,$ysize-$marg+35,"%",$black);
$im->string(gdTinyFont,$marg/2-15 ,$ysize-$marg+45,"Degrees",$black);

#side ticks
$im->line($marg,$ysize-$marg+50,$marg+10,$ysize-$marg+50,$black);
$im->line($marg,$ysize-$marg+25,$marg+6,$ysize-$marg+25,$black);
$im->line($marg,$ysize-$marg+75,$marg+6,$ysize-$marg+75,$black);



#redraw outer bloack box
#$im->rectangle($marg,$ysize-$marg,$xsize-$marg,$ysize+$extra_y-$marg,$black);


 

#years along bottom
foreach $i (0 .. ($year_max-$year_min+1))
        {
        $xplace=$marg+($i*$xstep);
        $yplace=$extra_y+$ysize-$marg+5;
        $im->string(gdGiantFont,$xplace,$yplace,int($year_min+$i),$black);
        $im->line($xplace,$yplace-5,$xplace,$yplace-15,$black);
	} 
#$image->copy($sourceImage, $dstX,$dstY, $srcX,$srcY, $width,$height)
 

PLOT:

if(GD->VERSION > 1.19)
	{
	#print "Content-type: image/png\n\n";  #need for browser
	binmode STDOUT;
	print $im->png if (!($bug));
	return;
	}
print "Content-type: image/gif\n\n";
binmode STDOUT;
# Convert the image to GIF and print it on standard output
               print $im->gif 

 

}
