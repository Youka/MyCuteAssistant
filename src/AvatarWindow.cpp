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
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include "resources.h"
#include <QtGui/QIcon>
#include "Config.hpp"
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

// External file access
IMPORT_RESOURCE_FILE(logo_ico)

namespace MCA{
	AvatarWindow::AvatarWindow(void) : QWidget(nullptr, Qt::Tool|Qt::FramelessWindowHint){
		// Set window position
		QRect desktop_geometry = QApplication::desktop()->availableGeometry();
		this->move(desktop_geometry.right() - this->width(), desktop_geometry.bottom() - this->height());	// Desktop: bottom-right
		// Set window properties
		this->setWindowIcon(QICON(logo_ico));	// In use by child windows
		this->setCursor(Qt::PointingHandCursor);
		this->alwaysOnTop(Config::instance()->alwaysOnTop());
	}

	void AvatarWindow::closeEvent(QCloseEvent* event){
		QWidget::closeEvent(event);
		QCoreApplication::instance()->quit();
	}

	void AvatarWindow::alwaysOnTop(bool on){
	#ifdef _WIN32
		SetWindowPos(reinterpret_cast<HWND>(this->winId()), on ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	#else
		bool was_shown = this->isVisible();
		this->setWindowFlags(on ?
					parent->windowFlags() | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint :
					parent->windowFlags() & ~(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint));
		if(was_shown)
			this->show();
	#endif // _WIN32
	}
}
