// Copyright(c) 2014-2018, Intel Corporation
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

#ifndef AFU_MODEL_H
#define AFU_MODEL_H

#include <iostream>
#include <map>
#include <memory>
#include <cstdint>
#include <chrono>

#include "ase_types_int.h"

namespace intel {
namespace fpga {
namespace ase {

template<class afu_core> class iafu_model_factory;

template<class afu_core> class iafu_model {
protected:

	afu_model_ID		  m_afu_model_id;

	std::shared_ptr<afu_core> m_afu_core;

public:
	static void registerType(afu_model_ID n, iafu_model_factory<afu_core> *factory) {
		get_factory_instance()[n] = factory;
	}

	static std::unique_ptr<iafu_model<afu_core>> create_unique(afu_model_ID n) {
		std::unique_ptr<iafu_model<afu_core>> afu_model_instance =
			std::move(get_factory_instance()[n]->create_unique());
		return afu_model_instance;
	}

	iafu_model() {}

	virtual ~iafu_model() {}

	virtual void reset() = 0;

	virtual void tick() = 0;

	virtual bool done() = 0;

protected:

	static std::map<afu_model_ID, iafu_model_factory <afu_core> *> &get_factory_instance() {
		static std::map<afu_model_ID, iafu_model_factory <afu_core> *> map_instance;
		return map_instance;
	}
};

} // end of namespace ase
} // end of namespace fpga
} // end of namespace intel

#endif
