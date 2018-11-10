;****** Procedures d'affichage pour langages evolues. ******

.model small

.stack 100h

MOD_GRAPH       = 0013H

.code

	public _init
	public _waitretrace
	public _pixel
	public _scroll

_init   proc near

	mov ah,00h
	mov al,MOD_GRAPH
	int 10h

	ret

_init   endp

_waitretrace proc near

	mov dx,3dah             ;Input Status Register 1
@wait1: in al,dx                ;Bit3 = 0 si le rayon est en train de
	test al,08h             ;dessiner l'image de l'ecran
	jnz @wait1
@wait2: in al,dx                ;Bit3 = 1 si le rayon est en retour de
	test al,08h             ;balayage
	jz @wait2
	ret                     ;le rayon est en bas de l'ecran

_waitretrace	endp

_pixel  proc near

sframe  struc
bp0             dw ?
ret_adr0        dw ?
posx            dw ?
posy            dw ?
couleur			 dw ?
sframe  ends

frame   equ [ bp - bp0 ]
	push bp
	mov bp,sp

	push di

	mov ax,0a000h           ;Adresse de la memoire graphique
	mov es,ax
	mov ax,320              ;Formule de calcul du point :
				;Offset = Y*320+X
	mul frame.posy
	add ax,frame.posx
	mov di,ax
	mov al,byte ptr frame.couleur
	mov es:[di],al

	pop di
	pop bp
	ret

_pixel  endp

_scroll proc near

sframe1         struc
bp1             dw ?
ret_adr1        dw ?
y               db ?
sframe1         ends

frame1  equ [ bp - bp1 ]
	push bp
	mov bp,sp

	mov dx,3d4h

	mov al,0ch
	mov ah,byte ptr frame1.y+1
	out dx,ax

	mov al,0dh
	mov ah,byte ptr frame1.y
	out dx,ax

	pop bp
	ret

_scroll endp

	end
