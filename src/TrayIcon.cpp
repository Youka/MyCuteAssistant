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
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLineEdit>
#include "AboutDialog.hpp"
#include "config.h"

// External file access
IMPORT_RESOURCE_FILE(logo_ico)
IMPORT_RESOURCE_FILE(show_hide_png)
IMPORT_RESOURCE_FILE(custom_png)
IMPORT_RESOURCE_FILE(characters_png)
IMPORT_RESOURCE_FILE(character_png)
IMPORT_RESOURCE_FILE(opacity_png)
IMPORT_RESOURCE_FILE(hotkey_png)
IMPORT_RESOURCE_FILE(about_png)
IMPORT_RESOURCE_FILE(bye_png)

namespace MCA{
	TrayIcon::TrayIcon(AvatarWindow* parent) : QSystemTrayIcon(QICON(logo_ico), parent), hotkey(Config::instance()->hotkey(), std::bind(TrayIcon::dbClick, this)){
		// Add tray icon menu
		QMenu* tray_menu = new QMenu(parent);
		QAction* tray_menu_show_hide = tray_menu->addAction(QICON(show_hide_png), ""),	// Set dynamically (see further down)
			*tray_menu_on_top = new QAction(tray_menu);
		tray_menu_on_top->setText("Jiiiiiii...");
		tray_menu_on_top->setCheckable(true);
		tray_menu_on_top->setChecked(Config::instance()->alwaysOnTop());
		QObject::connect(tray_menu_on_top, &QAction::toggled, [parent](bool checked){
			parent->alwaysOnTop(checked);
			Config::instance()->alwaysOnTop(checked);
		});
		tray_menu->addAction(tray_menu_on_top);
		tray_menu->addSeparator();
		QMenu* tray_menu_custom_menu = tray_menu->addMenu(QICON(custom_png), "Huh?");
		QMenu* tray_menu_custom_menu_characters = tray_menu_custom_menu->addMenu(QICON(characters_png), "Who?");
		QObject::connect(tray_menu_custom_menu_characters, &QMenu::aboutToShow, [tray_menu_custom_menu_characters](){
			tray_menu_custom_menu_characters->clear();
			QStringList characters = Character::possibleNames();
			for(QString& character : characters)
				tray_menu_custom_menu_characters->addAction(QICON(character_png), character);
		});
		QObject::connect(tray_menu_custom_menu_characters, &QMenu::triggered, [parent](QAction* action){
			parent->loadCharacter(action->text());
			Config::instance()->character(action->text());
		});
		QMenu* tray_menu_custom_menu_opacity = tray_menu_custom_menu->addMenu(QICON(opacity_png), "Buhuhuu");
		QSlider* opacity_slider = new QSlider;
		opacity_slider->setOrientation(Qt::Horizontal);
		opacity_slider->setMinimum(0);
		opacity_slider->setMaximum(255);
		opacity_slider->setSingleStep(1);
		opacity_slider->setPageStep(64);
		opacity_slider->setValue(Config::instance()->opacity());
		QObject::connect(opacity_slider, &QAbstractSlider::valueChanged, [parent](int slider_value){
			parent->setWindowOpacity(slider_value / 255.0);
			Config::instance()->opacity(slider_value);
		});
		QWidgetAction* opacity_slider_menu_item = new QWidgetAction(tray_menu_custom_menu_opacity);
		opacity_slider_menu_item->setDefaultWidget(opacity_slider);
		tray_menu_custom_menu_opacity->addAction(opacity_slider_menu_item);
		QMenu* tray_menu_hotkey = tray_menu->addMenu(QICON(hotkey_png), "Call me:");
		QLineEdit* hotkey_edit = new QLineEdit;
		hotkey_edit->setToolTip("Separate keys by '|'. Modifiers: SHIFT,CTRL,ALT.");
		hotkey_edit->setText(Config::instance()->hotkey());
		QObject::connect(hotkey_edit, &QLineEdit::returnPressed, [hotkey_edit,this](){
			GlobalHotkey new_hotkey(hotkey_edit->text(), std::bind(TrayIcon::dbClick, this));
			if(new_hotkey.isOk()){
				hotkey_edit->selectAll();
				this->hotkey = std::move(new_hotkey);
				Config::instance()->hotkey(hotkey_edit->text());
			}else{
				this->showMessage(APP_NAME, "Invalid hotkey!", QSystemTrayIcon::Information, 5000);
				hotkey_edit->setText(Config::instance()->hotkey());
			}
		});
		QWidgetAction* hotkey_edit_menu_item = new QWidgetAction(tray_menu_hotkey);
		hotkey_edit_menu_item->setDefaultWidget(hotkey_edit);
		tray_menu_hotkey->addAction(hotkey_edit_menu_item);
		QAction* tray_menu_about = tray_menu->addAction(QICON(about_png), "I'm...");
		QObject::connect(tray_menu_about, &QAction::triggered, [parent](){
			AboutDialog(parent).exec();
		});
		tray_menu->addSeparator();
		tray_menu->addAction(QICON(bye_png), "Bye-bye", parent, SLOT(close()));
		this->setContextMenu(tray_menu);
		// Set tray icon actions
		QObject::connect(this, &QSystemTrayIcon::activated, [parent,tray_menu_show_hide](QSystemTrayIcon::ActivationReason reason){
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
		// Set tray icon properties
		this->setToolTip(APP_NAME " v" APP_VERSION_STRING);
	}

	void TrayIcon::dbClick(void){
		this->activated(QSystemTrayIcon::ActivationReason::DoubleClick);
	}
}
