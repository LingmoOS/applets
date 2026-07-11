import QtQuick 2.15
import QtQuick.Controls 2.15
Rectangle { color:"transparent"
    Slider { from:0; to:100; value: applet.volume; onValueChanged: applet.volume = value; anchors.fill:parent } }
