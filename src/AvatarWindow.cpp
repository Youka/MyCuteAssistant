/*
Project: MyCuteAssistant
File: AvatarWindow.cpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#include "AvatarWindow.hpp"
#include <QtWidgets/QSystemTrayIcon.h>
#include <QtCore/QCoreApplication.h>
#include "config.h"

// External image data
extern "C" const unsigned char logo_png[];
extern "C" const unsigned logo_png_size;

AvatarWindow::AvatarWindow(QWidget* parent) : QWidget(parent){
	// Create icon out of external image
	QIcon icon(QPixmap::fromImage(QImage::fromData(logo_png, logo_png_size)));
	// Set window properties
	this->setWindowIcon(icon);
	// Create & show system tray icon
	QSystemTrayIcon* tray = new QSystemTrayIcon(icon, this);
	tray->setToolTip(APP_NAME " v" APP_VERSION_STRING);
	tray->show();
	tray->connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), SLOT(hide()));
	QObject::connect(tray, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason){this->close();});
}
