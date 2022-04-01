; Vernamova sifra na architekture DLX
; Valerii Batykov xbatyk00 xbatyk00-r2-r10-r11-r15-r20-r0

        .data 0x04          ; zacatek data segmentu v pameti
login:  .asciiz "xbatyk00"  ; <-- nahradte vasim loginem
cipher: .space 9 ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            ; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          ; adresa zacatku programu v pameti
        .global main        ; 

main:
	addi r15,r0,0       ; r15 to je index a on je na zacatku = 0

plusa:	
	lb r11,login(r15)    ; v r11 ulozime hodnotu z indexem r15
	addi r10,r0,97       ; v r10 zapiseme hodnotu a
	slt r20,r11,r10      ; if r11>r10 then r20 = 1
	bnez r20,cislice     ; jestli dostali cislice, skaceme do navesti cislice
	nop
	addi r11,r11,2  ; sifrujeme tak , ze nas znak posouvame o dva vpred
	addi r10,r0,122 ; v r10 zapiseme hodnotu z
	sgt r20,r11,r10 ; if r11>r10 then r20 = 1
        bnez r20,vetsi  ; jestli nase hodnota vetsi zedka musime odecist 26
	nop
	j mensi         ; jinak pokracujeme
	nop

vetsi: subi r11,r11,26       ; odecitame 26
       
mensi: sb cipher(r15),r11    ; zapiseme v cipher zasifrovany znak
       addi r15,r15,1        ; zvysujeme nas index o 1
       lb r11,login(r15)     ; v r11 ulozime hodnotu z indexem r15
       addi r10,r0,97        ; v r10 zapiseme hodnotu a
       slt r20,r11,r10       ; if r11>r10 then r20 = 1
       bnez r20,cislice      ; jestli dostali cislice, skaceme do navesti cislice
       nop
       subi r11,r11,1        ; sifrujeme tak , ze nas znak posouvame o jednu vzad
       addi r10,r0,97        ; v r10 zapiseme hodnotu acka
       sgt r20,r10,r11       ; if r10>r11 then r20 = 1
       bnez r20, mensi2      ; jestli dostali hodnotu mensi nez acko musime precist 26
       nop
       j vetsi2		     ; jinak pokracujeme
       nop

mensi2: addi r11,r11,26	   ; precitame 26

vetsi2: sb cipher(r15),r11 ; zapiseme v cipher zasifrovany znak
	addi r15,r15,1     ; zvysujeme index o 1
	j plusa            
	nop

cislice: addi r15,r15,1    ; zvysujeme index o 1
	 sb cipher(r15),r0 ; zapiseme v cipher koncovu nulu
       


end:    addi r14, r0, caddr ; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  ; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  ; ukonceni simulace
