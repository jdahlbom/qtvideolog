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
    }

