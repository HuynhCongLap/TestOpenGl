/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "logo.h"
#include <qmath.h>

void parseObjFile(const QString& fileName,
    QStringList& comments,
    QVector<QOpenGLTriangle3D>& triangles)
{
    comments.clear();
    triangles.clear();

    QFile file(fileName);
    if (file.exists())
    {
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QVector<QVector3D> v, vn;
            QVector<QVector2D> vt;

            while (!file.atEnd())
            {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if (lineParts.count() > 0)
                {

                    // if it's a comment
                    if (lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
                    {
                        comments.append(line.remove(0, 1).trimmed());
                    }

                    // if it's a vertex position (v)
                    else if (lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0)
                    {
                        v.append(QVector3D(lineParts.at(1).toFloat(),
                            lineParts.at(2).toFloat(),
                            lineParts.at(3).toFloat()));
                    }

                    // if it's a normal (vn)
                    else if (lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
                    {
                        vn.append(QVector3D(lineParts.at(1).toFloat(),
                            lineParts.at(2).toFloat(),
                            lineParts.at(3).toFloat()));
                    }

                    // if it's a texture (vt)
                    else if (lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
                    {
                        vt.append(QVector2D(lineParts.at(1).toFloat(),
                            lineParts.at(2).toFloat()));
                    }

                    // if it's face data (f)
                    // there's an assumption here that faces are all triangles
                    else if (lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0)
                    {
                        QOpenGLTriangle3D triangle;

                        // get points from v array
                        triangle.p1 = v.at(lineParts.at(1).split("/").at(0).toInt() - 1);
                        triangle.p2 = v.at(lineParts.at(2).split("/").at(0).toInt() - 1);
                        triangle.p3 = v.at(lineParts.at(3).split("/").at(0).toInt() - 1);

                        if (vt.count() > 0) // check if really there are any UV coords
                        {
                            triangle.p1UV = vt.at(lineParts.at(1).split("/").at(1).toInt() - 1);
                            triangle.p2UV = vt.at(lineParts.at(2).split("/").at(1).toInt() - 1);
                            triangle.p3UV = vt.at(lineParts.at(3).split("/").at(1).toInt() - 1);
                        }

                        // get normals from vn array
                        triangle.p1Normal = vn.at(lineParts.at(1).split("/").at(2).toInt() - 1);
                        triangle.p2Normal = vn.at(lineParts.at(2).split("/").at(2).toInt() - 1);
                        triangle.p3Normal = vn.at(lineParts.at(3).split("/").at(2).toInt() - 1);

                        triangles.append(triangle);
                    }

                }
            }

            file.close();
        }
    }
}


Logo::Logo()
    : m_count(0)
{
#ifndef MODDED
    qDebug("Log Modded ");

    qDebug("***********************");
    QString fEmp = QCoreApplication::applicationDirPath() + "/Cap2.obj";
    parseObjFile(fEmp,comments, triangles);
    qDebug() << fEmp;
    qDebug() << comments.size();
    qDebug() << triangles.size();
    m_data.resize( triangles.size()*3 * 6);
    for(int i=0; i<triangles.size(); i++)
    {
          add(triangles[i].p1, triangles[i].p1Normal);
          add(triangles[i].p2, triangles[i].p2Normal);
          add(triangles[i].p3, triangles[i].p3Normal);
    }
    qDebug("***********************");

#else
    m_data.resize(12 * 6);

    float d = 0.3f;
    quad(-d, -d, d, -d, d, d, -d, d);
#endif
}

void Logo::add(const QVector3D &v, const QVector3D &n)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_count += 6;
}

void Logo::quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4)
{
    QVector3D n = QVector3D::normal(QVector3D(x4 - x1, y4 - y1, 0.0f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, -0.05f), n);
    add(QVector3D(x4, y4, -0.05f), n);
    add(QVector3D(x2, y2, -0.05f), n);

    add(QVector3D(x3, y3, -0.05f), n);
    add(QVector3D(x2, y2, -0.05f), n);
    add(QVector3D(x4, y4, -0.05f), n);

    n = QVector3D::normal(QVector3D(x1 - x4, y1 - y4, 0.0f), QVector3D(x2 - x4, y2 - y4, 0.0f));

    add(QVector3D(x4, y4, 0.05f), n);
    add(QVector3D(x1, y1, 0.05f), n);
    add(QVector3D(x2, y2, 0.05f), n);

    add(QVector3D(x2, y2, 0.05f), n);
    add(QVector3D(x3, y3, 0.05f), n);
    add(QVector3D(x4, y4, 0.05f), n);
}

void Logo::extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    QVector3D n = QVector3D::normal(QVector3D(0.0f, 0.0f, -0.1f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, +0.05f), n);
    add(QVector3D(x1, y1, -0.05f), n);
    add(QVector3D(x2, y2, +0.05f), n);

    add(QVector3D(x2, y2, -0.05f), n);
    add(QVector3D(x2, y2, +0.05f), n);
    add(QVector3D(x1, y1, -0.05f), n);
}
