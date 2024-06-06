;	Static Name Aliases
;
	TITLE   sconfig.c
	.8087
INCLUDELIB	LLIBCE
SCONFIG_TEXT	SEGMENT  WORD PUBLIC 'CODE'
SCONFIG_TEXT	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
SCONFIG5_DATA	SEGMENT  PARA PUBLIC 'FAR_DATA'
SCONFIG5_DATA	ENDS
DGROUP	GROUP	CONST, _BSS, _DATA
	ASSUME DS: DGROUP, SS: DGROUP
PUBLIC  _evts
PUBLIC  _cur_evts
PUBLIC  _max_evts
PUBLIC  _usrs
PUBLIC  _cur_usrs
PUBLIC  _max_usrs
PUBLIC  _color
PUBLIC  _master_change
PUBLIC  _msgs
PUBLIC  _cur_msgs
PUBLIC  _max_msgs
PUBLIC  _files
PUBLIC  _cur_files
PUBLIC  _max_files
EXTRN	__acrtused:ABS
EXTRN	__cls:FAR
EXTRN	_malloc:FAR
EXTRN	_clrblk:FAR
EXTRN	_qsort:FAR
EXTRN	_prntnomov:FAR
EXTRN	_realloc:FAR
EXTRN	_open_help:FAR
EXTRN	_prntnomovf:FAR
EXTRN	_signal:FAR
EXTRN	_main_menu:FAR
EXTRN	_getcwd:FAR
EXTRN	_sprintf:FAR
EXTRN	_toupper:FAR
EXTRN	_chsize:FAR
EXTRN	_fclose:FAR
EXTRN	_fflush:FAR
EXTRN	_getch:FAR
EXTRN	_atexit:FAR
EXTRN	_fopen:FAR
EXTRN	_fprintf:FAR
EXTRN	_scr_init:FAR
EXTRN	_set_critical_error:FAR
EXTRN	_exit:FAR
EXTRN	_fread:FAR
EXTRN	_file_comp:FAR
EXTRN	_free:FAR
EXTRN	_set_cursor_type:FAR
EXTRN	_fseek:FAR
EXTRN	_msg_comp:FAR
EXTRN	_askyn:FAR
EXTRN	_getenv:FAR
EXTRN	_fwrite:FAR
EXTRN	__setcurpos:FAR
EXTRN	__iob:BYTE
EXTRN	_snow_flag:WORD
_BSS      SEGMENT
COMM NEAR	_tfile:	BYTE:	 169
COMM NEAR	_tuser:	BYTE:	 256
COMM NEAR	_tevt:	BYTE:	 28
COMM NEAR	_tmsg:	BYTE:	 119
COMM NEAR	_buffer:	 1:	 100
COMM NEAR	_cfgpath:	 1:	 257
COMM NEAR	_cptr:	BYTE:	 4
COMM NEAR	_cfgfd:	BYTE:	 4
COMM NEAR	_count:	BYTE:	 2
COMM NEAR	_evtfd:	BYTE:	 4
COMM NEAR	_key:	BYTE:	 2
COMM NEAR	_msgfd:	BYTE:	 4
COMM NEAR	_filefd:	BYTE:	 4
COMM NEAR	_userfd:	BYTE:	 4
COMM NEAR	_cfg:	BYTE:	 2043
_BSS      ENDS
_DATA      SEGMENT
$SG873	DB	'Aug 11 1992',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG874	DB	'SCONFIG (v %u.%02u of %s): Simplex BBS Configuration Editor.'
	DB	00H
$SG875	DB	'Copyright (c) 1989-92, Christopher Laforet and Chris Laforet'
	DB	' Software.',  00H
$SG876	DB	'All Rights Reserved.',  00H
$SG889	DB	'SCONFIG (v %u.%02u): Simplex BBS Configuration Editor.',  0aH
	DB	00H
$SG890	DB	'Copyright (c) 1989-92, Christopher Laforet and Chris Laforet'
	DB	' Software.',  0aH,  00H
$SG891	DB	'All Rights Reserved.',  0aH,  0aH,  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
	ORG	$+48
$SG901	DB	'SIMPLEX',  00H
$SG904	DB	'\',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG905	DB	'Opening help file....',  0aH,  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG906	DB	'%ssconfig.hlp',  00H
$SG913	DB	'sconfig.hlp',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG915	DB	'Notice:  No help file available',  0aH,  00H
$SG917	DB	'Notice:  No help file available',  0aH,  00H
$SG919	DB	'Notice:  No help file available',  0aH,  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG920	DB	'%sconfig.bbs',  00H
$SG922	DB	'r+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG923	DB	07H, 'Unable to open "%s".',  0aH, 'May I create it? ',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG928	DB	'Yes',  00H
$SG929	DB	'No',  00H
$SG930	DB	'%s',  0aH,  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG932	DB	0aH, 'Error: Unable to continue without open file!',  0aH,  0aH
	DB	00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG934	DB	'w+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG935	DB	0aH, 'Error: Unable to create %s!',  0aH,  0aH,  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG936	DB	'%sevents.bbs',  00H
$SG938	DB	'r+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG939	DB	07H, 'Unable to open "%s".',  0aH, 'May I create it? ',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG944	DB	'Yes',  00H
$SG945	DB	'No',  00H
$SG946	DB	'%s',  0aH,  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG948	DB	0aH, 'Error: Unable to continue without open file!',  0aH,  0aH
	DB	00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG950	DB	'w+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG951	DB	0aH, 'Error: Unable to create %s!',  0aH,  0aH,  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG952	DB	'%suserlist.bbs',  00H
$SG954	DB	'r+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG955	DB	07H, 'Unable to open "%s".',  0aH, 'May I create it? ',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG960	DB	'Yes',  00H
$SG961	DB	'No',  00H
$SG962	DB	'%s',  0aH,  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG964	DB	0aH, 'Error: Unable to continue without open file!',  0aH,  0aH
	DB	00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG966	DB	'w+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG967	DB	0aH, 'Error: Unable to create %s!',  0aH,  0aH,  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG968	DB	'%smsgarea.bbs',  00H
$SG970	DB	'r+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG971	DB	07H, 'Unable to open "%s".',  0aH, 'May I create it? ',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG976	DB	'Yes',  00H
$SG977	DB	'No',  00H
$SG978	DB	'%s',  0aH,  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG980	DB	0aH, 'Error: Unable to continue without open file!',  0aH,  0aH
	DB	00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG982	DB	'w+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG983	DB	0aH, 'Error: Unable to create %s!',  0aH,  0aH,  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG984	DB	'%sfilearea.bbs',  00H
$SG986	DB	'r+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG987	DB	07H, 'Unable to open "%s".',  0aH, 'May I create it? ',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG992	DB	'Yes',  00H
$SG993	DB	'No',  00H
$SG994	DB	'%s',  0aH,  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG996	DB	0aH, 'Error: Unable to continue without open file!',  0aH,  0aH
	DB	00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG998	DB	'w+b',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG999	DB	0aH, 'Error: Unable to create %s!',  0aH,  0aH,  00H
$SG1000	DB	'Reading configuration information....',  00H
$SG1002	DB	'ATZ|~~~AT&D3V1X4E0F1Q0C1H0M0S0=0S7=15|',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG1003	DB	'OK',  00H
$SG1004	DB	'CONNECT',  00H
$SG1005	DB	'CONNECT 1200',  00H
$SG1006	DB	'CONNECT 2400',  00H
$SG1007	DB	'CONNECT 4800',  00H
$SG1008	DB	'CONNECT 9600',  00H
$SG1009	DB	'CONNECT 7200',  00H
$SG1010	DB	'CONNECT 14400',  00H
$SG1011	DB	'CONNECT 12000',  00H
$SG1012	DB	'RING',  00H
$SG1013	DB	'ATA|',  00H
$SG1014	DB	'ATDT',  00H
$SG1015	DB	'|',  00H
$SG1016	DB	'~~~~+++~~~~ATH0|',  00H
$SG1017	DB	'ATM0H1|',  00H
$SG1018	DB	'ARC a %A %F',  00H
$SG1019	DB	'ARC xo %S %D*.*',  00H
$SG1020	DB	'PKZIP -a %A %F',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG1021	DB	'PKUNZIP -x -o %S %D',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
$SG1022	DB	'LHARC m %A %F',  00H
$SG1023	DB	'LHARC x %S %D',  00H
_DATA      ENDS
SCONFIG5_DATA      SEGMENT
$SG1029	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1031	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1037	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1039	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1041	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1043	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1049	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1051	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1058	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1060	DB	'Error: Out of memory to allocate structures!',  0aH,  00H
$SG1064	DB	'Do you want to save the changes made during this session? (Y'
	DB	'/N) ',  00H
$SG1065	DB	'Save Configuration Changes?',  00H
SCONFIG5_DATA      ENDS
_DATA      SEGMENT
	ORG	$-466
_master_change	DW	00H
_msgs	DD	00H
_cur_msgs	DW	00H
_max_msgs	DW	00H
_files	DD	00H
_cur_files	DW	00H
_max_files	DW	00H
_evts	DD	00H
_cur_evts	DW	00H
_max_evts	DW	00H
_usrs	DD	00H
_cur_usrs	DW	00H
_max_usrs	DW	00H
_color	DW	017H
	DW	01eH
	DW	04fH
	DW	047H
	DW	04eH
	DW	03H
	DW	0eH
_DATA      ENDS
SCONFIG_TEXT      SEGMENT
	ASSUME	CS: SCONFIG_TEXT
; Line 1
; Line 15
; Line 16
; Line 17
; Line 18
; Line 19
; Line 20
; Line 21
; Line 22
; Line 16
; Line 23
; Line 24
; Line 25
; Line 65
SCONFIG_TEXT      ENDS
CONST      SEGMENT
$T20008	DW SEG _filefd 
$T20009	DW SEG _msgfd 
$T20010	DW SEG _evtfd 
$T20011	DW SEG _cfgfd 
$T20012	DW SEG _userfd 
CONST      ENDS
SCONFIG_TEXT      SEGMENT
	ASSUME	CS: SCONFIG_TEXT
	PUBLIC	_cleanup
_cleanup	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,10
	push	si
;	count = -2
; Line 68
	mov	es,WORD PTR $T20008
	push	WORD PTR es:_filefd+2
	push	WORD PTR es:_filefd
	call	FAR PTR _fclose
	add	sp,4
; Line 69
	mov	es,WORD PTR $T20009
	push	WORD PTR es:_msgfd+2
	push	WORD PTR es:_msgfd
	call	FAR PTR _fclose
	add	sp,4
; Line 70
	mov	es,WORD PTR $T20010
	push	WORD PTR es:_evtfd+2
	push	WORD PTR es:_evtfd
	call	FAR PTR _fclose
	add	sp,4
; Line 71
	mov	es,WORD PTR $T20011
	push	WORD PTR es:_cfgfd+2
	push	WORD PTR es:_cfgfd
	call	FAR PTR _fclose
	add	sp,4
; Line 72
	mov	es,WORD PTR $T20012
	push	WORD PTR es:_userfd+2
	push	WORD PTR es:_userfd
	call	FAR PTR _fclose
	add	sp,4
; Line 74
	cmp	WORD PTR _max_msgs,0
	je	$I857
; Line 76
	mov	WORD PTR [bp-2],0	;count
	cmp	WORD PTR _cur_msgs,0
	jle	$FB860
	mov	WORD PTR [bp-4],0
$F858:
; Line 77
	les	bx,DWORD PTR _msgs
	mov	si,WORD PTR [bp-4]
	push	WORD PTR es:[bx+2][si]
	push	WORD PTR es:[bx][si]
	call	FAR PTR _free
	add	sp,4
	add	WORD PTR [bp-4],4
	mov	ax,WORD PTR _cur_msgs
	inc	WORD PTR [bp-2]	;count
	cmp	WORD PTR [bp-2],ax	;count
	jl	$F858
$FB860:
; Line 78
	push	WORD PTR _msgs+2
	push	WORD PTR _msgs
	call	FAR PTR _free
	add	sp,4
; Line 81
$I857:
	cmp	WORD PTR _max_files,0
	je	$I861
; Line 83
	mov	WORD PTR [bp-2],0	;count
	cmp	WORD PTR _cur_files,0
	jle	$FB864
	mov	WORD PTR [bp-6],0
$F862:
; Line 84
	les	bx,DWORD PTR _files
	mov	si,WORD PTR [bp-6]
	push	WORD PTR es:[bx+2][si]
	push	WORD PTR es:[bx][si]
	call	FAR PTR _free
	add	sp,4
	add	WORD PTR [bp-6],4
	mov	ax,WORD PTR _cur_files
	inc	WORD PTR [bp-2]	;count
	cmp	WORD PTR [bp-2],ax	;count
	jl	$F862
$FB864:
; Line 85
	push	WORD PTR _files+2
	push	WORD PTR _files
	call	FAR PTR _free
	add	sp,4
; Line 88
$I861:
	cmp	WORD PTR _max_evts,0
	je	$I865
; Line 90
	mov	WORD PTR [bp-2],0	;count
	cmp	WORD PTR _cur_evts,0
	jle	$FB868
	mov	WORD PTR [bp-8],0
$F866:
; Line 91
	les	bx,DWORD PTR _evts
	mov	si,WORD PTR [bp-8]
	push	WORD PTR es:[bx+2][si]
	push	WORD PTR es:[bx][si]
	call	FAR PTR _free
	add	sp,4
	add	WORD PTR [bp-8],4
	mov	ax,WORD PTR _cur_evts
	inc	WORD PTR [bp-2]	;count
	cmp	WORD PTR [bp-2],ax	;count
	jl	$F866
$FB868:
; Line 92
	push	WORD PTR _evts+2
	push	WORD PTR _evts
	call	FAR PTR _free
	add	sp,4
; Line 95
$I865:
	cmp	WORD PTR _max_usrs,0
	je	$I869
; Line 97
	mov	WORD PTR [bp-2],0	;count
	cmp	WORD PTR _cur_usrs,0
	jle	$FB872
	mov	WORD PTR [bp-10],0
$F870:
; Line 98
	les	bx,DWORD PTR _usrs
	mov	si,WORD PTR [bp-10]
	push	WORD PTR es:[bx+2][si]
	push	WORD PTR es:[bx][si]
	call	FAR PTR _free
	add	sp,4
	add	WORD PTR [bp-10],4
	mov	ax,WORD PTR _cur_usrs
	inc	WORD PTR [bp-2]	;count
	cmp	WORD PTR [bp-2],ax	;count
	jl	$F870
$FB872:
; Line 99
	push	WORD PTR _usrs+2
	push	WORD PTR _usrs
	call	FAR PTR _free
	add	sp,4
; Line 102
$I869:
	mov	ax,7
	push	ax
	call	FAR PTR __cls
	add	sp,2
; Line 103
	mov	ax,64
	push	ax
	mov	ax,847
	push	ax
	sub	ax,ax
	push	ax
	call	FAR PTR _clrblk
	add	sp,6
; Line 107
	mov	ax,OFFSET DGROUP:$SG873
	push	ds
	push	ax
	sub	ax,ax
	push	ax
	mov	cx,2
	push	cx
	mov	cx,OFFSET $SG874
	mov	dx,SEG $SG874
	push	dx
	push	cx
	mov	cx,78
	push	cx
	mov	cx,80
	push	cx
	push	ax
	call	FAR PTR _prntnomovf
	add	sp,18
; Line 109
	mov	ax,OFFSET $SG875
	mov	cx,SEG $SG875
	push	cx
	push	ax
	mov	ax,75
	push	ax
	mov	ax,80
	push	ax
	mov	cx,256
	push	cx
	call	FAR PTR _prntnomov
	add	sp,10
; Line 110
	mov	ax,OFFSET $SG876
	mov	cx,SEG $SG876
	push	cx
	push	ax
	mov	ax,74
	push	ax
	mov	ax,80
	push	ax
	mov	ax,512
	push	ax
	call	FAR PTR _prntnomov
	add	sp,10
; Line 111
	mov	ax,1
	push	ax
	call	FAR PTR _set_cursor_type
	add	sp,2
; Line 112
	mov	ax,1024
	push	ax
	call	FAR PTR __setcurpos
	add	sp,2
; Line 113
	pop	si
	mov	sp,bp
	pop	bp
	ret	

_cleanup	ENDP
; Line 126
SCONFIG_TEXT      ENDS
CONST      SEGMENT
$T20046	DW SEG _snow_flag 
$T20047	DW SEG _cptr 
$T20048	DW SEG _cfgpath 
$T20050	DW SEG _key 
$T20051	DW SEG _cfg 
$T20052	DW SEG _tuser 
$T20053	DW SEG _count 
CONST      ENDS
SCONFIG_TEXT      SEGMENT
	ASSUME	CS: SCONFIG_TEXT
	PUBLIC	_main
_main	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,74
	push	di
	push	si
;	argc = 6
;	argv = 8
; Line 131
	sub	ax,ax
	push	ax
	mov	ax,2
	push	ax
	mov	cx,OFFSET $SG889
	mov	dx,SEG $SG889
	push	dx
	push	cx
	mov	cx,OFFSET __iob+24
	push	ds
	push	cx
	call	FAR PTR _fprintf
	add	sp,12
; Line 133
	mov	ax,OFFSET $SG890
	mov	cx,SEG $SG890
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 134
	mov	ax,OFFSET $SG891
	mov	cx,SEG $SG891
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 136
	call	FAR PTR _scr_init
; Line 138
	mov	ax,1
	cwd	
	push	dx
	push	ax
	mov	ax,2
	push	ax
	call	FAR PTR _signal
	add	sp,6
; Line 140
	call	FAR PTR _set_critical_error
; Line 143
	cmp	WORD PTR [bp+6],1	;argc
	jle	$I892
; Line 145
	les	bx,DWORD PTR [bp+8]	;argv
	les	bx,DWORD PTR es:[bx+4]
	mov	WORD PTR [bp-4],bx
	mov	WORD PTR [bp-2],es
	cmp	BYTE PTR es:[bx],47
	je	$I894
	cmp	BYTE PTR es:[bx],45
	jne	$I892
$I894:
; Line 148
	cmp	BYTE PTR es:[bx+1],83
	je	$I896
	cmp	BYTE PTR es:[bx+1],115
	je	$I896
; Line 150
	cmp	BYTE PTR es:[bx+1],77
	je	$I899
	cmp	BYTE PTR es:[bx+1],109
	jne	$I892
$I899:
; Line 155
	mov	WORD PTR _color,7
; Line 156
	mov	WORD PTR _color+2,15
; Line 161
	mov	ax,112
	mov	WORD PTR _color+4,ax
	mov	WORD PTR _color+6,ax
	mov	WORD PTR _color+8,ax
	mov	WORD PTR _color+10,ax
	mov	WORD PTR _color+12,ax
	jmp	SHORT $I892
	nop	
; Line 148
$I896:
; Line 149
	mov	es,WORD PTR $T20046
	mov	WORD PTR es:_snow_flag,1
; Line 166
$I892:
	mov	ax,OFFSET DGROUP:$SG901
	push	ds
	push	ax
	call	FAR PTR _getenv
	add	sp,4
	mov	bx,OFFSET _cptr
	mov	es,WORD PTR $T20047
	mov	WORD PTR [bp-8],bx
	mov	WORD PTR [bp-6],es
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	je	$I900
; Line 167
	mov	ax,OFFSET _cfgpath
	mov	cx,SEG _cfgpath
	mov	WORD PTR [bp-12],ax
	mov	WORD PTR [bp-10],cx
	les	bx,DWORD PTR [bp-8]
	mov	dx,WORD PTR es:[bx]
	mov	si,WORD PTR es:[bx+2]
	mov	bx,ax
	mov	di,cx
	push	ds
	push	di
	push	si
	mov	di,dx
	mov	si,ax
	pop	es
	pop	ds
	ASSUME DS: DGROUP
	mov	cx,-1
	xor	ax,ax
	repnz	scasb
	not	cx
	sub	di,cx
	xchg	di,si
	push	ds
	push	es
	pop	ds
	ASSUME DS: DGROUP
	pop	es
	shr	cx,1
	rep	movsw
	adc	cx,cx
	rep	movsb
	pop	ds
	ASSUME DS: DGROUP
; Line 168
	jmp	SHORT $I902
$I900:
; Line 169
	mov	ax,257
	push	ax
	mov	ax,OFFSET _cfgpath
	mov	cx,SEG _cfgpath
	mov	WORD PTR [bp-12],ax
	mov	WORD PTR [bp-10],cx
	push	cx
	push	ax
	call	FAR PTR _getcwd
	add	sp,6
$I902:
; Line 170
	les	bx,DWORD PTR [bp-12]
	cmp	BYTE PTR es:[bx],0
	je	$I903
	mov	es,WORD PTR $T20048
	push	es
	les	di,DWORD PTR [bp-12]
	mov	cx,-1
	xor	ax,ax
	repnz	scasb
	not	cx
	dec	cx
	pop	es
	mov	bx,cx
	cmp	BYTE PTR es:_cfgpath[bx+65535],92
	je	$I903
; Line 171
	mov	di,OFFSET DGROUP:$SG904
	push	ds
	pop	es
	push	ds
	lds	si,DWORD PTR [bp-12]
	ASSUME DS: NOTHING
	mov	cx,-1
	repnz	scasb
	not	cx
	sub	di,cx
	mov	bx,cx
	xchg	di,si
	push	ds
	push	es
	pop	ds
	ASSUME DS: NOTHING
	pop	es
	mov	cx,-1
	repnz	scasb
	dec	di
	mov	cx,bx
	shr	cx,1
	rep	movsw
	adc	cx,cx
	rep	movsb
	pop	ds
	ASSUME DS: NOTHING
; Line 173
$I903:
	mov	ax,OFFSET $SG905
	mov	cx,SEG $SG905
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 174
	push	WORD PTR [bp-10]
	push	WORD PTR [bp-12]
	mov	ax,OFFSET DGROUP:$SG906
	push	ds
	push	ax
	mov	ax,OFFSET _buffer
	mov	cx,SEG _buffer
	mov	WORD PTR [bp-16],ax
	mov	WORD PTR [bp-14],cx
	push	cx
	push	ax
	call	FAR PTR _sprintf
	add	sp,12
; Line 175
	push	WORD PTR _color+10
	push	WORD PTR _color+12
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _open_help
	add	sp,8
	or	ax,ax
	jne	$JCC1075
	jmp	$I907
$JCC1075:
; Line 177
	les	bx,DWORD PTR [bp+8]	;argv
	mov	ax,WORD PTR es:[bx]
	mov	dx,WORD PTR es:[bx+2]
	mov	di,ax
	mov	es,dx
	push	ds
	lds	si,DWORD PTR [bp-16]
	ASSUME DS: NOTHING
	mov	cx,-1
	xor	ax,ax
	repnz	scasb
	not	cx
	sub	di,cx
	xchg	di,si
	push	ds
	push	es
	pop	ds
	ASSUME DS: NOTHING
	pop	es
	shr	cx,1
	rep	movsw
	adc	cx,cx
	rep	movsb
	pop	ds
	ASSUME DS: NOTHING
; Line 178
	les	bx,DWORD PTR [bp-16]
	cmp	BYTE PTR es:[bx],0
	jne	$JCC1131
	jmp	$I908
$JCC1131:
; Line 181
	mov	di,bx
	mov	cx,-1
	repnz	scasb
	not	cx
	dec	cx
	add	cx,OFFSET _buffer
	mov	ax,SEG _buffer
	les	bx,DWORD PTR [bp-8]
	mov	WORD PTR es:[bx],cx
	mov	WORD PTR es:[bx+2],ax
	cmp	cx,WORD PTR [bp-16]
	jne	$L20049
	cmp	ax,WORD PTR [bp-14]
	je	$FB911
$L20049:
	les	bx,DWORD PTR [bp-8]
	les	bx,DWORD PTR es:[bx]
	cmp	BYTE PTR es:[bx],92
	je	$FB911
; Line 182
	mov	ax,WORD PTR [bp-16]
	mov	dx,WORD PTR [bp-14]
	les	bx,DWORD PTR [bp-8]
	dec	WORD PTR es:[bx]
	cmp	WORD PTR es:[bx],ax
	jne	$L20049
	cmp	WORD PTR es:[bx+2],dx
	jne	$L20049
$FB911:
; Line 184
	les	bx,DWORD PTR [bp-8]
	les	bx,DWORD PTR es:[bx]
	cmp	BYTE PTR es:[bx],92
	jne	$I912
; Line 186
	les	bx,DWORD PTR [bp-8]
	inc	WORD PTR es:[bx]
; Line 187
	mov	ax,WORD PTR es:[bx]
	mov	dx,WORD PTR es:[bx+2]
	mov	di,ax
	mov	si,OFFSET DGROUP:$SG913
	mov	es,dx
	mov	cx,6
	rep	movsw
; Line 189
	push	WORD PTR _color+10
	push	WORD PTR _color+12
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _open_help
	add	sp,8
	or	ax,ax
	je	$I907
; Line 190
	mov	ax,OFFSET $SG915
	mov	cx,SEG $SG915
	jmp	SHORT $L20054
	nop	
$I912:
; Line 193
	mov	ax,OFFSET $SG917
	mov	cx,SEG $SG917
	jmp	SHORT $L20054
$I908:
; Line 196
	mov	ax,OFFSET $SG919
	mov	cx,SEG $SG919
$L20054:
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 199
$I907:
	push	WORD PTR [bp-10]
	push	WORD PTR [bp-12]
	mov	ax,OFFSET DGROUP:$SG920
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _sprintf
	add	sp,12
; Line 200
	mov	ax,OFFSET DGROUP:$SG922
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	mov	bx,OFFSET _cfgfd
	mov	es,WORD PTR $T20011
	mov	WORD PTR [bp-20],bx
	mov	WORD PTR [bp-18],es
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	je	$JCC1378
	jmp	$I921
$JCC1378:
; Line 202
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG923
	mov	cx,SEG $SG923
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 203
$D924:
; Line 205
	call	FAR PTR _getch
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 206
	push	ax
	call	FAR PTR _toupper
	add	sp,2
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 208
	mov	WORD PTR [bp-22],ax
	cmp	ax,89
	je	$D927
	cmp	ax,78
	jne	$D924
$D927:
; Line 209
	cmp	ax,89
	jne	$L20034
	mov	ax,OFFSET DGROUP:$SG928
	jmp	SHORT $L20035
	nop	
$L20034:
	mov	ax,OFFSET DGROUP:$SG929
$L20035:
	push	ds
	push	ax
	mov	ax,OFFSET DGROUP:$SG930
	push	ds
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 210
	mov	es,WORD PTR $T20050
	cmp	WORD PTR es:_key,78
	jne	$I931
; Line 212
	mov	ax,OFFSET $SG932
	mov	cx,SEG $SG932
$L20057:
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 213
$L20055:
	mov	ax,1
	pop	si
	pop	di
	mov	sp,bp
	pop	bp
	ret	
	nop	
; Line 215
$I931:
	mov	ax,OFFSET DGROUP:$SG934
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	les	bx,DWORD PTR [bp-20]
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	jne	$I921
; Line 217
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG935
	mov	cx,SEG $SG935
$L20056:
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
	jmp	SHORT $L20055
	nop	
; Line 222
$I921:
	push	WORD PTR [bp-10]
	push	WORD PTR [bp-12]
	mov	ax,OFFSET DGROUP:$SG936
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _sprintf
	add	sp,12
; Line 223
	mov	ax,OFFSET DGROUP:$SG938
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	mov	bx,OFFSET _evtfd
	mov	es,WORD PTR $T20010
	mov	WORD PTR [bp-26],bx
	mov	WORD PTR [bp-24],es
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	je	$JCC1665
	jmp	$I937
$JCC1665:
; Line 225
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG939
	mov	cx,SEG $SG939
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 226
$D940:
; Line 228
	call	FAR PTR _getch
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 229
	push	ax
	call	FAR PTR _toupper
	add	sp,2
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 231
	mov	WORD PTR [bp-22],ax
	cmp	ax,89
	je	$D943
	cmp	ax,78
	jne	$D940
$D943:
; Line 232
	cmp	ax,89
	jne	$L20036
	mov	ax,OFFSET DGROUP:$SG944
	jmp	SHORT $L20037
$L20036:
	mov	ax,OFFSET DGROUP:$SG945
$L20037:
	push	ds
	push	ax
	mov	ax,OFFSET DGROUP:$SG946
	push	ds
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 233
	mov	es,WORD PTR $T20050
	cmp	WORD PTR es:_key,78
	jne	$I947
; Line 235
	mov	ax,OFFSET $SG948
	mov	cx,SEG $SG948
	jmp	$L20057
; Line 238
$I947:
	mov	ax,OFFSET DGROUP:$SG950
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	les	bx,DWORD PTR [bp-26]
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	jne	$I937
; Line 240
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG951
	mov	cx,SEG $SG951
	jmp	$L20056
	nop	
; Line 245
$I937:
	push	WORD PTR [bp-10]
	push	WORD PTR [bp-12]
	mov	ax,OFFSET DGROUP:$SG952
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _sprintf
	add	sp,12
; Line 246
	mov	ax,OFFSET DGROUP:$SG954
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	mov	bx,OFFSET _userfd
	mov	es,WORD PTR $T20012
	mov	WORD PTR [bp-30],bx
	mov	WORD PTR [bp-28],es
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	je	$JCC1915
	jmp	$I953
$JCC1915:
; Line 248
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG955
	mov	cx,SEG $SG955
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 249
$D956:
; Line 251
	call	FAR PTR _getch
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 252
	push	ax
	call	FAR PTR _toupper
	add	sp,2
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 254
	mov	WORD PTR [bp-22],ax
	cmp	ax,89
	je	$D959
	cmp	ax,78
	jne	$D956
$D959:
; Line 255
	cmp	ax,89
	jne	$L20038
	mov	ax,OFFSET DGROUP:$SG960
	jmp	SHORT $L20039
$L20038:
	mov	ax,OFFSET DGROUP:$SG961
$L20039:
	push	ds
	push	ax
	mov	ax,OFFSET DGROUP:$SG962
	push	ds
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 256
	mov	es,WORD PTR $T20050
	cmp	WORD PTR es:_key,78
	jne	$I963
; Line 258
	mov	ax,OFFSET $SG964
	mov	cx,SEG $SG964
	jmp	$L20057
; Line 261
$I963:
	mov	ax,OFFSET DGROUP:$SG966
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	les	bx,DWORD PTR [bp-30]
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	jne	$I953
; Line 263
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG967
	mov	cx,SEG $SG967
	jmp	$L20056
	nop	
; Line 268
$I953:
	push	WORD PTR [bp-10]
	push	WORD PTR [bp-12]
	mov	ax,OFFSET DGROUP:$SG968
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _sprintf
	add	sp,12
; Line 269
	mov	ax,OFFSET DGROUP:$SG970
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	mov	bx,OFFSET _msgfd
	mov	es,WORD PTR $T20009
	mov	WORD PTR [bp-34],bx
	mov	WORD PTR [bp-32],es
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	je	$JCC2165
	jmp	$I969
$JCC2165:
; Line 271
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG971
	mov	cx,SEG $SG971
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 272
$D972:
; Line 274
	call	FAR PTR _getch
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 275
	push	ax
	call	FAR PTR _toupper
	add	sp,2
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 277
	mov	WORD PTR [bp-22],ax
	cmp	ax,89
	je	$D975
	cmp	ax,78
	jne	$D972
$D975:
; Line 278
	cmp	ax,89
	jne	$L20040
	mov	ax,OFFSET DGROUP:$SG976
	jmp	SHORT $L20041
$L20040:
	mov	ax,OFFSET DGROUP:$SG977
$L20041:
	push	ds
	push	ax
	mov	ax,OFFSET DGROUP:$SG978
	push	ds
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 279
	mov	es,WORD PTR $T20050
	cmp	WORD PTR es:_key,78
	jne	$I979
; Line 281
	mov	ax,OFFSET $SG980
	mov	cx,SEG $SG980
	jmp	$L20057
; Line 284
$I979:
	mov	ax,OFFSET DGROUP:$SG982
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	les	bx,DWORD PTR [bp-34]
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	jne	$I969
; Line 286
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG983
	mov	cx,SEG $SG983
	jmp	$L20056
	nop	
; Line 291
$I969:
	push	WORD PTR [bp-10]
	push	WORD PTR [bp-12]
	mov	ax,OFFSET DGROUP:$SG984
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _sprintf
	add	sp,12
; Line 292
	mov	ax,OFFSET DGROUP:$SG986
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	mov	bx,OFFSET _filefd
	mov	es,WORD PTR $T20008
	mov	WORD PTR [bp-38],bx
	mov	WORD PTR [bp-36],es
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	je	$JCC2415
	jmp	$I985
$JCC2415:
; Line 294
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG987
	mov	cx,SEG $SG987
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 295
$D988:
; Line 297
	call	FAR PTR _getch
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 298
	push	ax
	call	FAR PTR _toupper
	add	sp,2
	mov	es,WORD PTR $T20050
	mov	WORD PTR es:_key,ax
; Line 300
	mov	WORD PTR [bp-22],ax
	cmp	ax,89
	je	$D991
	cmp	ax,78
	jne	$D988
$D991:
; Line 301
	cmp	ax,89
	jne	$L20042
	mov	ax,OFFSET DGROUP:$SG992
	jmp	SHORT $L20043
$L20042:
	mov	ax,OFFSET DGROUP:$SG993
$L20043:
	push	ds
	push	ax
	mov	ax,OFFSET DGROUP:$SG994
	push	ds
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,12
; Line 302
	mov	es,WORD PTR $T20050
	cmp	WORD PTR es:_key,78
	jne	$I995
; Line 304
	mov	ax,OFFSET $SG996
	mov	cx,SEG $SG996
	jmp	$L20057
; Line 307
$I995:
	mov	ax,OFFSET DGROUP:$SG998
	push	ds
	push	ax
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	call	FAR PTR _fopen
	add	sp,8
	les	bx,DWORD PTR [bp-38]
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	ax,dx
	or	ax,WORD PTR es:[bx]
	jne	$I985
; Line 309
	push	WORD PTR [bp-14]
	push	WORD PTR [bp-16]
	mov	ax,OFFSET $SG999
	mov	cx,SEG $SG999
	jmp	$L20056
	nop	
; Line 314
$I985:
	mov	ax,OFFSET _cleanup
	mov	dx,SEG _cleanup
	push	dx
	push	ax
	call	FAR PTR _atexit
	add	sp,4
; Line 316
	mov	ax,OFFSET $SG1000
	mov	cx,SEG $SG1000
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 317
	sub	ax,ax
	mov	dx,OFFSET _cfg
	mov	cx,SEG _cfg
	mov	bx,cx
	mov	cx,2043
	mov	di,dx
	mov	es,bx
	rep	stosb
; Line 318
	les	si,DWORD PTR [bp-20]
	push	WORD PTR es:[si+2]
	push	WORD PTR es:[si]
	mov	ax,2043
	push	ax
	mov	ax,1
	push	ax
	push	bx
	push	dx
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	je	$JCC2682
	jmp	$L20019
$JCC2682:
; Line 320
	mov	es,WORD PTR $T20051
	mov	WORD PTR es:_cfg,2400
; Line 321
	mov	ax,SEG $SG1002
	push	es
	push	ds
	mov	di,OFFSET _cfg+4
	mov	si,OFFSET $SG1002
	mov	ds,ax
	ASSUME DS: SEG $SG1002
	mov	cx,19
	rep	movsw
	movsb
	pop	ds
	ASSUME DS: NOTHING
	pop	es
; Line 322
	push	es
	mov	di,OFFSET _cfg+65
	mov	si,OFFSET DGROUP:$SG1003
	movsw
	movsb
	pop	es
; Line 323
	push	es
	mov	di,OFFSET _cfg+128
	mov	si,OFFSET DGROUP:$SG1004
	movsw
	movsw
	movsw
	movsw
	pop	es
; Line 324
	push	es
	mov	di,OFFSET _cfg+170
	mov	si,OFFSET DGROUP:$SG1005
	mov	cx,6
	rep	movsw
	movsb
	pop	es
; Line 325
	push	es
	mov	di,OFFSET _cfg+212
	mov	si,OFFSET DGROUP:$SG1006
	mov	cx,6
	rep	movsw
	movsb
	pop	es
; Line 326
	push	es
	mov	di,OFFSET _cfg+254
	mov	si,OFFSET DGROUP:$SG1007
	mov	cx,6
	rep	movsw
	movsb
	pop	es
; Line 327
	push	es
	mov	di,OFFSET _cfg+296
	mov	si,OFFSET DGROUP:$SG1008
	mov	cx,6
	rep	movsw
	movsb
	pop	es
; Line 328
	push	es
	mov	di,OFFSET _cfg+317
	mov	si,OFFSET DGROUP:$SG1009
	mov	cx,6
	rep	movsw
	movsb
	pop	es
; Line 329
	push	es
	mov	di,OFFSET _cfg+338
	mov	si,OFFSET DGROUP:$SG1010
	mov	cx,7
	rep	movsw
	pop	es
; Line 330
	push	es
	mov	di,OFFSET _cfg+359
	mov	si,OFFSET DGROUP:$SG1011
	mov	cx,7
	rep	movsw
	pop	es
; Line 331
	push	es
	mov	di,OFFSET _cfg+422
	mov	si,OFFSET DGROUP:$SG1012
	movsw
	movsw
	movsb
	pop	es
; Line 332
	push	es
	mov	di,OFFSET _cfg+443
	mov	si,OFFSET DGROUP:$SG1013
	movsw
	movsw
	movsb
	pop	es
; Line 333
	push	es
	mov	di,OFFSET _cfg+86
	mov	si,OFFSET DGROUP:$SG1014
	movsw
	movsw
	movsb
	pop	es
; Line 334
	mov	ax,WORD PTR $SG1015
	mov	WORD PTR es:_cfg+107,ax
; Line 335
	push	es
	mov	di,OFFSET _cfg+464
	mov	si,OFFSET DGROUP:$SG1016
	mov	cx,8
	rep	movsw
	movsb
	pop	es
; Line 336
	push	es
	mov	di,OFFSET _cfg+485
	mov	si,OFFSET DGROUP:$SG1017
	movsw
	movsw
	movsw
	movsw
	pop	es
; Line 345
	push	es
	mov	di,OFFSET _cfg+1587
	mov	si,OFFSET DGROUP:$SG1018
	mov	cx,6
	rep	movsw
	pop	es
; Line 346
	push	es
	mov	di,OFFSET _cfg+1282
	mov	si,OFFSET DGROUP:$SG1019
	mov	cx,8
	rep	movsw
	pop	es
; Line 347
	push	es
	mov	di,OFFSET _cfg+1648
	mov	si,OFFSET DGROUP:$SG1020
	mov	cx,7
	rep	movsw
	movsb
	pop	es
; Line 348
	mov	ax,SEG $SG1021
	push	es
	push	ds
	mov	di,OFFSET _cfg+1343
	mov	si,OFFSET $SG1021
	mov	ds,ax
	ASSUME DS: SEG $SG1021
	mov	cx,10
	rep	movsw
	pop	ds
	ASSUME DS: NOTHING
	pop	es
; Line 349
	push	es
	mov	di,OFFSET _cfg+1770
	mov	si,OFFSET DGROUP:$SG1022
	mov	cx,7
	rep	movsw
	pop	es
; Line 350
	push	es
	mov	di,OFFSET _cfg+1465
	mov	si,OFFSET DGROUP:$SG1023
	mov	cx,7
	rep	movsw
	pop	es
; Line 353
	mov	WORD PTR es:_cfg+1902,300
; Line 354
	mov	WORD PTR es:_cfg+1904,1200
; Line 356
	mov	WORD PTR es:_cfg+1897,20
; Line 359
	mov	BYTE PTR es:_cfg+1909,3
; Line 360
	mov	BYTE PTR es:_cfg+1911,2
; Line 361
	mov	BYTE PTR es:_cfg+1912,10
; Line 363
	mov	ax,-16544
	mov	WORD PTR es:_cfg+1915,ax
	mov	WORD PTR es:_cfg+1919,ax
; Line 364
	mov	BYTE PTR es:_cfg+1910,5
; Line 367
	mov	al,1
	mov	BYTE PTR es:_cfg+1892,al
	mov	BYTE PTR es:_cfg+1900,al
	mov	BYTE PTR es:_cfg+1901,al
	mov	BYTE PTR es:_cfg+1907,al
; Line 368
	sub	al,al
	mov	BYTE PTR es:_cfg+1906,al
	mov	BYTE PTR es:_cfg+1908,al
$L20019:
; Line 371
	les	bx,DWORD PTR [bp-26]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,28
	push	ax
	mov	ax,1
	push	ax
	mov	ax,OFFSET _tevt
	mov	cx,SEG _tevt
	mov	WORD PTR [bp-42],ax
	mov	WORD PTR [bp-40],cx
	push	cx
	push	ax
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	jne	$JCC3115
	jmp	$L20020
$JCC3115:
$FC1025:
; Line 373
	mov	ax,WORD PTR _cur_evts
	cmp	WORD PTR _max_evts,ax
	jg	$I1027
; Line 375
	add	WORD PTR _max_evts,10
	mov	ax,WORD PTR _max_evts
	shl	ax,1
	shl	ax,1
	push	ax
	push	WORD PTR _evts+2
	push	WORD PTR _evts
	call	FAR PTR _realloc
	add	sp,6
	mov	WORD PTR _evts,ax
	mov	WORD PTR _evts+2,dx
	mov	ax,dx
	or	ax,WORD PTR _evts
	jne	$I1027
; Line 377
	mov	ax,OFFSET $SG1029
	mov	cx,SEG $SG1029
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 378
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 381
$I1027:
	mov	ax,28
	push	ax
	call	FAR PTR _malloc
	add	sp,2
	mov	bx,WORD PTR _cur_evts
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _evts
	mov	WORD PTR es:[bx][si],ax
	mov	WORD PTR es:[bx+2][si],dx
	mov	bx,WORD PTR _cur_evts
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _evts
	mov	ax,WORD PTR es:[bx+2][si]
	or	ax,WORD PTR es:[bx][si]
	jne	$I1030
; Line 383
	mov	ax,OFFSET $SG1031
	mov	cx,SEG $SG1031
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 384
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 386
$I1030:
	mov	bx,WORD PTR _cur_evts
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _evts
	mov	ax,WORD PTR es:[bx][si]
	mov	dx,WORD PTR es:[bx+2][si]
	mov	cx,WORD PTR [bp-42]
	mov	bx,WORD PTR [bp-40]
	push	cx
	push	ds
	mov	di,ax
	mov	si,cx
	mov	es,dx
	mov	ds,bx
	ASSUME DS: NOTHING
	mov	cx,14
	rep	movsw
	pop	ds
	ASSUME DS: NOTHING
	pop	cx
; Line 387
	inc	WORD PTR _cur_evts
; Line 388
	les	si,DWORD PTR [bp-26]
	push	WORD PTR es:[si+2]
	push	WORD PTR es:[si]
	mov	ax,28
	push	ax
	mov	ax,1
	push	ax
	push	bx
	push	cx
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	je	$JCC3367
	jmp	$FC1025
$JCC3367:
$L20020:
; Line 390
	les	bx,DWORD PTR [bp-30]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,256
	push	ax
	mov	ax,1
	push	ax
	mov	ax,OFFSET _tuser
	mov	cx,SEG _tuser
	mov	WORD PTR [bp-46],ax
	mov	WORD PTR [bp-44],cx
	push	cx
	push	ax
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	jne	$JCC3414
	jmp	$L20021
$JCC3414:
	mov	ax,OFFSET _tuser+103
	mov	cx,SEG _tuser
	mov	WORD PTR [bp-50],ax
	mov	WORD PTR [bp-48],cx
$FC1033:
; Line 392
	mov	ax,WORD PTR _cur_usrs
	cmp	WORD PTR _max_usrs,ax
	jg	$I1035
; Line 394
	add	WORD PTR _max_usrs,100
	mov	ax,WORD PTR _max_usrs
	shl	ax,1
	shl	ax,1
	push	ax
	push	WORD PTR _usrs+2
	push	WORD PTR _usrs
	call	FAR PTR _realloc
	add	sp,6
	mov	WORD PTR _usrs,ax
	mov	WORD PTR _usrs+2,dx
	mov	ax,dx
	or	ax,WORD PTR _usrs
	jne	$I1035
; Line 396
	mov	ax,OFFSET $SG1037
	mov	cx,SEG $SG1037
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 397
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 400
$I1035:
	mov	ax,12
	push	ax
	call	FAR PTR _malloc
	add	sp,2
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	mov	WORD PTR es:[bx][si],ax
	mov	WORD PTR es:[bx+2][si],dx
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	mov	ax,WORD PTR es:[bx+2][si]
	or	ax,WORD PTR es:[bx][si]
	jne	$I1038
; Line 402
	mov	ax,OFFSET $SG1039
	mov	cx,SEG $SG1039
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 403
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 406
$I1038:
	les	di,DWORD PTR [bp-46]
	mov	cx,-1
	xor	ax,ax
	repnz	scasb
	not	cx
	push	cx
	call	FAR PTR _malloc
	add	sp,2
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	WORD PTR es:[bx],ax
	mov	WORD PTR es:[bx+2],dx
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	ax,WORD PTR es:[bx+2]
	or	ax,WORD PTR es:[bx]
	jne	$I1040
; Line 408
	mov	ax,OFFSET $SG1041
	mov	cx,SEG $SG1041
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 409
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 411
$I1040:
	les	di,DWORD PTR [bp-50]
	mov	cx,-1
	xor	ax,ax
	repnz	scasb
	not	cx
	push	cx
	call	FAR PTR _malloc
	add	sp,2
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	WORD PTR es:[bx+4],ax
	mov	WORD PTR es:[bx+6],dx
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	ax,WORD PTR es:[bx+6]
	or	ax,WORD PTR es:[bx+4]
	jne	$I1042
; Line 413
	mov	ax,OFFSET $SG1043
	mov	cx,SEG $SG1043
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 414
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 417
$I1042:
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	ax,WORD PTR es:[bx]
	mov	dx,WORD PTR es:[bx+2]
	push	ds
	mov	si,ax
	mov	ds,dx
	ASSUME DS: NOTHING
	les	di,DWORD PTR [bp-46]
	mov	cx,-1
	xor	ax,ax
	repnz	scasb
	not	cx
	sub	di,cx
	xchg	di,si
	push	ds
	push	es
	pop	ds
	ASSUME DS: DGROUP
	pop	es
	shr	cx,1
	rep	movsw
	adc	cx,cx
	rep	movsb
	pop	ds
	ASSUME DS: DGROUP
; Line 418
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	ax,WORD PTR es:[bx+4]
	mov	dx,WORD PTR es:[bx+6]
	push	ds
	mov	si,ax
	mov	ds,dx
	ASSUME DS: NOTHING
	les	di,DWORD PTR [bp-50]
	mov	cx,-1
	xor	ax,ax
	repnz	scasb
	not	cx
	sub	di,cx
	xchg	di,si
	push	ds
	push	es
	pop	ds
	ASSUME DS: DGROUP
	pop	es
	shr	cx,1
	rep	movsw
	adc	cx,cx
	rep	movsb
	pop	ds
	ASSUME DS: DGROUP
; Line 419
	mov	es,WORD PTR $T20052
	mov	ah,BYTE PTR es:_tuser+224
	and	ax,-32768
	cmp	ax,1
	sbb	al,al
	inc	al
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	BYTE PTR es:[bx+8],al
; Line 420
	mov	es,WORD PTR $T20052
	mov	al,BYTE PTR es:_tuser+212
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	BYTE PTR es:[bx+9],al
; Line 421
	mov	es,WORD PTR $T20052
	mov	ax,WORD PTR es:_tuser+213
	mov	bx,WORD PTR _cur_usrs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _usrs
	les	bx,DWORD PTR es:[bx][si]
	mov	WORD PTR es:[bx+10],ax
; Line 423
	inc	WORD PTR _cur_usrs
; Line 424
	les	bx,DWORD PTR [bp-30]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,256
	push	ax
	mov	ax,1
	push	ax
	push	WORD PTR [bp-44]
	push	WORD PTR [bp-46]
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	je	$JCC4047
	jmp	$FC1033
$JCC4047:
$L20021:
; Line 426
	les	bx,DWORD PTR [bp-34]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,119
	push	ax
	mov	ax,1
	push	ax
	mov	ax,OFFSET _tmsg
	mov	cx,SEG _tmsg
	mov	WORD PTR [bp-54],ax
	mov	WORD PTR [bp-52],cx
	push	cx
	push	ax
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	jne	$JCC4094
	jmp	$FB1046
$JCC4094:
$FC1045:
; Line 428
	mov	ax,WORD PTR _cur_msgs
	cmp	WORD PTR _max_msgs,ax
	jg	$I1047
; Line 430
	add	WORD PTR _max_msgs,10
	mov	ax,WORD PTR _max_msgs
	shl	ax,1
	shl	ax,1
	push	ax
	push	WORD PTR _msgs+2
	push	WORD PTR _msgs
	call	FAR PTR _realloc
	add	sp,6
	mov	WORD PTR _msgs,ax
	mov	WORD PTR _msgs+2,dx
	mov	ax,dx
	or	ax,WORD PTR _msgs
	jne	$I1047
; Line 432
	mov	ax,OFFSET $SG1049
	mov	cx,SEG $SG1049
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 433
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 436
$I1047:
	mov	ax,119
	push	ax
	call	FAR PTR _malloc
	add	sp,2
	mov	bx,WORD PTR _cur_msgs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _msgs
	mov	WORD PTR es:[bx][si],ax
	mov	WORD PTR es:[bx+2][si],dx
	mov	bx,WORD PTR _cur_msgs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _msgs
	mov	ax,WORD PTR es:[bx+2][si]
	or	ax,WORD PTR es:[bx][si]
	jne	$I1050
; Line 438
	mov	ax,OFFSET $SG1051
	mov	cx,SEG $SG1051
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 439
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 441
$I1050:
	mov	bx,WORD PTR _cur_msgs
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _msgs
	mov	ax,WORD PTR es:[bx][si]
	mov	dx,WORD PTR es:[bx+2][si]
	mov	cx,WORD PTR [bp-54]
	mov	bx,WORD PTR [bp-52]
	push	cx
	push	ds
	mov	di,ax
	mov	si,cx
	mov	es,dx
	mov	ds,bx
	ASSUME DS: NOTHING
	mov	cx,59
	rep	movsw
	movsb
	pop	ds
	ASSUME DS: DGROUP
	pop	cx
; Line 442
	inc	WORD PTR _cur_msgs
; Line 443
	les	si,DWORD PTR [bp-34]
	push	WORD PTR es:[si+2]
	push	WORD PTR es:[si]
	mov	ax,119
	push	ax
	mov	ax,1
	push	ax
	push	bx
	push	cx
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	je	$JCC4347
	jmp	$FC1045
$JCC4347:
$FB1046:
; Line 444
	cmp	WORD PTR _cur_msgs,0
	je	$L20022
; Line 445
	mov	ax,OFFSET _msg_comp
	mov	dx,SEG _msg_comp
	push	dx
	push	ax
	mov	ax,4
	push	ax
	push	WORD PTR _cur_msgs
	push	WORD PTR _msgs+2
	push	WORD PTR _msgs
	call	FAR PTR _qsort
	add	sp,12
$L20022:
; Line 447
	les	bx,DWORD PTR [bp-38]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,169
	push	ax
	mov	ax,1
	push	ax
	mov	ax,OFFSET _tfile
	mov	cx,SEG _tfile
	mov	WORD PTR [bp-58],ax
	mov	WORD PTR [bp-56],cx
	push	cx
	push	ax
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	jne	$JCC4433
	jmp	$FB1055
$JCC4433:
$FC1054:
; Line 449
	mov	ax,WORD PTR _cur_files
	cmp	WORD PTR _max_files,ax
	jg	$I1056
; Line 451
	add	WORD PTR _max_files,10
	mov	ax,WORD PTR _max_files
	shl	ax,1
	shl	ax,1
	push	ax
	push	WORD PTR _files+2
	push	WORD PTR _files
	call	FAR PTR _realloc
	add	sp,6
	mov	WORD PTR _files,ax
	mov	WORD PTR _files+2,dx
	mov	ax,dx
	or	ax,WORD PTR _files
	jne	$I1056
; Line 453
	mov	ax,OFFSET $SG1058
	mov	cx,SEG $SG1058
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 454
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 457
$I1056:
	mov	ax,169
	push	ax
	call	FAR PTR _malloc
	add	sp,2
	mov	bx,WORD PTR _cur_files
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _files
	mov	WORD PTR es:[bx][si],ax
	mov	WORD PTR es:[bx+2][si],dx
	mov	bx,WORD PTR _cur_files
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _files
	mov	ax,WORD PTR es:[bx+2][si]
	or	ax,WORD PTR es:[bx][si]
	jne	$I1059
; Line 459
	mov	ax,OFFSET $SG1060
	mov	cx,SEG $SG1060
	push	cx
	push	ax
	mov	ax,OFFSET __iob+24
	push	ds
	push	ax
	call	FAR PTR _fprintf
	add	sp,8
; Line 460
	mov	ax,1
	push	ax
	call	FAR PTR _exit
	add	sp,2
; Line 462
$I1059:
	mov	bx,WORD PTR _cur_files
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _files
	mov	ax,WORD PTR es:[bx][si]
	mov	dx,WORD PTR es:[bx+2][si]
	mov	cx,WORD PTR [bp-58]
	mov	bx,WORD PTR [bp-56]
	push	cx
	push	ds
	mov	di,ax
	mov	si,cx
	mov	es,dx
	mov	ds,bx
	ASSUME DS: NOTHING
	mov	cx,84
	rep	movsw
	movsb
	pop	ds
	ASSUME DS: DGROUP
	pop	cx
; Line 463
	inc	WORD PTR _cur_files
; Line 464
	les	si,DWORD PTR [bp-38]
	push	WORD PTR es:[si+2]
	push	WORD PTR es:[si]
	mov	ax,169
	push	ax
	mov	ax,1
	push	ax
	push	bx
	push	cx
	call	FAR PTR _fread
	add	sp,12
	or	ax,ax
	je	$JCC4686
	jmp	$FC1054
$JCC4686:
$FB1055:
; Line 465
	cmp	WORD PTR _cur_files,0
	je	$I1061
; Line 466
	mov	ax,OFFSET _file_comp
	mov	dx,SEG _file_comp
	push	dx
	push	ax
	mov	ax,4
	push	ax
	push	WORD PTR _cur_files
	push	WORD PTR _files+2
	push	WORD PTR _files
	call	FAR PTR _qsort
	add	sp,12
; Line 468
$I1061:
	call	FAR PTR _main_menu
; Line 470
	cmp	WORD PTR _master_change,0
	jne	$JCC4740
	jmp	$I1062
$JCC4740:
; Line 472
	sub	ax,ax
	push	ax
	push	ax
	push	ax
	push	WORD PTR _color+6
	push	WORD PTR _color+8
	mov	cx,OFFSET $SG1064
	mov	dx,SEG $SG1064
	push	dx
	push	cx
	mov	cx,OFFSET $SG1065
	mov	dx,SEG $SG1065
	push	dx
	push	cx
	call	FAR PTR _askyn
	add	sp,18
	or	ax,ax
	jne	$JCC4784
	jmp	$I1062
$JCC4784:
; Line 474
	sub	ax,ax
	push	ax
	push	ax
	les	bx,DWORD PTR [bp-20]
	les	si,DWORD PTR es:[bx]
	mov	al,BYTE PTR es:[si+11]
	sub	ah,ah
	push	ax
	call	FAR PTR _chsize
	add	sp,6
; Line 475
	sub	ax,ax
	push	ax
	sub	cx,cx
	push	cx
	push	cx
	les	bx,DWORD PTR [bp-20]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fseek
	add	sp,10
; Line 476
	les	bx,DWORD PTR [bp-20]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,2043
	push	ax
	mov	ax,1
	push	ax
	mov	ax,OFFSET _cfg
	mov	cx,SEG _cfg
	push	cx
	push	ax
	call	FAR PTR _fwrite
	add	sp,12
; Line 477
	les	bx,DWORD PTR [bp-20]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fflush
	add	sp,4
; Line 479
	sub	ax,ax
	push	ax
	sub	cx,cx
	push	cx
	push	cx
	les	bx,DWORD PTR [bp-34]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fseek
	add	sp,10
; Line 480
	sub	ax,ax
	push	ax
	push	ax
	les	bx,DWORD PTR [bp-34]
	les	bx,DWORD PTR es:[bx]
	mov	al,BYTE PTR es:[bx+11]
	sub	ah,ah
	push	ax
	call	FAR PTR _chsize
	add	sp,6
; Line 481
	mov	bx,OFFSET _count
	mov	es,WORD PTR $T20053
	mov	WORD PTR [bp-62],bx
	mov	WORD PTR [bp-60],es
	mov	WORD PTR es:[bx],0
	cmp	WORD PTR _cur_msgs,0
	jle	$FB1068
$F1066:
; Line 483
	les	bx,DWORD PTR [bp-62]
	mov	bx,WORD PTR es:[bx]
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _msgs
	les	bx,DWORD PTR es:[bx][si]
	mov	WORD PTR [bp-66],bx
	mov	WORD PTR [bp-64],es
	cmp	BYTE PTR es:[bx+118],0
	jne	$FC1067
; Line 484
	les	bx,DWORD PTR [bp-34]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,119
	push	ax
	mov	ax,1
	push	ax
	push	WORD PTR [bp-64]
	push	WORD PTR [bp-66]
	call	FAR PTR _fwrite
	add	sp,12
; Line 481
$FC1067:
	mov	ax,WORD PTR _cur_msgs
	les	bx,DWORD PTR [bp-62]
	inc	WORD PTR es:[bx]
	cmp	WORD PTR es:[bx],ax
	jl	$F1066
; Line 485
$FB1068:
; Line 486
	les	bx,DWORD PTR [bp-34]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fflush
	add	sp,4
; Line 488
	sub	ax,ax
	push	ax
	sub	cx,cx
	push	cx
	push	cx
	les	bx,DWORD PTR [bp-38]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fseek
	add	sp,10
; Line 489
	sub	ax,ax
	push	ax
	push	ax
	les	bx,DWORD PTR [bp-38]
	les	bx,DWORD PTR es:[bx]
	mov	al,BYTE PTR es:[bx+11]
	sub	ah,ah
	push	ax
	call	FAR PTR _chsize
	add	sp,6
; Line 490
	les	bx,DWORD PTR [bp-62]
	mov	WORD PTR es:[bx],0
	cmp	WORD PTR _cur_files,0
	jle	$FB1072
$F1070:
; Line 492
	les	bx,DWORD PTR [bp-62]
	mov	bx,WORD PTR es:[bx]
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _files
	les	bx,DWORD PTR es:[bx][si]
	mov	WORD PTR [bp-70],bx
	mov	WORD PTR [bp-68],es
	cmp	BYTE PTR es:[bx+168],0
	jne	$FC1071
; Line 493
	les	bx,DWORD PTR [bp-38]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,169
	push	ax
	mov	ax,1
	push	ax
	push	WORD PTR [bp-68]
	push	WORD PTR [bp-70]
	call	FAR PTR _fwrite
	add	sp,12
; Line 490
$FC1071:
	mov	ax,WORD PTR _cur_files
	les	bx,DWORD PTR [bp-62]
	inc	WORD PTR es:[bx]
	cmp	WORD PTR es:[bx],ax
	jl	$F1070
; Line 494
$FB1072:
; Line 495
	les	bx,DWORD PTR [bp-38]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fflush
	add	sp,4
; Line 497
	sub	ax,ax
	push	ax
	sub	cx,cx
	push	cx
	push	cx
	les	bx,DWORD PTR [bp-26]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fseek
	add	sp,10
; Line 498
	sub	ax,ax
	push	ax
	push	ax
	les	bx,DWORD PTR [bp-26]
	les	bx,DWORD PTR es:[bx]
	mov	al,BYTE PTR es:[bx+11]
	sub	ah,ah
	push	ax
	call	FAR PTR _chsize
	add	sp,6
; Line 499
	les	bx,DWORD PTR [bp-62]
	mov	WORD PTR es:[bx],0
	cmp	WORD PTR _cur_evts,0
	jle	$FB1076
$F1074:
; Line 501
	les	bx,DWORD PTR [bp-62]
	mov	bx,WORD PTR es:[bx]
	shl	bx,1
	shl	bx,1
	les	si,DWORD PTR _evts
	les	bx,DWORD PTR es:[bx][si]
	mov	WORD PTR [bp-74],bx
	mov	WORD PTR [bp-72],es
	cmp	BYTE PTR es:[bx+27],0
	jne	$FC1075
; Line 502
	les	bx,DWORD PTR [bp-26]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	mov	ax,28
	push	ax
	mov	ax,1
	push	ax
	push	WORD PTR [bp-72]
	push	WORD PTR [bp-74]
	call	FAR PTR _fwrite
	add	sp,12
; Line 499
$FC1075:
	mov	ax,WORD PTR _cur_evts
	les	bx,DWORD PTR [bp-62]
	inc	WORD PTR es:[bx]
	cmp	WORD PTR es:[bx],ax
	jl	$F1074
; Line 503
$FB1076:
; Line 504
	les	bx,DWORD PTR [bp-26]
	push	WORD PTR es:[bx+2]
	push	WORD PTR es:[bx]
	call	FAR PTR _fflush
	add	sp,4
; Line 508
$I1062:
	sub	ax,ax
; Line 509
	pop	si
	pop	di
	mov	sp,bp
	pop	bp
	ret	
	nop	

_main	ENDP
SCONFIG_TEXT	ENDS
END
