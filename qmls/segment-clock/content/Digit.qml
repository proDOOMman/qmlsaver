import Qt 4.7 as QML

QML.Rectangle {
    id: dig
    width: 200
    height: 400
    property int val: 8
    color: parent.parent.color

    QML.Image {
        id: image1
        x: 27
        width: 144
        height: 35
        fillMode: "PreserveAspectFit"
        source: segmentImg
    }

    QML.Image {
        id: image2
        x: 28
        y: 183
        width: 144
        height: 35
        source: segmentImg
        fillMode: "PreserveAspectFit"
    }

    QML.Image {
        id: image3
        x: 28
        y: 365
        width: 144
        source: segmentImg
        fillMode: "PreserveAspectFit"
    }

    QML.Image {
        id: image4
        x: -54
        y: 95
        width: 144
        height: 35
        rotation: 90
        source: segmentImg
        fillMode: "PreserveAspectFit"
    }

    QML.Image {
        id: image5
        x: -53
        y: 277
        width: 144
        height: 35
        source: segmentImg
        fillMode: "PreserveAspectFit"
        rotation: 90
    }

    QML.Image {
        id: image6
        x: 111
        y: 93
        width: 144
        height: 35
        source: segmentImg
        fillMode: "PreserveAspectFit"
        rotation: 90
    }

    QML.Image {
        id: image7
        x: 112
        y: 278
        width: 144
        height: 35
        source: segmentImg
        fillMode: "PreserveAspectFit"
        rotation: 90
    }
    states: [
        QML.State {
            name: "0"
            when: val == 0

            QML.PropertyChanges {
                target: image2
                x: 28
                y: 0
            }
        },
        QML.State {
            name: "1"
            when:  val == 1
            QML.PropertyChanges {
                target: image2
                x: 112
                y: 286
                rotation: 90
            }

            QML.PropertyChanges {
                target: image4
                x: 112
                y: 94
            }

            QML.PropertyChanges {
                target: image5
                x: 112
                y: 284
            }

            QML.PropertyChanges {
                target: image3
                x: 112
                y: 285
                rotation: 90
            }

            QML.PropertyChanges {
                target: image7
                x: 112
                y: 285
            }

            QML.PropertyChanges {
                target: image1
                x: 111
                y: 95
                rotation: 90
            }
        },
        QML.State {
            name: "2"
            when:  val == 2
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 183
            }

            QML.PropertyChanges {
                target: image4
                x: 112
                y: 94
            }

            QML.PropertyChanges {
                target: image7
                x: -52
                y: 278
            }
        },
        QML.State {
            name: "3"
            when: val == 3
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 183
            }

            QML.PropertyChanges {
                target: image4
                x: 112
                y: 94
            }

            QML.PropertyChanges {
                target: image5
                x: 112
                y: 279
            }
        },
        QML.State {
            name: "4"
            when:  val == 4
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 183
            }

            QML.PropertyChanges {
                target: image1
                x: 28
                y: 182
            }

            QML.PropertyChanges {
                target: image3
                x: 28
                y: 183
            }

            QML.PropertyChanges {
                target: image5
                x: 112
                y: 279
            }
        },
        QML.State {
            name: "5"
            when: val == 5
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 183
            }

            QML.PropertyChanges {
                target: image6
                x: -53
                y: 96
            }

            QML.PropertyChanges {
                target: image5
                x: 111
                y: 279
            }
        },
        QML.State {
            name: "6"
            when:  val == 6
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 183
            }

            QML.PropertyChanges {
                target: image6
                x: -53
                y: 96
            }
        },
        QML.State {
            name: "7"
            when: val == 7
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 0
            }

            QML.PropertyChanges {
                target: image4
                x: 112
                y: 94
            }

            QML.PropertyChanges {
                target: image3
                x: 28
                y: 0
            }

            QML.PropertyChanges {
                target: image5
                x: 112
                y: 279
            }
        },
        QML.State {
            name: "8"
            when:  val == 8
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 183
            }
        },
        QML.State {
            name: "9"
            when: val == 9
            QML.PropertyChanges {
                target: image2
                x: 28
                y: 183
            }

            QML.PropertyChanges {
                target: image5
                x: 112
                y: 279
            }
        }
    ]

    transitions: [
        QML.Transition {
            QML.ParallelAnimation{
                QML.NumberAnimation{
                    property: "x"
                    duration: animSpeed
                }
                QML.NumberAnimation{
                    property: "y"
                    duration: animSpeed
                }
                QML.NumberAnimation{
                    property: "rotation"
                    duration: animSpeed
                }
            }
        }
    ]

}

