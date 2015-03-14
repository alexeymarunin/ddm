#ifndef DDM_SETTINGS_H
#define DDM_SETTINGS_H

#include <QSettings>

class ddmSettings : public QSettings
{
    Q_OBJECT

public:
    static ddmSettings* instance();

    virtual ~ddmSettings();

private:

    static ddmSettings* m_instance;

    ddmSettings( const QString& fileName, QObject* parent = 0 );
};

#endif // DDM_SETTINGS_H

