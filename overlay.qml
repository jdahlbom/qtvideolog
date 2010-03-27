import Qt 4.6

    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        width: 400
        height: 300
        color: "lightSteelBlue"

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
                    anchors.fill: parent
                    onClicked: {
                        fake_model.moreInfoRequested(index)
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

            color : detailObject.color
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
        }
    }

