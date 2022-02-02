/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 01.09.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTCODEEDITOR_H
#define GTCODEEDITOR_H

#include "gt_gui_exports.h"

#include <QPlainTextEdit>

class GT_GUI_EXPORT GtCodeEditor: public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit GtCodeEditor(QWidget* parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent* event);

    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent* event);

    virtual void dragEnterEvent(QDragEnterEvent* event);

    virtual void dragMoveEvent(QDragMoveEvent* event);

    virtual void dropEvent(QDropEvent* event);

    virtual void wheelEvent(QWheelEvent* event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);

    void highlightCurrentLine();

    void updateLineNumberArea(const QRect&, int);

private:
    QWidget* m_lineNumberArea;

};

#endif // GTCODEEDITOR_H
