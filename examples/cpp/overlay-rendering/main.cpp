//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Adrian Draghici <draghici.adrian.b@gmail.com>
//

#include <QDebug>
#include <QFileInfo>
#include <QApplication>
#include <QImage>

#include <marble/MarbleWidget.h>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataGroundOverlay.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleModel.h>

using namespace Marble;

int main(int argc, char** argv) {

    QApplication app(argc,argv);

    QFileInfo inputFile( app.arguments().last() );
    if ( app.arguments().size() < 2 || !inputFile.exists() ) {
        qWarning() << "Usage: " << app.arguments().first() << "file.png";
        return 1;
    }

    // Create a Marble QWidget without a parent
    MarbleWidget *mapWidget = new MarbleWidget();

    // Load the BlueMarble map
    mapWidget->setMapThemeId( "earth/bluemarble/bluemarble.dgml" );

    GeoDataGroundOverlay *overlay = new GeoDataGroundOverlay;

    GeoDataLatLonBox box( 55, 48, 14.5, 6, GeoDataCoordinates::Degree );
    box.setRotation( 0, GeoDataCoordinates::Degree );

    overlay->setLatLonBox( box );
    overlay->setIcon( QImage( inputFile.absoluteFilePath() ) );

    GeoDataDocument *document = new GeoDataDocument();
    document->append( overlay );

    // Add the document to MarbleWidget's tree model
    mapWidget->model()->treeModel()->addDocument( document );

    mapWidget->show();

    return app.exec();
}

