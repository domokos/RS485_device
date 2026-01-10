/*
 * MCP416x.h
 *
 * Common function of the MCP416x family programmable resistors
 *
 *  Created on: Jan 7, 2026
 *      Author: doma
 */

#ifndef MCP416X_MCP416X_H_
#define MCP416X_MCP416X_H_

#include "Base.h"

// helper to set clock line with wait times
void set_clock_hi(void);

// helper to clear clock line with wait times
void set_clock_lo(void);

// Precision wait for 60us
void sck_delay(void);

#endif /* MCP416X_MCP416X_H_ */
