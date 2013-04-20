/* Written by Giri
 * A program to suspend a VM */

#include "header.h"

static virConnectPtr conn = NULL; /* the hypervisor connection */

void SuspendAndResumeDomain(int id) {
	virDomainPtr dom = NULL;   /* the domain being checked */
	int ret, state;

	/* Find the domain of the given id */
	dom = virDomainLookupByID(conn, id);
	if (dom == NULL) {
		fprintf(stderr, "Failed to find Domain %d\n", id);
		goto error;
	}

	/* Check state */
	state = checkDomainState(dom);
	if ((state == VIR_DOMAIN_RUNNING) ||
			(state == VIR_DOMAIN_NOSTATE) ||
			(state == VIR_DOMAIN_BLOCKED)) {
		printf("Suspending domain...\n");
		ret = virDomainSuspend(dom);
		if (ret < 0) {
			fprintf(stderr, "Failed to suspend Domain %d\n", id);
			goto error;
		}
		state = checkDomainState(dom);
		if (state != VIR_DOMAIN_PAUSED) {
			fprintf(stderr, "Domain %d state is not suspended\n", id);
		} else {
			printf("Domain suspended, resuming it...\n");
		}
		ret = virDomainResume(dom);
		if (ret < 0) {
			fprintf(stderr, "Failed to resume Domain %d\n", id);
			goto error;
		}
		state = checkDomainState(dom);
		if ((state == VIR_DOMAIN_RUNNING) ||
				(state == VIR_DOMAIN_NOSTATE) ||
				(state == VIR_DOMAIN_BLOCKED)) {
			printf("Domain resumed\n");
		} else {
			fprintf(stderr, "Domain %d state indicate it is not resumed\n", id);
		}
	} else {
		fprintf(stderr, "Domain %d is not in a state where it should be suspended\n", id);
		goto error;
	}

error:
	if (dom != NULL)
		virDomainFree(dom);
}

int main () {
	unsigned int id = 0;

	/* NULL means connect to local Xen hypervisor */
	conn = virConnectOpenReadOnly(NULL);
	if (conn == NULL) {
		fprintf(stderr, "Failed to connect to hypervisor\n");
		goto error;
	}

	printf ("Enter the domain ID: ");
	scanf ("%du", &id);
			
	if (id == 0) {
		int i, j, ids[10];
		i = virConnectListDomains(conn, &ids[0], 10);
		if (i < 0) {
			fprintf(stderr, "Failed to list the domains\n");
			goto error;
		}
		for (j = 0;j < i;j++) {
			if (ids[j] != 0) {
				id = ids[j];
				break;
			}
		}
	}
	if (id == 0) {
		fprintf(stderr, "Failed find a running guest domain\n");
		goto error;
	}

	SuspendAndResumeDomain(id);

error:
	if (conn != NULL)
		virConnectClose(conn);
	return(0);
}
