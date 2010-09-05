import Qt 4.7 as QML
import "content" as D

QML.Rectangle {
    id: scene
    width: sceneWidth
    height: sceneHeight
    color: bgcolor
    QML.Text{
        anchors.horizontalCenter: parent.horizontalCenter
        color: "White"
        text: upperText
    }

    property int hours
    property int minutes
    property int seconds 

    function timeChanged() {
        var date = new Date;
        hours = date.getHours()
        minutes = date.getMinutes()
        seconds = date.getUTCSeconds();
    }

    QML.Timer {
        interval: 100; running: true; repeat: true;
        onTriggered: scene.timeChanged()
    }
    
    QML.Row{
        anchors.centerIn: parent
        scale: sceneWidth/1510*clockScale
        spacing: 30
        D.Digit {
            id: h1
            val: Math.floor(hours/10)
        }
        D.Digit {
            id: h2
            val: hours - Math.floor(hours/10)*10
        }
        QML.Column{
            anchors.verticalCenter: parent.verticalCenter
            spacing: 50
            QML.Image {
                width: 50
                id: dot1
                source: dotFile
                fillMode: "PreserveAspectFit"
            }
            QML.Image {
                width: 50
                id: dot2
                source: dotFile
                fillMode: "PreserveAspectFit"
            }
        }
        D.Digit {
            id: m1
            val: Math.floor(minutes/10)
        }
        D.Digit {
            id: m2
            val: minutes - Math.floor(minutes/10)*10
        }
        QML.Column{
            anchors.verticalCenter: parent.verticalCenter
            spacing: 50
            QML.Image {
                width: 50
                id: dot3
                source: dotFile
                fillMode: "PreserveAspectFit"
            }
            QML.Image {
                width: 50
                id: dot4
                source: dotFile
                fillMode: "PreserveAspectFit"
            }
        }
        D.Digit {
            id: s1
            val: Math.floor(seconds/10)
        }
        D.Digit {
            id: s2
            val: seconds - Math.floor(seconds/10)*10
        }
    }
}
