#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <switch.h>

bool isServiceRunning(const char *serviceName) {
  Handle handle;
  SmServiceName service_name = smEncodeName(serviceName);
  bool running = R_FAILED(smRegisterService(&handle, service_name, false, 1));

  svcCloseHandle(handle);

  if (!running) smUnregisterService(service_name);

  return running;
}

bool CheckPort () {
	Result ret;
	Handle saltysd;
    for (int i = 0; i < 200; i++)
    {
        ret = svcConnectToNamedPort(&saltysd, "InjectServ");
        svcSleepThread(1000*1000);
        
        if (!ret) break;
    }
	svcCloseHandle(saltysd);
	if (ret != 0x0) return false;
	else return true;
}

int main(int argc, char **argv)
{
    consoleInit(NULL);
	FILE *disable_flag = fopen("sdmc:/SaltySD/flags/disable.flag", "r");
	
	if (disable_flag) {
		fclose(disable_flag);
		goto disabled;
	}
	else goto normal;

bool inj;

disabled:
	inj = CheckPort();
	if (inj == true) printf("SaltyNX is injected properly.\n");
	else printf(CONSOLE_RED "SaltyNX is not injected!!\n");
	remove("sdmc:/SaltySD/FPSoffset.hex");
	printf("SaltyNX is disabled.\n\n");
	printf("To enable loading SaltyNX, press A.\n");
	printf("Press X to exit.\n");
	consoleUpdate(NULL);
    while(appletMainLoop())
    {	
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_X) {
			goto close;
		}
		else if (kDown & KEY_A) {
			remove("sdmc:/SaltySD/flags/disable.flag");
			consoleClear();
			goto normal;
		}
	}
	goto close;
			
normal:
	inj = CheckPort();
	if (inj == true) printf("SaltyNX is injected properly.\n");
	else printf(CONSOLE_RED "SaltyNX is not injected!!\n");
	printf("SaltyNX is enabled.\n\n");
	printf("To disable loading SaltyNX, press A.\n");
	printf("Press X to exit.\n");
	consoleUpdate(NULL);
    while(appletMainLoop())
    {	
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_X) goto close;
		else if (kDown & KEY_A) {
			FILE* disableflag = fopen("sdmc:/SaltySD/flags/disable.flag", "w");
			fclose(disableflag);
			consoleClear();
			goto disabled;
		}
	}
	goto close;

close:
    consoleExit(NULL);
    return 0;
}
