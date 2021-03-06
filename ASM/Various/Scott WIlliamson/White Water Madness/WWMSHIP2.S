  text
  move.l #0,-(sp)       ; go to supervisory mode
  move #$20,-(sp)
  trap #1
  add.l #6,sp
  move.l d0,-(sp)       ; and get ready to return to the user mode
  move #$20,-(sp)

  move #34,-(sp)        ; get kbdvec table
  trap #14
  addq.l #2,sp
  move.l d0,a0          ; get joyvec
  move.l a0,kbdvec
  move.l 24(a0),joyvecsave
  move.l #myjoy,24(a0)  ; & install my joy handler

  move.l #enablejoy,-(sp) ; tell kbd procesor to send joy msgs
  clr -(sp)
  move #25,-(sp)
  trap #14
  addq.l #8,sp

  lea screen1+34,a0     ; copy loaded screen to a page boundry
  move.l #screen+$100,d1
  and #$ffffff00,d1
  move.l d1,a1
  move.l a1,screena
  move.l #32000-1,d0
copyscreen:
  move.b (a0)+,(a1)+
  dbra d0,copyscreen

  move #0,-(sp)        ; res   set screen
  move.l screena,-(sp)     ; physbase
  move.l #-1,-(sp)      ; logbase
  move #5,-(sp)
  trap #14
  add.l #12,sp

  move.l #screen1+2,-(sp)    ; set palette
  move #6,-(sp)
  trap #14
  addq.l #6,sp

  move #100,x           ; init x & y
  move #100,y
  jmp draw
; ***** the main ship driver begins here *****

mainloop:               ; this is the main display loop
        MOVE.B  JOY1,D0         ; GET JOY VAL
        AND     #$F,D0          ; MASK OFF UNWANTED INFO
        BEQ     NOJOY           ; JOY IS STILL - SKIP MOVEMENT
        LSL     #1,D0           ; MULT FOR WORD BOUNDRY
        LEA     DIRTAB,A0       ; LOOKUP DIRECTION
        MOVE    0(A0,D0.W),D0   ;
        SUB     DIRECTION,D0    ; (JOY-DIRECTION)&$F
        AND     #$F,D0
        BEQ     AFTERCCW        ; 0 SAME DIRECTION
        CMP     #8,D0
        BLT     CW              ; <8 CLOCKWISE
CCW:    SUBQ    #1,DIRECTION    ; >=8 COUNTERCLOCKWISE
        BRA     AFTERCCW
CW:     ADDQ    #1,DIRECTION
AFTERCCW:

maxframe = 16           ; equate for the maximum # of frames
  cmp #maxframe,direction     ; boundrycheck direction & wraparound
  blt ltmaxframe
  move #0,direction
  bra ge0
ltmaxframe:
  cmp #0,direction
  bge ge0
  move #maxframe-1,direction
ge0:

  lea accel,a0          ; accel table
  move direction,d0     ; get direction
  lsl #1,d0             ; ofset for word lookup
  move (a0,d0.w),d1     ; get y accel
  move d1,vy             ; and adjust velocity
  move 8(a0,d0.w),d1    ; get x accel
  move d1,vx
                        ; boundry check velocity
maxvel = 2              ; maximum velocity
  cmp #maxvel,vy        ; going up too fast?
  ble notfastup
  move #maxvel,vy
  bra chkxvel
notfastup:
  cmp #-maxvel,vy       ; going down too fast?
  bge notfastdown
  move #-maxvel,vy
notfastdown:
chkxvel:
  cmp #maxvel,vx        ; going right too fast?
  ble notfastright
  move #maxvel,vx
  bra velchkd
notfastright:
  cmp #-maxvel,vx       ; going left too fast?
  bge notfastleft
  move #-maxvel,vx
notfastleft:
velchkd:
  bra endfric
NOJOY:                  ; end processing of joystick

  tst.w vx              ; account for horiz friction
  beq vertfric          ; no movement no friction
  bmi leftfric          ; check direction
  sub #1,vx
  bra vertfric
leftfric:
  add #1,vx
vertfric:
  tst.w vy              ; account for vert friction
  beq endfric           ; no movement no friction
  bmi upfric            ; check direction
  sub #1,vy
  bra endfric
upfric:
  add #1,vy
endfric:

undrawship:
  lea undership,a0
  move.l undershipa,a1
  move #16-1,d0         ; undraw 16 lines
undr1:
  move.l (a0)+,(a1)     ; 2 screen words for each line
  move.l (a0)+,4(a1)
  move.l (a0)+,8(a1)
  move.l (a0)+,12(a1)
  add.l #160,a1
  dbra d0,undr1

  move vx,d0
  add d0,x              ; actually move ship
  move vy,d0
  add d0,y

maxx = 150
maxy = 150
minx = 17
miny = 17

  cmp #(maxx)*2,x         ; boundry check ship position
  ble xrok
  move #maxx*2,x
xrok:
  cmp #minx*2,x
  bge xlok
  move #minx*2,x
xlok:
  cmp #maxy*2,y
  ble ybok
  move #maxy*2,y
ybok:
  cmp #miny*2,y
  bge ytok
  move #miny*2,y
ytok:

draw:
  move direction,d0     ; direction is offset into frames
  asl #3,d0             ; bult by 8 (width of a frame in bytes)
  move.l screena,a0     ; fetch screen base for frame lookup
  add d0,a0

  move.l screena,a1     ; and for drawing on
  move y,d0             ; fetch y
  lsr #1,d0             ; ditch decimal
  mulu #160,d0          ; mult by line length
  add.l d0,a1           ; & add to screen base

  move x,d0             ; fetch x
  lsr #1,d0             ; ditch decimal
  move d0,d7            ; d7 & d0 are x (integer)
  and #%1111,d7         ; d7 is ror val
  lsr #1,d0             ; (x/16)*8 = x/2 & %1111111111111000
  and #%1111111111111000,d0
  add d0,a1             ; add word offset to screen ptr
  move.l a1,undershipa  ; save adress of patch

  lea undership,a2      ; save area

  move #16-1,d1         ; 16 lines per cell
drawloop:
  move.l (a1),(a2)+     ; save data
  move.l 4(a1),(a2)+
  move.l 8(a1),(a2)+
  move.l 12(a1),(a2)+
                         ; bpl 0
  clr.l d0              ; clear work reg
  move.w (a0),d0        ; get data
  ror.l d7,d0           ; rotate it
  move.w d0,(a1)        ; store left side
  swap d0               ;
  move.w d0,8(a1)       ; and store right side
                         ; bpl 1
  clr.l d0              ; clear work reg
  move.w 2(a0),d0       ; get data
  ror.l d7,d0           ; rotate it
  move.w d0,2(a1)       ; store left side
  swap d0               ;
  move.w d0,10(a1)      ; and store right side
                         ; bpl 2
  clr.l d0              ; clear work reg
  move.w 4(a0),d0       ; get data
  ror.l d7,d0           ; rotate it
  move.w d0,4(a1)       ; store left side
  swap d0               ;
  move.w d0,12(a1)      ; and store right side
                         ; bpl 3
  clr.l d0              ; clear work reg
  move.w 6(a0),d0       ; get data
  ror.l d7,d0           ; rotate it
  move.w d0,6(a1)       ; store left side
  swap d0               ;
  move.w d0,14(a1)      ; and store right side

  add.l #160,a0         ; next line read
  add.l #160,a1         ; next line write
  dbra d1,drawloop

  btst #7,joy1
  bne mainloop
  move #37,-(sp)        ; wvbl
  trap #14
  addq.l #2,sp
  bra mainloop

exit:
  move.l kbdvec,a0      ; restore old joy routine
  move.l joyvecsave,24(a0)

  trap #1               ; back to user
  clr -(sp)
  trap #1               ; term - goodbye

myjoy:                  ; my joystick handling routine
  move.b 1(a0),joy0     ; fetch & store vals
  move.b 2(a0),joy1
notjoy1:
  rts

  data
  align
accel:      ; ACCELERATION TABLE
  dc.w -2,-2,-2,-1
  dc.w 0,1,2,2,2,2,2,1,0,-1,-2,-2,-2,-2,-2,-1
;JOY  1      DIR    0
;    5 9          14 2
;   4   8        12   4
;    6 10         10 6
;     2             8
DIRTAB:
        DC.W 0,0,8,0,12,14,10,0,4,2,6,0,0,0,0,0

screen1:
  ibytes 'D:\artprogs\degas\pictures\wwmship.pi1',32034
enablejoy:              ; kbd command to enable the joystick
  dc.b $14

  bss
  align
screen:                 ; screen buffer to align on page boundry
  ds.b 32000+$100
screena:                ; adress of the screen adress
  ds.l 1
joyvecsave:             ; save area for old joy vector
  ds.l 1
kbdvec:                 ; ditto for kbd vector
  ds.l 1
direction:              ; direction ship is facing
  ds.w 1
vx:                     ; ship velocity
  ds.w 1
vy:
  ds.w 1
x:                      ; ship position
  ds.w 1
y:
  ds.w 1
undership:              ; background save area
  ds.b 16*16            ; 2 words by 16 lines
undershipa:             ; adr of this patch on screen
  ds.l 1
joy0:                   ; obviously joysticks
  ds.b 1
joy1:
  ds.b 1
  end
 