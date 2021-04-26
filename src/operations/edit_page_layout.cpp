/* Copyright (C) 2020-2021 Marco Scarpetta
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

#include "edit_page_layout.h"

#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <cmath>

#include "../gui_utils.h"

EditPageLayout::EditPageLayout(const PdfInfo &pdf_info,
                               QWidget *parent) :
    AbstractOperation(pdf_info, parent),
    m_multipage_editor{new MultipageEditor{this}},
    m_pages_selector{new PagesSelector{true, true, this}},
    m_output_preview{new OutputPreview{this}}
{
    m_name = tr("Pages layout");
    m_icon = QIcon(m_icon_dir.filePath("pages_layout.svg"));

    QVBoxLayout *v_layout = new QVBoxLayout();
    this->setLayout(v_layout);

    QGridLayout *grid_layout = new QGridLayout();
    v_layout->addLayout(grid_layout);
    grid_layout->addWidget(m_multipage_editor, 1, 1);

    QLabel *apply_to_label = new QLabel(tr("Apply to:"), this);
    QFont bold = apply_to_label->font();
    bold.setBold(true);
    apply_to_label->setContentsMargins(0, 20, 0, 5);
    apply_to_label->setFont(bold);
    grid_layout->addWidget(apply_to_label, 2, 1);

    grid_layout->addWidget(m_pages_selector, 3, 1);
    grid_layout->addWidget(m_output_preview, 1, 2, 3, 1);

    m_output_preview->set_multipage_enabled(true);
    m_output_preview->setMinimumSize(200, 200);
    m_output_preview->setMaximumSize(400, 400);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    grid_layout->addWidget(spacer, 4, 1, 1, 2);

    QHBoxLayout *h_layout = new QHBoxLayout();
    v_layout->addLayout(h_layout);
    h_layout->addItem(new QSpacerItem(0, 0,
                                      QSizePolicy::Expanding,
                                      QSizePolicy::Minimum));

    QPushButton *save_as_button = new QPushButton(
                QIcon::fromTheme("document-save-as"),
                tr("Save as…"),
                this);
    save_as_button->setShortcut(QKeySequence::SaveAs);
    save_as_button->setToolTip(
                QString(TOOLTIP_STRING)
                .arg(
                    save_as_button->text(),
                    save_as_button->shortcut().toString()));

    h_layout->addWidget(&m_save_button);
    h_layout->addWidget(save_as_button);

    connect(m_multipage_editor, &MultipageEditor::multipage_changed,
            m_output_preview, &OutputPreview::set_multipage);

    connect(m_multipage_editor, &MultipageEditor::subpages_number_changed,
            [=](int subpages) {
        bool enabled = subpages == 1;
        m_pages_selector->setEnabled(enabled);
        apply_to_label->setEnabled(enabled);
    });

    m_multipage_editor->set_multipage(
                settings->value("edit_pages_layout_multipage").value<Multipage>());

    connect(&m_save_button, &QPushButton::pressed,
            [=]() {if (show_overwrite_dialog()) save();});
    connect(save_as_button, &QPushButton::pressed,
            [=]() {if (show_save_as_dialog()) save();});
}

void EditPageLayout::pdf_info_changed()
{
    AbstractOperation::pdf_info_changed();

    m_output_preview->set_pdf_info(*m_pdf_info);
}

void EditPageLayout::save()
{

    QString selection{""};

    if (m_pages_selector->isEnabled())
    {
        selection = m_pages_selector->get_selection_as_text(
                    m_pdf_info->n_pages());

        if (selection.isNull())
            return;
    }

    // save multipage profile
    Multipage multipage = m_multipage_editor->get_multipage();
    settings->setValue("edit_pages_layout_multipage",
                       QVariant::fromValue<Multipage>(multipage));

    emit write_started();

    try
    {
        PdfEditor editor;
        unsigned int id = editor.add_file(m_pdf_info->filename());

        int output_pages_count;
        std::vector<std::pair<int, int>> rotate_intervals;

        parse_output_pages_string(selection.toStdString(),
                                  m_pdf_info->n_pages(),
                                  rotate_intervals,
                                  output_pages_count);

        std::vector<bool> rotate_pages;
        for (int i = 0; i < m_pdf_info->n_pages(); i++)
            rotate_pages.push_back(false);

        std::vector<std::pair<int, int>>::iterator it;
        for (it = rotate_intervals.begin(); it != rotate_intervals.end(); ++it)
            for (int i = it->first; i <= it->second; i++)
                rotate_pages[i] = true;

        // add dummy last element
        rotate_pages.push_back(!rotate_pages.back());

        PdfEditor::PageLayout page_layout{multipage};

        int first{0};
        std::vector<std::pair<int, int>> intervals;
        for (unsigned int i{1}; i < rotate_pages.size(); i++)
        {
            if (rotate_pages[i - 1] != rotate_pages[i])
            {
                if (rotate_pages[i - 1])
                    editor.add_pages(id, 0,
                                     new PdfEditor::PageLayout(page_layout),
                                     {std::pair<int, int>{first, i - 1}});
                else
                    editor.add_pages(id, 0, nullptr,
                                     {std::pair<int, int>{first, i - 1}});

                first = i;
            }
        }
        emit progress_changed(70);

        editor.write(m_save_filename.toStdString());

        emit write_finished(m_save_filename);
    }
    catch (std::exception &e)
    {
        emit write_error(QString::fromStdString(e.what()));
    }
}
