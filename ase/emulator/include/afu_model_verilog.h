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

#ifndef AFU_MODEL_VERILATOR_H
#define AFU_MODEL_VERILATOR_H

#ifdef __cplusplus

#include <iostream>
#include <map>
#include <memory>
#include <cstdbool>

#include "ase_types_int.h"
#include "iafu_model.h"

namespace intel {
namespace fpga {
namespace ase {

template<class afu_core> class afu_model_verilog : public iafu_model <afu_core> {

protected:
	unsigned long long        m_tickcount;

public:
	using iafu_model<afu_core>::m_afu_model_id;

	using iafu_model<afu_core>::m_afu_core;

	afu_model_verilog() {
		m_tickcount = 0;
		m_afu_core = std::make_shared<afu_core>();
		m_afu_model_id = verilog;
	}

	void reset(void) {
		// m_core->i_reset = 1;
		// Make sure any inheritance gets applied
		this->tick();
		// m_core->i_reset = 0;
	}

	void tick(void) {
		// Increment our own internal time reference
		// m_tickcount++;

		// Make sure any combinatorial logic depending upon
		// inputs that may have changed before we called tick()
		// has settled before the rising edge of the clock.
		// m_core->i_clk = 0;
		// m_core->eval();

		// Toggle the clock

		// Rising edge
		// m_core->i_clk = 1;
		// m_core->eval();

		// Falling edge
		// m_core->i_clk = 0;
		// m_core->eval();
	}

	bool done(void) {
		// return (Verilated::gotFinish());
		return true;
	}
};

} // end of namespace ase
} // end of namespace fpga
} // end of namespace intel

#endif //__cplusplus


#endif
