#ifndef DDM_INFO_LOGGER_H
#define DDM_INFO_LOGGER_H

#include <QTextEdit>


class ddmInfoLogger: public QObject
{
    Q_OBJECT

public:

    static ddmInfoLogger& instance();

    ~ddmInfoLogger();

    void writeDebug( const QString& text );

    void writeInfo( const QString& text );

    void writeWarning( const QString& text );

    void writeError( const QString& text );

    QTextEdit* outputWindow() const;
    void setOutputWindow( QTextEdit* outputWindow );



private:

    static ddmInfoLogger* m_instance;

    explicit ddmInfoLogger();

    void writeLog( const QString& icon, const QString& text );

    QString getCurrentTime();

    QString getCurrentPathToIcon();

    void insertImage( const QString& fullFileName );

private:

    QTextEdit* m_outputWindow;

};

#endif // DDM_INFO_LOGGER_H
