; UNLOCKSN - Gibt das Soundsubsystem frei
; z.B. nachdem Tetrax abgest�rzt ist

            move.w  #129,-(sp)
            trap    #14             ; Unlocksnd()
            addq.l  #2,sp

            clr.w   -(sp)
            trap    #1              ; Pterm0
