/*
Project: MyCuteAssistant
File: main.cpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#include <QtWidgets/QApplication>
#include "Config.hpp"
#include "TrayIcon.hpp"

// Windows expects a static Qt5, so have to link plugins in source
#ifdef _WIN32
#include <QtCore/QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
#endif

// Program entry
int main(int argc, char** argv){
	// Create global Qt application instance
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);	// Prevent stupid behaviour: before even closed when windows are still hidden!
	// Create global application configuration instance
	Config conf(app.applicationDirPath() + "/config.ini");
	// Create & show main windows
	AvatarWindow main_window;	// Quits application on closure
	main_window.show();
	TrayIcon tray(&main_window);
	tray.show();
	// Run Qt application & return his status code to this program
	return app.exec();
}
