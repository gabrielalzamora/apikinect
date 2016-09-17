/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include "configdata.h"

/*!
  \class ConfigData
  \brief A class to handle configuration data

  Used to show parámeters and allow input actions from user
  Used to allow update of server data with client data
 */

/*!
 * \brief ConfigData::ConfigData
 * constructor that show all buttons, sliders... and \
 * connect changes on them to data update functions
 * \param parent
 * pointer to QWidget parent of ConfigData
 */
ConfigData::ConfigData(QObject *parent) : QObject(parent)
{
    ledOption = 0;//led off
    srvK.m_fAngulo = 0.0;//not used
    srvK.m_iAnguloKinect = 0;//level
    srvK.m_fAltura = 0.56;//camera hight above ground level in meters
    srvK.m_fYMin = -1000;//minimum vertical high in mm to show (sign oposite to reference axis)
    srvK.m_fYMax = 2000;//maximum vertical level in mm to show (sign oposite to reference axis)
    srvK.m_fZMax = 10000;//maximum distance to show in mm
    srvK.m_ulRefresco3D = 33;//image refresh rate in milliseconds (22-2000)
    srvK.m_usModulo3D = 1;//points you read to send 1 (i.e. if 5 you send a sigle point each 5 you got)
    srvK.m_bEnvio3D = 1;
    srvK.m_bEnvio2D = 1;
    srvK.m_bEnvioBarrido = 1;
    srvK.m_bCompress3D = 0;//compression not available actually
    srvK.m_iBarridoEcu = 10000;//not used, = srvK.m_fZMax
    srvK.m_iBarridoYMin = -1000;//min vertical level to show on barrido (swept) > srvK.m_fYMin
    srvK.m_iBarridoYMax = 2000;//max vertical level to show on barrido (swept) < srvK.m_fYMax
    srvK.m_ulRefrescoDepth = 33;
    srvK.m_bEnvioDepth = 1;
    srvK.m_bCompressDepth = 0;//compression not available actually
    srvK.m_ulRefrescoColor = 33;
    srvK.m_bEnvioColor = 1;
    srvK.m_bCompressColor = 0;//compression not available actually
}

/*!
 * \brief ConfigData::setLedOption
 * to set led light on kinect
 * \param integer holding freenect_led_options value to set led light
 */
void ConfigData::setLedOption(int option)
{
    if(option != ledOption){
        ledOption = option;
        emit ledOptionChanged(option);
    }
}
/*!
 * \brief ConfigData::getLedOption
 * \return int value of freenect_led_options
 */
int ConfigData::getLedOption()
{
    return ledOption;
}
/*!
 * \brief ConfigData::setSrvK
 * sets individual values to all members of srvKinect
 * \param newSrvK   srvKinect to copy in ConfigData.srvK
 */
void ConfigData::setSrvK(srvKinect newSrvK)
{
    srvK.m_fAngulo = newSrvK.m_fAngulo;
    srvK.m_fAngulo = newSrvK.m_fAngulo;
    srvK.m_iAnguloKinect = newSrvK.m_iAnguloKinect;
    srvK.m_fAltura = newSrvK.m_fAltura;
    srvK.m_fYMin = newSrvK.m_fYMin;
    srvK.m_fYMax = newSrvK.m_fYMax;
    srvK.m_fZMax = newSrvK.m_fZMax;
    srvK.m_ulRefresco3D = newSrvK.m_ulRefresco3D;
    srvK.m_usModulo3D = newSrvK.m_usModulo3D;
    srvK.m_bEnvio3D = newSrvK.m_bEnvio3D;
    srvK.m_bEnvio2D = newSrvK.m_bEnvio2D;
    srvK.m_bEnvioBarrido = newSrvK.m_bEnvioBarrido;
    srvK.m_bCompress3D = newSrvK.m_bCompress3D;
    srvK.m_iBarridoEcu = newSrvK.m_iBarridoEcu;
    srvK.m_iBarridoYMin = newSrvK.m_iBarridoYMin;
    srvK.m_iBarridoYMax = newSrvK.m_iBarridoYMax;
    srvK.m_ulRefrescoDepth = newSrvK.m_ulRefrescoDepth;
    srvK.m_bEnvioDepth = newSrvK.m_bEnvioDepth;
    srvK.m_bCompressDepth = newSrvK.m_bCompressDepth;
    srvK.m_ulRefrescoColor = newSrvK.m_ulRefrescoColor;
    srvK.m_bEnvioColor = newSrvK.m_bEnvioColor;
    srvK.m_bCompressColor = newSrvK.m_bCompressColor;
    emit srvKChanged(newSrvK);
}
/*!
 * \brief ConfigData::getSrvK
 * \return ConfigData.srvK
 */
srvKinect ConfigData::getSrvK()
{
    return srvK;
}
/*!
 * \brief getter for irOption
 * \return return 0 if ir is not checked, 1 if checked
 */
int ConfigData::getIrOption() const
{
    return irOption;
}
/*!
 * \brief setter for irOption
 * \param value 0 means not checked, otherwise means it's checked
 */
void ConfigData::setIrOption(int value)
{
    irOption = value;
}


