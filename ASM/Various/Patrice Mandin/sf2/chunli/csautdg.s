; ----------------------------------
;	sauts diag
; ----------------------------------

vbl_coupssautdg	move	#-1,attack(a6)

	lea	sinus2,a0
	moveq	#0,d0
	add	seq_cpt(a6),a0
	move.b	(a0)+,d0
	ext	d0
	add	d0,yn(a6)
	moveq	#0,d0
	move.b	(a0),d0
	ext	d0

	tst	saut(a6)
	beq.s	coupsdg_sub
	add	d0,xn(a6)
	bra.s	coupsdg_add
coupsdg_sub	sub	d0,xn(a6)
coupsdg_add	addq.w	#2,seq_cpt(a6)

	cmp	#48,seq_cpt(a6)
	bmi.s	coupsdg_cont
	cmp	#base_y,yn(a6)
	bpl.s	coupsdg_fin
	
coupsdg_cont	move.l	seq_spr(a6),a0
	move.b	5(a0),d0
	addq.w	#1,seq_vbl(a6)
	cmp.b	1+seq_vbl(a6),d0
	bne	coupsdg_limit
	clr	seq_vbl(a6)
	addq	#8,a0
	move.l	a0,seq_spr(a6)
	tst.l	(a0)
	bne	coupsdg_limit
coupsdg_fin	clr	seq_vbl(a6)
	SEQUENCE	#vbl_releve,#fin_de_saut
	move	#base_y,yn(a6)
	clr	attack(a6)
	SAMPLE	endjump

coupsdg_limit	move	maxecrx(a6),d0
	sub	#22,d0
	cmp	xn(a6),d0
	bpl	coupsdg_minx
	move	d0,xn(a6)
	bra	finjoueur
coupsdg_minx	move	minecrx(a6),d0
	add	#22,d0
	cmp	xn(a6),d0
	bmi	finjoueur
	move	d0,xn(a6)
	bra	finjoueur
