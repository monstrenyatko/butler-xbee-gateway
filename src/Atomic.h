/*
 *******************************************************************************
 *
 * Purpose: Utils. Atomic types helper.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_ATOMIC_H_
#define UTILS_ATOMIC_H_

#ifndef __has_feature
#define __has_feature(__x) 0
#endif

#if __cplusplus < 201103L
#include <boost/atomic.hpp>
#define UTILS_USE_BOOST_ATOMIC
#else
#include <atomic>
#endif

namespace Utils {

#ifdef UTILS_USE_BOOST_ATOMIC
	typedef boost::atomic_bool atomic_bool;
	typedef boost::atomic_uint32_t atomic_uint32_t;
#else
	typedef std::atomic_bool atomic_bool;
	typedef std::atomic<uint32_t> atomic_uint32_t;
#endif

} /* namespace Utils */

#endif /* UTILS_ATOMIC_H_ */
