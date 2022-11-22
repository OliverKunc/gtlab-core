#include <QTextStream>

#include "gt_exceptions.h"


GTlabException::GTlabException(const QString& where, const QString& what) :
    m_where(where.toStdString()),
    m_what(what.toStdString())
{
}

QString
GTlabException::errMsg() const
{
    QString msg;
    QTextStream out(&msg);

    out << Qt::endl
        << QObject::tr("GTlab - EXCEPTION EMERGED") << Qt::endl
        << QStringLiteral("---------------------") << Qt::endl
        << QStringLiteral("IN:")  << QStringLiteral("\t") << where() << Qt::endl
        << QStringLiteral("MSG:") << QStringLiteral("\t") << what() << Qt::endl;

    return msg;
}

void
GTlabException::pushWhere(QString const& function, QString const& name)
{
    if (name.isEmpty())
    {
        m_where = QString("%1-->%2").arg(function, where()).toStdString();
    }
    else
    {
        m_where = QString("%1@%2-->%3").arg(function, name, where())
                      .toStdString();
    }
}

void
GTlabException::setWhere(QString const& where)
{
    m_where = where.toStdString();
}

QString
GTlabException::where() const
{
    return m_where.c_str();
}

const char *
GTlabException::what() const
{
    return m_what.c_str();
}
