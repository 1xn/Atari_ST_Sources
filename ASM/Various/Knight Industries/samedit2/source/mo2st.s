	SECTION	text
convertMonoToStereo
	lea	sampleInfoTable,a3

	cmpi.w	#1,sampleChannels(a3)
	bne	.notMono

	graf_mouse	#2,#0

	tst.w	sampleMode(a3)
	bne	.convertMonoToStereoD2D

	move.l	sampleDataSize(a3),d3
	rol.l	#1,d3

; reserve new memory

	m_xalloc	#0,d3

	tst.l	d0
	beq	.noMemory

	move.l	d0,a1	; new address

	move.l	sampleAddress(a3),a0
	move.l	sampleDataSize(a3),d0
	move.l	d3,d1
	move.l	d1,sampleDataSize(a3)

	move.w	#2,sampleChannels(a3)
	move.l	a1,a2

	subq.l	#2,d0
	cmpi.w	#16,sampleResolution(a3)
	beq	.loop16

	addq.l	#1,d0
.loop8
	move.b	(a0)+,d2
	move.b	d2,(a1)+
	move.b	d2,(a1)+
	subq.l	#1,d0
	bgt	.loop8
	bra	.free

.loop16
	move.w	(a0)+,d2
	move.w	d2,(a1)+
	move.w	d2,(a1)+
	subq.l	#2,d0
	bgt	.loop16

; erase old memory
.free
	tst.w	sampleMode(a3)
	bne	.mo2stDone

	m_free	sampleAddress(a3)

	move.l	a2,sampleAddress(a3)
.done
	clr.w	redrawCached

	move.w	mainWindowHandle,d0
	wind_get	d0,#4
	movem.w	intout+2,d1-d4
	jsr	generalRedrawHandler

	jsr	enterSampleInfo

	lea	blockArea,a0
	clr.l	blockX(a0)
	clr.l	blockX2(a0)
	clr.w	blockDefined(a0)
	move.l	sampleDataSize(a3),d0
	clr.l	blockStart
	move.l	d0,blockEnd
	move.l	d0,blockSize

	graf_mouse	#0,#0
.mo2stDone
	rts

.notMono
	rts

.noMemory
	rsrc_gaddr	#5,#OUTOFMEMORY
	form_alert	#1,addrout
	graf_mouse	#0,#0
	rts
;----------------------------------------------------------------------------
.convertMonoToStereoD2D
	clr.l	blockStart
	move.l	sampleDataSize(a3),d0
	move.l	d0,blockSize
	move.l	d0,blockEnd
	lea	blockArea,a0
	clr.l	blockX(a0)
	clr.l	blockX2(a0)
	clr.w	blockDefined(a0)	; remove defined block

	jsr	initD2DFunction	; open files and read/write
				; header info
; d2 = D2D Buffer Size
; d4 = read file handle
; d5 = write file handle
	move.l	blockSize,d6	; size to process
	move.l	d2,d3
	asr.l	#2,d3	; find quarter buffer size

	lea	.loop8,a4
	cmpi.w	#16,sampleResolution(a3)
	bne	.loop
	lea	.loop16,a4
.loop
	cmp.l	d3,d6
	bgt	.load
	move.l	d6,d3
.load
	f_read	D2DBuffer,d3,d4	; read into buffer

	move.l	D2DBuffer,a0
	move.l	a0,a1
	add.l	d3,a1

	move.l	d3,d0
	move.l	a1,-(sp)
	pea	.returnD2D
	jmp	(a4)
.returnD2D
	move.l	(sp)+,a1
	move.l	d3,d2
	asl.l	#1,d2
	f_write	a1,d2,d5
	cmp.l	d0,d2
	bne	diskFull

	sub.l	d3,d6
	bgt	.loop

	f_close	d4

	lea	sampleInfoTable,a3
	move.l	sampleDataSize(a3),d0
	asl.l	#1,d0
	move.l	d0,sampleDataSize(a3)
	move.w	#2,sampleChannels(a3)
	f_seek	#0,d5,#0
	movem.l	d1-d7/a0-a6,-(sp)
	callModule	#4
	movem.l	(sp)+,d1-d7/a0-a6
	move.w	sampleHeaderSize(a3),d0
	ext.l	d0
	f_write	#sampleHeader,d0,d5
	f_close	d5

	bsr	copyFileD2D

	clr.w	redrawCached
	move.w	mainWindowHandle,d0
	wind_get	d0,#4
	movem.w	intout+2,d1-d4
	jsr	generalRedrawHandler

	jsr	enterSampleInfo
	graf_mouse	#0,#0
	rts