//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010      Siddharth Srivastava <akssps011@gmail.com>
// Copyright 2011      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//


#include "AdjustNavigation.h"

#include "MarbleWidget.h"
#include "GeoDataCoordinates.h"
#include "PositionTracking.h"
#include "MarbleDebug.h"
#include "MarbleModel.h"
#include "MarbleMath.h"
#include "global.h"

#include <QtGui/QPixmap>
#include <QtGui/QWidget>
#include <QtCore/QRect>
#include <QtCore/QPointF>
#include <QtCore/QTime>
#include <math.h>

namespace Marble {

class AdjustNavigationPrivate
{
public:

    MarbleWidget        *const m_widget;
    const PositionTracking *const m_tracking;
    qreal                m_gpsSpeed;
    qreal                m_gpsDirection;
    AdjustNavigation::CenterMode m_recenterMode;
    bool                 m_adjustZoom;
    QTime                m_lastWidgetInteraction;
    bool                 m_selfInteraction;

    /** Constructor */
    AdjustNavigationPrivate( MarbleWidget *widget );

    /**
     * @brief To center on when reaching custom defined border
     * @param position current gps location
     * @param speed optional speed argument
     */
     void moveOnBorderToCenter( const GeoDataCoordinates &position, qreal speed );

    /**
     * For calculating intersection point of projected LineString from
     * current gps location with the map border
     * @param position current gps location
     */
     GeoDataCoordinates findIntersection( qreal currentX, qreal currentY ) const;

    /**
     * @brief Adjust the zoom value of the map
     * @param currentPosition current location of the gps device
     * would reach if allowed to move in that direction
     */
     void adjustZoom( const GeoDataCoordinates &currentPosition );

     /**
       * Center the widget on the given position unless recentering is currently inhibited
       */
     void centerOn( const GeoDataCoordinates &position );
};

AdjustNavigationPrivate::AdjustNavigationPrivate( MarbleWidget *widget ) :
        m_widget( widget ),
        m_tracking( widget->model()->positionTracking() ),
        m_gpsSpeed( 0 ),
        m_gpsDirection( 0 ),
        m_recenterMode( AdjustNavigation::DontRecenter ),
        m_adjustZoom( 0 ),
        m_selfInteraction( false )
{
    m_lastWidgetInteraction.start();
}

void AdjustNavigationPrivate::moveOnBorderToCenter( const GeoDataCoordinates &position, qreal )
{
    qreal lon = 0.0;
    qreal lat = 0.0;

    position.geoCoordinates( lon, lat, GeoDataCoordinates::Degree );

    qreal x = 0.0;
    qreal y = 0.0;
    //recenter if initially the gps location is not visible on the screen
    if(!( m_widget->screenCoordinates( lon, lat, x, y ) ) ) {
         centerOn( position );
    }
    qreal centerLon = m_widget->centerLongitude();
    qreal centerLat = m_widget->centerLatitude();

    qreal centerX = 0.0;
    qreal centerY = 0.0;

    m_widget->screenCoordinates( centerLon, centerLat, centerX, centerY );

    const qreal borderRatio = 0.25;
    //defining the default border distance from map center
    int shiftX = qRound( centerX * borderRatio );
    int shiftY = qRound( centerY * borderRatio );

    QRect recenterBorderBound;
    recenterBorderBound.setCoords( centerX-shiftX, centerY-shiftY, centerX+shiftX,  centerY+shiftY );

    if( !recenterBorderBound.contains( x,y ) ) {
        centerOn( position );
    }
}

GeoDataCoordinates AdjustNavigationPrivate::findIntersection( qreal currentX, qreal currentY ) const
{
    qreal direction = m_gpsDirection;

    if ( direction >= 360 ) {
        direction = fmod( direction,360.0 );
    }

    const qreal width = m_widget->width();
    const qreal height = m_widget->height();

    QPointF intercept;
    QPointF destinationHorizontal;
    QPointF destinationVertical;
    QPointF destination;

    bool crossHorizontal =  false;
    bool crossVertical = false;

    //calculation of intersection point
    if( 0 < direction && direction < 90 ) {
        const qreal angle = direction;

        //Intersection with line x = width
        intercept.setX( width - currentX );
        intercept.setY( intercept.x() / tan( angle ) );
        destinationVertical.setX( width );
        destinationVertical.setY( currentY-intercept.y() );

        //Intersection with line y = 0
        intercept.setY( currentY );
        intercept.setX( intercept.y() * tan( angle ) );
        destinationHorizontal.setX( currentX + intercept.x() );
        destinationHorizontal.setY( 0 );

        if ( destinationVertical.y() < 0 ) {
            crossHorizontal = true;
        }
        else if( destinationHorizontal.x() > width ) {
            crossVertical = true;
        }

    }
    else if( 270 < direction && direction < 360 ) {
        const qreal angle = direction - 270;

        //Intersection with line y = 0
        intercept.setY( currentY );
        intercept.setX( intercept.y() / tan( angle ) );
        destinationHorizontal.setX( currentX - intercept.x() );
        destinationHorizontal.setY( 0 );

        //Intersection with line x = 0
        intercept.setX( currentX );
        intercept.setY( intercept.x() * tan( angle ) );
        destinationVertical.setY( currentY - intercept.y() );
        destinationVertical.setX( 0 );

        if( destinationHorizontal.x() > width ) {
            crossVertical = true;
        }
        else if( destinationVertical.y() < 0 ) {
            crossHorizontal = true;
        }

    }
    else if( 180 < direction && direction < 270  ) {
        const qreal angle = direction - 180;

        //Intersection with line x = 0
        intercept.setX( currentX );
        intercept.setY( intercept.x() / tan( angle ) );
        destinationVertical.setY( currentY + intercept.y() );
        destinationVertical.setX( 0 );

        //Intersection with line y = height
        intercept.setY( currentY );
        intercept.setX( intercept.y() * tan( angle ) );
        destinationHorizontal.setX( currentX - intercept.x() );
        destinationHorizontal.setY( height );

        if ( destinationVertical.y() > height ) {
            crossHorizontal = true;
        }
        else if ( destinationHorizontal.x() < 0 ) {
            crossVertical = true;
        }

    }
    else if( 90 < direction && direction < 180  ) {
        const qreal angle = direction - 90;

        //Intersection with line y = height
        intercept.setY( height - currentY );
        intercept.setX( intercept.y() / tan( angle ) );
        destinationHorizontal.setX( currentX + intercept.x() );
        destinationHorizontal.setY( height );

        //Intersection with line x = width
        intercept.setX( width - currentX );
        intercept.setY( intercept.x() * tan( angle ) );
        destinationVertical.setX( width );
        destinationVertical.setY( currentY + intercept.y() );

        if ( destinationHorizontal.x() > width ) {
            crossVertical = true;
        }
        else if( destinationVertical.y() > height ) {
            crossHorizontal = true;
        }

    }
    else if( direction == 0 ) {
        destinationHorizontal.setX( currentX );
        destinationHorizontal.setY( 0 );
        crossHorizontal = true;
    }
    else if( direction == 90 ) {
        destinationVertical.setX( width );
        destinationVertical.setY( currentY );
        crossVertical = true;
    }
    else if( direction == 190 ) {
        destinationHorizontal.setX( currentX );
        destinationHorizontal.setY( height );
        crossHorizontal = true;
    }
    else if( direction == 270 ) {
        destinationVertical.setX( 0 );
        destinationVertical.setY( currentY );
        crossVertical = true;
    }

    if ( crossHorizontal == true && crossVertical == false ) {
        destination.setX( destinationHorizontal.x() );
        destination.setY( destinationHorizontal.y() );
    }
    else if ( crossVertical == true && crossHorizontal == false ) {
        destination.setX( destinationVertical.x() );
        destination.setY( destinationVertical.y() );
    }

    qreal destinationLon = 0.0;
    qreal destinationLat = 0.0;
    m_widget->geoCoordinates( destination.x(), destination.y(), destinationLon, destinationLat,
                              GeoDataCoordinates::Radian );
    GeoDataCoordinates destinationCoord( destinationLon, destinationLat, GeoDataCoordinates::Radian );

    return destinationCoord;
}

void AdjustNavigationPrivate::adjustZoom( const GeoDataCoordinates &currentPosition )
{
    const qreal lon = currentPosition.longitude( GeoDataCoordinates::Degree );
    const qreal lat = currentPosition.latitude( GeoDataCoordinates::Degree );

    qreal currentX = 0;
    qreal currentY = 0;

    if( !m_widget->screenCoordinates( lon, lat, currentX, currentY ) ) {
        return;
    }

    const GeoDataCoordinates destination = findIntersection( currentX, currentY );

    qreal greatCircleDistance = distanceSphere( currentPosition, destination );
    qreal radius = m_widget->model()->planetRadius();
    qreal distance = greatCircleDistance *  radius;

    if( m_gpsSpeed != 0 ) {
        //time(in minutes) remaining to reach the border of the map
        qreal  remainingTime = ( distance / m_gpsSpeed ) * SEC2MIN;

        //tolerance time limits( in minutes ) before auto zooming
        qreal thresholdVeryLow = 0.5;
        qreal thresholdLow = 2.0 * thresholdVeryLow;
        qreal thresholdHigh = 12.0 * thresholdLow;
        qreal thresholdVeryHigh = 3.0 * thresholdHigh;
        qreal thresholdExtreme = 4.0 * thresholdVeryHigh;

        int zoom = 0;
        m_selfInteraction = true;
        if ( remainingTime <= thresholdVeryLow ) {
            zoom = 3100;
            m_widget->zoomView( zoom );
        }

        if ( remainingTime > thresholdVeryLow && remainingTime < thresholdLow ) {
            m_widget->zoomOut();
        }

        if ( remainingTime >= thresholdHigh && remainingTime < thresholdVeryHigh ) {
            m_widget->zoomIn();
        }

        if ( remainingTime >= thresholdVeryHigh && remainingTime < thresholdExtreme ) {
            int zoomStep = 55;
            m_widget->zoomViewBy( zoomStep );
        }

        if ( remainingTime >= thresholdExtreme ) {
             zoom = 3000;
             m_widget->zoomView( zoom );
        }
        m_selfInteraction = false;
    }
}

void AdjustNavigationPrivate::centerOn( const GeoDataCoordinates &position )
{
    if ( m_lastWidgetInteraction.elapsed() > 10 * 1000 ) {
        m_selfInteraction = true;
        m_widget->centerOn( position, false );
        m_selfInteraction = false;
    }
}

AdjustNavigation::AdjustNavigation( MarbleWidget *widget, QObject *parent )
    :QObject( parent ), d( new AdjustNavigationPrivate( widget ) )
{
    connect( widget, SIGNAL( visibleLatLonAltBoxChanged( GeoDataLatLonAltBox ) ),
             this, SLOT( inhibitAutoAdjustments() ) );

    connect( d->m_tracking, SIGNAL( gpsLocation( GeoDataCoordinates, qreal ) ),
                this, SLOT( adjust( GeoDataCoordinates, qreal ) ) );
}

AdjustNavigation::~AdjustNavigation()
{
    delete d;
}

void AdjustNavigation::adjust( const GeoDataCoordinates &position, qreal speed )
{
    d->m_gpsDirection = d->m_tracking->direction();
    d->m_gpsSpeed = speed;

    switch( d->m_recenterMode ) {
    case DontRecenter:
        /* nothing to do */
        break;
    case AlwaysRecenter:
        d->centerOn( position );
        break;
    case RecenterOnBorder:
        d->moveOnBorderToCenter( position, speed );
        break;
    }

    if ( d->m_adjustZoom ) {
        switch( d->m_recenterMode ) {
        case DontRecenter:
            /* nothing to do */
            break;
        case AlwaysRecenter:
        case RecenterOnBorder: // fallthrough
            d->adjustZoom( position );
            break;
        }
    }
}

void AdjustNavigation::setAutoZoom( bool autoZoom )
{
    d->m_adjustZoom = autoZoom;
    emit autoZoomToggled( autoZoom );
}

void AdjustNavigation::setRecenter( CenterMode recenterMode )
{
    d->m_recenterMode = recenterMode;
    emit recenterModeChanged( recenterMode );
}

void AdjustNavigation::inhibitAutoAdjustments()
{
    if ( !d->m_selfInteraction ) {
        d->m_lastWidgetInteraction.restart();
    }
}

AdjustNavigation::CenterMode AdjustNavigation::recenterMode() const
{
    return d->m_recenterMode;
}

bool AdjustNavigation::autoZoom() const
{
    return d->m_adjustZoom;
}

} // namespace Marble

#include "AdjustNavigation.moc"
