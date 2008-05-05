//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2007      Inge Wallin  <ingwa@kde.org>"
//


// Local
#include "AbstractProjection.h"

// Marble
#include "ViewportParams.h"


AbstractProjection::AbstractProjection()
{
    m_repeatX = false;
}

AbstractProjection::~AbstractProjection()
{
}

bool AbstractProjection::screenCoordinates( const GeoDataPoint &geopoint, 
                         const ViewportParams *viewport, int &x, int &y )
{
    bool occulted;
    return screenCoordinates( geopoint, viewport, x, y, occulted );
}
