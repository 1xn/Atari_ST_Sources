*-------------------------------------------------------------------------*
		TEXT
		XREF	tc_palette
*-------------------------------------------------------------------------*
	*/------------------------------------/*
		*/------------/*
		
		rsreset
_TRN_SMAP	rs.l	1
_TRN_DMAP	rs.l	1
_TRN_W		rs.w	1
_TRN_H		rs.w	1
_TRN_SREZ	rs.w	1
_TRN_DREZ	rs.w	1
		
		*/------------/*

		* entr�es:
		* pile > long = pointeur sur bloc transico ...
		
		*/------------/*
_TRNF_HARD:
		link	a6,#0
		movem.l	d0-a5,-(sp)
		
		move.l	8(a6),a5
		
		move	12(a5),d6
		move	14(a5),d7
		
		cmp	d6,d7
		blo	.end
		cmp	#16,d7
		beq	.tc_target
		
		*/------------/*
.bit_target
		cmp	#8,d6
		beq	.8_plans
		cmp	#4,d6
		beq	.4_plans
		cmp	#2,d6
		beq	.2_plans
		
		; MONO
		
		bra	.end
		
		*/------------/*
.2_plans		
		cmp	d6,d7
		beq	.2_to_2
		cmp	#4,d7
		beq	.2_to_4
		cmp	#8,d7
		beq	.2_to_8
.2_to_2
		move	8(a5),d4
		lsr	#4,d4
		mulu	10(a5),d4
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	(a4,d4*2),a2
		bra	.do_22
.yop_22		move	(a4)+,(a3)+
		move	(a2)+,(a3)+
.do_22		dbra	d4,.yop_22
		bra	.end		
.2_to_4
		move	8(a5),d4
		lsr	#4,d4
		mulu	10(a5),d4
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	(a4,d4*2),a2
		bra	.do_24
.yop_24		move	(a4)+,d0
		move	(a2)+,d1
		move	d0,(a3)+
		move	d1,(a3)+
		and	d0,d1
		move	d1,(a3)+
		move	d1,(a3)+
.do_24		dbra	d4,.yop_24		
		bra	.end		
.2_to_8
		move	8(a5),d4
		lsr	#4,d4
		mulu	10(a5),d4
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	(a4,d4*2),a2
		bra	.do_28
.yop_28		move	(a4)+,d0
		move	(a2)+,d1
		move	d0,(a3)+
		move	d1,(a3)+
		and	d0,d1
		move	d1,(a3)+
		move	d1,(a3)+
		clr.l	(a3)+
		clr.l	(a3)+
.do_28		dbra	d4,.yop_28		
		bra	.end		
		
		*/------------/*
.4_plans
		cmp	d6,d7
		beq	.4_to_4
		cmp	#8,d7
		beq	.4_to_8

.4_to_4
		move	8(a5),d4
		lsr	#4,d4
		mulu	10(a5),d4
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	(a4,d4*2),a2
		lea	(a2,d4*2),a1
		lea	(a1,d4*2),a0
		bra	.do_44
		
.yop_44		move	(a4)+,(a3)+
		move	(a2)+,(a3)+
		move	(a1)+,(a3)+
		move	(a0)+,(a3)+
.do_44		dbra	d4,.yop_44
		bra	.end		

.4_to_8
		move	8(a5),d4
		lsr	#4,d4
		mulu	10(a5),d4
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	(a4,d4*2),a2
		lea	(a2,d4*2),a1
		lea	(a1,d4*2),a0
		bra	.do_48
.yop_48		move	(a4)+,(a3)+
		move	(a2)+,(a3)+
		move	(a1)+,(a3)+
		move	(a0)+,(a3)+
		clr.l	(a3)+
		clr.l	(a3)+
.do_48		dbra	d4,.yop_48
		bra	.end		

		*/------------/*
.8_plans
		move	8(a5),d4
		mulu	10(a5),d4
		lsr.l	#3,d4
		move	d4,d3
		lsr	d3
		
		move.l	(a5),a4
		move.l	4(a5),a3
		
		lea	(a4,d4*2),a2
		lea	(a2,d4*2),a1
		lea	(a1,d4*2),a0
		bra.s	.do_88
.yop_88		
		move	(a4)+,(a3)+
		move	-2(a4,d4),(a3)+
		move	(a2)+,(a3)+
		move	-2(a2,d4),(a3)+
		move	(a1)+,(a3)+
		move	-2(a1,d4),(a3)+
		move	(a0)+,(a3)+
		move	-2(a0,d4),(a3)+
.do_88		
		dbra	d3,.yop_88
		bra	.end		

		*/------------/*
.tc_target
		cmp	#2,d6
		beq	.2_to_16
		cmp	#4,d6
		beq	.4_to_16
		cmp	#8,d6
		beq	.8_to_16
		
		*/------------/*
.2_to_16
		move	8(a5),d4
		lsr	#4,d4
		mulu	10(a5),d4
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	(a4,d4*2),a2
		lea	tc_palette,a1
		bra	.gloup_216
.zou_216
		move	(a4)+,d0
		move	(a2)+,d1
		moveq	#15,d3
.yop_216
		add	d1,d1
		addx	d2,d2
		add	d0,d0
		addx	d2,d2
		
		move	(a1,d2*2),(a3)+
		dbra	d3,.yop_216
.gloup_216
		dbra	d4,.zou_216
		
		bra	.end
		
		*/------------/*
.4_to_16
		move	8(a5),d4
		mulu	10(a5),d4
		lsr.l	#3,d4
		move	d4,d7
		lsr	d7
		
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	tc_palette,a5
		
		lea	(a4,d4),a2
		lea	(a2,d4),a1
		lea	(a1,d4),a0
		bra.s	.gloup_416
.zou_416
		move	(a4)+,d0
		move	(a2)+,d1
		move	(a1)+,d2
		move	(a0)+,d3
		moveq	#15,d6
.yop_416
		clr	d5
		
		add	d3,d3
		addx	d5,d5
		add	d2,d2
		addx	d5,d5
		add	d1,d1
		addx	d5,d5
		add	d0,d0
		addx	d5,d5
		
		move	(a5,d5*2),(a3)+
		dbra	d6,.yop_416
.gloup_416
		dbra	d7,.zou_416
		
		bra	.end
		
		*/------------/*
.8_to_16
		move	8(a5),d7
		mulu	10(a5),d7
		lsr.l	#3,d7
		move	d7,d4
		lsr	d4
		
		move.l	(a5),a4
		move.l	4(a5),a3
		lea	tc_palette,a5
		
		lea	(a4,d7*2),a2
		lea	(a2,d7*2),a1
		lea	(a1,d7*2),a0
		swap	d4
		swap	d7
		bra.s	.gloup_816
.zou_816
		swap	d4
		move	d7,d4
		swap	d7
		move	d4,d7
		
		move	(a4)+,d0
		move	-2(a4,d7),d1
		move	(a2)+,d2
		move	-2(a2,d7),d3
		move	(a1)+,d4
		move	-2(a1,d7),d5
		move	(a0)+,d6
		move	-2(a0,d7),d7
		
		swap	d6
		move	#15,d6
.yop_816
		swap	d6
		swap	d5
		clr	d5
		
		add	d7,d7
		addx	d5,d5
		add	d6,d6
		addx	d5,d5
		
		swap	d5
		add	d5,d5
		swap	d5
		addx	d5,d5
		
		add	d4,d4
		addx	d5,d5
		add	d3,d3
		addx	d5,d5
		add	d2,d2
		addx	d5,d5
		
		add	d1,d1
		addx	d5,d5
		add	d0,d0
		addx	d5,d5
		
		move	(a5,d5*2),(a3)+
		
		swap	d5
		swap	d6
		dbra	d6,.yop_816
.gloup_816
		swap	d7
		swap	d4
		dbf	d4,.zou_816
		
		*/------------/*
.end
		movem.l	(sp)+,d0-a5
		unlk	a6
		rtd	#4

		*/------------/*
	*/------------------------------------/*
*-------------------------------------------------------------------------*
