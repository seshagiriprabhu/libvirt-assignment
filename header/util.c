/* Written by Giri
 * A program which holds all the common functions
 * to prevent DRY */

#include "header.h"
/* A function to check the domain state of a given domain */
int checkDomainState(virDomainPtr dom) {
	virDomainInfo info;        /* the information being fetched */
	int ret;

	ret = virDomainGetInfo(dom, &info);
	if (ret < 0) 
		return(-1);
	
	return(info.state);
}

/* A function to list the inactive_domains */
void inactive_domains (virConnectPtr conn) {
	int i;
	int numDomains;
	int *inactiveDomains;

	numDomains = virConnectNumOfDefinedDomains(conn);

	inactiveDomains = malloc(sizeof(char *) * numDomains);
	numDomains = virConnectListDomains(conn, inactiveDomains, numDomains);

	printf("Inactive domain names: ");
	for (i = 0 ; i < numDomains ; i++) {
		printf("%d ", inactiveDomains[i]);
	}
	free(inactiveDomains);
}

/* A function to list the active domains */
void active_domains (virConnectPtr conn) {
	int i;
	int numDomains;
	int *activeDomains;

	numDomains = virConnectNumOfDomains(conn);

	activeDomains = malloc(sizeof(int) * numDomains);
	numDomains = virConnectListDomains(conn, activeDomains, numDomains);

	printf("Active domain IDs: ");
	for (i = 0 ; i < numDomains ; i++) {
		    printf("%d ", activeDomains[i]);
	}
	free(activeDomains);
}
