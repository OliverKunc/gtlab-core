/* GTlab - Gas Turbine laboratory
 * Source File: gt_palette.cpp
 * copyright 2009-2020 by DLR
 *
 *  Created on: 06.11.2020
 *  Author: Jens Schmeink (AT-TW)
 *  Tel.: +49 2203 601 2191
 */

#include "gt_palette.h"

#include <QSettings>
#include <QWidget>
#include <QStyleFactory>

QPalette
GtPalette::darkTheme()
{
    QPalette palette;
    QColor darkColor = QColor(45,45,45);
    QColor disabledColor = QColor(127,127,127);
    palette.setColor(QPalette::Window, darkColor);
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(18,18,18));
    palette.setColor(QPalette::AlternateBase, darkColor);
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::Text,
                     disabledColor);
    palette.setColor(QPalette::Button, darkColor);
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,
                     disabledColor);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));

    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                     disabledColor);
    return palette;

}

QPalette
GtPalette::standardTheme()
{
    QPalette palette;

    QColor mainColor = QColor(245, 245, 245);
    QColor disabledColor = QColor(127, 127, 127);

    palette.setColor(QPalette::ColorRole::Background, Qt::white);

    palette.setColor(QPalette::Window, mainColor);
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::AlternateBase, mainColor);
    palette.setColor(QPalette::ToolTipBase, Qt::black);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    palette.setColor(QPalette::Button, mainColor);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                     disabledColor);

    return palette;
}

void
GtPalette::applyThemeToWidget(QWidget* w, bool dark)
{
    if (dark)
    {
        w->setPalette(GtPalette::darkTheme());
        w->setStyle(QStyleFactory::create("Fusion"));
        w->setStyleSheet("QToolTip { color: #ffffff; "
                      "background-color: #2a82da; "
                      "border: 1px solid white; }");
    }
    else
    {
        w->setPalette(GtPalette::standardTheme());
        QString style = "Default";
#ifdef Q_OS_WIN
        style = "windowsvista";
#endif
        w->setStyle(QStyleFactory::create(style));
        w->setStyleSheet("QToolTip { color: black; "
                      "background-color: white; "
                      "border: 1px solid black; }");
    }
}
