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

#ifndef CCIP_EMULATOR_H
#define CCIP_EMULATOR_H

#ifdef __cplusplus

/* Abstract classes and data constants */
#include "ccip_common.h"
#include "ase_types_int.h"
#include "ifpga_emulator.h"

/* Concrete classes */
#include "afu_model_factory_verilog.h"
#include "fpga_simulator_factory_verilator.h"

#include "ccip_common.h"

/* Adapter to Verilator AFU class */
#include "ccip_std_afu.h"

namespace intel {
namespace fpga {
namespace ase {

class ccip_std_afu;

template <class afu_core> class ccip_emulator : public ifpga_emulator <afu_core> {
protected:
	/// Receive a transaction request from the OPAE application
	fpga_command_result opae_command_receive(fpga_command_ID tranID);

	/// Transmit transaction to the fpga_interface threads
	fpga_command_result fpga_command_dispatch(const fpga_command& app_transaction);

public:

	fpga_command_result mmio_map_receive(fpga_command_ID tranID);

	fpga_command_result mmio_unmap_receive(fpga_command_ID tranID);

	fpga_command_result mmio_read_receive(fpga_command_ID tranID);

	fpga_command_result mmio_write_receive(fpga_command_ID tranID);

	fpga_command_result buffer_prepare_receive(fpga_command_ID tranID);

	fpga_command_result buffer_release_receive(fpga_command_ID tranID);

};


/* Global object which interfaces with the OPAE application */
#ifdef SIM_SIDE
std::shared_ptr<ccip_emulator<ccip_std_afu>> ccip_emulator<ccip_std_afu>::_instance(0);
#endif

} // end of namespace ase
} // end of namespace fpga
} // end of namespace intel

#endif //__cplusplus

#endif // CCIP_EMULATOR_H
