; +======================================================================+
; | Overscan/Fullscreen routines                                         |
; +======================================================================+
; | Copyright (c) 2015-2017 Frederic Poeydomenge <dyno@aldabase.com>     |
; +----------------------------------------------------------------------+
; | This program is free software: you can redistribute it and/or modify |
; | it under the terms of the GNU General Public License as published by |
; | the Free Software Foundation, either version 3 of the License, or    |
; | (at your option) any later version.                                  |
; |                                                                      |
; | This program is distributed in the hope that it will be useful,      |
; | but WITHOUT ANY WARRANTY; without even the implied warranty of       |
; | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         |
; | GNU General Public License for more details.                         |
; |                                                                      |
; | You should have received a copy of the GNU General Public License    |
; | along with this program. If not, see <http://www.gnu.org/licenses/>. |
; +======================================================================+

;#========#
;# Macros #
;#========#

switch_res_hi_low: MACRO
	move.w	a1,(a1)		; [ 2] Change the resolution to high-res
	nop			; [ 1]
	move.b	d0,(a1)		; [ 2] Get back to low-res
	ENDM

switch_freq_60_50: MACRO
	move.b	d0,(a0)		; [ 2] Change the display frequency to 60Hz
	move.w	a0,(a0)		; [ 2] Get back to 50Hz
	ENDM

stabilisator: MACRO
	switch_res_hi_low
	ENDM

;#========================#
;# Fullscreen VBL routine #
;#========================#

full_vbl:	move.l	#.open_top,HBL.w	; HBL for the upper border
	move.w	#33,full_hbl_count	; Set line counter
	move.w	#$2100,sr		; SR set
	bsr.s	call_qvbl		; Call QVBL
	move.w	#$2100,(sp)		; SR set
	rte

.open_top:	subq.w	#1,full_hbl_count	; Count lines
	beq.s	.open_top1		; At line 34, open top border
	rte

.open_top1:	move.l	#.open_top2,HBL.w
	stop	#$2100
	; The processor can be awakened only by HBL back to life.

.open_top2:	move	#$2700,sr		; [ 4] No disturbance
	addq.l	#6,sp		; [ 2] Repair stack
	move.l	d0,-(sp)		; [ 3] Save d0
	lea	SCREEN_FREQ.w,a0	; [ 2] Video synchronization mode
	lea	SCREEN_RES.w,a1	; [ 2] Shifter resolution
	moveq	#0,d0		; [ 1]
	fill_nop_78
	move.b	d0,(a0)		; [ 2] Set synchronization mode to 60 Hz
	fill_nop_19
	move.w	a0,(a0)		; [ 2] Set synchronization mode to 50 Hz
	; From here the top border is opened
	
.wait_sync:	move.b	VIDEO_COUNTER.w,d0	; Video address pointer (Low byte)
	beq.s	.wait_sync
	not.w	d0		; Negate value
	lsr.w	d0,d0		; Synchronization with the beam
	; From here the code is synchronous with the scanning beam

full_vbl_jump:
	lea	full_table,a6	; [ 3]
	move.l	(a6)+,a5		; [ 3]
	jmp	(a5)		; [ 2]

full_vbl_end:
	; End of the fullscreen VBL routine
	move.l	(sp)+,d0		; Restore d0
	move.w	#$2300,(sp)		; SR set
	addq.w	#1,K_NB_VBL.w	; Increment number of VBL routines
	rte

;#=========================#
;# Install fullscreen code #
;#=========================#
; a0 = table address

full_setup_realtime:
	move.l	a0,full_base_address
	move.l	a0,full_vbl_jump+2
	rts

;#==========================#
;# Generate fullscreen code #
;#==========================#
; Generates a buffer, which procure more free time (5 nops)
;   on each scanline, but at the expense of the occupied memory size.
; Warning : there is absolutely no check for buffer overflow !
;
; Input:
;     a0 = table address
;     a1 = generated buffer address

full_setup_generated:
	move.l	a1,full_base_address
	move.l	a1,full_vbl_jump+2

	lea	32(a1),a2		; a2 = @buffer + 32
	moveq	#8-1,d2
.copyg:	move.l	a2,(a1)+		; store a2 in 8 first positions of the buffer
	dbra	d2,.copyg		; at the end, a1 = @buffer + 32

.loopg:	move.l	(a0)+,a2
.gencg:	move.w	(a2)+,(a1)+
		
	cmpi.w	#OPCODE_RTE,(a2)
	beq.s	.endg

	cmpi.l	#$2A5E4ED5,(a2)	; move.l (a6)+,a5 + jmp (a5)
	bne.s	.nojumpg
	REPT	5
	move.w  	#OPCODE_NOP,(a1)+
	ENDR
	addq.l	#4,a2
	bra.s	.loopg		; next rout
.nojumpg:
	cmpi.w	#OPCODE_RTS,(a2)
	bne.s	.gencg

	bra.s	.loopg		; next rout
.endg:	move.w	#OPCODE_RTE,(a1)+

	rts

;#=================================================#
;# Routs table for the synchro-line + 276 overscan #
;# lines + end of the fullscreen VBL routine       #
;#=================================================#

full_table:	; Reserved for Sync-scroll
	dc.l	sync_F160
	REPT 5
	dc.l	sync_L230
	ENDR
	; Following overscan lines
	REPT 227-5
	dc.l	sync_L230
	ENDR
	dc.l	rout_bottom_1
	dc.l	rout_bottom_2
	REPT 47
	dc.l	sync_L230
	ENDR
	dc.l	full_vbl_end

;#===================#
;# Overscan routines #
;#===================#

;+---------------------------------+
;| Open bottom border #1 (127 nop) |
;+---------------------------------+

rout_bottom_1:
	switch_res_hi_low		; [ 5]
	fill_nop_89			; [89]
	switch_freq_60_50		; [ 4]
	fill_nop_10			; [10]
	move.w	d0,(a0)		; [ 2] Bottom margin 1
	nop			; [ 1]
	stabilisator		; [ 5]
	fill_nop_6			; [ 6]
	move.l	(a6)+,a5		; [ 3] Next line
	jmp	(a5)		; [ 2]

;+---------------------------------+
;| Open bottom border #2 (129 nop) |
;+---------------------------------+

rout_bottom_2:
	move.w	a1,(a1)		; [ 2] Left border
	move.w	a0,(a0)		; [ 2] Bottom margin 2
	move.b	d0,(a1)		; [ 2]
	fill_nop_89			; [89]
	switch_freq_60_50		; [ 4]
	fill_nop_13			; [13]
	stabilisator		; [ 5]
	fill_nop_7			; [ 7]
	move.l	(a6)+,a5		; [ 3] Next line
	jmp	(a5)		; [ 2]

;#=======================#
;# Configuration Address #
;#=======================#
; address of the table (Real-time mode) or
; address of the buffer (Generated-code mode)

full_base_address:	dc.l full_table

;#===================================#
;# Line counter for the 33 top lines #
;#===================================#

full_hbl_count:	dc.w 0
