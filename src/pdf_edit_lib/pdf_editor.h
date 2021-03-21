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

#ifndef PDFEDITOR_H
#define PDFEDITOR_H

#include <string>
#include <functional>
#include <vector>
#include <atomic>

#include <qpdf/QPDF.hh>
#include <qpdf/QUtil.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>
#include <qpdf/QPDFWriter.hh>
#include <qpdf/QPDFPageLabelDocumentHelper.hh>
#include <qpdf/QPDFOutlineDocumentHelper.hh>

#include "definitions.h"

class PdfEditor
{
public:

    class Page {
    public:
        double x, y, width, height;
        double crop_top, crop_bottom, crop_left, crop_right;
        int relative_rotation;
    };

    class PageLayout {
    public:
        PageLayout() {};
        PageLayout(const Multipage &mp);
        double width, height;
        std::vector<Page> pages;
    };

    PdfEditor();

    // returns the file_id to be used in add_pages
    unsigned int add_file(const std::string &filename);

    void add_blank_pages(double width, double height, int count);

    // add pages from pdf denoted by file_id.
    // layout is deleted by this function.
    // intervals pages numbering starts from 0.
    void add_pages(unsigned int file_id,
                   int relative_rotation = 0,
                   const PageLayout *layout = nullptr,
                   const std::vector<std::pair<int, int>> &intervals = {},
                   const std::string &outline_entry = {});

    void write(const std::string &output_filename);

private:
    enum class Move {
        up,
        down,
        next,
    };

    struct FlatOutline {
        Move next_move;
        std::string title;
        int page;
        double top;
        double left;
    };

    std::locale m_old_locale;
    int m_last_page;

    std::vector<std::string> m_input_filenames;
    std::vector<QPDF> m_input_files;
    std::vector<std::vector<QPDFPageObjectHelper>> m_pages;
    std::vector<std::vector<FlatOutline>> m_flat_outlines;

    static void m_add_flatten_outlines(const std::vector<QPDFPageObjectHelper> &pages,
                                     const std::vector<QPDFOutlineObjectHelper> &outlines,
                                     std::vector<FlatOutline> &flat_outlines);

    static QPDFObjectHandle m_create_blank_page(double width, double height);

    int m_build_outline_level(const std::vector<FlatOutline> &flat_outlines,
                            QPDFObjectHandle &parent,
                            unsigned int starting_index);

    void m_build_outlines(const std::vector<FlatOutline> &flat_outlines);

    void m_set_outline_destination(QPDFObjectHandle &outline,
                                   unsigned int page_index);

    void m_impose_page(QPDFObjectHandle &outer_page_obj,
                       QPDFPageObjectHelper &page,
                       int relative_rotation,
                       double x,
                       double y,
                       double width,
                       double height);

    QPDF m_output_pdf;
    std::vector<FlatOutline> m_output_outlines;

    QPDFObjectHandle m_last_outline;
    QPDFObjectHandle m_last_first_level_outline;
};

#endif // PDFEDITOR_H