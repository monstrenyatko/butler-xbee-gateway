/*
 *******************************************************************************
 *
 * Purpose: Asynchronous commands implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Commands.h"
#include "Application.h"
/* External Includes */
/* System Includes */


void CommandApplicationStop::execute() {
	Application::get().stop(mReason);
}
