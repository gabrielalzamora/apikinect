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

  Store led, configuration parameters (srvKinect struct)
  and if you want to show infrared Kinect output
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
    irOption = 0;//ir off => video on
    srvK.m_fAngulo = 0.0;//not used
    srvK.m_iAnguloKinect = 0;//level
    srvK.m_fAltura = 0.56;//camera hight above ground level in meters
    srvK.m_fYMin = -1000;//minimum vertical high in mm to show (sign oposite to reference axis)
    srvK.m_fYMax = 2000;//maximum vertical level in mm to show (sign oposite to reference axis)
    srvK.m_fZMax = 10000;//maximum distance to show in mm
    srvK.m_ulRefresco3D = 50;//image refresh rate in milliseconds (22-2000)
    srvK.m_usModulo3D = 3;//points you read to send 1 (i.e. if 5 you send a sigle point each 5 you got)
    srvK.m_bEnvio3D = 0;
    srvK.m_bEnvio2D = 0;
    srvK.m_bEnvioBarrido = 1;
    srvK.m_bCompress3D = 0;//compression not available actually
    srvK.m_iBarridoEcu = 10000;//not used, = srvK.m_fZMax
    srvK.m_iBarridoYMin = -1000;//min vertical level to show on barrido (swept) > srvK.m_fYMin
    srvK.m_iBarridoYMax = 2000;//max vertical level to show on barrido (swept) < srvK.m_fYMax
    srvK.m_ulRefrescoDepth = 50;
    srvK.m_bEnvioDepth = 0;
    srvK.m_bCompressDepth = 0;//compression not available actually
    srvK.m_ulRefrescoColor = 50;
    srvK.m_bEnvioColor = 0;
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
        if(0<=option && option < 7)
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
 * \brief sets individual values to all members of srvKinect
 * \param newSrvK srvKinect to copy in ConfigData.srvK
 */
void ConfigData::setSrvK(srvKinect newSrvK)
{
    qDebug("ConfigData::setSrvK");
/*    qDebug("-----------------------------------");
    qDebug("m_fAngulo %f ponemos %f",srvK.m_fAngulo,newSrvK.m_fAngulo);
    qDebug("m_iAnguloKinect %d ponemos %d",srvK.m_iAnguloKinect,newSrvK.m_iAnguloKinect);
    qDebug("m_fAltura %f ponemos %f",srvK.m_fAltura,newSrvK.m_fAltura);
    qDebug("m_fYMin %f ponemos %f",srvK.m_fYMin,newSrvK.m_fYMin);
    qDebug("m_fYMax %f ponemos %f",srvK.m_fYMax,newSrvK.m_fYMax);
    qDebug("m_fZMax %f ponemos %f",srvK.m_fZMax,newSrvK.m_fZMax);
    qDebug("m_ulRefresco3D %lu ponemos %lu",srvK.m_ulRefresco3D,newSrvK.m_ulRefresco3D);
    qDebug("m_usModulo3D %u ponemos %u",srvK.m_usModulo3D,newSrvK.m_usModulo3D);
    qDebug("m_bEnvio3D %u ponemos %u",srvK.m_bEnvio3D,newSrvK.m_bEnvio3D);
    qDebug("m_bEnvio2D %u ponemos %u",srvK.m_bEnvio2D,newSrvK.m_bEnvio2D);
    qDebug("m_bEnvioBarrido %u ponemos %u",srvK.m_bEnvioBarrido,newSrvK.m_bEnvioBarrido);
    qDebug("m_bCompress3D %u ponemos %u",srvK.m_bCompress3D,newSrvK.m_bCompress3D);
    qDebug("m_iBarridoEcu %d ponemos %u",srvK.m_iBarridoEcu,newSrvK.m_iBarridoEcu);
    qDebug("m_iBarridoYMin %d ponemos %d",srvK.m_iBarridoYMin,newSrvK.m_iBarridoYMin);
    qDebug("m_iBarridoYMax %d ponemos %d",srvK.m_iBarridoYMax,newSrvK.m_iBarridoYMax);
    qDebug("m_ulRefrescoDepth %lu ponemos %lu",srvK.m_ulRefrescoDepth,newSrvK.m_ulRefrescoDepth);
    qDebug("m_bEnvioDepth %u ponemos %u",srvK.m_bEnvioDepth,newSrvK.m_bEnvioDepth);
    qDebug("m_bCompressDepth %u ponemos %u",srvK.m_bCompressDepth,newSrvK.m_bCompressDepth);
    qDebug("m_ulRefrescoColor %lu ponemos %lu",srvK.m_ulRefrescoColor,newSrvK.m_ulRefrescoColor);
    qDebug("m_bEnvioColor %u ponemos %u",srvK.m_bEnvioColor,newSrvK.m_bEnvioColor);
    qDebug("m_bCompressColor %u ponemos %u",srvK.m_bCompressColor,newSrvK.m_bCompressColor);
    qDebug("-----------------------------------");
*/
    srvK.m_fAngulo = newSrvK.m_fAngulo;
    if(-27 < newSrvK.m_iAnguloKinect && newSrvK.m_iAnguloKinect < 27){
        srvK.m_iAnguloKinect = newSrvK.m_iAnguloKinect;
    }else if(newSrvK.m_iAnguloKinect < -27){
        srvK.m_iAnguloKinect = -27;
    }else if(27 < newSrvK.m_iAnguloKinect){
        srvK.m_iAnguloKinect = 27;
    }
    srvK.m_fAltura = newSrvK.m_fAltura;
    srvK.m_fYMin = newSrvK.m_fYMin;
    srvK.m_fYMax = newSrvK.m_fYMax;
    srvK.m_fZMax = newSrvK.m_fZMax;
    if( 30 <= newSrvK.m_ulRefresco3D ){
        srvK.m_ulRefresco3D = newSrvK.m_ulRefresco3D;
    }else{
        srvK.m_ulRefresco3D = 30;
    }
    if(0 < newSrvK.m_usModulo3D && newSrvK.m_usModulo3D < 11){
        srvK.m_usModulo3D = newSrvK.m_usModulo3D;
    }else{
        srvK.m_usModulo3D = 3;
    }
    srvK.m_bEnvio3D = newSrvK.m_bEnvio3D;
    srvK.m_bEnvio2D = newSrvK.m_bEnvio2D;
    srvK.m_bEnvioBarrido = newSrvK.m_bEnvioBarrido;
    srvK.m_bCompress3D = newSrvK.m_bCompress3D;
    srvK.m_iBarridoEcu = newSrvK.m_iBarridoEcu;
    srvK.m_iBarridoYMin = newSrvK.m_iBarridoYMin;
    srvK.m_iBarridoYMax = newSrvK.m_iBarridoYMax;
    if( 30 <= newSrvK.m_ulRefrescoDepth ){
        srvK.m_ulRefrescoDepth = newSrvK.m_ulRefrescoDepth;
    }else{
        srvK.m_ulRefrescoDepth = 30;
    }
    srvK.m_bEnvioDepth = newSrvK.m_bEnvioDepth;
    srvK.m_bCompressDepth = newSrvK.m_bCompressDepth;
    if( 30 <= newSrvK.m_ulRefrescoColor ){
        srvK.m_ulRefrescoColor = newSrvK.m_ulRefrescoColor;
    }else{
        srvK.m_ulRefrescoColor = 30;
    }
    srvK.m_bEnvioColor = newSrvK.m_bEnvioColor;
    srvK.m_bCompressColor = newSrvK.m_bCompressColor;
}
/*!
 * \brief ConfigData::srvK is copied on newSrvK
 * \param newSrvK [OUT] where you get the copy
 */
void ConfigData::copySrvK(srvKinect *newSrvK)
{
    newSrvK->m_fAngulo =srvK.m_fAngulo;
    newSrvK->m_iAnguloKinect = srvK.m_iAnguloKinect;
    newSrvK->m_fAltura = srvK.m_fAltura;
    newSrvK->m_fYMin = srvK.m_fYMin;
    newSrvK->m_fYMax = srvK.m_fYMax;
    newSrvK->m_fZMax = srvK.m_fZMax;
    newSrvK->m_ulRefresco3D = srvK.m_ulRefresco3D;
    newSrvK->m_usModulo3D = srvK.m_usModulo3D;
    newSrvK->m_bEnvio3D = srvK.m_bEnvio3D;
    newSrvK->m_bEnvio2D = srvK.m_bEnvio2D;
    newSrvK->m_bEnvioBarrido = srvK.m_bEnvioBarrido;
    newSrvK->m_bCompress3D = srvK.m_bCompress3D;
    newSrvK->m_iBarridoEcu = srvK.m_iBarridoEcu;
    newSrvK->m_iBarridoYMin = srvK.m_iBarridoYMin;
    newSrvK->m_iBarridoYMax = srvK.m_iBarridoYMax;
    newSrvK->m_ulRefrescoDepth = srvK.m_ulRefrescoDepth;
    newSrvK->m_bEnvioDepth = srvK.m_bEnvioDepth;
    newSrvK->m_bCompressDepth = srvK.m_bCompressDepth;
    newSrvK->m_ulRefrescoColor = srvK.m_ulRefrescoColor;
    newSrvK->m_bEnvioColor = srvK.m_bEnvioColor;
    newSrvK->m_bCompressColor = srvK.m_bCompressColor;
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
 * \brief setter for irOption
 * \param value 0 means not checked, otherwise means it's checked
 */
void ConfigData::setIrOption(int value)
{
    if(value != irOption ){
        irOption = value;
        emit irOptionChanged(value);
    }
}/*!
 * \brief getter for irOption
 * \return return 0 if ir is not checked, 1 if checked
 */
int ConfigData::getIrOption() const
{
    return irOption;
}
