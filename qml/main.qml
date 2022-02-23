import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Subscribe Tool")

    Item {
        id: tokenConfig
        width: root.width/2
        height: 50
        anchors.horizontalCenter: startBtn.horizontalCenter
        anchors.bottom: threadConfig.top
        Text {
            id: tokenLabel
            anchors.verticalCenter: parent.verticalCenter
            width: contentWidth
            height: 50
            verticalAlignment: Text.AlignVCenter
            text: qsTr("Token: ")
            font.pixelSize: 15
        }

        TextField {
            height: 30
            anchors.left : tokenLabel.right
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            text: AppModel.token
            font.pixelSize: 15
            onTextChanged: {
                AppModel.token = text
            }
        }
    }


    Item {
        id: threadConfig
        width: root.width/2
        height: 100
        anchors.horizontalCenter: startBtn.horizontalCenter
        anchors.bottom: startBtn.top
        Text {
            id: threadLabel
            anchors.verticalCenter: parent.verticalCenter
            width: contentWidth
            height: 50
            verticalAlignment: Text.AlignVCenter
            text: qsTr("Thread: ")
            font.pixelSize: 15
        }

        TextField {
            height: 30
            anchors.left : threadLabel.right
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            text: AppModel.maxThread
            font.pixelSize: 15
            onTextChanged: {
                AppModel.maxThread = Number(text)
            }
        }
    }

    Rectangle {
        id : startBtn
        width: 100
        height: 50
        color: "grey"
        opacity: AppModel.appStarted? 0.6 : 1
        border.width: 2
        border.color: mouseArea.pressed? "blue" : "transparent"
        radius: 4
        anchors.centerIn: parent
        Text {
            text: AppModel.appStarted? qsTr("Stop") : qsTr("Start")
            anchors.fill: parent
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                if(AppModel.appStarted)
                    AppMain.stop();
                else
                    AppMain.start()
            }
        }
    }
}
