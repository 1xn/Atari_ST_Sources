	opt	x+
	lea	$80000,a7
	bsr	res
lets_loop
	bsr	zgas
	bsr	FALUJ_Z_VATem
	move.w	#0,$ffff8240.w
	bsr	vbl
	move.w	#39,$ffff8240.w
	bra.s	lets_loop




zgas
	move.l	screen(pc),a0
	moveq	#0,d0
	moveq	#39,d7
pipka	move.l	d0,(a0)
	move.l	d0,8(a0)
	move.l	d0,16(a0)
	move.l	d0,24(a0)
	move.l	d0,32(a0)
	move.l	d0,40(a0)
	move.l	d0,48(a0)
	move.l	d0,56(a0)
	move.l	d0,64(a0)
	move.l	d0,72(a0)
	lea	160(a0),a0
	dbf	d7,pipka
	rts
FALUJ_Z_VATem
	moveq	#39,d7
	move.l	screen(pc),a3
	move.w	last(pc),d0
	lea	droga(pc),a0
	add.l	d0,a0
	addq.w	#2,last
pierdol
	lea	adresiki(pc),a1
	moveq	#0,d0
	move.w	(a0),d0
	cmp.w	#-1,d0
	bne.s	dallej
	clr.w	last
	lea	droga(pc),a0
dallej	move.w	(a0)+,d0
	ror.l	#4,d0
	cmp.b	#0,d0
	beq.s	nie_zm
	moveq	#0,d1
	move.b	d0,d1
	lsl.w	#3,d1
	add.w	d1,a3
	bset	#0,zmiana
nie_zm
	rol.l	#4,d0
	and.w	#15,d0
	add.w	d0,d0
	add.w	d0,d0
	move.l	(a1,d0.w),a2
	move.w	linia(pc),d0
	mulu	#7*4,d0
	add.l	d0,a2
	move.l	(a2)+,(a3)
	move.l	(a2)+,8(a3)
	move.l	(a2)+,16(a3)
	move.l	(a2)+,24(a3)
	move.l	(a2)+,32(a3)
	move.l	(a2)+,40(a3)
	move.l	(a2)+,48(a3)
	addq.w	#1,linia
	lea	160(a3),a3
	tst.b	zmiana
	beq.s	bez_zmian
	clr.w	zmiana
	sub.l	#8,a3
bez_zmian
	dbf	d7,pierdol
	clr.w	linia
	rts
end	move.w	#-1,$ffff8240.w
	clr.l	(a7)
	dc.w	$4e41
	illegal
vbl	move.l	d0,-(a7)
	move.l	$466.w,d0
vbl2	cmp.l	$466.w,d0
	beq.s	vbl2
	cmp.b	#$39,$fffffc02.w
	beq.s	end
	move.l	(a7)+,d0
	rts
res	clr.l	-(a7)
	move.w	#32,-(a7)
	trap	#1
	addq.l	#6,a7
	clr.w	-(a7)
	pea	$78000
	pea	$78000
	move.w	#5,-(a7)
	trap	#14
	adda.l	#12,a7
	move.l	(pal+2)(pc),$ffff8242.w
	move.l	(pal+6)(pc),$ffff8246.w
	bsr	prepare1
	bsr	prepare2
	rts
prepare1
	lea	copies(pc),a0
	moveq	#0,d0
	move.w	#1000,d7
wycz_it	rept	8
	move.l	d0,(a0)+
	endr
	dbf	d7,wycz_it
	rts
prepare2
	lea	copies(pc),a1
	lea	adresiki(pc),a2
	moveq	#15,d7 ;ilosc obrazow
	moveq	#00,d4 ;wart. przesun
Do_It	moveq	#39,d6 ;wysokosc (40)
	moveq	#05,d5 ;dlugosc  (96)
	lea	oryginal(pc),a0
	move.l	a1,(a2)+
Go_And_Do
	moveq	#0,d0
	move.w	(a0)+,d0
	swap	d0
	lsr.l	d4,d0
	or.w	d0,4(a1)
	swap	d0
	or.w	d0,(a1)
	addq.l	#2,a1
	moveq	#0,d0
	move.w	(a0)+,d0
	swap	d0
	lsr.l	d4,d0
	or.w	d0,4(a1)
	swap	d0
	or.w	d0,(a1)
	addq.l	#2,a1	
	dbf	d5,Go_And_Do
	addq.l	#4,a1
	moveq	#5,d5
	dbf	d6,Go_And_Do
	addq.w	#1,d4
	dbf	d7,Do_It
	rts
zmiana	ds.w	1
linia	ds.w	1
last	ds.w	1
droga	
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,30,31,31
 dc.w 31,30,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,0,0
 dc.w 1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,30,31,31
 dc.w 31,30,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 1,2,4,6,8,10,12,13,14,14,15,16
 dc.w 16,15,14,14,13,12,10,8,6,4,2,1
 dc.w 1,2,4,6,8,10,12,13,14,14,15,16
 dc.w 16,15,14,14,13,12,10,8,6,4,2,1
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 1,0,0,1,0,0,1,0,0,1
 dc.w 0,0,1,0,0,1,0,0,1,0
 dc.w 0,1,0,0,1,0,0,1,0,0
 dc.w 1,0,0,1,0,0,1,0,0,1
 dc.w 0,0,1,0,0,1,0,0,1,0
 dc.w 0,1,0,0,1,0,0,1,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,1
 dc.w 2,4,6,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,6,5,5,6,8,9
 dc.w 9,10,9,8,9,6,4,2,1,0
 dc.w 9,6,4,2,1,0,0,1,0,1
 dc.w 0,2,0,2,0,3,0,3,0,4
 dc.w 0,4,0,5,0,5,0,6,0,6
 dc.w 0,7,0,7,0,8,0,8,0,9
 dc.w 0,9,0,10,0,10,0,11,0,11
 dc.w 0,12,0,12,0,13,0,13,0,12
 dc.w 0,12,0,11,0,11,0,10,0,10
 dc.w 0,9,0,9,0,8,0,8,0,7
 dc.w 0,7,0,6,0,6,0,5,0,5
 dc.w 0,4,0,4,0,3,0,3,0,2
 dc.w 0,2,0,1,0,1,0,1,0,2
 dc.w 0,3,0,4,0,5,0,6,0,7
 dc.w 0,8,0,9,0,10,0,11,0,12
 dc.w 0,13,0,12,0,11,0,10,0,9
 dc.w 0,8,0,7,0,6,0,5,0,4
 dc.w 0,3,0,2,0,1,0,1,0,2
 dc.w 0,3,0,4,0,5,0,6,0,7
 dc.w 0,8,0,9,0,10,0,11,0,12
 dc.w 0,13,0,12,0,11,0,10,0,9
 dc.w 0,8,0,7,0,6,0,5,0,4
 dc.w 0,3,0,2,0,1,0,1,0,2
 dc.w 0,3,0,4,0,5,0,6,0,7
 dc.w 0,8,0,9,0,10,0,11,0,12
 dc.w 0,13,0,12,0,11,0,10,0,9
 dc.w 0,8,0,7,0,6,0,5,0,4
 dc.w 0,3,0,2,0,1,0,1,0,1
 dc.w 0,2,0,2,0,3,0,3,0,4
 dc.w 0,4,0,5,0,5,0,6,0,6
 dc.w 0,7,0,7,0,8,0,8,0,9
 dc.w 0,9,0,10,0,10,0,11,0,11
 dc.w 0,12,0,12,0,13,0,13,0,12
 dc.w 0,12,0,11,0,11,0,10,0,10
 dc.w 0,9,0,9,0,8,0,8,0,7
 dc.w 0,7,0,6,0,6,0,5,0,5
 dc.w 0,4,0,4,0,3,0,3,0,2
 dc.w 0,2,0,1,0,1,4,8,12,15
 dc.w 14,13,11,9,6,4,2,1,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 1,0,0,1,0,0,1,0,0,1
 dc.w 0,0,1,0,0,1,0,0,1,0
 dc.w 0,1,0,0,1,0,0,1,0,0
 dc.w 1,0,0,1,0,0,1,0,0,1
 dc.w 0,0,1,0,0,1,0,0,1,0
 dc.w 0,1,0,0,1,0,0,1,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 1,0,0,1,0,0,1,0,0,1
 dc.w 0,0,1,0,0,1,0,0,1,0
 dc.w 0,1,0,0,1,0,0,1,0,0
 dc.w 1,0,0,1,0,0,1,0,0,1
 dc.w 0,0,1,0,0,1,0,0,1,0
 dc.w 0,1,0,0,1,0,0,1,0,0
 dc.w 0,2,4,4,2,0,0,2,4,4
 dc.w 2,0,0,2,4,4,2,0,0,2
 dc.w 4,4,2,0,0,2,4,4,2,0
 dc.w 0,2,4,4,2,0,0,2,4,4
 dc.w 2,0,0,2,4,4,2,0,0,2
 dc.w 4,4,2,0,0,2,4,4,2,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 5,5,6,8,9,9,10,9,8,6
 dc.w 4,2,1,0,1,2,2,1,0,0
 dc.w 1,2,2,1,0,0,1,2,2,1
 dc.w 0,0,1,2,2,1,0,0,1,2
 dc.w 2,1,0,0,1,2,2,1,0,0
 dc.w 1,2,2,1,0,0,1,2,2,1
 dc.w 0,0,1,2,2,1,0,0,1,2
 dc.w 2,1,0,0,2,4,4,2,0,0
 dc.w 2,4,4,2,0,0,2,4,4,2
 dc.w 0,0,2,4,4,2,0,0,2,4
 dc.w 4,2,0,0,2,4,4,2,0,0
 dc.w 2,4,4,2,0,0,2,4,4,2
 dc.w 0,0,2,4,4,2,0,0,2,4
 dc.w 4,2,0,0,1,2,3,4,5,6
 dc.w 2,1,0,6,4,2,1,0,1,2
 dc.w 2,1,0,0,1,2,2,1,0,0
 dc.w 1,2,2,1,0,0,1,2,2,1
 dc.w 0,0,1,2,2,1,0,0,1,2
 dc.w 2,1,0,0,1,2,2,1,0,0
 dc.w 1,2,2,1,0,0,1,2,2,1
 dc.w 0,0,1,2,2,1,0,0,2,4
 dc.w 4,2,0,0,2,4,4,2,0,0
 dc.w 2,4,4,2,0,0,2,4,4,2
 dc.w 0,0,2,4,4,2,0,0,2,4
 dc.w 4,2,0,0,2,4,4,2,0,0
 dc.w 2,4,4,2,0,0,2,4,4,2
 dc.w 0,0,2,4,4,2,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,0,0,0
 dc.w 0,0,0,0,0,0,0,-1,-1



	dc.l	-1
screen	dc.l	$78020
adresiki ds.l	17
pal	dc.w	$000,$666,$444,$555,$510,$620,$730,$740
	dc.w	$750,$760,$002,$013,$024,$035,$046,$057
oryginal dc.l	$FFFC8004,$00030002,$FF830004,$80014001
	dc.l	$FFFFFFFF,$FFFCFFFC,$FFFC0000,$00030000
	dc.l	$FF870008,$E0012002,$FFFF0000,$FFFC0002
	dc.l	$7FF88004,$00010003,$FF0F0090,$E0031000
	dc.l	$FFFF0000,$FFFE0002,$3FF00000,$00000000
	dc.l	$FE1F8110,$F0030804,$FFFF0000,$FFFE0000
	dc.l	$1FF02000,$00000000,$7C1F8220,$F8030804
	dc.l	$F83F0600,$E0FE0301,$1FF00000,$00000000
	dc.l	$7C3F8400,$F8070404,$E03F1000,$E03E0041
	dc.l	$0FF01008,$00000000,$783F8440,$FC070000
	dc.l	$C03F2000,$E01E0021,$0FF81008,$00000000
	dc.l	$F87F0000,$FC070200,$803F4000,$E00E0011
	dc.l	$0FF80000,$00000001,$F87B0884,$FE070000
	dc.l	$003F8000,$E000000F,$07F80804,$00000001
	dc.l	$F0F9080A,$FE000106,$003F0000,$E0000000
	dc.l	$07F80804,$00010001,$F0F10908,$FF000000
	dc.l	$003F0000,$E0000000,$07FC0000,$00010000
	dc.l	$F1F00001,$FF000080,$003F0000,$E0000000
	dc.l	$03FC0402,$00010002,$E1E01210,$FF800000
	dc.l	$003F0000,$E0000000,$03FC0002,$00030002
	dc.l	$E3E01000,$7F808040,$003F0000,$E0000000
	dc.l	$03FE0200,$00030000,$E3C00420,$7FC00000
	dc.l	$003F0000,$E0000000,$01FE0201,$00030004
	dc.l	$C7C02400,$7FC04020,$003F0000,$E0000000
	dc.l	$01FE0001,$00070004,$C7C00040,$3FE04020
	dc.l	$003F0000,$E0000000,$00FF0100,$00070000
	dc.l	$C7804840,$3FE02010,$003F0000,$E0000000
	dc.l	$00FF0100,$00078008,$8F804000,$1FE02010
	dc.l	$003F0000,$E0000000,$00FF0080,$800F8000
	dc.l	$8F001080,$1FF00000,$003F0000,$E0000000
	dc.l	$007F0080,$800F4010,$8F009080,$0FF01008
	dc.l	$003F0000,$E0000000,$007F0000,$801F4010
	dc.l	$1F008000,$0FF80008,$003F0000,$E0000000
	dc.l	$003F0040,$C01F0020,$1E002100,$0FF80804
	dc.l	$003F0000,$E0000000,$003F0040,$C01E2021
	dc.l	$3FFF2000,$FFF80004,$003F0000,$E0000000
	dc.l	$003F0020,$E03E0000,$3FFF0000,$FFFC0000
	dc.l	$003F0000,$E0000000,$001F0020,$E03E1042
	dc.l	$3FFF4000,$FFFC0002,$003F0000,$E0000000
	dc.l	$001F0010,$F07C0002,$7FFF4000,$FFFE0002
	dc.l	$003F0000,$E0000000,$000F0010,$F07C0880
	dc.l	$7C000000,$03FE0000,$003F0000,$E0000000
	dc.l	$000F0000,$F8F80084,$78008400,$01FE0201
	dc.l	$003F0000,$E0000000,$00070008,$F8F80500
	dc.l	$F8008400,$01FF0201,$003F0000,$E0000000
	dc.l	$00070000,$FDF00108,$F8000000,$01FF0100
	dc.l	$003F8000,$E0000000,$00030004,$FFF00201
	dc.l	$F0000800,$00FF0100,$003F8020,$E0000000
	dc.l	$00030000,$FFE00011,$F0000800,$00FF0000
	dc.l	$803F0020,$E0000000,$00010002,$FFE10000
	dc.l	$F0000000,$00FF0080,$803F4020,$E0000000
	dc.l	$00010001,$FFC10022,$F0000000,$007F0080
	dc.l	$803F4020,$E0000000,$00000001,$FFC30040
	dc.l	$F0000000,$007F0080,$C03F0000,$E0001000
	dc.l	$00000000,$7F878044,$F0000800,$00FF0080
	dc.l	$E03F2040,$F0000800,$00000000,$7F0F4090
	dc.l	$FC000400,$01FF0300,$F0FF1900,$FC000600
	dc.l	$00000000,$3F0F2110,$FC000200,$01FF0200
	dc.l	$F1FF0800,$FC000200,$00000000,$1E0F1210
	dc.l	$FC000400,$01FF0200,$F0FF0900,$FC000200
copies	ds.l	2
