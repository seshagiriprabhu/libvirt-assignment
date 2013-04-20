/* Written by Giri
 * A program to find the information of a domain */
#include <stdio.h>
#include <libvirt/libvirt.h>

static void
getDomainInfo(int id) {
	virConnectPtr conn = NULL; /* the hypervisor connection */
	virDomainPtr dom = NULL;   /* the domain being checked */
	virDomainInfo info;        /* the information being fetched */
	int ret;

	/* NULL means connect to local Xen hypervisor */
	conn = virConnectOpenReadOnly(NULL);
	if (conn == NULL) {
		fprintf(stderr, "Failed to connect to hypervisor\n");
		goto error;
	}

	/* Find the domain of the given id */
	dom = virDomainLookupByID(conn, id);
	if (dom == NULL) {
		fprintf(stderr, "Failed to find Domain %d\n", id);
		goto error;
	}

	/* Get the information */
	ret = virDomainGetInfo(dom, &info);
	if (ret < 0) {
		fprintf(stderr, "Failed to get information for Domain %d\n", id);
		goto error;
	}

	printf("Domains %d: %d CPUs\n", id, info.nrVirtCpu);

error:
	if (dom != NULL)
		virDomainFree(dom);
	if (conn != NULL)
		virConnectClose(conn);
}

int main() {
	unsigned int id;
	printf ("Enter the id of the domain: ");
	scanf ("%du", &id);
	getDomainInfo(id);

	return(0);
}
