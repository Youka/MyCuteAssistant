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
#include "resources.h"
#include <QtWidgets/QMenu>
#include "Config.hpp"
#include <QtWidgets/QInputDialog>
#include "AboutDialog.hpp"
#include "config.h"

// External file access
IMPORT_RESOURCE_FILE(logo_ico)
IMPORT_RESOURCE_FILE(bye_png)
IMPORT_RESOURCE_FILE(show_hide_png)
IMPORT_RESOURCE_FILE(hotkey_png)
IMPORT_RESOURCE_FILE(about_png)

namespace MCA{
	TrayIcon::TrayIcon(AvatarWindow* parent) : QSystemTrayIcon(QICON(logo_ico), parent), hotkey(Config::instance()->hotkey(), std::bind(TrayIcon::dbClick, this)){
		// Add tray icon menu
		QMenu* tray_menu = new QMenu(parent);
		QAction* tray_menu_show_hide = tray_menu->addAction(QICON(show_hide_png), ""),	// Set dynamically (see below)
			*tray_menu_on_top = new QAction(tray_menu);	// Set dynamically (see below)
		tray_menu_on_top->setText("Jiiiiiii...");
		tray_menu_on_top->setCheckable(true);
		tray_menu_on_top->setChecked(Config::instance()->alwaysOnTop());
		tray_menu->addAction(tray_menu_on_top);
		QAction* tray_menu_hotkey = tray_menu->addAction(QICON(hotkey_png), "Call me:"),	// Set dynamically (see below)
			*tray_menu_about = tray_menu->addAction(QICON(about_png), "I'm...");
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
		QObject::connect(tray_menu_on_top, &QAction::toggled, [=](bool checked){
			parent->alwaysOnTop(checked);
			Config::instance()->alwaysOnTop(checked);
		});
		QObject::connect(tray_menu_hotkey, &QAction::triggered, [=](){
			QString new_keys = QInputDialog::getText(parent, QString(APP_NAME) + " - Hotkey", "Define my hotkey!\nKeys are separated by '|'.\nModifiers: ALT, CTRL, SHIFT.", QLineEdit::Normal, Config::instance()->hotkey(), nullptr, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
			if(!new_keys.isEmpty()){
				GlobalHotkey new_hotkey(new_keys, std::bind(TrayIcon::dbClick, this));
				if(new_hotkey.isOk()){
					this->hotkey = std::move(new_hotkey);
					Config::instance()->hotkey(new_keys);
				}else
					this->showMessage(APP_NAME, "Invalid hotkey!", QSystemTrayIcon::Information, 5000);
			}
		});
		QObject::connect(tray_menu_about, &QAction::triggered, [=](){
			AboutDialog(parent).exec();
		});
		// Set tray icon properties
		this->setToolTip(APP_NAME " v" APP_VERSION_STRING);
	}

	void TrayIcon::dbClick(void){
		this->activated(QSystemTrayIcon::ActivationReason::DoubleClick);
	}
}
