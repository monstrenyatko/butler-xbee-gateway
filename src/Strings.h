/*
 *******************************************************************************
 *
 * Purpose: Utils. Strings helper.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_STRINGS_H_
#define UTILS_STRINGS_H_

/**
 * Generates string class_name::__FUNCTION__
 */
#define UTILS_STR_CLASS_FUNCTION(class_name) (std::string( #class_name "::" ) + __FUNCTION__)
#define UTILS_STR_FUNCTION (std::string(__FUNCTION__) + "()")

#endif /* UTILS_STRINGS_H_ */
