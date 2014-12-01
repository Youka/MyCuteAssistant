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
#include <QtWidgets/QMenu.h>

// External image access
#define IMPORT_RESOURCE_IMAGE(name) extern "C" const unsigned char name[]; extern "C" const unsigned name##_size;
IMPORT_RESOURCE_IMAGE(logo_ico)
IMPORT_RESOURCE_IMAGE(bye_png)
IMPORT_RESOURCE_IMAGE(show_hide_png)
#define QICON(image) QIcon(QPixmap::fromImage(QImage::fromData(image, image##_size)))

TrayIcon::TrayIcon(QWidget* parent) : QSystemTrayIcon(QICON(logo_ico), parent){
	// Set tray icon properties
	this->setToolTip(APP_NAME " v" APP_VERSION_STRING);
	// Add tray icon menu
	QMenu* tray_menu = new QMenu(parent);
	QAction* tray_menu_first_item = tray_menu->addAction(QICON(show_hide_png), "");	// Set dynamically (see below)
	tray_menu->addSeparator();
	tray_menu->addAction(QICON(bye_png), "Bye-bye", parent, SLOT(close()));
	this->setContextMenu(tray_menu);
	// Set tray icon action
	QObject::connect(this, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason){
		switch(reason){
			case QSystemTrayIcon::ActivationReason::DoubleClick:
				parent->activateWindow();
				break;
			case QSystemTrayIcon::ActivationReason::Context:
				if(parent->isVisible() && tray_menu_first_item->text() != "*Crouch*"){
					tray_menu_first_item->setText("*Crouch*");
					tray_menu_first_item->disconnect(parent);
					parent->connect(tray_menu_first_item, SIGNAL(triggered()), SLOT(hide()));
				}else if(!parent->isVisible() && tray_menu_first_item->text() != "*Emerge*"){
					tray_menu_first_item->setText("*Emerge*");
					tray_menu_first_item->disconnect(parent);
					parent->connect(tray_menu_first_item, SIGNAL(triggered()), SLOT(show()));
				}
				break;
			case QSystemTrayIcon::ActivationReason::Unknown:
			case QSystemTrayIcon::ActivationReason::Trigger:
			case QSystemTrayIcon::ActivationReason::MiddleClick: break;
		}
	});
}
