#include<stdio.h>
#include<stdlib.h>
#include<libvirt/libvirt.h>
#include<libvirt/virterror.h>

int main(int arc, char *argv[]) {
	int ret = 0;

	printf("Attempting to connect to hypervisor.\n");
	virConnectPtr conn = virConnectOpen("qemu:///system");

	if (conn == NULL) {
		printf("Error occured: %s\n", virGetLastErrorMessage());
		return 1;
	} else {
		printf("Successfully connected to hypervisor.\n");
	}

	ret = virConnectClose(conn);
	if (ret == 0) {
		printf("Successfully disconnected from hypervisor.\n");
	} else if (ret > 0) {
		printf("Disconnected from hypervisor, some references remain.\n");
	} else {
		printf("Failed to disconnect from hypervisor.\n");
	}

	return ret;
}
