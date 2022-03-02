import QtQuick 2.15
import Qt.labs.platform 1.1
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import an.qt.Repack 1.0
ApplicationWindow{
    id:root
    width: 640
    height: 480
    visible: true
    title: qsTr("阿里网盘rar转换-0.1-mojovs")
    property var filePaths;
    property var folderPaths;

    //菜单栏
    menuBar:MenuBar {
        //文件选项
        Menu{
            title:"文件"
            Action{
                id:act_openFiles
                text: "打开文件"
                //打开文件
                onTriggered: {
                    dialog_choseFile.open();
                }
            }
            Action{
                id:act_openFolders
                text: "打开文件夹"
                //打开文件
                onTriggered: {
                    dialog_choseFolder.open();
                }
            }
        }

        //设置选项
        Menu{
            title: "设置"
            Action{
                id:act_option
                text:"选项"
                onTriggered: {
                    dialog_set.visible=true
                }

            }

        }
    }
    //工具栏
    header:ToolBar{
        RowLayout{
            anchors.fill: parent
            spacing: 4
            //重新打包
            ToolButton{
                id:btn_pack
                text:"打包全部文件"
                onClicked: {
                    repack.start();
                    //为所有listModel添加完成选项
                    var i=0;
                    for(i=0;i<view.count;i++)
                    {
                        listModel.setProperty(i,"complete","正在打包")
                    }
                }
            }

            //打开阿里云盘网址
            ToolButton{

                id:btn_openUrl
                text:"打开阿里云"

                onClicked: {
                    Qt.openUrlExternally("https://www.aliyundrive.com/drive/")
                }
            }
            ToolButton{
                text: "缩小"
                onClicked: {
                    trayIcon.show()	//显示图标
                    root.hide()

                }
            }
        }
    }

    //显示已经打开的文件
    Rectangle{
        anchors.fill: parent
        ListView{
            id:view
            anchors.fill: parent
            delegate: listDelegate
            header: headerDelegate
            model:listModel
            spacing: 4
            focus: true

            //高量
            highlight: Rectangle{
                color: "yellow"
            }
            //动画效果
            add: Transition{
                ParallelAnimation{
                    //变化透明度
                    NumberAnimation{
                        property: "opacity"
                        from:0
                        to:1
                        duration:500

                    }
                    SpringAnimation{
                        property: "y"
                        spring: 3
                        from:0
                        damping: 0.1
                        epsilon: 0.25

                    }

                }

            }

        }

    }


    //打开文件
    FileDialog{
        id:dialog_choseFile
        nameFilters: ["rar文件 (*.rar)", "所有文件 (*.*)"]
        selectMultiple: true
        onAccepted: {
            root.filePaths= dialog_choseFile.fileUrls;	//所有地址给到r
            repack.fileUrls=filePaths;		//设置C++对象的url地址
            listModel.clear();	//清除model
            var i=0;
            for(i=0;i<root.filePaths.length;i++){
                var filePath =root.filePaths[i];
                //添加到model里面
                listModel.append({"name":getFileName( repack.filePaths[i])});
                console.log("结果为"+repack.filePaths[i]);

            }

        }

    }

    FolderDialog{
        id:dialog_choseFolder
        options: FolderDialog.ShowDirsOnly
        onAccepted: {
            root.folderPaths=dialog_choseFolder.currentFolder;
            //获取路径
        }
    }
    //listView代理
    Component{
        id:listDelegate
        Item{
            id:wrapper
            width: view.width
            height:30
            //鼠标点击选中当前
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    wrapper.ListView.view.currentIndex=index
                    console.log("点击"+index)
                }
            }

            RowLayout{
                anchors.left: parent.left
                spacing: 4
                //序号框
                    //序号
                 Text{
                     id:txtIndex
                     text: index
                     //选中变色及变大
                     font.pixelSize:wrapper.ListView.isCurrentItem?22:18
                     color:wrapper.ListView.isCurrentItem?"red":"black"
                     //设置大小
                     Layout.preferredWidth:  30
                 }


                 Text{
                     text:name;
                     wrapMode: Text.WrapAnywhere
                     //选中变色及变大
                     font.pixelSize:wrapper.ListView.isCurrentItem?22:18
                     color:wrapper.ListView.isCurrentItem?"red":"black"
                     Layout.preferredWidth: 450
                 }
                 //是否完成打包
                 Text {
                     text:complete;
                     //选中变色及变大
                     font.pixelSize:wrapper.ListView.isCurrentItem?22:18;
                     color:wrapper.ListView.isCurrentItem?"red":"black";
                     Layout.preferredWidth: 50;

                 }
            }
        }

    }
    //表头代理
    Component{
        id:headerDelegate
        Item{
            id:wrapper
            width:parent.width
            height: 30
            RowLayout{
                anchors.left: parent.left
                spacing: 4
                Text{
                    text:"序号"
                    font.bold: true
                    Layout.preferredWidth:30
                }
                Text{
                    text:"文件名"
                    font.bold: true
                    Layout.preferredWidth:450
                }
                Text{

                    text:"完成"
                    font.bold: true
                    Layout.preferredWidth:50
                }
            }
        }
    }

    SettingDialog
    {
        id:dialog_set
        visible: false
        taskNum: repack.taskNum

    }

    ListModel{
        id:listModel

        ListElement{
            name:" "
            complete:"否"
        }
    }
    //系统托盘图标设置
    SystemTrayIcon{
        id:trayIcon
        iconSource: "res/shark.ico"
        tooltip:"打开"
        visible:false
        onActivated:{ root.show() }
    }

    //从一串字符获取文件名称，包括扩展名
    function getFileName(str)
    {
       return (String(str).slice(String(str).lastIndexOf("/")+1));

    }
    //repack处理文件完成，槽
    Connections{
        target: repack
        //槽,文件处理完成，就改变列表，当前状态
        function onFileNoFinish(n){
            console.log("完成的文件编号为"+n);
            listModel.setProperty(n,"complete","是");
        }

    }
    //设置属性更改
    Connections{
        target:dialog_set
        function onSetFinish()
        {
            repack.taskNum=dialog_set.taskNum
            repack.outputDir=dialog_set.outputPath
        }
    }
}
