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
	BubbleWindow::BubbleWindow(QWidget* parent) : QLabel(parent, Qt::Tool|Qt::FramelessWindowHint), edit(this){
		// Set window properties
		this->setAttribute(Qt::WA_TranslucentBackground);
		// Add child widgets
		this->connect(&this->edit, SIGNAL(editingFinished(void)), SLOT(hide(void)));
	}

	void BubbleWindow::show(Character* character){
		// Get bubble informations & parent widget
		Character::Bubble& bubble_info = character->bubble();
		QWidget* parent = reinterpret_cast<QWidget*>(this->parent());
		// Set bubble
		this->setPixmap(bubble_info.pixmap);
		this->adjustSize();
		QRect geometry = this->geometry();
		geometry.moveBottomRight(QPoint(parent->x() + parent->width() * bubble_info.x, parent->y() + parent->height() * bubble_info.y));
		this->setGeometry(geometry);
		// Set bubble text edit
		this->edit.move(this->width() * bubble_info.edit_x, this->height() * bubble_info.edit_y);
		this->edit.setFixedSize(this->width() * bubble_info.edit_width, this->height() * bubble_info.edit_height);
		// Show bubble & focus edit
		QLabel::show();
		this->edit.activateWindow();
	}
}
