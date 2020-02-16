/* Copyright (C) 2017-2020 Marco Scarpetta
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

#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <QPainter>

#include "pdf_edit_lib/definitions.h"

#define FILE_PATH_ROLE Qt::UserRole + 1 // QString
#define PAGE_WIDTH_ROLE Qt::UserRole + 2 // int
#define PAGE_HEIGHT_ROLE Qt::UserRole + 3 // int
#define PAPER_SIZE_ROLE Qt::UserRole + 4 // QString
#define IS_PORTRAIT_ROLE Qt::UserRole + 5 // bool
#define N_PAGES_ROLE Qt::UserRole + 6 // int
#define OUTPUT_PAGES_ROLE Qt::UserRole + 7 // QString
#define MULTIPAGE_ROLE Qt::UserRole + 8 // int
#define ROTATION_ROLE Qt::UserRole + 9 // int
#define OUTLINE_ENTRY_ROLE Qt::UserRole + 10 // QString
#define REVERSE_ORDER_ROLE Qt::UserRole + 11 // bool

Q_DECLARE_METATYPE(Multipage)

QDataStream &operator<<(QDataStream &out, const Multipage &maltipage);

QDataStream &operator>>(QDataStream &in, Multipage &maltipage);

double draw_preview_page(QPainter *painter,
                         int max_width,
                         int max_height,
                         double page_width,
                         double page_height,
                         Multipage::Alignment h_alignment,
                         Multipage::Alignment v_alignment,
                         const QString &text);

void draw_preview(QPainter *painter,
                  const QRect &rect,
                  double source_width,
                  double source_height,
                  int rotation,
                  bool multipage_enabled,
                  const Multipage &multipage);

#endif // GUI_UTILS_H