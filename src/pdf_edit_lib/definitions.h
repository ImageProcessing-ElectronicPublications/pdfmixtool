/* Copyright (C) 2017-2019 Marco Scarpetta
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

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include <map>
#include <iostream>
#include <vector>

const double inch = 72;
const double cm = inch / 2.54;
const double mm = cm / 10;

struct PaperSize {
    double width;
    double height;
    std::string name;
    bool portrait;
};

const PaperSize paper_sizes[] = {
    {21.6, 27.9, "US letter", true},
    {84.1, 118.9, "A0", true},
    {59.4, 84.1, "A1", true},
    {42.0, 59.4, "A2", true},
    {29.7, 42.0, "A3", true},
    {21.0, 29.7, "A4", true},
    {14.8, 21.0, "A5", true},
    {10.5, 14.8, "A6", true},
    {7.4, 10.5, "A7", true},
    {5.2, 7.4, "A8", true},
    {3.7, 5.2, "A9", true},
    {2.6, 3.7, "A10", true},
};

struct Multipage {
    enum Alignment {
        Left,
        Right,
        Top,
        Bottom,
        Center
    };

    bool enabled;

    std::string name;

    double  page_width;
    double  page_height;

    int rows;
    int columns;

    int rotation;

    Alignment h_alignment;
    Alignment v_alignment;

    double margin_left;
    double margin_right;
    double margin_top;
    double margin_bottom;

    double spacing;
};

const Multipage multipage_defaults[] = {
    {
        false,
        "",
        0, 0,
        0, 0, 0,
        Multipage::Center, Multipage::Center,
        0, 0, 0, 0, 0
    },
    {
        true,
        "2x1, A4, 0°",
        21.0, 29.7,
        2, 1, 0,
        Multipage::Center, Multipage::Center,
        1, 1, 1, 1, 1
    },
    {
        true,
        "2x1, A4, 90°",
        21.0, 29.7,
        2, 1, 90,
        Multipage::Center, Multipage::Center,
        1, 1, 1, 1, 1
    },
    {
        true,
        "2x2, A4, 0°",
        21.0, 29.7,
        2, 2, 0,
        Multipage::Center, Multipage::Center,
        1, 1, 1, 1, 1
    },
    {
        true,
        "2x2, A4, 90°",
        21.0, 29.7,
        2, 2, 90,
        Multipage::Center, Multipage::Center,
        1, 1, 1, 1, 1
    }
};

struct FileConf {
    FileConf() :
        multipage_enabled(false),
        multipage(nullptr),
        rotation(0)
    {

    }
    std::string path;
    std::string ouput_pages;
    bool multipage_enabled;
    const Multipage *multipage;
    int rotation;
};

struct Conf {
    std::string output_path;

    std::vector<FileConf> files;
};

#endif // DEFINITIONS_H
