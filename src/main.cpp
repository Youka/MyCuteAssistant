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

#include <QtWidgets/QApplication.h>
#include <QtWidgets/QLabel.h>
#include <QtWidgets/QSystemTrayIcon.h>
#include "config.h"

// Windows expects a static Qt5, so have to link the platform plugin in source
#ifdef _WIN32
#include <QtCore/QPlugin.h>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

// External image data
extern "C" const unsigned char logo_png[];
extern "C" const unsigned logo_png_size;

// Program entry
int main(int argc, char** argv){
	// Create global Qt application instance
	QApplication app(argc, argv);
	// Create & show test window
	QLabel* label = new QLabel("Hello world!");
	label->show();
	// Create & show system tray icon
	QIcon icon(QPixmap::fromImage(QImage::fromData(logo_png, logo_png_size)));
	QSystemTrayIcon* tray = new QSystemTrayIcon(icon, label);
	tray->setToolTip(APP_NAME " v" APP_VERSION_STRING);
	tray->show();
	QObject::connect(&app, SIGNAL(aboutToQuit()), tray, SLOT(hide()));
	// Run Qt application & return his status code to this program
	return app.exec();
}
