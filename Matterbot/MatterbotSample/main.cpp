#include "Matterbot.h"
#include <iostream>

#define ERROR_SUCCESS 0
#define ERROR_FAILURE -1

using namespace std;
using namespace lospi;

class EchoCommand : public ICommand {
public:
	wstring get_name() override {
		return L"echo";
	}

	wstring get_help() override {
		return L"`echo [MESSAGE]`\n===\n`echo` will respond with whatever message you give it.";
	}

	wstring handle_command(wstring team, wstring channel, wstring user, wstring command_text) override {
		return command_text;
	}
};

class CustomLogger : public ILogger {
	void info(const wstring &msg) override {
		wcout << "INFO: " << msg;
	}
	void warn(const wstring &msg) override {
		wcout << "WARN: " << msg;
	}
	void error(const wstring &msg) override {
		wcerr << "INFO: " << msg;
	}
};

int main() {
	//TODO: Put your own routes and tokens here
	wstring mattermost_url = L"https://hooks.slack.com",									// URL to the incoming webhook for Mattermost/Slack
		incoming_hook_route = L"services/AAAAAAAAA/BBBBBBBBB/CCCCCCCCCCCCCCCCCCCCCCCC",		// Route 
		outgoing_hook_route = L"http://127.0.0.1:8000/",									// URL of the box running matterbot
		outgoing_hook_token = L"CCCCCCCCCCCCCCCCCCCCCCCC";
	try {
		Matterbot bot(mattermost_url, incoming_hook_route, outgoing_hook_route, outgoing_hook_token);
		bot.post_message(L"Bot is up.");
		
		//TODO: Add your other commands here
		bot.register_command(make_shared<EchoCommand>());
		
		//TODO: optional, you can implement your own logger. Default logger sends to wclog.
		bot.set_logger(make_unique<CustomLogger>());

		// Here's the listen/command loop.
		wstring console;
		wclog << "[ ] Type \'quit\' to quit. Any other input will be sent to the bot." << endl;
		while (getline(wcin, console)) {
			if (L"quit" == console)
			{
				wclog << "[ ] Quitting." << endl;
				return ERROR_SUCCESS;
			}
			if (L"" != console)
			{
				bot.post_message(console);
			}
		}
	}
	catch (exception e)
	{
		wcerr << "[-] Unhandled exception:" << e.what() << endl;
		return ERROR_FAILURE;
	}
}