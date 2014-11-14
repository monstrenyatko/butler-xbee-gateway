/*
 *******************************************************************************
 *
 * Purpose: Routing between networks
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef ROUTER_H_
#define ROUTER_H_

/* Internal Includes */
/* External Includes */
/* System Includes */
#include <memory>

/* Forward declaration */
struct RouterContext;
namespace Networking {class DataUnit;}

/**
 * Routing events and data between networks
 */
class Router {
public:
	/**
	 * Constructor
	 */
	Router();

	/**
	 * Destructor
	 */
	~Router();

	/**
	 * Starts the processing
	 */
	void start();

	/**
	 * Stops the processing
	 */
	void stop();

	/**
	 * Routes the data unit to configured direction
	 *
	 * @param unit the data unit
	 */
	void process(std::unique_ptr<Networking::DataUnit> unit);
private:
	// Objects
	RouterContext*				mCtx;

	// Do not copy
	Router(const Router&);
	Router &operator=(const Router&);

	// Methods
	void onProcess(std::unique_ptr<Networking::DataUnit>);
};

#endif /* ROUTER_H_ */
