	opt	d+

	move	#4,-(sp)
	trap	#14
	addq.l	#2,sp
	cmpi	#2,d0
	bne.s	NM
	clr	-(sp)
	trap	#1
NM	move	d0,sres
	move	#2,-(sp)
	trap	#14
	addq.l	#2,sp
	move.l	d0,screen
	move.l	#0,-(sp)
	move	#32,-(sp)
	trap	#1
	addq.l	#6,sp
	move.l	d0,super
	clr	-(sp)
	move.l	#-1,-(sp)
	move.l	#-1,-(sp)
	move	#5,-(sp)
	trap	#14
	lea	12(sp),sp

	lea	pic+34,a0
	move.l	screen,a1
	move	#7999,d7
CPYPIC	move.l	(a0)+,(a1)+
	dbf	d7,CPYPIC

	lea	smfp+32,a0
	movem.l	$ffff8240.w,d0-d7
	movem.l	d0-d7,-32(a0)
	moveq	#0,d0
	move.b	$fffffa07.w,(a0)+
	move.b	$fffffa09.w,(a0)+
	move.b	$fffffa13.w,(a0)+
	move.b	$fffffa17.w,(a0)+
	move.b	$fffffa1b.w,(a0)+
	move.b	$fffffa21.w,(a0)+
	move.l	$70.w,(a0)+
	move.l	$120.w,(a0)+
	move	sr,(a0)+
	move	#$2700,sr
	move.b	#1,$fffffa07.w
	move.b	#0,$fffffa09.w
	bset	#0,$fffffa13.w
	bclr	#3,$fffffa17.w
	move.b	#0,$fffffa1b.w
	move.b	#1,$fffffa21.w
	move.l	#VBL,$70.w
	move.l	#HBL,$120.w
	move	#$2300,sr
	move.b	#8,$fffffa1b.w

LOOP1	clr.l	test
LOOP2	addq.l	#1,test
	cmp	#1,vsync
	bne.s	LOOP2
	clr	vsync
	cmpi.b	#$39,$fffffc02.w
	bne.s	LOOP1

	move	#$2700,sr
	lea	smfp,a0
	movem.l	(a0)+,d0-d7
	movem.l	d0-d7,$ffff8240.w
	move.b	(a0)+,$fffffa07.w
	move.b	(a0)+,$fffffa09.w
	move.b	(a0)+,$fffffa13.w
	move.b	(a0)+,$fffffa17.w
	move.b	(a0)+,$fffffa1b.w
	move.b	(a0)+,$fffffa21.w
	move.l	(a0)+,$70.w
	move.l	(a0)+,$120.w
	move	(a0)+,sr
	move	sres,-(sp)
	move.l	#-1,-(sp)
	move.l	#-1,-(sp)
	move	#5,-(sp)
	trap	#14
	lea	12(sp),sp
	move.l	super,-(sp)
	move	#32,-(sp)
	trap	#1
	addq.l	#6,sp
	clr	-(sp)
	trap	#1

VBL	move	#1,vsync
	lea	buff,a1
	lea	buff,a6
	move.l	addbuff,d0
	mulu	#32,d0
	adda.l	d0,a6
	add.l	#1,addbuff
	cmp.l	#40,addbuff
	bne	NOT40
	clr.l	addbuff
NOT40	move	#5,d7	; 6 letters
CPY1	lea	font,a0
	move	#39,d6	; 40 scanlines
CPY2	move	#7,d5	; 8 columns
CPY3	move.l	(a0)+,(a1)+
	dbf	d5,CPY3
	dbf	d6,CPY2
	dbf	d7,CPY1
	rte

HBL	move.l	(a6)+,$ffff8240.w
	move.l	(a6)+,$ffff8244.w
	move.l	(a6)+,$ffff8248.w
	move.l	(a6)+,$ffff824c.w
	move.l	(a6)+,$ffff8250.w
	move.l	(a6)+,$ffff8254.w
	move.l	(a6)+,$ffff8258.w
	move.l	(a6)+,$ffff825c.w
	rte

vsync		dc.w	0
test		dc.l	0
super		dc.l	0
screen		dc.l	0
sres		dc.w	0
subbuff		dc.w	0
smfp		ds.b	48
buff		ds.b	7680
addbuff		dc.l	0

font		dc.w	$000,$000,$000,$000,$000,$000,$000,$000,$111,$000,$000,$000,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$000,$000,$000,$111,$222,$111,$000,$000,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$000,$000,$111,$222,$333,$222,$111,$000,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$000,$111,$222,$333,$444,$333,$222,$111,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$111,$222,$333,$444,$000,$444,$333,$222,$111,$000,$000,$000

two		dc.w	$000,$000,$000,$111,$222,$333,$444,$000,$000,$000,$444,$333,$222,$111,$000,$000
		dc.w	$000,$000,$111,$222,$333,$444,$000,$000,$000,$000,$000,$444,$333,$222,$111,$000
		dc.w	$000,$111,$222,$333,$444,$000,$000,$000,$000,$000,$000,$000,$444,$333,$222,$111
		dc.w	$000,$222,$333,$444,$000,$000,$000,$000,$000,$000,$000,$000,$000,$444,$333,$222
		dc.w	$000,$333,$444,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$444,$333

		dc.w	$000,$444,$555,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$555,$444
		dc.w	$000,$555,$666,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$666,$555
		dc.w	$000,$666,$777,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$777,$666
		dc.w	$000,$777,$776,$111,$111,$111,$111,$111,$111,$111,$111,$111,$111,$111,$767,$777
		dc.w	$000,$677,$775,$222,$222,$222,$222,$222,$222,$222,$222,$222,$222,$222,$757,$676

four		dc.w	$000,$577,$774,$333,$333,$333,$333,$333,$333,$333,$333,$333,$333,$333,$747,$575
		dc.w	$000,$477,$773,$444,$444,$444,$444,$444,$444,$444,$444,$444,$444,$444,$737,$474
		dc.w	$000,$377,$772,$555,$555,$555,$555,$555,$555,$555,$555,$555,$555,$555,$727,$373
		dc.w	$000,$277,$771,$666,$666,$666,$666,$666,$666,$666,$666,$666,$666,$666,$717,$272
		dc.w	$000,$177,$770,$777,$777,$777,$777,$777,$777,$777,$777,$777,$777,$777,$707,$171

		dc.w	$000,$077,$670,$666,$666,$666,$666,$666,$666,$666,$666,$666,$666,$666,$706,$070
		dc.w	$000,$067,$570,$555,$555,$555,$555,$555,$555,$555,$555,$555,$555,$555,$705,$071
		dc.w	$000,$057,$470,$444,$444,$444,$444,$444,$444,$444,$444,$444,$444,$444,$704,$072
		dc.w	$000,$047,$370,$333,$333,$333,$333,$333,$333,$333,$333,$333,$333,$333,$703,$073
		dc.w	$000,$037,$270,$222,$222,$222,$222,$222,$222,$222,$222,$222,$222,$222,$702,$074

six		dc.w	$000,$027,$170,$111,$111,$111,$111,$111,$111,$111,$111,$111,$111,$111,$701,$075
		dc.w	$000,$017,$070,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$700,$076
		dc.w	$000,$007,$071,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$710,$077
		dc.w	$000,$107,$070,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$720,$177
		dc.w	$000,$207,$170,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$730,$277

		dc.w	$000,$307,$270,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$740,$377
		dc.w	$000,$407,$370,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$750,$477
		dc.w	$000,$507,$470,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$760,$577
		dc.w	$000,$607,$570,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$770,$677
		dc.w	$000,$707,$670,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$770,$777

		dc.w	$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000
		dc.w	$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000,$000

patt		dc.w	$ffff,$0000,$0000,$0000,$0000,$ffff,$0000,$0000
		dc.w	$ffff,$ffff,$0000,$0000,$0000,$0000,$ffff,$0000
		dc.w	$ffff,$0000,$ffff,$0000,$0000,$ffff,$ffff,$0000
		dc.w	$ffff,$ffff,$ffff,$0000,$0000,$0000,$0000,$ffff
		dc.w	$ffff,$0000,$0000,$ffff,$0000,$ffff,$0000,$ffff

		dc.w	$ffff,$ffff,$0000,$ffff,$0000,$0000,$ffff,$ffff
		dc.w	$ffff,$0000,$ffff,$ffff,$0000,$ffff,$ffff,$ffff
		dc.w	$ffff,$ffff,$ffff,$ffff,0,0,0,0
		dc.w	0,0,0,0,0,0,0,0
		dc.w	0,0,0,0,0,0,0,0
pic		INCBIN	A:\3D_SCRL.ONC\3DSCRPIC.PI1
		END
