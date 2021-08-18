/**
	******************************************************************************
	* This file is part of the MicroPython project, http://micropython.org/
	* Copyright (C), 2021 -2023, 01studio Tech. Co., Ltd.http://bbs.01studio.org/
	* File Name 				 :	modtftlcd.c
	* Author						 :	Folktale
	* Version 					 :	v1.0
	* date							 :	2021/3/18
	* Description 			 :	
	******************************************************************************
**/

#include <stdint.h>
#include <stdio.h>

#include "py/runtime.h"
#include "py/mphal.h"
#include "lib/utils/pyexec.h"

#include "storage.h"
#include "sdcard.h"
#include "portmodules.h"
#include "modmachine.h"
#include "extmod/vfs.h"
#include "extmod/utime_mphal.h"

#if MICROPY_HW_LCD43M
#include "lcd43m.h"
#endif

#if MICROPY_HW_LCD43G
#include "lcd43g.h"
#endif

#if MICROPY_ENABLE_TFTLCD

STATIC const mp_rom_map_elem_t tftlcd_module_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_tftlcd) },
	
	#if MICROPY_HW_LCD43M
	{ MP_ROM_QSTR(MP_QSTR_LCD43M), MP_ROM_PTR(&tftlcd_lcd43m_type) },
	#endif
	#if MICROPY_HW_LCD43G
	{ MP_ROM_QSTR(MP_QSTR_LCD43R), MP_ROM_PTR(&tftlcd_lcd43g_type) },
	#endif
	#if (MICROPY_ENABLE_TFTLCD && MICROPY_HW_LCD7R && MICROPY_HW_LCD43G)
	{ MP_ROM_QSTR(MP_QSTR_LCD7R), MP_ROM_PTR(&tftlcd_lcd7r_type) },
	#endif
};
STATIC MP_DEFINE_CONST_DICT(tftlcd_module_globals, tftlcd_module_globals_table);

const mp_obj_module_t tftlcd_module = {
		.base = { &mp_type_module },
		.globals = (mp_obj_dict_t *)&tftlcd_module_globals,
};
#endif
