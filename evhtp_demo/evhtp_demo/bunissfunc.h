/*
 * bunissfunc.h
 *
 *  Created on: May 16, 2018
 *      Author: root
 */

#ifndef BUNISSFUNC_H_
#define BUNISSFUNC_H_

#include <evhtp/evhtp.h>

void registercb(evhtp_request_t * req, void * args);
void logincb(evhtp_request_t * req, void * args);
void testcb(evhtp_request_t * req, void * args);
void addcb(evhtp_request_t * req, void * args);
void subcb(evhtp_request_t * req, void * args);
void factcb(evhtp_request_t * req, void * args);
void vh_testcb(evhtp_request_t * req, void * args);

int registerAllfunction(evhtp_request_t * req, void * args);


#endif /* BUNISSFUNC_H_ */
