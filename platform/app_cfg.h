#ifndef __PLATFORM_APP_CFG_H__
#define __PLATFORM_APP_CFG_H__
#define PRINT_STR_CFG_USE_FUNCTION_POINTER
#ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
#define PRINT_STR_OUTPUT_BYTE(__TARGET,__BYTE) ((*this.fnPrintByte)(__TARGET, __BYTE))
#else
#define PRINT_STR_OUTPUT_BYTE(__BYTE) (serial_out(__BYTE))
#endif
#endif
