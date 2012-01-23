//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// The code in this file is largely based on KDE's KLineEdit class
// as included in KDE 4.5. See there for its authors:
// http://api.kde.org/4.x-api/kdelibs-apidocs/kdeui/html/klineedit_8cpp.html
//
// Copyright 2010      Dennis Nienhüser <earthwings@gentoo.org>
//

#ifndef MARBLE_ROUTINGLINEEDIT_H
#define MARBLE_ROUTINGLINEEDIT_H

#include <QtGui/QLineEdit>

namespace Marble
{

class RoutingLineEditPrivate;

/**
  * A QLineEdit with an embedded clear button. The clear button removes any input
  * in the line edit when clicked with the left mouse button. It replaces the line
  * edit input with the current clipboard content on a middle mouse button click.
  */
class RoutingLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    /** Constructor */
    RoutingLineEdit( QWidget *parent = 0 );

    /** Destructor */
    ~RoutingLineEdit();

Q_SIGNALS:
    /** The clear button was clicked with the left mouse button */
    void clearButtonClicked();

protected:
    virtual void mouseReleaseEvent( QMouseEvent* event );

    virtual void resizeEvent( QResizeEvent* event );

private Q_SLOTS:
    void updateClearButtonIcon( const QString& text );

    void updateClearButton();

private:
    RoutingLineEditPrivate* const d;
};

} // namespace Marble

#endif // MARBLE_ROUTINGLINEEDIT_H