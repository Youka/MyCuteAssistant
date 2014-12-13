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
#include <QtWidgets/QMessageBox>
#include "config.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include "resources.h"
#include <QtGui/QIcon>
#include "Config.hpp"
#include <memory>
#include <QtGui/QMouseEvent>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

// External file access
IMPORT_RESOURCE_FILE(logo_ico)

namespace MCA{
	AvatarWindow::AvatarWindow(void) : QLabel(nullptr, Qt::Tool|Qt::FramelessWindowHint), bubble(this){
		// Set window icon (in use by child windows)
		this->setWindowIcon(QICON(logo_ico));
		// Set character image
		this->setAttribute(Qt::WA_TranslucentBackground);
		this->loadCharacter(Config::instance()->character());
		QObject::connect(&this->idle_timer, &QTimer::timeout, [this](void){
			this->character.state(Character::State::IDLE);
			this->loadCharacter();
		});
		// Set window properties
		this->setCursor(Qt::PointingHandCursor);
		this->alwaysOnTop(Config::instance()->alwaysOnTop());
		this->setWindowOpacity(Config::instance()->opacity() / 255.0);
		// Set window position
		QPoint config_pos = Config::instance()->position();
		if(config_pos.x() < 0 || config_pos.y() < 0){
			QRect widget_geometry = this->geometry();
			widget_geometry.moveBottomRight(QApplication::desktop()->availableGeometry().bottomRight());	// Desktop: bottom-right
			this->setGeometry(widget_geometry);
		}else
			this->move(config_pos);
	}

	void AvatarWindow::loadCharacter(QString name){
		if(!this->character.load(name))
			QMessageBox(QMessageBox::Warning, APP_NAME, QString("Couldn't load character '%1' completely!\n%2").arg(this->character.name(), this->character.errorString()), QMessageBox::Ok, this, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint).exec();
		this->loadCharacter();
	}
	void AvatarWindow::loadCharacter(void){
		// Delete old movie of this window (prevent memory leak)
		std::unique_ptr<QMovie>(this->movie());
		// Collect relevant character data
		QMovie& movie = this->character.currentImage();
		unsigned short size = Config::instance()->size();
		// Single image
		if(movie.frameCount() < 2){
			// Cache & read first movie frame
			movie.jumpToFrame(0);
			QPixmap pixmap = movie.currentPixmap();
			// Set scaled frame to this window
			this->setPixmap(size == 100 ? pixmap : pixmap.scaled(pixmap.width() * size / 100, pixmap.height() * size / 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		// Animation
		}else{
			// Create new movie instance, belonging to this window
			QMovie* movie_copy = new QMovie(movie.fileName(), QByteArray(), this);
			// Cache all frames (better solution for small, repeating animations)
			movie_copy->setCacheMode(QMovie::CacheAll);
			// Scale movie on need
			if(size != 100){
				movie_copy->jumpToFrame(0);
				movie_copy->setScaledSize(movie_copy->currentImage().size() * size / 100);
			}
			// Set movie to this window and start the animation
			this->setMovie(movie_copy);
			movie_copy->start();
		}
		// Set window size to new contents
		this->adjustSize();
		// Adjust bubble to avatar changes
		this->bubble.loadCharacter(&this->character);
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

	void AvatarWindow::closeEvent(QCloseEvent* event){
		QLabel::closeEvent(event);
		QCoreApplication::instance()->quit();
	}

	void AvatarWindow::mousePressEvent(QMouseEvent* event){
		if(event->button() == Qt::LeftButton){
			event->setAccepted(true);
			this->mouse_press_pos = event->pos();
		}else if(event->button() == Qt::RightButton){
			event->setAccepted(true);
			this->bubble.show();
		}
	}
	void AvatarWindow::mouseMoveEvent(QMouseEvent* event){
		if(event->buttons() & Qt::LeftButton){
			event->setAccepted(true);
			QRect geometry = this->geometry(),
				desktop_geometry = QApplication::desktop()->availableGeometry();
			geometry.translate(event->pos() - this->mouse_press_pos);
			if(geometry.left() < desktop_geometry.left())
				geometry.translate(desktop_geometry.left() - geometry.left(), 0);
			if(geometry.top() < desktop_geometry.top())
				geometry.translate(0, desktop_geometry.top() - geometry.top());
			if(geometry.right() > desktop_geometry.right())
				geometry.translate(desktop_geometry.right() - geometry.right(), 0);
			if(geometry.bottom() > desktop_geometry.bottom())
				geometry.translate(0, desktop_geometry.bottom() - geometry.bottom());
			this->setGeometry(geometry);
			if(this->character.state() != Character::State::MOVE){
				this->character.state(Character::State::MOVE);
				this->loadCharacter();
			}
			this->idle_timer.start(100);
		}
	}
	void AvatarWindow::mouseReleaseEvent(QMouseEvent* event){
		if(event->button() == Qt::LeftButton){
			event->setAccepted(true);
			Config::instance()->position(this->pos());
		}
	}
}
