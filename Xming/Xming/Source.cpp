#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream> 
#include <windows.h>
#include <stdlib.h>

using namespace std;


int main(int argc, TCHAR* argv[])
{

	//----------INITIALIZATION-------------------------------------------------------------------------------
	FILE* pipe;
	char psBuffer[128];
	string command;
	int port = 6000; //starting point number
	string FunctionInput;
	ofstream fout;

	//Defaults
	string WindowMode = "WindowMode";
	string AccessControl = "true";
	//-------------------------------------------------------------------------------------------------------


	for (int i = 1; i < argc; i++)
	{
		FunctionInput = argv[i];

		if (FunctionInput.compare("-h") == 0 || FunctionInput.compare("-help") == 0 || FunctionInput.compare("--h") == 0)
		{
			help:
			cout << "-----------------Instructions--------------" << endl;
			cout << "OPTIONS:" << endl;
			cout << "WindowMode    .....................Default: MultiWindow" << endl;
			cout << "--MultiWindow .....................Multiple Window Mode" << endl;
			cout << "--Fullscreen  .....................FUllscreen window Mode" << endl;
			cout << "------------------------------------------------------------------" << endl;
			cout << "Access Control.....................Default: OFF" << endl;
			cout << "--ac_true     .....................Access Control ON" << endl;
			cout << "--ac_false    .....................Access Control OFF" << endl;

			return 1;
		}
		else if (FunctionInput.compare("--MultiWindow") == 0)
		{
			WindowMode = "MultiWindow";
		}
		else if (FunctionInput.compare("--Fullscreen") == 0)
		{
			WindowMode = "Fullscreen";
		}
		else if (FunctionInput.compare("--ac_true") == 0)
		{
			AccessControl = "false";
		}
		else if (FunctionInput.compare("--ac_false") == 0)
		{
			AccessControl = "true";
		}
		else if (FunctionInput.compare("") == 0)
		{
			break;
		}
		else
		{
			cout << "INVALID OPTION: " << argv[i] << endl;
			goto help;
		}

	}
	


psBuffer[0] = 0;													//Zero buffer before beginning


//--------------GET USER-----------------------------------------------------------------------------------------
command = "whoami";													//get user who is calling this script
pipe = _popen(command.c_str(), "r");								//Run command in console and read output
fgets(psBuffer, 128, pipe);											//Read output from buffer
string user(psBuffer);												//Store output in user string
user = user.substr(0,user.length() - 1);							//Remove terminating character
string s = user;													//Need to remove Domain from "whoami" output as well
size_t pos = 0;
if ((pos = s.find("\\")) != std::string::npos)
{
	s.erase(0, pos + 1);
	user = s;
}
psBuffer[0] = 0;													//Re-zero buffer
//---------------------------------------------------------------------------------------------------------------

//-------------CONFIG FILE DEFINITION----------------------------------------------------------------------------
//string userspace = "C:\\Users\\Massimo\\Desktop";					//FOR TESTING PURPOSES REMOVE WHEN NOT TESTING
string userspace = "C:\\Users\\" + user + "\\Documents\\";			//Userspace directory where config file will be stored
string configfile = userspace + "config.xlaunch";					//Entire path and name of Config file
string logfile = userspace + "XmingWrapper.log";					//For log creation - coming soon
//---------------------------------------------------------------------------------------------------------------



for (int port = 6000; port < 6100; port++)
{

	command = "netstat -ano | find \"0.0.0.0:" + to_string(port) + "\"";	//Setup Command with port number

	pipe = _popen(command.c_str(), "r");									//Perform netstat command
	fgets(psBuffer, 128, pipe);												//Pass result to buffer in case we want to use

	if (psBuffer[0] == 0)
	{
		cout << "Port " << port << " is Empty - Starting Xming with display:" << to_string(port - 6000) << endl;

		fout.open(configfile);
		if (!fout)
		{
			cout << "Unable to open file" << endl;
			return 1;
		}

		//Write to file
		fout << "<?xml version=\"1.0\"?>";
		fout << endl << "<XLaunch xmlns=\"http://www.straightrunning.com/XmingNotes\" xmlns:xsi=\"http://www.w3.org//2001//XMLSchema-instance\" xsi:schemaLocation=\"http://www.straightrunning.com/XmingNotes XLaunch.xsd\" WindowMode=\"" + WindowMode + "\" ClientMode=\"NoClient\" Display=\"" + to_string(port - 6000) + "\" Clipboard=\"true\" NoAccessControl=\"" + AccessControl + "\"/>";
		fout.close();


		//command = "start \"\" \"C:\\Program Files (x86)\\Xming\\Xming.exe\" :" + to_string(port - 6000) + " -clipboard -multiwindow"; //Use this to use xming executable
		command = "start \"\" " + configfile;																							//Use this to use config file to execute from - need to specify Xlaunch as executor
		system(command.c_str());																										//Start Xming with unique Display

		break;
	}
	else
	{
		psBuffer[0] = 0;													//Re-zero buffer
	}

}


	return 0;
}