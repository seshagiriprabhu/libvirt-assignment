/* Written by Giri
 * A program to start a VM */
#include "header.h"
/* STATUS: INCOMPLETE */

static virConnectPtr conn = NULL;
/* A function to start a domain */
void start_domain (int id) {
	virDomainPtr dom = NULL; /* the domain being checked */
	int ret, state;

	/* Find the domain of the given id */
	dom = virDomainLookupByID(conn, id);
	if (dom == NULL) {
		fprintf (stderr, "Failed to find Domain %d\n", id);
		goto error;
	}

	/* Check state */
	state = checkDomainState(dom);
	if ((state == VIR_DOMAIN_SHUTDOWN) ||
			(state == VIR_DOMAIN_SHUTOFF)) {
			ret = virDomainCreate(dom);
			printf ("Powering on domain....\n");
			if (ret < 0) {
				fprintf(stderr, "Failed to power on dmain %d\n", id);
				goto error;
			}
			state = checkDomainState(dom);
			if (state != VIR_DOMAIN_SHUTDOWN) {
				fprintf(stderr, "Domain %d state is not on\n", id);
				goto error;
			} else {
				printf ("Domain shutdown\n");
			}
	} else {
		fprintf(stderr, "Domain %d is not in a state where it should be turned on\n", id);
		goto error;
	}
error:
	if (dom != NULL)
		virDomainFree(dom);
}

int main() {
	unsigned int id = 0;
	
	/* NULL means connect to local Xen hypervisor */
	conn = virConnectOpen(NULL);
	if (conn == NULL) {
		fprintf (stderr, "Failed to connect to hypervisor\n");
		goto error;
	} else {
		inactive_domains(conn);
		active_domains(conn);
	}
	
	printf ("\nEnter the ID of the domain: ");
	scanf ("%du", &id);

	if (id == 0) {
		int i, j, ids[10];
		i = virConnectListDomains(conn, &ids[0], 10);
		if (i < 0) {
			fprintf (stderr, "Failed to list the domain\n");
			goto error;
		}
		for (j = 0; j < i; j++) {
			if (ids[j] != 0) {
				id = ids[j];
				break;
			}
		}
	}
	if (id == 0) {
		fprintf (stderr, "Failed find a running guest domain\n");
		goto error;
	}
	start_domain(id);
error:
	if (conn != NULL)
		virConnectClose(conn);
	return 0;
}
