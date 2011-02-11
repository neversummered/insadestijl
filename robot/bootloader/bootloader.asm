;***************************************************************************
;*
;* Bootloader
;*
;*
;***************************************************************************
.include "m88Pdef.inc"

.org 		0xF00 		;Pour un bootloader de 256 mots

;	RJMP	RESET		;Reset Handle
	
;***************************************************************************
;* 
;* RESET
;*
;* Blabla
;*
;***************************************************************************

;***** Subroutine register variables

;***** Code
RESET:
;***** Initialize stack pointer
;* Initialize stack pointer to highest address in internal SRAM
;* Comment out for devices without SRAM

	LDI		R16,high(RAMEND) ;High byte only required if 
	OUT		SPH,R16	         ;RAM is bigger than 256 Bytes
	LDI		R16,low(RAMEND)	 
	OUT		SPL,R16

;	RJMP 	main

;***************************************************************************
;* 
;* main
;*
;* Blabla
;*
;***************************************************************************

;***** Subroutine register variables

;***** Code
main:
	
	STS		UDR0,R16
	

forever:
	RJMP	forever		;eternal loop

;***************************************************************************
;* 
;* writeChar
;*
;* Blabla
;*
;***************************************************************************

;***** Subroutine register variables

;***** Code
writeChar:

