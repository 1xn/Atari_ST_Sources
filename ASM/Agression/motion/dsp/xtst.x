;---------------------------------------------------------------
; dsp relocator, feeds p/x/y mem freely..
;---------------------------------------------------------------

PBC	equ	$ffe0
HSR	equ	$ffe9
HTX	equ	$ffeb

	org	p:$0
	jmp	start

;	org	p:$40
	dc	0,0,0,0,0,0,0,0,0,0
	dc	0,0,0,0,0,0,0,0,0,0
	dc	0,0,0,0,0,0,0,0,0,0
	dc	0,0,0,0,0,0,0,0,0,0
	dc	0,0,0,0,0,0,0,0,0,0
	dc	0,0,0,0,0,0,0,0,0,0
	dc	0,0
start	
	add	x,a


secure	movep	#1,X:PBC
	jclr	#0,x:HSR,secure
	move	x:HTX,x0			;pituus

	move	#$200,r0
	
	do	x0,nemesis
genesis	movep	#1,X:PBC
	jclr	#0,x:HSR,genesis
	move	x:HTX,x0
	movem	x0,p:(r0)+
nemesis

	jmp	$200

;	movep	#0,x:$fffe		;port a (luku tahtoo t�kki� ilman..)
;secure2	jclr	#1,X:HSR,secure2
;	move	a1,X:HTX		


	end