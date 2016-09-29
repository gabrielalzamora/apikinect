/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef DATOS_H
#define DATOS_H

#include <QObject>
#include "typekinect.h"

class ConfigData : public QObject
{
    Q_OBJECT
public:
    explicit ConfigData(QObject *parent = 0);
    srvKinect srvK;
    int ledOption;
    int irOption;

signals:
    void srvKChanged(srvKinect newSrvK);
    void ledOptionChanged(int option);
    void irOptionChanged(int value);
    void configDataChanged();

public slots:
    int getIrOption() const;
    void setIrOption(int value);
    int getLedOption();
    void setLedOption(int option);
    srvKinect getSrvK();
    void setSrvK(srvKinect newSrvK);

private:

};

#endif // DATOS_H
