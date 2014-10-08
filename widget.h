#ifndef widget_H
#define widget_H

#include <QtGui/QClipboard>
#include <QtMultimedia/QtMultimedia>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QtWidgets/QtWidgets>

#include "ui_widget.h"
#include "ui_webcamselect.h"

class widget : public QWidget
{
    Q_OBJECT

public:
    widget();
    ~widget();
    
public slots:
    void capture( bool checked = false );
    
    private:
    Ui::Widget ui;
    Ui::webcamselect select_ui;

    static QByteArray m_defaultDevice;
    QDialog *m_selectDialog;

    QPointer< QCamera > m_camera;
    QPointer< QCameraImageCapture > m_imageCapture;

    QTimer *m_timer;

    int m_timerPaintState;
};
#endif // widget_H
