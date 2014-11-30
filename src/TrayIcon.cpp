/*
Project: MyCuteAssistant
File: TrayIcon.cpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#include "TrayIcon.hpp"
#include <QtWidgets/QWidget.h>
#include "config.h"
#include <QtCore/QCoreApplication.h>

// External image data
extern "C" const unsigned char logo_ico[];
extern "C" const unsigned logo_ico_size;

TrayIcon::TrayIcon(QWidget* parent) : QSystemTrayIcon(QIcon(QPixmap::fromImage(QImage::fromData(logo_ico, logo_ico_size))), parent){
	// Set tray icon properties
	this->setToolTip(APP_NAME " v" APP_VERSION_STRING);
	// Ensure taskicon removement from system tray (correct destruction)
	this->connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), SLOT(deleteLater()));
}
