#include <switch.h>
#include <dirent.h>

#include <borealis.hpp>

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

bool isLogging = false;
bool isDisabled = false;
bool isAlbum = false;
bool isSaltyActive = false;

void init_app(void) {}

void exit_app(void) {}

int main(int argc, char *argv[])
{
	// Init the app

	if (!brls::Application::init("SaltyNX-Tool"))
	{
		brls::Logger::error("Unable to init SaltyNX-Tool");
		return EXIT_FAILURE;
	}
	
	switch(appletGetAppletType()) {
		case AppletType_Application:
			break;
		case AppletType_SystemApplication:
			break;
		
		default:
			isAlbum = true;
			isSaltyActive = CheckPort();
			break;
	}
		
	if (isAlbum == false) {
		brls::Application::crash("Checking SaltyNX is not possible!\n Run homebrew in Applet Mode (from Album)!");
		while (brls::Application::mainLoop());
		return EXIT_SUCCESS;
	}
	else if (isSaltyActive == false) {
		brls::Application::crash("SaltyNX was not detected! Restart Switch or check\n if SaltyNX is installed properly.");
		while (brls::Application::mainLoop());
		return EXIT_SUCCESS;
	}
	
	DIR* flags_dir = opendir("sdmc:/SaltySD/flags");
	if (flags_dir == NULL) mkdir("sdmc:/SaltySD/flags", S_IRWXU|S_IRWXG|S_IRWXO);
	else closedir(flags_dir);
	
	FILE* logflag = fopen("sdmc:/SaltySD/flags/log.flag", "r");
	if (logflag) {
		isLogging = true;
		fclose(logflag);
	}
	
	FILE *disable_flag = fopen("sdmc:/SaltySD/flags/disable.flag", "r");
	if (disable_flag) {
		fclose(disable_flag);
		isDisabled = true;
	}

	// Create a sample view
	brls::TabFrame* rootFrame = new brls::TabFrame();
	rootFrame->setTitle("SaltyNX-Tool");
	rootFrame->setIcon(BOREALIS_ASSET("icon.jpg"));
	
	brls::List* OptionsList = new brls::List();
	brls::List* AboutList = new brls::List();

	brls::ListItem* StatusItem = new brls::ToggleListItem("Status", !isDisabled, "Enable/Disable SaltyNX (it doesn't kill process)", "Enabled", "Disabled");
	StatusItem->getClickEvent()->subscribe([](brls::View* view) {
		if (isDisabled == true) {
			remove("sdmc:/SaltySD/flags/disable.flag");
			isDisabled = false;
		}
		else {
			FILE* disableflag = fopen("sdmc:/SaltySD/flags/disable.flag", "w");
			fclose(disableflag);
			isDisabled = true;
		}
	});
	
	OptionsList->addView(StatusItem);
	
	brls::ListItem* LogItem = new brls::ToggleListItem("Logs", isLogging, "Enable/Disable logging (.log files are in SaltyNX folder)", "Enabled", "Disabled");
	
	LogItem->getClickEvent()->subscribe([](brls::View* view) {
		if (isLogging == true) {
			remove("sdmc:/SaltySD/flags/log.flag");
			isLogging = false;
		}
		else {
			FILE* logflag = fopen("sdmc:/SaltySD/flags/log.flag", "w");
			fclose(logflag);
			isLogging = true;
		}
	});
	
	OptionsList->addView(LogItem);
	
	rootFrame->addTab("Options", OptionsList);
	
    brls::Label* Label1 = new brls::Label(brls::LabelStyle::REGULAR, "SaltyNX-Tool 1.1", false);
    AboutList->addView(Label1);

    brls::Label* Label2 = new brls::Label(brls::LabelStyle::MEDIUM, "Research & Development of SaltyNX: Shiny Quagsire\nDevelopment of SaltyNX and SaltyNX-Tool: MasaGratoR", true);
    AboutList->addView(Label2);
	
	brls::Label* Label3 = new brls::Label(brls::LabelStyle::DESCRIPTION, "Copyright (C) 2020 MasaGratoR\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3 of the License. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program.  If not, see https://www.gnu.org/licenses/", true);
    AboutList->addView(Label3);
	
	rootFrame->addTab("About", AboutList);


	// Add the root view to the stack
	brls::Application::pushView(rootFrame);

	// Run the app
	while (brls::Application::mainLoop());

	// Exit
	return EXIT_SUCCESS;
}