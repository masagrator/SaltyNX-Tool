#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <switch.h>

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
	bool log = false;
	AppletType at = appletGetAppletType();
	consoleInit(NULL);
	DIR* flags_dir = opendir("sdmc:/SaltySD/flags");
	if (!flags_dir) {
		mkdir("sdmc:/SaltySD/flags", ACCESSPERMS);
	}
	else closedir(flags_dir);
	FILE* logflag = fopen("sdmc:/SaltySD/flags/log.flag", "r");
	if (logflag) {
		log = true;
		fclose(logflag);
	}
	FILE *disable_flag = fopen("sdmc:/SaltySD/flags/disable.flag", "r");

	if (disable_flag) {
		fclose(disable_flag);
		goto disabled;
	}
	else goto normal;

bool inj = false;

disabled:
	if (at != AppletType_Application && at != AppletType_SystemApplication) {
		inj = CheckPort();
		if (inj == true) printf("SaltyNX is injected properly.\n");
		else printf(CONSOLE_RED "SaltyNX is not injected!!\n");
	}
	else printf(CONSOLE_RED "Checking SaltyNX is not possible! Run homebrew in Applet Mode!\n");
	remove("sdmc:/SaltySD/FPSoffset.hex");
	printf("SaltyNX is disabled.\n\n");
	printf("To enable loading SaltyNX, press A.\n");
	if (log == true) printf("Press ZL to disable log writing.\n");
	else printf("Press ZL to enable log writing.\n");
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
		else if (kDown & KEY_ZL) {
			if (log == true) {
				remove("sdmc:/SaltySD/flags/log.flag");
				log = false;
				consoleClear();
				goto disabled;
			}
			else {
				logflag = fopen("sdmc:/SaltySD/flags/log.flag", "w");
				fclose(logflag);
				log = true;
				consoleClear();
				goto disabled;
			}
		}
	}
	goto close;
			
normal:
	if (at != AppletType_Application && at != AppletType_SystemApplication) {
		inj = CheckPort();
		if (inj == true) printf("SaltyNX is injected properly.\n");
		else printf(CONSOLE_RED "SaltyNX is not injected!!\n");
	}
	else printf(CONSOLE_RED "Checking SaltyNX is not possible! Run homebrew in Applet Mode!\n");
	printf("SaltyNX is enabled.\n\n");
	printf("To disable loading SaltyNX, press A.\n");
	if (log == true) printf("Press ZL to disable log writing.\n");
	else printf("Press ZL to enable log writing.\n");
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
		else if (kDown & KEY_ZL) {
			if (log == true) {
				remove("sdmc:/SaltySD/flags/log.flag");
				log = false;
				consoleClear();
				goto normal;
			}
			else {
				logflag = fopen("sdmc:/SaltySD/flags/log.flag", "w");
				fclose(logflag);
				log = true;
				consoleClear();
				goto normal;
			}
		}
	}
	goto close;

close:
	consoleExit(NULL);
	return 0;
}
