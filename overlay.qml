import Qt 4.6

    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        width: 300
        height: 300
        color: "lightSteelBlue"

        Component {
            id: listItemDelegate

            Item {
                id: delegateWrapper
                width: 150
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
                        console.log(model.text);
                        console.log(model.bgColor);
                        console.log(model.image);
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
            width: 300
            height: 300
        }
    }

