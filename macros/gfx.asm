; Graphics macros

MACRO? assert_valid_rgb
	rept? _NARG
		assert 0 <= (\1) && (\1) <= 31, "RGB channel must be 0-31"
		shift
	endr
ENDM

MACRO? RGB
	rept? _NARG / 3
		assert_valid_rgb \1, \2, \3
		_dw_adjusted_rgb \1, \2, \3
		shift 3
	endr
ENDM

MACRO? _start_filter
	; initialize inputs equal to outputs
	for x, 3
		DEF in{x} = out{x}
	endr
ENDM

MACRO? _end_filter
	; limit outputs to between 0 and 31
	for x, 3
		if out{x} < 0
			DEF out{x} = 0
		elif out{x} > 31
			DEF out{x} = 31
		endc
	endr
ENDM

MACRO? _adjust_channel ; args: out, in, ...list of 32 delta values
	; adds the 'in'th delta value to 'out'
	assert _NARG == 34, "_adjust_channel must have 32 delta values"
	DEF x = \2 + 3
	DEF \1 += \<x>
ENDM

MACRO? _dw_adjusted_rgb ; args: R, G, B
	; use $0, $1, $2 for G, R, B -- numbers are more convenient to {interpolate},
	; and green is sometimes a special case for which 0 is convenient to handle separately
	DEF out$1 = \1 ; red
	DEF out$0 = \2 ; green
	DEF out$2 = \3 ; blue
	; do not filter if the color is pure black (0, 0, 0) or white (31, 31, 31)
	if !((out$0 == 0 || out$0 == 31) && out$0 == out$1 && out$1 == out$2)
		; GBC color filter
		_start_filter
		_adjust_channel out$0, in$0, 0, -1, -2, -3, -3, -4, -4, -4, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -7, -7, -7, -8, -8, -9, -10, -10
		_adjust_channel out$0, in$1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2
		_adjust_channel out$0, in$2, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 10, 10
		for x, 1, 3
			_adjust_channel out{x}, in{x}, 0, -1, -2, -2, -3, -3, -3, -3, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0
		endr
		_end_filter
		; gamma filter
		_start_filter
		for x, 3
			_adjust_channel out{x}, in{x}, 0, 0, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 4, 3, 3, 2, 2, 2, 1, 1, 0, 0, 0
		endr
		_end_filter
		; desaturation filter
		_start_filter
		for x, 3
			_adjust_channel out{x}, in{x}, -0, -0, -0, -0, -0, -0, -0, -0, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3
		endr
		if in$1 > 11
			DEF out$0 += 1
		endc
		if in$2 > 11
			DEF out$0 += 1
		endc
		if in$0 > 11
			DEF out$1 += 1
			DEF out$2 += 1
		endc
		if in$2 > 26
			DEF out$1 += 2
		elif in$2 > 10
			DEF out$1 += 1
		endc
		if in$1 > 26
			DEF out$2 += 2
		elif in$1 > 10
			DEF out$2 += 1
		endc
		_end_filter
	endc
	dw palred out$1 + palgreen out$0 + palblue out$2
ENDM


DEF palred   EQUS "(1 << B_COLOR_RED) *"
DEF palgreen EQUS "(1 << B_COLOR_GREEN) *"
DEF palblue  EQUS "(1 << B_COLOR_BLUE) *"

DEF palettes EQUS "* PAL_SIZE"
DEF palette  EQUS "+ PAL_SIZE *"
DEF color    EQUS "+ COLOR_SIZE *"
DEF colors   EQUS "* COLOR_SIZE"

DEF tiles EQUS "* TILE_SIZE"
DEF tile  EQUS "+ TILE_SIZE *"

; extracts the middle two colors from a 2bpp binary palette
; example usage:
; INCBIN "foo.gbcpal", middle_colors
DEF middle_colors EQUS "COLOR_SIZE, COLOR_SIZE * 2"

MACRO? dbpixel
	if _NARG >= 4
	; x tile, y tile, x pixel, y pixel
		db \1 * TILE_WIDTH + \3, \2 * TILE_WIDTH + \4
	else
	; x tile, y tile
		db \1 * TILE_WIDTH, \2 * TILE_WIDTH
	endc
ENDM

MACRO? hlpixel
	ldpixel hl, \#
ENDM

MACRO? bcpixel
	ldpixel bc, \#
ENDM

MACRO? depixel
	ldpixel de, \#
ENDM

MACRO? ldpixel
	if _NARG >= 5
	; register, x tile, y tile, x pixel, y pixel
		lb \1, \2 * TILE_WIDTH + \4, \3 * TILE_WIDTH + \5
	else
	; register, x tile, y tile
		lb \1, \2 * TILE_WIDTH, \3 * TILE_WIDTH
	endc
ENDM

MACRO? dbsprite
; x tile, y tile, x pixel, y pixel, vtile offset, attributes
	db (\2 * TILE_WIDTH) % $100 + \4, (\1 * TILE_WIDTH) % $100 + \3, \5, \6
ENDM
