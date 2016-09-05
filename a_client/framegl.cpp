/*
 * Copyright (c) 2016  Gabriel Alzamora.
 * Copyright (c) 2016  Raúl Alves.
 * Copyright (c) 2010 individual OpenKinect contributors.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include <QMouseEvent>
#include <QWheelEvent>
#include "framegl.h"

/*!
 * \class FrameGL
 * \brief paint 3D point cloud on QOpenGLWidget.
 *
 * QOpenGLWidget should inherit from this class to allow
 * mouse interaction with plotted cloud turning the image
 * as you drag over it.
 */

/*!
 * \brief FrameGL::FrameGL
 * \param parent
 */
FrameGL::FrameGL(QWidget *parent) : QOpenGLWidget(parent)
{
//    xRot = yRot = zRot = 0;
    anglex = angley = 0;
    zoom = 1;
    pcloud_3d.resize(0);
    pcloud_2d.resize(0);
}
/*!
 * \brief FrameGL::~FrameGL
 */
FrameGL::~FrameGL()
{
}
/*!
 * \brief FrameGL::minimumSizeHint
 * QT auxiliari function to keep item size when resizing
 * \return QSize minimum size to shrink item
 */
QSize FrameGL::minimumSizeHint() const
{
    return QSize(320,240);
}
/*!
 * \brief FrameGL::sizeHint
 * QT auxiliari function to keep item size when resizing
 * \return QSize  ideal size to show item
 */
QSize FrameGL::sizeHint() const
{
    return QSize(320,240);
}

/*
void FrameGL::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}*/

/*!
 * \brief FrameGL::setClearColor set color to repaint
 * \param color
 */
void FrameGL::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

/*!
 * \brief save incomming point cloud into pcloud_3d
 *
 * Overloaded function that allow to save 3d and 2d point cloud
 * \param [in] c 3d+color point cloud to save
 */
void FrameGL::setCloud(std::vector<point3c> c)
{
    //std::lock_guard<std::mutex> lock(mtxCloud);//actually not necesary, its the only fuction writting on pCloud
    pcloud_3d=c;
}
/*!
 * \brief save incomming point cloud into pcloud_2d
 *
 * Overloaded function that allow to save 3d and 2d point cloud
 * \param [in] d 2d point cloud to save
 */
void FrameGL::setCloud(std::vector<point2> d)
{
    pcloud_2d=d;
}
/*!
 * \brief Set geometry of GL view
 */
void FrameGL::initializeGL()
{
    glClearColor(0.8313f, 0.9450f, 0.9607f, 1.0f );
    glEnable(GL_DEPTH_TEST);
    GLfloat fAspect, fNearPlane, fFarPlane;
    glClearDepth(1.0f);
    fAspect = 1.0f;
    fNearPlane = 900.0f;
    fFarPlane = 11000.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, fAspect, fNearPlane, fFarPlane);
    //gluPerspective(50.0,1.0,900.0,11000.0);
    glMatrixMode(GL_MODELVIEW);
}
/*!
 * \brief where points are draw and point of view set.
 */
void FrameGL::paintGL()
{
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Se seleeciona el punto de vista desde donde se mira.
    //float ptoVista[3]={-50.f,-100.f,-1000.f};

    // Define una transformacion de vision.Punto de vista XYZ, centro de la
    // escena donde se mira XYZ y vector de direccion ascendente
    //gluLookAt(ptoVista[0],ptoVista[1],ptoVista[2],0.0,0.0,5000.0,0.0,-1.0,0.0);
    gluLookAt(-7*anglex, -7*angley-1000,-3000.0,    0.0,-1000.0,3000.0,   0.0,-1.0,0.0);
    glScalef(zoom, zoom, 1);

    //glRotatef(m_x,1.0f, 0.0f, 0.0f); // Rotacion X (ángulo, vector alrededor del que giras)
    //glRotatef(m_y,0.0f, 1.0f, 0.0f); // Rotacion Y

    drawAxis();
    drawCloud();
    //drawLines();

    glPopMatrix();
}
/*!
 * \brief Adapt GL window to new size
 * \param width
 * \param height
 */
void FrameGL::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}
/*!
 * \brief FrameGL::mousePressEvent
 * \param event
 */
void FrameGL::mousePressEvent(QMouseEvent *event)
{
    lastPosition = event->pos();
}
/*!
 * \brief to change point of view of 3D image with drag&drop
 * \param event
 */
void FrameGL::mouseMoveEvent(QMouseEvent *event)
{
    if (lastPosition.x()>=0 && lastPosition.y()>=0 ) {
        anglex += event->x() - lastPosition.x();
        angley += event->y() - lastPosition.y();
    }
    lastPosition = event->pos();
}
/*!
 * \brief if wheel rotate zooms FrameGL
 * \param event
 */
void FrameGL::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta();
    if( numDegrees < 0 ){
        zoom /= 1.1f;
    }else{
        zoom *= 1.1f;
    }
    event->accept();
}
/*!
 * \brief to shot drag&drop signal to calculate move event
 * \param event
 */
void FrameGL::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked();
}
/*!
 * \brief draw points cloud
 *
 * Draw 3d+color if pcloud_3d has data (size>0) and
 * draw 2d in red color at heigh=0 if pcloud_2d >0
 */
void FrameGL::drawCloud()
{
    glPointSize(1.0f);
    glBegin(GL_POINTS);
        if(pcloud_3d.size()>0){
            for(int i = 0; i < pcloud_3d.size(); i++){
                glColor3b(pcloud_3d[i].color.rgbRed,pcloud_3d[i].color.rgbGreen,pcloud_3d[i].color.rgbBlue);
                glVertex3s(pcloud_3d[i].x,pcloud_3d[i].y,pcloud_3d[i].z);
            }
        }

        glColor3ub(0,255,0);
        if(pcloud_2d.size()>0){
            for(int i = 0; i < pcloud_2d.size(); i++){
                glVertex3s(pcloud_2d[i].x,0,pcloud_2d[i].z);
            }
        }
    glEnd();
    pcloud_3d.resize(0);
    pcloud_2d.resize(0);
}
/*!
 * \brief Ox red, Oy green and Oz blue
 */
void FrameGL::drawAxis()
{
    //draw the world coordinate frame
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glColor3ub(255, 0, 0);  // X-axis red
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(500.0,0.0,0.0);
        glColor3ub(0, 255, 0);  // Y-axis green
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,500.0,0.0);
        glColor3ub(0, 0, 255);  // Z-axis blue
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,500.0);
    glEnd();

    glLineWidth(0.5f);
    glBegin(GL_LINES);
        for (int i=-11; i<12;i++)
        {
            glColor3ub(255, 0, 0);// X parallel
            glVertex3f(-13000, 0.0, 1000*i);
            glVertex3f( 13000, 0.0, 1000*i);
            glColor3ub(0,0,255);// Z parallel
            glVertex3f(1000*i, 0.0,-13000);
            glVertex3f(1000*i, 0.0, 13000);
        }
    glEnd();
}
/*!
 * \brief draw auxiliary lines paralel to axis
 */
void FrameGL::drawLines()
{
    glLineWidth(1.0f);

        glColor3f(1.0f, 0.0f, 0.0f );// x red
        glBegin(GL_LINES);
            glVertex3f(-40000.0,0.0,0.0);
            glVertex3f(40000.0,0.0,0.0);
        glEnd();

        glColor3f(0.0f, 1.0f, 0.0f );// y green
        glBegin(GL_LINES);
            glVertex3f(0.0,-40000.0,0.0);
            glVertex3f(0.0,40000.0,0.0);
        glEnd();

        glColor3f(0.0f, 0.0f, 1.0f );// z blue
        glBegin(GL_LINES);
            glVertex3f(0.0,0.0,-40000.0);
            glVertex3f(0.0,0.0,40000.0);
        glEnd();

        glColor3f(0.0f, 0.75f, 0.75f );
        glBegin(GL_LINES);
            for (int i=0; i<24;i++)
            {
                if (i%4==0)
                    glColor3f(0.0f, 0.5f, 0.5f );
                else
                    glColor3f(0.0f, 0.75f, 0.75f );
                glVertex3f(2500*i,0.0,-40000);
                glVertex3f(2500*i,0.0,40000);
            }
            for (int i=0; i<24;i++)
            {
                if (i%4==0)
                    glColor3f(0.0f, 0.5f, 0.5f );
                else
                    glColor3f(0.0f, 0.75f, 0.75f );
                glVertex3f(-2500*i,0.0,-40000);
                glVertex3f(-2500*i,0.0,40000);
            }
        glEnd();

        glColor3f(0.0f, 0.75f, 0.75f );
        glBegin(GL_LINES);
            for (int i=0; i<24;i++)
            {
                if (i%4==0)
                    glColor3f(0.0f, 0.5f, 0.5f );
                else
                    glColor3f(0.0f, 0.75f, 0.75f );
                glVertex3f(-40000,0.0,2500*i);
                glVertex3f(40000,0.0,2500*i);
            }
            for (int i=0; i<24;i++)
            {
                if (i%4==0)
                    glColor3f(0.0f, 0.5f, 0.5f );
                else
                    glColor3f(0.0f, 0.75f, 0.75f );
                glVertex3f(-40000,0.0,-2500*i);
                glVertex3f(40000,0.0,-2500*i);
            }
        glEnd();
}
