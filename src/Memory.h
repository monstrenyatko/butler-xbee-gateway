/*
 *******************************************************************************
 *
 * Purpose: Utils. Memory tools.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_MEMORY_H_
#define UTILS_MEMORY_H_

/* Internal Includes */
/* External Includes */
/* System Includes */
#include <memory>

namespace Utils {

template<typename DerivedT, typename BaseT>
std::unique_ptr<DerivedT>
dynamic_unique_ptr_cast(std::unique_ptr<BaseT>& v)
{
	std::unique_ptr<DerivedT> res;
	DerivedT* p = dynamic_cast<DerivedT*>(v.get());
	if (p) {
		res = std::unique_ptr<DerivedT>(p);
		v.release();
	}
	return res;
}

} /* namespace Utils */

#endif /* UTILS_MEMORY_H_ */
