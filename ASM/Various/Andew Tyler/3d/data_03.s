* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*			data_03.s                                   *
*	          A sine look-up table                              *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

* A table of sines from 0 to 90 degrees in increments of 1 degree
* multiplied by 2^14 (16384). It can be used to find the sine or cosine
* of any angle. 
sintable:
	dc.w	0,286,572,857,1143,1428,1713,1997,2280,2563,2845,3126
	dc.w	3406,3686,3964,4240,4516,4790,5063,5334,5604,5872,6138
	dc.w	6402,6664,6924,7182,7438,7692,7943,8192,8438,8682,8923
	dc.w	9162,9397,9630,9860,10087,10311,10531,10749,10963,11174
	dc.w	11381,11585,11786,11982,12176,12365,12551,12733,12911
	dc.w	13085,13255,13421,13583,13741,13894,14044,14189,14330
	dc.w	14466,14598,14726,14849,14968,15082,15191,15296,15396
	dc.w	15491,15582,15668,15749,15826,15897,15964,16026,16083
	dc.w	16135,16182,16225,16262,16294,16322,16344,16362,16374
	dc.w	16382,16384

	include	data_02.s		the perspective transform
	include data_00.s
