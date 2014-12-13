/*
Project: MyCuteAssistant
File: BubbleWindow.cpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#include "BubbleWindow.hpp"

namespace MCA{
	BubbleWindow::BubbleWindow(QWidget* parent) : QLabel(nullptr, Qt::Popup|Qt::NoDropShadowWindowHint|Qt::FramelessWindowHint), parent(parent){
		this->setAttribute(Qt::WA_TranslucentBackground);
		this->setFocusPolicy(Qt::StrongFocus);	// Needed for focus events
	}

	void BubbleWindow::show(Character* character){
		Character::Bubble& bubble_info = character->bubble();
		this->setPixmap(bubble_info.pixmap);
		this->adjustSize();
		QRect geometry = this->geometry();
		geometry.moveBottomRight(QPoint(this->parent->x() + this->parent->width() * bubble_info.x, this->parent->y() + this->parent->height() * bubble_info.y));
		this->setGeometry(geometry);
		QLabel::show();
	}

	void BubbleWindow::focusOutEvent(QFocusEvent*){
		this->hide();
	}
}
