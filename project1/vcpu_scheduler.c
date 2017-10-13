#include<stdio.h>
#include<stdlib.h>
#include<libvirt/libvirt.h>
#include<libvirt/virterror.h>

int main(int arc, char *argv[]) {
	if (arc != 2) {
		printf("Incorrect number of arguments.\nExpecting: ./vcpu_scheduler <interval>\n");
		return 1;
	}

	int ret = 0;
	int interval_seconds = atoi(argv[1]);
	virConnectPtr conn;
	virDomainPtr *domains;
	int amt_running_domains = 0;
	virTypedParameterPtr old_params = NULL;
	virTypedParameterPtr new_params = NULL;
	int amt_params = 0;

	/*
	 * Connecting to the hypervisor
	 */
	printf("Attempting to connect to hypervisor.\n");
	conn = virConnectOpen("qemu:///system"); // won't work without three '/'

	if (conn != NULL) {
		printf("Successfully connected to hypervisor.\n");
	} else {
		printf("Error occured: %s\n", virGetLastErrorMessage());
		return 1;
	}

	/*
	 * Getting domain stats
	 */
	amt_running_domains = virConnectListAllDomains(conn, &domains, VIR_CONNECT_LIST_DOMAINS_RUNNING);
	if (amt_running_domains	< 0) {
		printf("Error listing domains: %s", virGetLastErrorMessage());
		return ret;
	}

	while (1) {
		for (int i = 0; i < amt_running domains; i++) {
			amt_params = virDomainGetCPUStats(domains[i], NULL, 0, 0 ,1, 0);
			if (amt_params < 0) {
				printf("Unable to get amount of CPU parameters.\n");
				return 1
			}


		}
	}

	/*
	 * Disconnecting from hypervisor
	 */
	ret = virConnectClose(conn);
	if (ret < 0) {
		printf("Failed to disconnect from hypervisor.\n");
	} else if (ret == 0) {
		printf("Successfully disconnected from hypervisor.\n");
	} else {
		printf("Disconnected from hypervisor, some references remain.\n");
	}

	return ret;
}
