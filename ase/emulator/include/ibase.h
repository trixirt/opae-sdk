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

#ifndef IBASE_H
#define IBASE_H

#ifdef __cplusplus

#include <cstdlib>
#include <cstdio>
#include <cstdbool>
#include <cstdint>

namespace intel {

namespace fpga {

namespace ase {

/// @interface IBase
/// @brief Public Interface base class for active objects.
class IBase
{
public:
// IBase Destructor.
	virtual ~IBase() {}
/// @brief Query interface for a given interface id.
///
/// @param[in] Interface The interface ID.
/// @returns An interface.
	virtual void* Interface (uint32_t Interface) const = 0;
/// @brief Determine whether this object contains the specified interface.
///
/// @param[in] Interface The interface ID.
/// @retval True if the interface exists.
/// @retval False if the interface does not exist.
	virtual bool Has (uint32_t Interface) const = 0;
/// @brief IBase inequality.
///
/// @param[in] rother A reference to another Interface to compare.
/// @retval True if the Interfaces are not equal.
/// @retval False if the Interfaces are equal.
	virtual bool operator != (IBase const &rother) const = 0;
/// @brief IBase equality.
///
/// @param[in] rother A reference to another Interface to compare.
/// @retval True if the Interfaces are equal.
/// @retval False if the Interfaces are not equal.
	virtual bool operator == (IBase const &rother) const = 0;
/// @brief Internal state check.
///
/// @retval True if internal state of the Interface is OK.
/// @retval False if internal state of the Interface is not OK.
	virtual bool IsOK() const = 0;
};

/// Cast an IBase const & to a T & by interface id.
template<class T>
T & dynamic_ref(uint32_t Name, IBase const &obj) {
	return *static_cast<T *>(obj.Interface(Name));
}

/// Cast an IBase const * to a T & by interface id.
template<class T>
T & dynamic_ref(uint32_t Name, IBase const *obj) {
	return *static_cast<T *>(obj->Interface(Name));
}

/// Cast an IBase const & to a T * by interface id.
/// @retval  NULL  No such interface.
template<class T>
T * dynamic_ptr(uint32_t Name, IBase const &obj) {
	return static_cast<T *>(obj.Interface(Name));
}

/// Cast an IBase const * to a T * by interface id.
/// @retval  NULL  No such interface.
template<class T>
T * dynamic_ptr(uint32_t Name, IBase const *obj) {
	return static_cast<T *>(obj->Interface(Name));
}

} // end of namespace ase

} // end of namespace fpga

} // end of namespace intel

#endif //__cplusplus

#endif
