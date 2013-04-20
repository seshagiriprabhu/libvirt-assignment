/****************Header files*****************/
#include <stdlib.h>
#include <stdio.h>
#include <libvirt/libvirt.h>

/*************Function declaration************/
int checkDomainState(virDomainPtr dom);
void inactive_domains(virConnectPtr conn);
void active_domains(virConnectPtr conn);
