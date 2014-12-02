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
#include <QtWidgets/QWidget>
#include "config.h"
#include <QtWidgets/QMenu>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

// External image access
#define IMPORT_RESOURCE_IMAGE(name) extern "C" const unsigned char name[]; extern "C" const unsigned name##_size;
IMPORT_RESOURCE_IMAGE(logo_ico)
IMPORT_RESOURCE_IMAGE(bye_png)
IMPORT_RESOURCE_IMAGE(show_hide_png)
#define QICON(image) QIcon(QPixmap::fromImage(QImage::fromData(image, image##_size)))

TrayIcon::TrayIcon(QWidget* parent) : QSystemTrayIcon(QICON(logo_ico), parent), hotkey("CTRL|ALT|Y", [=](){this->activated(QSystemTrayIcon::ActivationReason::DoubleClick);}){
	// Set tray icon properties
	this->setToolTip(APP_NAME " v" APP_VERSION_STRING);
	// Add tray icon menu
	QMenu* tray_menu = new QMenu(parent);
	QAction* tray_menu_show_hide = tray_menu->addAction(QICON(show_hide_png), "");	// Set dynamically (see below)
	QAction* tray_menu_on_top = new QAction(tray_menu);	// Set dynamically (see below)
	tray_menu_on_top->setText("Buuhuhu");
	tray_menu_on_top->setCheckable(true);
	tray_menu->addAction(tray_menu_on_top);
	tray_menu->addSeparator();
	tray_menu->addAction(QICON(bye_png), "Bye-bye", parent, SLOT(close()));
	this->setContextMenu(tray_menu);
	// Set tray icon actions
	QObject::connect(this, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason){
		switch(reason){
			case QSystemTrayIcon::ActivationReason::DoubleClick:
				if(parent->isVisible())
					parent->activateWindow();
				break;
			case QSystemTrayIcon::ActivationReason::Context:
				if(parent->isVisible() && tray_menu_show_hide->text() != "*Crouch*"){
					tray_menu_show_hide->setText("*Crouch*");
					tray_menu_show_hide->disconnect(parent);
					parent->connect(tray_menu_show_hide, SIGNAL(triggered()), SLOT(hide()));
				}else if(parent->isHidden() && tray_menu_show_hide->text() != "*Emerge*"){
					tray_menu_show_hide->setText("*Emerge*");
					tray_menu_show_hide->disconnect(parent);
					parent->connect(tray_menu_show_hide, SIGNAL(triggered()), SLOT(show()));
				}
				break;
			case QSystemTrayIcon::ActivationReason::Unknown:
			case QSystemTrayIcon::ActivationReason::Trigger:
			case QSystemTrayIcon::ActivationReason::MiddleClick: break;
		}
	});
	QObject::connect(tray_menu_on_top, &QAction::triggered, [=](bool checked){
#ifdef _WIN32
		SetWindowPos(reinterpret_cast<HWND>(parent->winId()), checked ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
#else
		bool was_shown = parent->isVisible();
		parent->setWindowFlags(checked ?
					parent->windowFlags() | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint :
					parent->windowFlags() & ~(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint));
		if(was_shown)
			parent->show();
#endif // _WIN32
	});
}

void TrayIcon::show(void){
	QSystemTrayIcon::show();
	if(!this->hotkey.isOk())
		this->showMessage(APP_NAME, "Couldn't bind hotkey!", QSystemTrayIcon::Warning, 8000);
}
