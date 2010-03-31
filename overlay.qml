import Qt 4.6

Item {
    width: 1200
    height: 800

    Component {
        id: listItemDelegate

        Item {
            id: delegateWrapper
            width: 200
            height: 70
            Rectangle {
                anchors.fill : parent
                color : model.bgColor
                z : 1
            }

            Row  {
                z : 2
                Image {
                    source: model.image
                }
                Text {
                    text: model.text
                }
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
        width: 200
        height: 300
    }

    Rectangle {
        id: detailsArea
        anchors.right : stringList.left
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        opacity: 0.7
        color : detailObject.color
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


        Column {
            Image {
                source: detailObject.url
            }
            Text {
                text: detailObject.name
            }
            Text {
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

