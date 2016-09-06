#include <QtWidgets/QtWidgets>
#include "imhmainwindow.h"
//XXX: 2 places need to be aware if need
//to ensure usage after 2019.9 out of 20 places
//1. imhmainwindow.cpp: display:judy()
//2. main.cpp vr_main

int main(int argc, char* argv[])
{
    int ret = 0;
    ret = vr_main(
                (char*)"http://www.somewebsite.com",
                (char*)"hint",
                bias,
                (char*)"https://api.github.com/users/username/repos",
                (char*)"https://api.github.com/user/repos",
                (char*)"username:passwd",
                (char*)"projectname"
           );
    if (!ret)
        exit(0);
    QApplication imhapp(argc,argv);
    QTranslator qtCHN;
    qtCHN.load("imhqttr_chn.qm");
    imhapp.installTranslator(&qtCHN);
    //QTextCodec *codec = QTextCodec::codecForName("GBK");
    //QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);
    IMHMainWindow imhmw;
    imhmw.setWindowTitle(QObject::tr("imh"));
    imhmw.resize(800,600);
    imhmw.show();

    return imhapp.exec();
}

