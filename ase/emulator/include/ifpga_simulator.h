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

#ifndef FPGA_SIMULATOR_H
#define FPGA_SIMULATOR_H

#include "ase_types_int.h"

#ifdef __cplusplus

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <memory> /* add unique_ptr */
#include <cassert>
#include <cerrno>
#include <stdexcept>
#include <deque>
#include <list>
#include <atomic>
#include <chrono>
#include <mutex>

#include "ifpga_interface.h"
#include "iafu_model_factory.h"

namespace intel {
namespace fpga {
namespace ase {

template<class afu_core> class fpga_simulator_factory;

template<class afu_core> class ifpga_simulator {

protected:
	fpga_simulator_ID		      m_fpga_simulator_id;

	std::shared_ptr<iafu_model<afu_core>> m_afu_model;

protected:
	using Lock = std::unique_lock<std::mutex>;
	typedef std::shared_ptr<ifpga_simulator> ptr_t;

	pid_t	m_pid;
	bool	_init;

	/// Operation dispatcher and monitoring threads
	std::thread             m_operation_thread;
	std::thread             m_monitor_thread;

        /// Lock to the operation dispatcher thread
	mutable Lock		m_operation_lock;
	mutable std::mutex	m_operation_mutex;

	/// semaphores notified when Operation is ready to be processed and when
	/// the operation is complete.
	std::atomic<bool>	m_operation_pending;
	std::atomic<bool>	m_operation_done;

protected:

	static std::map<fpga_simulator_ID, fpga_simulator_factory<afu_core> *> &get_factory_instance() {
		static std::map<fpga_simulator_ID, fpga_simulator_factory<afu_core> *> map_instance;
		return map_instance;
	}

public:

	static void registerType(fpga_simulator_ID n, fpga_simulator_factory<afu_core> *factory) {
		get_factory_instance()[n] = factory;
	}

	static std::unique_ptr<ifpga_simulator<afu_core>> create_unique(fpga_simulator_ID n) {
		std::unique_ptr<ifpga_simulator<afu_core>> fpga_simulator_instance =
			std::move(get_factory_instance()[n]->create_unique());
		return fpga_simulator_instance;
	}

	ifpga_simulator() {}

	virtual ~ifpga_simulator(void) {}

	virtual void init() = 0;

	virtual void run() = 0;

	virtual void reset() = 0;

	virtual void shutdown() = 0;

	virtual void load_afu_model() = 0;

	virtual void load_sim_libraries() = 0;

	virtual void register_observer() = 0;

	virtual void notify_observers() = 0;

};

} // end of namespace ase
} // end of namespace fpga
} // end of namespace intel

#endif //__cplusplus

#endif
