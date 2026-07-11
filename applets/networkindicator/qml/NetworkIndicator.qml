import QtQuick 2.15
Rectangle { color:"transparent"; Text { text: applet.connected ? applet.ssid : "Disconnected"; anchors.centerIn: parent } }
