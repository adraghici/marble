//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012      Dennis Nienhüser <earthwings@gentoo.org>
//


#ifndef MARBLE_GEODATAGROUNDOVERLAY_H
#define MARBLE_GEODATAGROUNDOVERLAY_H

#include "GeoDataOverlay.h"
#include "MarbleGlobal.h"
#include "GeoDataLatLonBox.h"
#include "GeoDataLatLonQuad.h"

namespace Marble {

class GeoDataGroundOverlayPrivate;

class MARBLE_EXPORT GeoDataGroundOverlay: public GeoDataOverlay
{
public:
    GeoDataGroundOverlay();

    GeoDataGroundOverlay( const GeoDataGroundOverlay &other );

    GeoDataGroundOverlay& operator=( const GeoDataGroundOverlay &other );

    ~GeoDataGroundOverlay();

    /** Provides type information for downcasting a GeoNode */
    virtual const char* nodeType() const;

    double altitude() const;

    void setAltitude( double altitude );

    AltitudeMode altitudeMode() const;

    void setAltitudeMode( const AltitudeMode altitudeMode );

    GeoDataLatLonBox& latLonBox() const;

    void setLatLonBox( const GeoDataLatLonBox &box );

    const GeoDataLatLonQuad& latLonQuad() const;

    GeoDataLatLonQuad& latLonQuad();

    void setLatLonQuad( const GeoDataLatLonQuad &quad );

private:
    GeoDataGroundOverlayPrivate* const d;
};

}

Q_DECLARE_METATYPE( Marble::GeoDataGroundOverlay* )

#endif
