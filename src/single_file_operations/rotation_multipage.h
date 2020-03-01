/* Copyright (C) 2020 Marco Scarpetta
 *
 * This file is part of PDF Mix Tool.
 *
 * PDF Mix Tool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PDF Mix Tool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PDF Mix Tool. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ROTATEMULTIPAGE_H
#define ROTATEMULTIPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>

#include "../pdf_edit_lib/pdf_info.h"

class RotationMultipage : public QWidget
{
    Q_OBJECT
public:
    explicit RotationMultipage(QWidget *parent = nullptr);

    QComboBox rotation;
    QComboBox multipage;
    QLabel preview_image;
    PdfInfo opened_pdf_info;

public slots:
    void update_multipage_profiles();

    void update_preview_image();

    void multipage_activated(int index);

    void profile_created(int index);

signals:
    void trigger_new_profile();
    void save_button_pressed();
    void save_as_button_pressed();

};

#endif // ROTATEMULTIPAGE_H
