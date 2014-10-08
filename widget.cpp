#include "widget.h"
#include "ui_widget.h"

widget::~widget()
{
    if ( m_camera )
    {
        m_camera->stop();
        m_camera->deleteLater();
    }

    if ( m_imageCapture )
        m_imageCapture->deleteLater();
}

void widget::capture( bool )
{
    m_camera->searchAndLock();
    m_imageCapture->capture( QCoreApplication::applicationDirPath() + "/image.jpg" );
    m_camera->unlock();

    ui.captureButton->setEnabled( true );
    ui.timerButton->setEnabled( true );
}

QByteArray widget::m_defaultDevice = QByteArray();

widget::widget()
{
    ui.setupUi( this );

    QList< QByteArray > cams = QCamera::availableDevices();
    if ( !cams.contains( m_defaultDevice ) )
    {
        if ( cams.count() == 0 )
        {
            QMessageBox::critical( this, "Error", "Web Cams are not found!" );

            deleteLater();
            return;
        }
        else if ( cams.count() == 1 )
        {
            m_defaultDevice = cams.at( 0 );
        }
        else
        {
            m_selectDialog = new QDialog();
            m_selectDialog->setAttribute( Qt::WA_DeleteOnClose );
            m_selectDialog->setAttribute( Qt::WA_QuitOnClose, false );
            m_selectDialog->setWindowFlags( m_selectDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint );

            select_ui.setupUi( m_selectDialog );

            foreach( QByteArray webCam, cams )
            {
                auto commandLinkButton = new QCommandLinkButton( QCamera::deviceDescription( webCam ) );
                commandLinkButton->setProperty( "webCam", webCam );

                connect( commandLinkButton, &QCommandLinkButton::clicked, [=]( bool )
                    {
                        m_defaultDevice = commandLinkButton->property( "webCam" ).toByteArray();
                        m_selectDialog->accept();
                    }
                );

                select_ui.verticalLayout->addWidget( commandLinkButton );
            }

            if ( m_selectDialog->exec() == QDialog::Rejected )
            {
                deleteLater();
                return;
            }
        }
    }


m_camera = new QCamera( m_defaultDevice );
connect( m_camera, SIGNAL( error( QCamera::Error ) ), this, SLOT( cameraError( QCamera::Error ) ) );
connect( m_camera, SIGNAL( stateChanged( QCamera::State ) ), this, SLOT ( cameraStateChanged( QCamera::State ) ) );

auto viewfinder = new QCameraViewfinder;
viewfinder->setMinimumSize( 50, 50 );

m_camera->setViewfinder( viewfinder );
m_camera->setCaptureMode( QCamera::CaptureStillImage );

auto timerLabel = new QLabel;
QString timerLabelTpl = "<p align=\"center\"><span style=\"font-size:50pt; font-weight:600; color:#FF0000;\">%1</span></p>";

ui.gridLayout_7->addWidget( viewfinder, 0, 0 );
ui.gridLayout_7->addWidget( timerLabel, 0, 0 );

m_timerPaintState = 0;

m_timer = new QTimer( this );
m_timer->setInterval( 1000 );

m_camera->start();

m_imageCapture = new QCameraImageCapture( m_camera );
//m_imageCapture->setCaptureDestination( QCameraImageCapture::CaptureToBuffer );
m_imageCapture->setCaptureDestination( QCameraImageCapture::CaptureToFile );

connect( m_timer, &QTimer::timeout, [=]()
    {
        m_timerPaintState--;

        if ( m_timerPaintState )
        {
            timerLabel->setText( timerLabelTpl.arg( QString::number( m_timerPaintState ) ) );
        }
        else
        {
            m_timer->stop();
            timerLabel->hide();

            capture();
        }
    }
);

connect( ui.copyButton, &QPushButton::clicked, [=]( bool )
    {
        QApplication::clipboard()->setImage( m_pixmap.toImage() );
    }
);
}
