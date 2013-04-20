/* Written by Giri
 * A program which holds all the common functions
 * to prevent DRY */

#include "header.h"

int checkDomainState(virDomainPtr dom) {
	virDomainInfo info;        /* the information being fetched */
	int ret;

	ret = virDomainGetInfo(dom, &info);
	if (ret < 0) 
		return(-1);
	
	return(info.state);
}

