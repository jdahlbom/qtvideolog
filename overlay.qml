import Qt 4.6

Item {
    id: root
    property color textColor: "#BBBBBB"
    property color bgColor: "#DDDDDD"

    width: 1200
    height: 800

    Component {
        id: listItemDelegate

        Item {
            id: delegateWrapper
            width: 250
            height: 80
            Rectangle {
                id: bgrect
                radius: 10
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop {
                        position: 1.0
                        color: "#000011"
                    }
                    GradientStop {
                        position: 0.5
                        color: "#111122"
                    }
                    GradientStop {
                        position: 0.2
                        color: "#222244"
                    }
                    GradientStop {
                        position: 0.0
                        color: "#111122"
                    }
                }
            }

            Image {
                anchors.left: bgrect.left
                anchors.leftMargin: 10
                anchors.top: bgrect.top
                anchors.topMargin: 10

                id: delegateImage
                source: model.image
                width: 60
                height: 60
                smooth: true
                fillMode: "PreserveAspectFit"
            }
            Text {
                width: bgrect.width - delegateImage.width - 20

                anchors.left: delegateImage.right
                anchors.leftMargin: 10
                anchors.verticalCenter: delegateImage.verticalCenter
                wrap: true
                text: model.text
                color: root.textColor
            }


            MouseArea {
                id: listItemMouseArea
                anchors.fill: parent


                onClicked: {
                    detailAreaTimer.clickCount = detailAreaTimer.clickCount + 1;

                    if (detailAreaTimer.requestedIndex != index) {
                        detailAreaTimer.requestedIndex = index
                        detailAreaTimer.running = false
                    }

                    if (! detailAreaTimer.running)
                        detailAreaTimer.running = true
                }

                onDoubleClicked: {
                    detailsArea.state = "detailsHidden"
                    fake_model.requestVideoLaunch(index)
                    playButton.state = "playing"
                }
            }
        }
    }

    ListView {
        id: stringList
        model: fake_model
        delegate: listItemDelegate
        anchors.top: parent.top
        anchors.right: parent.right
        width: 250
        height: 300
    }

    Rectangle {
        id: detailsArea
        anchors.right : stringList.left

        width: 500
        y: 200
        height: 300

        opacity: 0.7
        color : root.bgColor
        radius: 10

        state: "detailsHidden"

        property int clickCount : 0
        property int shownIndex : -1

        Timer {
            id: detailAreaTimer
            interval: 250
            running: false; repeat: false;

            property int requestedIndex : -1
            property int clickCount : 0
            property int currentIndex : -1

            onTriggered: {
                if (clickCount == 1) {
                    if (currentIndex == requestedIndex) {
                        currentIndex = -1
                        detailsArea.state = "detailsHidden"
                    } else {
                        currentIndex = requestedIndex
                        fake_model.requestDetails(requestedIndex)
                        detailsArea.state = "detailsShown"
                    }
                }
                clickCount = 0
            }
        }

        Item {
            anchors.margins: 20

            Image {
                id: detailImg
                x: 30
                y: 30
                width: 80
                smooth: true
                fillMode: "PreserveAspectFit"
                source: detailObject.url
            }
            Text {
                id : detailTitle
                anchors.left: detailImg.right
                anchors.leftMargin: 20
                anchors.topMargin: 20
                anchors.top: parent.top
                wrap: true
                font.pointSize: 20
                font.bold: true
                width: detailsArea.width - 40 - detailImg.width
                smooth: true
                text: detailObject.name
            }

            Text {
                id: detailDesc
                anchors.top: detailTitle.bottom
                anchors.left: detailImg.right
                anchors.leftMargin: 20
                width: detailsArea.width - 40 - detailImg.width
                text: "Description: "
                font.bold: true
            }

            Text {
                anchors.left: detailImg.right
                anchors.top: detailDesc.bottom
                anchors.leftMargin: 20
                width: detailsArea.width - 40 - detailImg.width
                wrap: true

                text: detailObject.description
            }
        }



        states: [
        State {
            id: detailsHidden
            name: "detailsHidden"
            PropertyChanges {
                target: detailsArea
                visible: false
            }

        },
        State {
            id: detailsShown
            name: "detailsShown"
            PropertyChanges {
                target: detailsArea
                visible: true
            }
        }
        ]

        transitions: [
        Transition {
            from: "detailsHidden"
            to: "detailsShown"
        }
        ]
    }

    Item {
        id: videoControls
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height-100

        width: 1000
        height: 80
        state: "videoCtlHidden"

        states: [
            State {
                name: "videoCtlShown"
            },
            State {
                name: "videoCtlHidden"
            }
        ]

        Image {
            id: playButton
            anchors.left: parent.left
            anchors.top: parent.top
            width: 80
            height: 80
            source: "media/play.png"

            states: [
                State {
                    name: "playing"
                    PropertyChanges {
                        target: playButton
                        source: "media/pause.png"
                    }
                },
                State {
                    name: "paused"
                    PropertyChanges {
                        target: playButton
                        source: "media/play.png"
                    }
                }
            ]

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (playButton.state=="playing") {
                        playButton.state="paused"
                        overlayManager.requestVideoPause()
                    }
                    else {
                        playButton.state="playing"
                        overlayManager.requestVideoPlay()
                    }
                }
            }
        }

        Rectangle {
            id: seekerBorder
            anchors.verticalCenter: playButton.verticalCenter
            x: 10 + playButton.x + playButton.width

            height: 26
            width: parent.width - x
            radius: 10
            color: "#aaaaaa"
            opacity: 0.5

            Rectangle {
                id: seekerContent
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 8
                width: parent.width - totalTime.width
                color: "#111111"
                radius: 3

                MouseArea {
                    property real seekPercentage : 0

                    anchors.fill: parent
                    drag.target: timeMarker
                    drag.axis: "XAxis"
                    drag.minimumX: 8
                    drag.maximumX: 8 + parent.width - timeMarker.width

                    onReleased: {
                        seekPercentage = (mouse.x - 8) / (parent.width - timeMarker.width)
                        overlayManager.requestVideoSeek(seekPercentage)
                        timeMarker.x = seekPercentage * (parent.width - timeMarker.width) + 8
                    }
                }

            } // Rectangle seekerContent

            Rectangle {
                property int minX : seekerContent.x + 8
                property int maxX : seekerContent.x + 8 + seekerContent.width - timeMarker.width

                id: timeMarker
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 2
                anchors.bottomMargin: 2
                x: {
                    if (videoData.length > 0) {
                        return (videoData.currentTime / videoData.length) * (maxX-minX) + minX
                    }
                    return minX;
                }

                width: 10
                color: "#CCCCCC"
            }

            Text {
                property int hours: 0
                property int minutes: 0
                property int seconds: 0
                property string timeAsStr: "";

                id: totalTime
                anchors.left: seekerContent.right
                anchors.leftMargin: 20
                anchors.verticalCenter: seekerContent.verticalCenter
                width: 90
                text: videoData.totalTimeStr;

                color: "white"
            }

        } // Rectangle seekerBorder
    } // Item videoControls
}

