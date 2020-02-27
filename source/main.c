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

void renametocheatstemp() {
	renametocheatstempcontents();
	char cheatspath[64];
	char cheatspathtemp[64];
	DIR *dirp;
	struct dirent *dp;
	while(1) {
		dirp = opendir("sdmc:/Atmosphere/titles/");
		while (dirp) {
			if ((dp = readdir(dirp)) != NULL) {
				snprintf(cheatspath, sizeof cheatspath, "sdmc:/Atmosphere/titles/%s/cheats", dp->d_name);
				snprintf(cheatspathtemp, sizeof cheatspathtemp, "%stemp", cheatspath);
				rename(cheatspath, cheatspathtemp);
				printf(".");
				consoleUpdate(NULL);
				char cheatspath = "";
				char cheatspathtemp = "";
			}
			else {
				FILE* renametocheats = fopen("sdmc:/SaltySD/flags/renametocheats.flag", "w");
				fclose(renametocheats);
				closedir(dirp);
				return;
			}
		}
		closedir(dirp);
		return;
	}
}

void renametocheatstempcontents() {
	char cheatspath[64];
	char cheatspathtemp[64];
	DIR *dirp;
	struct dirent *dp;
	while(1) {
		dirp = opendir("sdmc:/Atmosphere/contents/");
		while (dirp) {
			if ((dp = readdir(dirp)) != NULL) {
				snprintf(cheatspath, sizeof cheatspath, "sdmc:/Atmosphere/contents/%s/cheats", dp->d_name);
				snprintf(cheatspathtemp, sizeof cheatspathtemp, "%stemp", cheatspath);
				rename(cheatspath, cheatspathtemp);
				printf(".");
				consoleUpdate(NULL);
				char cheatspath = "";
				char cheatspathtemp = "";
			}
			else {
				FILE* renametocheats = fopen("sdmc:/SaltySD/flags/renametocheats.flag", "w");
				fclose(renametocheats);
				closedir(dirp);
				return;
			}
		}
		closedir(dirp);
		return;
	}
}

void renametocheats() {
	renametocheatscontents();
	char cheatspath[64];
	char cheatspathtemp[64];
	DIR *dirp;
	struct dirent *dp;
	while(1) {
		dirp = opendir("sdmc:/Atmosphere/titles/");
		while (dirp) {
			if ((dp = readdir(dirp)) != NULL) {
				snprintf(cheatspath, sizeof cheatspath, "sdmc:/Atmosphere/titles/%s/cheats", dp->d_name);
				snprintf(cheatspathtemp, sizeof cheatspathtemp, "%stemp", cheatspath);
				rename(cheatspathtemp, cheatspath);
				printf(".");
				consoleUpdate(NULL);
				char cheatspath = "";
				char cheatspathtemp = "";
			}
			else {
				closedir(dirp);
				remove("sdmc:/SaltySD/flags/renametocheats.flag");
				return;
			}
		}
		closedir(dirp);
		return;
	}
}

void renametocheatscontents() {
	char cheatspath[64];
	char cheatspathtemp[64];
	DIR *dirp;
	struct dirent *dp;
	while(1) {
		dirp = opendir("sdmc:/Atmosphere/contents/");
		while (dirp) {
			if ((dp = readdir(dirp)) != NULL) {
				snprintf(cheatspath, sizeof cheatspath, "sdmc:/Atmosphere/contents/%s/cheats", dp->d_name);
				snprintf(cheatspathtemp, sizeof cheatspathtemp, "%stemp", cheatspath);
				rename(cheatspathtemp, cheatspath);
				printf(".");
				consoleUpdate(NULL);
				char cheatspath = "";
				char cheatspathtemp = "";
			}
			else {
				closedir(dirp);
				remove("sdmc:/SaltySD/flags/renametocheats.flag");
				return;
			}
		}
		closedir(dirp);
		return;
	}
}

int main(int argc, char **argv)
{
	bool Atmosphere_present = false;
    consoleInit(NULL);
	FILE *disable_flag = fopen("sdmc:/SaltySD/flags/disable.flag", "r");
	Atmosphere_present = isServiceRunning("dmnt:cht") && !(isServiceRunning("tx") && !isServiceRunning("rnx"));
	
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
	printf("SaltyNX is disabled.\n\n");
	printf("To enable loading SaltyNX, press A.\n");
	FILE* cheats1 = fopen("sdmc:/SaltySD/flags/renametocheats.flag", "r");
	if (Atmosphere_present == true) {
		if (cheats1 == NULL) printf("To disable cheats, press B.\n\n");
		else printf("To enable cheats, press B.\n\n");
	}
	printf("Press X to exit.\n");
	consoleUpdate(NULL);
    while(appletMainLoop())
    {	
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_X) {
			fclose(cheats1);
			goto close;
		}
		else if (kDown & KEY_A) {
			remove("sdmc:/SaltySD/flags/disable.flag");
			consoleClear();
			fclose(cheats1);
			goto normal;
		}
		else if (kDown & KEY_B && Atmosphere_present == true) {
			if (cheats1 == NULL) {
				fclose(cheats1);
				renametocheatstemp();
				consoleClear();
			}
			else {
				fclose(cheats1);
				renametocheats();
				consoleClear();
			}
			goto disabled;
		}
	}
	goto close;
			
normal:
	inj = CheckPort();
	if (inj == true) printf("SaltyNX is injected properly.\n");
	else printf(CONSOLE_RED "SaltyNX is not injected!!\n");
	printf("SaltyNX is enabled.\n\n");
	printf("To disable loading SaltyNX, press A.\n");
	FILE* cheats2 = fopen("sdmc:/SaltySD/flags/renametocheats.flag", "r");
	if (Atmosphere_present == true) {
		if (cheats2 == NULL) printf("To disable cheats, press B.\n\n");
		else printf("To enable cheats, press B.\n\n");
	}
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
			fclose(cheats2);
			goto disabled;
		}
		else if (kDown & KEY_B && Atmosphere_present == true) {
			if (cheats2 == NULL) {
				fclose(cheats2);
				renametocheatstemp();
				consoleClear();
			}
			else {
				fclose(cheats2);
				renametocheats();
				consoleClear();
			}
			goto normal;
		}
	}
	goto close;

close:
    consoleExit(NULL);
    return 0;
}
