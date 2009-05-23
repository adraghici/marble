//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008      Torsten Rahn   <rahn@kde.org>
// Copyright 2008      Inge Wallin    <inge@lysator.liu.se>"
//


#include "RenderPlugin.h"
#include "MarbleDataFacade.h"

#include <QtGui/QAction>
#include <QtCore/QDebug>
#include <QtGui/QStandardItem>


namespace Marble
{

class RenderPluginPrivate
{
  public:
    RenderPluginPrivate()
        : m_dataFacade(0),
          m_action(0),
          m_item(0),
          m_enabled(true),
          m_visible(true)
    {
    }

    ~RenderPluginPrivate()
    {
    }

    MarbleDataFacade   *m_dataFacade;
    QAction            *m_action;
    QStandardItem      *m_item;

    bool                m_enabled;
    bool                m_visible;
};


RenderPlugin::RenderPlugin()
    : d( new RenderPluginPrivate() )
{
    d->m_action = new QAction( this );
    connect( d->m_action, SIGNAL( toggled( bool ) ),
	     this,        SLOT( setVisible( bool ) ) );

    d->m_item = new QStandardItem();
}

RenderPlugin::~RenderPlugin()
{
    delete d->m_item;
    delete d;
}

MarbleDataFacade* RenderPlugin::dataFacade() const
{
    return d->m_dataFacade;
}

void RenderPlugin::setDataFacade( MarbleDataFacade* dataFacade )
{
    d->m_dataFacade = dataFacade;
}

QAction* RenderPlugin::action() const
{
    d->m_action->setCheckable( true );
    d->m_action->setChecked( visible() );
    d->m_action->setIcon( icon() );
    d->m_action->setText( guiString() );
    d->m_action->setToolTip( description() );
    return d->m_action;
}

QStandardItem* RenderPlugin::item() const
{
    d->m_item->setIcon( icon() );
    d->m_item->setText( name() );
    d->m_item->setEditable( false );
    d->m_item->setCheckable( true );
    d->m_item->setCheckState( enabled() ?  Qt::Checked : Qt::Unchecked  );

    d->m_item->setToolTip( description() );
 

    return d->m_item;
}

void RenderPlugin::applyItemState()
{
    d->m_enabled = ( d->m_item->checkState() == Qt::Checked );

    d->m_action->setVisible( d->m_enabled );
}

void RenderPlugin::retrieveItemState()
{
    d->m_item->setCheckState( enabled() ?  Qt::Checked : Qt::Unchecked  );
}

void RenderPlugin::setEnabled( bool enabled )
{
    if ( enabled == d->m_enabled )
        return;

    d->m_enabled = enabled;
    d->m_action->setVisible( enabled );

    d->m_item->setCheckState( enabled ?  Qt::Checked : Qt::Unchecked  );
}

void RenderPlugin::setVisible( bool visible )
{
    if ( visible == d->m_visible )
        return;

    d->m_visible = visible;
    d->m_action->setChecked( visible );

    emit valueChanged( nameId(), visible );
}

bool RenderPlugin::enabled() const
{
    return d->m_enabled;
}

bool RenderPlugin::visible() const
{
    return d->m_visible;
}

bool RenderPlugin::eventFilter( QObject *, QEvent * ) {
    return false;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   

}

#include "RenderPlugin.moc"
