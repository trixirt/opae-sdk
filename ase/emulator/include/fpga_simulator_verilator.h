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

#ifndef FPGA_SIMULATOR_VERILATOR_H
#define FPGA_SIMULATOR_VERILATOR_H

#include <iostream>
#include <map>
#include <memory>
#include <cstdbool>

#include "ase_types_int.h"
#include "ifpga_simulator.h"

namespace intel {
namespace fpga {
namespace ase {

/* Concrete FPGA simulator class for Verilator */
template<class afu_core> class fpga_simulator_verilator : public ifpga_simulator <afu_core> {
public:

	using ifpga_simulator<afu_core>::m_fpga_simulator_id;

	fpga_simulator_verilator() {
		m_fpga_simulator_id = verilator;
	}

	void init();

	void run();

	void reset();

	void shutdown();

	void load_afu_model();

	void load_sim_libraries();

	void register_observer();

	void notify_observers();

};

} // end of namespace ase
} // end of namespace fpga
} // end of namespace intel

#endif
