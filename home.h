#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QtWidgets>
#include <QString>
#include <QApplication>
#include <QtWidgets>
#include<iostream>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/core.hpp"
#include <opencv2/core/core.hpp>

#include <fstream>
#include <sstream>

class Home;

using namespace std;
using namespace cv;
using namespace cv::face;

void detectAndDisplay(Mat frame,  QSpinBox *rollEdit);
static void dbread(vector<Mat> &images, vector<int> &labels);
void eigenFaceTrainer();
void faceRecognition( QString file, Home *home, QString cf, QComboBox *comboBox, QSpinBox *batchEdit,QString attendanceFile, QDateEdit *today);
void addFace(int i, QSpinBox *rollEdit, QString classFolder, QString vidPath);
void mark_attendance(QString file, Home *home, QString cf, QComboBox *comboBox, QSpinBox *batchEdit,int label);



class Home : public QMainWindow
{
    Q_OBJECT
private:
    QMainWindow *menu;
    QWidget *addStudentPage;

    QLineEdit *nameEdit;
    QSpinBox *rollEdit;
    QComboBox *comboBox;
    QSpinBox *batchEdit;

    QString dir = "/home/teja/Mark/mark/Database/";

    QString classFolder;
    QString faceFolder;


public:
    QString today;
    bool present =false;
    Home();
    ~Home();
    QString Label;

private slots:
    void showMenu();
    void dateChanged(const QDate &date);
    void addFaceButton(QString classFolder, QString faceFolder, QSpinBox *rollEdit);
    void recognizeFaceButton();
    void showMessage(const QString &message);
    void addStudentButton();
    void deleteStudentButton();
    void webCamButton(QSpinBox *rollEdit, QString classFolder);
    void videoPathButton(QSpinBox *rollEdit, QString classFolder);
};

#endif // HOME_H
