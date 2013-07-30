//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013       Adrian Draghici <draghici.adrian.b@gmail.com>
//

#include <QtTest>

#include "MarbleGlobal.h"
#include "MarbleMath.h"
#include "TestUtils.h"

namespace Marble
{

class TestGudermannian : public QObject
{
    Q_OBJECT

private slots:
    void testGudermannian_data();
    void testGudermannian();
};

void TestGudermannian::testGudermannian_data()
{
    QTest::addColumn<qreal>( "angle" );

    addRow() << 1 * DEG2RAD;
    addRow() << 5 * DEG2RAD;
    addRow() << 15 * DEG2RAD;
    addRow() << 20 * DEG2RAD;
    addRow() << 30 * DEG2RAD;
    addRow() << 40 * DEG2RAD;
    addRow() << 60 * DEG2RAD;
    addRow() << 70 * DEG2RAD;
    addRow() << 80 * DEG2RAD;
    addRow() << 90 * DEG2RAD;
}

void TestGudermannian::testGudermannian()
{
    QFETCH( qreal, angle );

    QCOMPARE( atan( sinh (angle ) ) * RAD2DEG, gd( angle ) * RAD2DEG );
}

}

QTEST_MAIN( Marble::TestGudermannian )

#include "TestGudermannian.moc"
