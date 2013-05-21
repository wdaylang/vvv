// (c) by Stefan Roettger, licensed under GPL 2+

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "volren_qgl.h"

class QTV3MainWindow: public QMainWindow
{
   Q_OBJECT; // Qt Metacall object for signal/slot connections

public:

   //! default ctor
   QTV3MainWindow(QWidget *parent = 0);

   //! dtor
   ~QTV3MainWindow();

   //! return preferred minimum window size
   QSize minimumSizeHint() const;

   //! return preferred window size
   QSize sizeHint() const;

   //! load a volume
   void loadvolume(const char *filename);

   //! load a DICOM series
   void loadseries(const std::vector<std::string> list);

   //! set volume rotation speed
   void setrotation(double omega);

private:

   QVBoxLayout *layout_;
   QGLVolRenWidget *vrw_;
   QLabel *label_;

   void createMenus();
   void createWidgets();

protected:

   void dragEnterEvent(QDragEnterEvent *event);
   void dragMoveEvent(QDragMoveEvent *event);
   void dragLeaveEvent(QDragLeaveEvent *event);

public:

   void dropEvent(QDropEvent *event);

protected slots:

   void about();
};

#endif