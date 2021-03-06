  data
fileprompt:             ; prompt for filename
  dc.b 'Please enter the filename (& pathname if needed) of the',10,13,0
picture:
  DC.B 'CHARACTER file to extract the characters from.',13,10,0
characters:
  dc.b 'file to output the IMAGE to.',10,13,0
reading:
  dc.b 10,13,'Loading CHARACTERS. ',10,13,0
saveing:
  dc.b 10,13,'Saving IMAGES. ',10,13,10,0

input:                  ; adress of input string
  dc.b 64,0
filespec:               ; actual string begins here
  ds.b 64
        ALIGN
HEADER:
  dc.b $00,$00,$00,$00,$02,$22,$07,$56,$00,$57,$00,$07,$02,$27,$02,$77
  dc.b $00,$47,$04,$55,$00,$30,$00,$70,$07,$00,$06,$41,$07,$70,$05,$31
  dc.b $07,$77

  bss
handle:                 ; space for file handle
  ds.w 1
HANDLE2 DS.W 1

BUFFER:
  DS.w 1                ; res value
pal:
  ds.w 16               ; palette
bitmap:                 ; image
  ds.b 64000
BITMAP2:                   ; converted data
  ds.b 32000

BUFFER3:
   DS.B 64000

  text
  move.l #fileprompt,d0 ; prompt for filename input
  jsr prtline
  move.l #picture,d0
  jsr prtline

  move.l #input,-(sp)   ; input filename
  move #$a,-(sp)
  trap #1
  addq.l #6,sp

  lea filespec,a0
  move.b #0,(a0,d0.w)   ; null terminate the string

  move #2,-(sp)         ; open file
  move.l #filespec,-(sp)
  move #$3d,-(sp)
  trap #1
  addq.l #8,sp
  move d0,handle

  move.l #reading,d0    ; inform reading
  jsr prtline

  move.l #buffer3,-(sp)  ; read
  move.l #64000,-(sp)
  move handle,-(sp)
  move #$3f,-(sp)
  trap #1
  add.l #12,sp

  move handle,-(sp)     ; close file
  move #$3e,-(sp)
  trap #1
  addq.l #6,sp

        LEA     BUFFER3,A0
        LEA     BITMAP,A1
        MOVE    #25-1,D2        ; ROWS
COPYLOOP3:
        MOVE    #20-1,D1        ; COLUMNS
COPYLOOP2:
        MOVE    #16-1,D0        ; LINES/CHR
COPYLOOP1:
        MOVE.L  (A0)+,(A1)
        MOVE.L  (A0)+,4(A1)
        ADD.L   #160,A1
        DBRA    D0,COPYLOOP1
        SUB.L   #(160*16)-8,A1
        DBRA    D1,COPYLOOP2
        ADD.L   #160*15,A1
        DBRA    D2,COPYLOOP3

  move.l #fileprompt,d0 ; prompt for filename output
  jsr prtline
  move.l #characters,d0
  jsr prtline

  move.l #input,-(sp)   ; input filename
  move #$a,-(sp)
  trap #1
  addq.l #6,sp

  lea filespec,a0
  move.b #0,(a0,d0.w)   ; null terminate the string

  move #0,-(sp)         ; create file
  move.l #filespec,-(sp)
  move #$3c,-(sp)
  trap #1
  addq.l #8,sp
  move d0,handle

  move.l #saveing,d0    ; inform saveing
  jsr prtline

  move.l #HEADER,-(sp)  ; save
  move.l #34,-(sp)
  move handle,-(sp)
  move #$40,-(sp)
  trap #1
  add.l #12,sp

  move.l #BITMAP,-(sp)  ; save
  move.l #32000,-(sp)
  move handle,-(sp)
  move #$40,-(sp)
  trap #1
  add.l #12,sp

  move handle,-(sp)     ; close file
  move #$3e,-(sp)
  trap #1
  addq.l #6,sp

  move.l #fileprompt,d0 ; prompt for filename output
  jsr prtline
  move.l #characters,d0
  jsr prtline

  move.l #input,-(sp)   ; input filename
  move #$a,-(sp)
  trap #1
  addq.l #6,sp

  lea filespec,a0
  move.b #0,(a0,d0.w)   ; null terminate the string

  move #0,-(sp)         ; create file
  move.l #filespec,-(sp)
  move #$3c,-(sp)
  trap #1
  addq.l #8,sp
  move d0,handle

  move.l #saveing,d0    ; inform saveing
  jsr prtline

  move.l #HEADER,-(sp)  ; save
  move.l #34,-(sp)
  move handle,-(sp)
  move #$40,-(sp)
  trap #1
  add.l #12,sp

  move.l #BITMAP+32000,-(sp)  ; save
  move.l #32000,-(sp)
  move handle,-(sp)
  move #$40,-(sp)
  trap #1
  add.l #12,sp

  move handle,-(sp)     ; close file
  move #$3e,-(sp)
  trap #1
  addq.l #6,sp

  clr -(sp)             ; term
  trap #1

; ***** subroutines *****

prtline:
  move.l d0,-(sp)
  move #$09,-(sp)
  trap #1
  addq.l #6,sp
  rts

  end
 