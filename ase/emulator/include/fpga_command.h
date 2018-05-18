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

#ifndef FPGA_COMMAND_H
#define FPGA_COMMAND_H

#include "ase_types_int.h"
#include "opae/fpga.h"

#ifdef __cplusplus

namespace intel {
namespace fpga {
namespace ase {

class fpga_command
{
public:
	typedef std::shared_ptr<fpga_command> ptr_t;

	fpga_command();

	fpga_command(const fpga_command_t &stTid);

	fpga_command(const fpga_command &rOther);

	fpga_command(app_context Context);

	fpga_command(fpga_command_ID intID);

	fpga_command(fpga_command_ID intID,
		     app_context Context);

	fpga_command(app_context Context,
		     fpga_event_handle handle,
		     bool Filter=true);

	fpga_command(fpga_command_ID intID,
		     fpga_event_handle handle,
		     bool Filter=true);

	fpga_command(uint32_t intID,
		     app_context Context,
		     fpga_event_handle handle,
		     bool Filter=true);

	fpga_command(fpga_event_handle handle,
		     bool Filter=true);

	fpga_command(app_context Context,
		     IBase *pIBase,
		     bool Filter=true);

	fpga_command(fpga_command_ID intID,
		     IBase *pIBase,
		     bool Filter=true);

	fpga_command(fpga_command_ID intID,
		     app_context Context,
		     IBase *pIBase,
		     bool Filter=true);

	fpga_command(IBase *pIBase,
		     bool Filter=true);

	// fpga_command Destructor.
	virtual ~fpga_command();

	// Accessors
	app_context Context() const;
	fpga_event_handle Handler() const;
	IBase *Ibase() const;
	bool Filter() const;
	uint32_t ID() const;

	// Mutators
	void Context(app_context Context);
	void Handler(fpga_event_handle Handler);
	void Ibase(IBase *pIBase);
	void Filter(bool Filter);
	void ID(fpga_command_ID intID);

	// Operators
	fpga_command& operator = (const fpga_command&);
	fpga_command& operator = (const fpga_command_t&);
	operator const fpga_command_t& () const;
	operator fpga_command_t& ();
	bool operator == (const fpga_command&) const;

	static fpga_command_ID NextUniqueID();

private:
	fpga_command_t m_tid;

	static uint32_t sm_NextUniqueID;
};

/// fpga_command streamer.
std::ostream & operator << (std::ostream &s, const fpga_command &TranID);

} // end of namespace ase
} // end of namespace fpga
} // end of namespace intel

#endif //__cplusplus

#endif // FPGA_COMMAND_H
