/*
 * bunissfunc.h
 *
 *  Created on: May 16, 2018
 *      Author: root
 */

#ifndef BUNISSFUNC_H_
#define BUNISSFUNC_H_

#include <evhtp/evhtp.h>

void http_registercb(evhtp_request_t * req, void * args);
void http_logincb(evhtp_request_t * req, void * args);
void http_testcb(evhtp_request_t * req, void * args);
void http_addcb(evhtp_request_t * req, void * args);
void http_subcb(evhtp_request_t * req, void * args);
void http_factcb(evhtp_request_t * req, void * args);
void http_vh_testcb(evhtp_request_t * req, void * args);

int registerAllfunction(evhtp_request_t * req, void * args);


#endif /* BUNISSFUNC_H_ */
