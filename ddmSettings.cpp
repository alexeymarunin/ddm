#include <QDebug.h>
#include <QApplication.h>
#include <QFileInfo>
#include "ddmSettings.h"

ddmSettings* ddmSettings::m_instance = NULL;

ddmSettings* ddmSettings::instance()
{
    if ( !ddmSettings::m_instance )
    {
        QFileInfo appFileInfo( QApplication::applicationFilePath() );
        QString appIniFile = QApplication::applicationDirPath() + "/" + appFileInfo.baseName() + ".ini";
        qDebug() << "Setting:" << appIniFile;
        ddmSettings::m_instance = new ddmSettings( appIniFile, QApplication::instance() );
    }
    return ddmSettings::m_instance;
}

ddmSettings::ddmSettings( const QString& fileName, QObject* parent ) : QSettings ( fileName, QSettings::IniFormat, parent )
{
    this->setIniCodec( "UTF-8" );
}

ddmSettings::~ddmSettings()
{
}
