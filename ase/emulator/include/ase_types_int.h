// Copyright(c) 2018, Intel Corporation
//
// Redistribution  and  use  in source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of  source code  must retain the  above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name  of Intel Corporation  nor the names of its contributors
//   may be used to  endorse or promote  products derived  from this  software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
// IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
// LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
// CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
// SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
// INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
// CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// **************************************************************************

#ifndef ASE_TYPES_INT_H
#define ASE_TYPES_INT_H

#include "opae/fpga.h"

/*
 * ASE Debug log-level
 */
enum ase_loglevel {
	ASE_LOG_ERROR     = -3,
	ASE_LOG_INFO_2    = -2,
	ASE_LOG_INFO      = -1,
	ASE_LOG_SILENT    = 0,   // Only INFO, ERR messages are posted
	ASE_LOG_MESSAGE,         // All MSG, INFO, ERR messages are posted
	ASE_LOG_DEBUG
};

enum fpga_simulator_ID {
	verilator = 0,
	modelsim,
	icarus,
	vcs,
};

enum afu_model_ID {
	cpp = 0,
	verilog,
	opencl,
};

enum fpga_emulator_result {
	success = 0,
	error,
	triggered,
	timeout,
	canceled
};

enum fpga_command_result {
	afu_success = 0,
	afu_interrupt,
	afu_error,
	afu_event
};

typedef enum {
	AFU_RESET,
	ASE_SIMKILL,
	ASE_INIT,
	UMSG_MODE
} ase_portctrl_cmd;

typedef uint32_t fpga_command_ID;

void ase_print(int loglevel, char *fmt, ...);
int generate_sockname(char *);

#ifdef SIM_SIDE
#define LOG_PREFIX "  [SIM]  "
#else
#define LOG_PREFIX "  [APP]  "
#endif

// Shorten filename
#ifdef __SHORTEN_FILE__
#undef __SHORTEN_FILE__
#endif // __SHORT_FILE__
#define __SHORTEN_FILE__			\
	({	const char *file = __FILE__;	\
		const char *p    = file;	\
		while (*p)			\
			++p;			\
		while ((p > file) &&		\
		       ('/' != *p) &&		\
		       ('\\' != *p))		\
			--p;			\
		if (p > file)			\
			++p;			\
		p;				\
	})					\
						\

#ifdef ASE_DEBUG
#undef ASE_ERR
#endif

#ifdef ASE_INFO
#undef ASE_INFO
#endif

#ifdef ASE_DBG
#undef ASE_DBG
#endif

#define ASE_ERR(format, ...)   \
 	ase_print(ASE_LOG_ERROR, LOG_PREFIX "%s:%u:%s()\t" format, __SHORTEN_FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__) \

#define ASE_INFO(format, ...)						\
 	ase_print(ASE_LOG_INFO, LOG_PREFIX format, ## __VA_ARGS__)	\

#define ASE_DBG(format, ...)						\
	ase_print(ASE_LOG_DEBUG, LOG_PREFIX "%s:%u:%s()\t" format, __SHORTEN_FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__) \
									\
// SHM memory name length
#define ASE_FILENAME_LEN        40

// ASE filepath length
#define ASE_FILEPATH_LEN        256

// ASE logger len
#define ASE_LOGGER_LEN          1024

// Timestamp session code length
#define ASE_SESSION_CODE_LEN    20

// work Directory location
extern char *ase_workdir_path;

// Timestamp IPC file
#define TSTAMP_FILENAME ".ase_timestamp"
extern char tstamp_filepath[ASE_FILEPATH_LEN];

// READY file name
#define ASE_READY_FILENAME ".ase_ready.pid"
#define APP_LOCK_FILENAME  ".app_lock.pid"

// ASE Mode macros
#define ASE_MODE_DAEMON_NO_SIMKILL   1
#define ASE_MODE_DAEMON_SIMKILL      2
#define ASE_MODE_DAEMON_SW_SIMKILL   3
#define ASE_MODE_REGRESSION          4

// UMAS establishment status
#define NOT_ESTABLISHED 0xC0C0
#define ESTABLISHED     0xBEEF

/*
 * Console colors
 */

// ERROR codes are in RED color
#define BEGIN_RED_FONTCOLOR    printf("\033[1;31m");
#define END_RED_FONTCOLOR      printf("\033[0m");

// INFO or INSTRUCTIONS are in GREEN color
#define BEGIN_GREEN_FONTCOLOR  printf("\033[32;1m");
#define END_GREEN_FONTCOLOR    printf("\033[0m");

// WARNING codes in YELLOW color
#define BEGIN_YELLOW_FONTCOLOR printf("\033[0;33m");
#define END_YELLOW_FONTCOLOR   printf("\033[0m");

// Wipeout current line in printf
#define WIPEOUT_LINE           printf("]\n\033[F\033[J");

/*
 * ASE Error codes
 */
#define ASE_USR_CAPCM_NOINIT           0x1	// CAPCM not initialized
#define ASE_OS_MQUEUE_ERR              0x2	// MQ open error
#define ASE_OS_SHM_ERR                 0x3	// SHM open error
#define ASE_OS_FOPEN_ERR               0x4	// Normal fopen failure
#define ASE_OS_MEMMAP_ERR              0x5	// Memory map/unmap errors
#define ASE_OS_MQTXRX_ERR              0x6	// MQ send receive error
#define ASE_OS_MALLOC_ERR              0x7	// Malloc error
#define ASE_OS_STRING_ERR              0x8	// String operations error
#define ASE_IPCKILL_CATERR             0xA	// Catastropic error when cleaning

// IPCs, manual intervention required
#define ASE_UNDEF_ERROR                0xFF	// Undefined error, pls report

// Simkill message
#define ASE_SIMKILL_MSG                0xDEADDEAD

// Test complete separator
#define TEST_SEPARATOR                 "#####################################################"

#ifdef __cplusplus

#include <cstdlib>
#include <cstdio>
#include <cstdbool>

#include "ibase.h"

namespace intel {

namespace fpga {

namespace ase {

typedef struct fpga_command_t {
	void *m_Context;
	void (*m_Handler) (void *);
	IBase *m_IBase;
	bool m_Filter;
	unsigned long long m_intID;
} fpga_command_t;

typedef void* app_context;

typedef void* ase_afu_dut;

} // end of namespace ase

} // end of namespace fpga

} // end of namespace intel

#endif //__cplusplus

#endif
