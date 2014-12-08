/*
Project: MyCuteAssistant
File: AboutDialog.cpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#include "AboutDialog.hpp"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include "resources.h"
#include "config.h"

// External file access
IMPORT_RESOURCE_FILE(logo_big_png)

namespace MCA{
	AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent, Qt::CustomizeWindowHint){
		// Create childs
		QLabel* image_display = new QLabel,
			*head_display = new QLabel,
			*text_display = new QLabel,
			*foot_display = new QLabel;
		image_display->setPixmap(QPIXMAP(logo_big_png));
		head_display->setTextFormat(Qt::RichText);
		head_display->setText(QString("<center><b><u><font size=4>%1 v%2</font></u><br>%3</b></center>").arg(APP_NAME, APP_VERSION_STRING, APP_DESCRIPTION));
		text_display->setTextFormat(Qt::RichText);
		text_display->setOpenExternalLinks(true);
		text_display->setText(QString("<u>Programmer:</u><br>&nbsp;&nbsp;&nbsp;&nbsp;%2<br><u>Powered by:</u><br>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"http://qt-project.org\">Qt5</a>").arg(APP_AUTHOR));
		foot_display->setTextFormat(Qt::RichText);
		foot_display->setText(QString("<font size=2>Build on %1, %2.</font>").arg(__DATE__, __TIME__));
		// Set layout
		QVBoxLayout* vbox = new QVBoxLayout;
		vbox->addWidget(image_display, 0, Qt::AlignHCenter);
		vbox->addWidget(head_display, 0, Qt::AlignHCenter);
		vbox->addSpacing(8);
		vbox->addWidget(text_display, 0, Qt::AlignLeft);
		vbox->addSpacing(8);
		vbox->addWidget(foot_display, 0, Qt::AlignHCenter);
		this->setLayout(vbox);
		// Set dialog properties
		this->setFocusPolicy(Qt::StrongFocus);	// Needed for focus events
		this->adjustSize();
		this->setFixedSize(this->size());
		QPalette palette = this->palette();
		palette.setColor(QPalette::Window, Qt::white);
		this->setPalette(palette);
		this->setWindowOpacity(0.9);
	}

	void AboutDialog::focusOutEvent(QFocusEvent*){
		this->close();
	}
}
