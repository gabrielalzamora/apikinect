/*
 * Copyright (c) 2016  Gabriel Alzamora
 * Copyright (c) 2015 The Qt Company Ltd
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("apikinect server");
    w.show();

    return a.exec();
}
