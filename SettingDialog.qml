import QtQuick 2.15
import QtQuick.Window 2.15
import Qt.labs.platform 1.1
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3


Window{
    id:root
    width: 640;
    height: 480;
    visible:false
    modality: Qt.WindowModal
    property var outputPath:"";
    property var taskNum: 2;//线程数
    signal setFinish();

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
            id:txtOutputPath
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
    //设置线程数量
    RowLayout{
        id:rowTaskNum
        anchors.margins: 4
        anchors.top:row1.bottom
        anchors.left: parent.left
        spacing: 4
        Text{
            text:"设置线程数"
            Layout.preferredWidth: 100
            //设置水平对齐
        }
        //大小
        SpinBox{
            id:spinBoxTaskNum
            editable: true
            from:1
            to:4
            stepSize: 1
            value: root.taskNum
        }

    }

    //最下面的确定、取消和应用按键
    RowLayout{
        id:rowBottom
         anchors.margins: 4
         anchors.bottom: parent.bottom
         anchors.right: parent.right
         spacing: 4
         Button{
             id:btnAccept
             text: "确定"
             //一旦点击，那么所有的操作都会被确定，且退出设置界面
             onClicked: {
                 //设置任务数
                 taskNum=spinBoxTaskNum.value
                 //发送属性更改信号
                root.setFinish();
                 //关闭设置窗口
                 root.close()
             }
         }
         Button{
             id:btnCancel
             text: "取消"
                onClicked: {
                //关闭设置窗口
                root.close()
             }
         }
         Button{
             id:btnApply
             text:"应用"
             onClicked: {
                //设置任务数
                taskNum=spinBoxTaskNum.value
                //发送属性更改信号
                root.setFinish();
             }

         }

    }
    FolderDialog{
        id:chose_outputDir
        title: "选择一个文件夹"
        folder: outputPath
        onAccepted: {
            var curPath =getFilePathFromUrl(folder);
            txtOutputPath.text= curPath;	//设置输入框文字
            outputPath = curPath;	//设置属性
        }
    }

    function getFilePathFromUrl(iUrl){
        var str =iUrl.toString();
        str=str.replace(/^(file:\/{3})/,"")
        var cleanPath = decodeURIComponent(str);
        console.log(cleanPath)
        return cleanPath;

    }


}
