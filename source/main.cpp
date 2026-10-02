#include <switch.h>
#include <dirent.h>
#include <sys/stat.h>

#include <borealis.hpp>
#include <cstdlib>

#include "About_tab.hpp"
#include "Options_tab.hpp"

bool CheckPort () {
	Handle saltysd;
	for (int i = 0; i < 67; i++) {
		if (R_SUCCEEDED(svcConnectToNamedPort(&saltysd, "InjectServ"))) {
			svcCloseHandle(saltysd);
			break;
		}
		else {
			if (i == 66) return false;
			svcSleepThread(1'000'000);
		}
	}
	for (int i = 0; i < 67; i++) {
		if (R_SUCCEEDED(svcConnectToNamedPort(&saltysd, "InjectServ"))) {
			svcCloseHandle(saltysd);
			return true;
		}
		else svcSleepThread(1'000'000);
	}
	return false;
}

class MainActivity : public brls::Activity
{
  public:
	CONTENT_FROM_XML_RES("activity/main.xml");
};

// Full screen error message, replacement for the old brls::Application::crash()
static void showError(const std::string& message)
{
	brls::Label* label = new brls::Label();
	label->setText(message);
	label->setFontSize(24);
	label->setHorizontalAlign(brls::HorizontalAlign::CENTER);

	brls::Box* box = new brls::Box(brls::Axis::COLUMN);
	box->setJustifyContent(brls::JustifyContent::CENTER);
	box->setAlignItems(brls::AlignItems::CENTER);
	box->setGrow(1.0f);
	box->addView(label);
	// Focusable (but invisible focus) so the footer shows the exit hint
	box->setFocusable(true);
	box->setHideHighlight(true);
	box->setHideClickAnimation(true);

	brls::AppletFrame* frame = new brls::AppletFrame(box);
	frame->setTitle(APP_TITLE);
	frame->setIcon(std::string(BRLS_RESOURCES) + "icon.jpg");
	box->registerAction("Exit", brls::BUTTON_A, [](brls::View*) {
		brls::Application::quit();
		return true;
	});

	brls::Application::pushActivity(new brls::Activity(frame));
}

int main(int argc, char *argv[])
{
	// Init the app
	if (!brls::Application::init())
	{
		brls::Logger::error("Unable to init SaltyNX-Tool");
		return EXIT_FAILURE;
	}

	brls::Application::createWindow(APP_TITLE);
	brls::Application::setGlobalQuit(true);

	brls::Application::registerXMLView("OptionsTab", OptionsTab::create);
	brls::Application::registerXMLView("AboutTab", AboutTab::create);

	bool isAlbum = false;
	bool isSaltyActive = false;

	switch(appletGetAppletType()) {
		case AppletType_Application:
		case AppletType_SystemApplication:
			break;

		default:
			isAlbum = true;
			isSaltyActive = CheckPort();
			break;
	}

	if (!isAlbum)
		showError("Checking SaltyNX is not possible!\nRun homebrew in Applet Mode (from Album)!");
	else if (!isSaltyActive)
		showError("SaltyNX was not detected! Restart Switch or check\nif SaltyNX is installed properly.");
	else
	{
		DIR* flags_dir = opendir("sdmc:/SaltySD/flags");
		if (flags_dir == NULL) mkdir("sdmc:/SaltySD/flags", S_IRWXU|S_IRWXG|S_IRWXO);
		else closedir(flags_dir);

		brls::Application::pushActivity(new MainActivity());
	}

	// Run the app
	while (brls::Application::mainLoop());

	// Exit
	return EXIT_SUCCESS;
}
