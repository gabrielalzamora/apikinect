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

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QLayout>
#include "typekinect.h"

class Data : public QWidget
{
    Q_OBJECT
public:
    explicit Data(QWidget *parent = 0);
    srvKinect m_srvK;
    int ledOption;

    void setLimitsLineEAngulo(double value);
    void setLimitsLineEAngK(int8_t value);
    void setLimitsLineEAltura(double value);
    void setLimitsLineEYmin(double value);
    void setLimitsLineEYmax(double value);
    void setLimitsLineEZmax(double value);
    void setPointsSlider(uint32_t value);
    void setPointsSliderM(uint8_t value);
    void setPointsCBenvio3D(uint8_t value);
    void setPointsCBenvio2(uint8_t value);
    void setPointsCBenvioB(uint8_t value);
    void setPointsCBcomprimido(uint8_t value);
    void setPointsLineEEcu(int32_t value);
    void setPointsLineEYmin(int32_t value);
    void setPointsLineEYmax(int32_t value);
    void setDepthSlider(uint32_t value);
    void setDepthCBenvio(uint8_t value);
    void setDepthCBcomprimido(uint8_t value);
    void setVideoSlider(uint32_t value);
    void setVideoCBenvio(uint8_t value);
    void setVideoCBcomprimido(uint8_t value);

signals:
    void ledOptionChanged();
    void dataChanged();
    void srvKChanged(srvKinect newSrvK);

public slots:
    void setLedOption(int option);
    int getLedOption();
    void setData();
    void setSrvK(srvKinect newSrvK);
    srvKinect getSrvK();

private:
//  tab_2 data
    QLineEdit *LimitsLineEAngulo;
    QLineEdit *LimitsLineEAngK;
    QLineEdit *LimitsLineEAltura;
    QLineEdit *LimitsLineEYmin;
    QLineEdit *LimitsLineEYmax;
    QLineEdit *LimitsLineEZmax;
    QSlider *PointsSlider;
    QLabel *PointsLabelSlider;
    QSlider *PointsSliderM;
    QLabel *PointsLabelSliderM;
    QCheckBox *PointsCBenvio3D;
    QCheckBox *PointsCBenvio2;
    QCheckBox *PointsCBenvioB;
    QCheckBox *PointsCBcomprimido;
    QLineEdit *PointsLineEEcu;
    QLineEdit *PointsLineEYmin;
    QLineEdit *PointsLineEYmax;
    QSlider *DepthSlider;
    QLabel *DepthLabelSlider;
    QCheckBox *DepthCBenvio;
    QCheckBox *DepthCBcomprimido;
    QSlider *VideoSlider;
    QLabel *VideoLabelSlider;
    QCheckBox *VideoCBenvio;
    QCheckBox *VideoCBcomprimido;
    QButtonGroup *ledGroup;
};

#endif // DATOS_H
