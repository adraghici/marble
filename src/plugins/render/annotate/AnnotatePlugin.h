//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2009 Andrew Manson <g.real.ate@gmail.com>
// Copyright 2013      Thibaut Gridel <tgridel@free.fr>
//

//
// This class provides a Marble plugin to annotate maps with polygons
// and placemarks
//

#ifndef MARBLE_ANNOTATEPLUGIN_H
#define MARBLE_ANNOTATEPLUGIN_H

#include "RenderPlugin.h"
#include "SceneGraphicsItem.h"
#include "GeoDataLatLonBox.h"
#include "GeoDataGroundOverlay.h"
#include "GroundOverlayFrame.h"

#include <QObject>
#include <QErrorMessage>
#include <QMenu>
#include <QSortFilterProxyModel>

class QNetworkAccessManager;
class QNetworkReply;


namespace Marble
{
class MarbleWidget;
class TextureLayer;
class PlacemarkTextAnnotation;
class GeoDataDocument;
class GeoDataLinearRing;
class GeoDataLineString;

/**
 * @short The class that specifies the Marble layer interface of a plugin.
 *
 */

class AnnotatePlugin :  public RenderPlugin
{
    Q_OBJECT
    Q_INTERFACES( Marble::RenderPluginInterface )
    MARBLE_PLUGIN( AnnotatePlugin )

    public:
        explicit AnnotatePlugin(const MarbleModel *model = 0);
    virtual ~AnnotatePlugin();

    QStringList backendTypes() const;

    QString renderPolicy() const;

    QStringList renderPosition() const;

    QString name() const;

    QString guiString() const;

    QString nameId() const;

    QString version() const;

    QString description() const;

    QIcon icon () const;

    QString copyrightYears() const;

    QList<PluginAuthor> pluginAuthors() const;

    void initialize ();

    bool isInitialized () const;

    virtual QString runtimeTrace() const;

    virtual const QList<QActionGroup*>* actionGroups() const;
    virtual const QList<QActionGroup*>* toolbarActionGroups() const;

    bool render( GeoPainter *painter, ViewportParams *viewport,
                 const QString& renderPos, GeoSceneLayer * layer = 0 );

signals:
    void placemarkAdded();
    void overlayAdded();
    void itemRemoved();

public slots:
    void enableModel( bool enabled );

    void setAddingPlacemark( bool );
    void setDrawingPolygon( bool );
    void setAddingOverlay( bool );
    void setRemovingItems( bool );

    void addOverlay();

    //    void receiveNetworkReply( QNetworkReply* );
    //    void downloadOsmFile();

    void clearAnnotations();
    void saveAnnotationFile();
    void loadAnnotationFile();

protected:
    bool eventFilter(QObject* watched, QEvent* event);
private:
    void setupActions(MarbleWidget* m);
    void setupGroundOverlayModel();
    void setupOverlayRmbMenu();
    //    void readOsmFile( QIODevice* device, bool flyToFile );

    void showOverlayRmbMenu( GeoDataGroundOverlay *overlay, qreal x, qreal y );

    void displayOverlayEditDialog( GeoDataGroundOverlay *overlay );
    void displayOverlayFrame( GeoDataGroundOverlay *overlay );
    void clearOverlayFrames();

    bool    m_widgetInitialized;
    MarbleWidget* m_marbleWidget;

    QMenu*                  m_overlayRmbMenu;
    QList<QActionGroup*>    m_actions;
    QList<QActionGroup*>    m_toolbarActions;

    GeoDataDocument *m_annotationDocument;
    QList<SceneGraphicsItem*> m_graphicsItems;
    QSortFilterProxyModel m_groundOverlayModel;
    QMap<GeoDataGroundOverlay*, SceneGraphicsItem*> m_groundOverlayFrames;

    //used while creating new polygons
    GeoDataPlacemark* m_polygon_placemark;
    SceneGraphicsItem *m_selectedItem;

    GeoDataGroundOverlay *m_rmbOverlay;

    bool m_addingPlacemark;
    bool m_drawingPolygon;
    bool m_addingOverlay;
    bool m_removingItem;
    //    QNetworkAccessManager* m_networkAccessManager;
    //    QErrorMessage m_errorMessage;
    bool m_isInitialized;

private slots:
    void editOverlay();
    void removeOverlay();
    void updateOverlayFrame( GeoDataGroundOverlay* overlay );
};

}

#endif // MARBLE_ANNOTATEPLUGIN_H
