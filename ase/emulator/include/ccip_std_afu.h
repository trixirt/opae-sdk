// Copyright(c) 2014-2017, Intel Corporation
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

#ifndef _ccip_std_afu_H_
#define _ccip_std_afu_H_

#include <cstdlib>
#include <cstdbool>
#include <cstdint>

#include "ccip_common.h"

class ccip_std_afu {
public:

	uint8_t             pClk;                     // 400MHz - CCI-P clock domain. Primary interface clock
	uint8_t             pClkDiv2;                 // 200MHz - CCI-P clock domain.
	uint8_t             pClkDiv4;                 // 100MHz - CCI-P clock domain.
	uint8_t             uClk_usr;                 // User clock domain.
	uint8_t             uClk_usrDiv2;             // User clock domain.
	uint8_t             pck_cp2af_softReset;      // CCI-P ACTIVE HIGH Soft Reset
	uint8_t             pck_cp2af_pwrState;       // CCI-P AFU Power State
	uint8_t             pck_cp2af_error;          // CCI-P Protocol Error Detected

	// Interface structures
	ccip_pkt_t          pck_cp2af_sRx;           // CCI-P Rx Port
	ccip_pkt_t          pck_af2cp_sTx;           // CCI-P Tx Port

public:
	// CONSTRUCTORS

	/// Construct the model; called by application code
	ccip_std_afu(const char* name="TOP");

	/// Destroy the model; called (often implicitly) by application code
	~ccip_std_afu();

	// API METHODS

	/// Evaluate the model.  Application must call when inputs change.
	void eval();

	/// Simulation complete, run final blocks.  Application must call on completion.
	void final();

};

#endif  /*guard*/
