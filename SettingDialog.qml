import QtQuick 2.15
import QtQuick.Window 2.15
import Qt.labs.platform 1.1
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import an.qt.Repack 1.0


Window{
    id:root
    width: 640;
    height: 480;
    visible:false
    modality: Qt.WindowModal
    property var outputPath:"D:/";



    //设置默认路径
    RowLayout{
        id:row1
        anchors.margins: 4
        anchors.top: parent.top
        anchors.left: parent.left
        spacing: 4
        Text{
            text:"设置输出路径"
            Layout.preferredWidth: 100
            //设置水瓶对齐
        }

        TextField{
            text:outputPath
            selectByMouse: true
            background:Rectangle {
                border.width: 1
                border.color: "gray"
            }
            Layout.preferredWidth: 350
        }
        Button{
            text:"选择路径"
            onClicked: {
                chose_outputDir.open()
            }
        }


    }
    RowLayout{
        id:rowBottom
         anchors.margins: 4
         anchors.bottom: parent.bottom
         anchors.right: parent.right
         spacing: 4
         Button{
             id:btnAccept
             text: "确定"
         }
         Button{
             id:btnCancel
             text: "取消"
         }
         Button{
             id:btnApply
             text:"应用"
         }

    }
    FolderDialog{
        id:chose_outputDir
        title: "选择一个文件夹"
        folder: outputPath
        onAccepted: {
            outputPath = currentFolder
        }
    }
    function getFilePathFromUrl(iUrl){
        var str = url(iUrl).toString();
        return str;

    }


}
