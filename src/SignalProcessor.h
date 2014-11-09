/*
 *******************************************************************************
 *
 * Purpose: Processor of the system signals.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef SIGNALPROCESSOR_H_
#define SIGNALPROCESSOR_H_

/* Internal Includes */
/* External Includes */
/* System Includes */


class SignalListener;

class SignalProcessor {
public:
	/**
	 * Constructor
	 */
	SignalProcessor();

	/**
	 * Destructor
	 */
	~SignalProcessor() throw();

	/**
	 * Starts the processor.
	 */
	void start(void);

	/**
	 * Stops the processor.
	 */
	void stop(void);
private:
	// Objects
	SignalListener*									mSignalListener;

	// Do not copy
	SignalProcessor(const SignalProcessor&);
	SignalProcessor &operator=(const SignalProcessor&);

	// Internal
};

#endif /* SIGNALPROCESSOR_H_ */
