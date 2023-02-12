#include "home.h"

bool isFaceAdded = false;
using namespace std;

void Home::dateChanged(const QDate &date)
{
    today = date.toString("dd-MM-yyyy");
}

void Home:: showMessage(const QString &message)
{
    QWidget *messagePage = new QWidget;
    messagePage->setWindowTitle("Message");
    messagePage->setFixedSize(339, 599);

    QLabel *bg = new QLabel(messagePage);
    bg->setPixmap(QPixmap("bg(2).png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *label = new QLabel(message);
    QFont font = label->font();
    font.setPointSize(16);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *backButton = new QPushButton("Back");
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, [this, messagePage]()
    {
        this->hide();
        menu->show();
        messagePage->hide();
    });

    messagePage->setLayout(layout);
    messagePage->show();
    addStudentPage->hide();
    menu->hide();
}

void Home:: videoPathButton(QSpinBox *rollEdit, QString classFolder)
{
    QWidget *videoPathPage = new QWidget;
    videoPathPage->setWindowTitle("Add Video");
    videoPathPage->setFixedSize(339, 599);

    QLabel *bg = new QLabel(videoPathPage);
    bg->setPixmap(QPixmap("bg(2).png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *videoLabel = new QLabel(videoPathPage);
    QLineEdit *vidname = new QLineEdit;
    layout->addWidget(videoLabel);
    layout->addWidget(vidname);

    cout<<vidname->text().toStdString();
    QPushButton *submitButton = new QPushButton("Submit");
    layout->addWidget(submitButton);

    QPushButton *back = new QPushButton("Back");
    layout->addWidget(back);

    connect(back, &QPushButton::clicked, [this, videoPathPage]()
    {
        this->hide();
        addStudentPage->show();
        videoPathPage->close();
    });

    connect(submitButton, &QPushButton::clicked, [this, vidname, classFolder, videoPathPage, rollEdit]()
    {
        QString vidPath = vidname->text();
        QString vidFolder = classFolder + "video";
        QString full = vidFolder + "/" + vidPath;

        QFile vidFile(full);

        QDir vdir;

        if(vdir.exists(vidFolder))
        {
            if(vidFile.exists())
            {
                //attendance = false;
                addFace(1, rollEdit, classFolder, vidPath);
            }
            else
            {
                videoPathPage->close();
                this->showMessage("Video Does'nt Exist");
            }
        }
        else
        {
            videoPathPage->close();
            this->showMessage("Directory Does'nt Exist");
        }
    });

    videoPathPage->setLayout(layout);
    videoPathPage->show();
}

void Home:: webCamButton(QSpinBox *rollEdit, QString classFolder)
{
    //attendance = false;
    addFace(2, rollEdit, classFolder, "/video");
    eigenFaceTrainer();

    isFaceAdded = true;
}

void Home:: addFaceButton(QString classFolder, QString faceFolder, QSpinBox *rollEdit)
{
    addStudentPage->close();

    QMainWindow *addFacePage = new QMainWindow;
    addFacePage->setWindowTitle("Add Face");
    addFacePage->setFixedSize(339, 599);

    QLabel *bg = new QLabel(addFacePage);
    bg->setPixmap(QPixmap("bg(2).png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    int roll = rollEdit->value();

    if(roll >=1 && roll < 10)
    {
        rollEdit->setPrefix("0");
    }

    QDir cdir, fdir;
    qDebug()<<classFolder;
    if(!cdir.exists(classFolder))
    {
        cdir.mkpath(classFolder);

        if(!fdir.exists(faceFolder))
        {
            fdir.mkpath(faceFolder);
        }

        classFolder += "/";
    }
    else
    {

        if(!fdir.exists(faceFolder))
        {
            fdir.mkpath(faceFolder);
        }

        classFolder += "/";
    }

    QPushButton *video = new QPushButton("Video", addFacePage);
    video->setGeometry(110, 150, 120, 30);

    QPushButton *webCam = new QPushButton("Web Cam", addFacePage);
    webCam->setGeometry(110, 250, 120, 30);

    QPushButton *back = new QPushButton("Back", addFacePage);
    back->setGeometry(110, 350, 120, 30);

    addFacePage->show();
    menu->close();

    connect(video, &QPushButton::clicked, [this, rollEdit, classFolder, addFacePage](bool)
    {
        addFacePage->close();
        this->videoPathButton(rollEdit, classFolder);
        isFaceAdded=true;
    });

    connect(webCam, &QPushButton::clicked, [this, rollEdit, classFolder](bool)
    {
        this->webCamButton(rollEdit, classFolder);
        isFaceAdded=true;
    });


    connect(back, &QPushButton::clicked, [this, addFacePage]()
    {
        this->hide();
        addStudentPage->show();
        addFacePage->close();
    });
}

void Home:: recognizeFaceButton()
{
    QWidget *recognizeFacePage = new QWidget;
    recognizeFacePage->setWindowTitle("Recognize Face");
    recognizeFacePage->setFixedSize(339, 599);

    QLabel *bg = new QLabel(recognizeFacePage);
    bg->setPixmap(QPixmap("bg(2).png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *label1 = new QLabel("Branch");
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("CSE");
    comboBox->addItem("ECE");
    layout->addWidget(label1);
    layout->addWidget(comboBox);

    QLabel *label2 = new QLabel("Batch");
    QSpinBox *batchEdit = new QSpinBox;
    batchEdit ->setRange(2018, 2099);
    layout->addWidget(label2);
    layout->addWidget(batchEdit);

    QLabel *dateLabel = new QLabel();
    QDateEdit *dateEdit = new QDateEdit;
    dateEdit->setDisplayFormat("dd-MM-yyyy");
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);

    layout->addWidget(new QLabel("Enter date : "));
    layout->addWidget(dateEdit);
    layout->addWidget(dateLabel);

    QPushButton *markButton = new QPushButton("Mark");
    layout->addWidget(markButton);

    QPushButton *back = new QPushButton("Back");
    layout->addWidget(back);

    connect(dateEdit, &QDateEdit::dateChanged, this, &Home::dateChanged);

    QFrame* qframe = new QFrame(this);
    qframe->setGeometry(0, 0, 500, 500);

    connect(markButton, &QPushButton::clicked, [this, qframe, comboBox, batchEdit, dateEdit, recognizeFacePage](bool)
    {
        QString cf =  dir + comboBox->currentText() + batchEdit->text();
        QString ff = cf + "/faces";

        qDebug() <<cf;

        QDir dir(ff);
        QString attendanceFile=cf+"/"+dateEdit->text()+".txt";

        if(dir.exists())
        {
          QString filename = cf + "/eigenface.yml";
          QFile file(filename);

          if(file.exists())
          {
              faceRecognition(filename, this, cf, comboBox, batchEdit, attendanceFile, dateEdit);
          }
          else
          {
              recognizeFacePage->close();
              this->showMessage("Data Not Trained Yet");
          }

        }
        else
        {
            recognizeFacePage->close();
            this->showMessage("Directory Does'nt Exist");
        }
    });

    connect(back, &QPushButton::clicked, [this, qframe, recognizeFacePage]()
    {
        this->hide();
        menu->show();
        qframe->close();
        recognizeFacePage->close();
    });

    recognizeFacePage->setLayout(layout);
    recognizeFacePage->show();
    menu->close();
}

void Home:: addStudentButton()
{
    addStudentPage = new QWidget;
    addStudentPage->setWindowTitle("Add Student");
    addStudentPage->setFixedSize(339, 559);

    QLabel *bg = new QLabel(addStudentPage);
    bg->setPixmap(QPixmap("bg(2).png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *label1 = new QLabel("Student Name");
    QLineEdit *nameEdit = new QLineEdit;
    layout->addWidget(label1);
    layout->addWidget(nameEdit);

    QLabel *label2 = new QLabel("Roll Number");
    QSpinBox *rollEdit = new QSpinBox;
    rollEdit->setRange(1, 99);
    layout->addWidget(label2);
    layout->addWidget(rollEdit);

    QLabel *label3 = new QLabel("Branch");
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("CSE");
    comboBox->addItem("ECE");
    layout->addWidget(label3);
    layout->addWidget(comboBox);

    QLabel *label4 = new QLabel("Batch");
    QSpinBox *batchEdit = new QSpinBox;
    batchEdit->setRange(2018, 2099);
    layout->addWidget(label4);
    layout->addWidget(batchEdit);

    QPushButton *addFace = new QPushButton("Add Face");
    layout->addWidget(addFace);

    QPushButton *submitButton = new QPushButton("Submit");
    layout->addWidget(submitButton);

    QPushButton *backButton = new QPushButton("Back");
    layout->addWidget(backButton);

    connect(addFace, &QPushButton::clicked, [this, comboBox, batchEdit, rollEdit](bool)
    {
        QString classFolder = dir + comboBox->currentText() + batchEdit->text();
        QString faceFolder = classFolder + "/faces";
        this->addFaceButton(classFolder, faceFolder, rollEdit);
    });

    connect(submitButton, &QPushButton::clicked, [this, comboBox, batchEdit, nameEdit, rollEdit]()
    {
        if(isFaceAdded)
        {
            QString cf = dir + comboBox->currentText() + batchEdit->text() + "/";
            QString filename = cf + comboBox->currentText() + batchEdit->text() + ".txt";
            cout<<filename.toStdString();
            QFile file(filename);

            if (file.open(QIODevice::Append | QIODevice::Text))
            {
                QTextStream stream(&file);
                stream << nameEdit->text() << " " << rollEdit->text() << " " << batchEdit->text() << " " << comboBox->currentText() << " 0" << "\n";
                file.close();
                addStudentPage->close();
                this->showMessage("Data Entered Successfully!!");
            }


            isFaceAdded = false;
        }
        else
        {
            addStudentPage->close();
            this->showMessage("Add Face To Save The Data!!");
        }
    });


    connect(backButton, &QPushButton::clicked, [this]()
    {
        this->hide();
        menu->show();
        addStudentPage->close();
    });

    addStudentPage->setLayout(layout);
    addStudentPage->show();
    menu->close();
}

void Home:: deleteStudentButton()
{
    QWidget *deleteStudentPage = new QWidget;
    deleteStudentPage->setWindowTitle("Delete Student");
    deleteStudentPage->setFixedSize(339, 599);

    QLabel *bg = new QLabel(deleteStudentPage);
    bg->setPixmap(QPixmap("bg(2).png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *label1 = new QLabel("Roll Number");
    QSpinBox *rollEdit = new QSpinBox;
    rollEdit->setRange(1, 99);
    layout->addWidget(label1);
    layout->addWidget(rollEdit);

    QLabel *label2 = new QLabel("Branch");
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("CSE");
    comboBox->addItem("ECE");
    layout->addWidget(label2);
    layout->addWidget(comboBox);

    QLabel *label3 = new QLabel("Batch");
    QSpinBox *batchEdit = new QSpinBox;
    batchEdit ->setRange(2000, 2099);
    layout->addWidget(label3);
    layout->addWidget(batchEdit);

    QPushButton *deleteButton = new QPushButton("Delete");
    layout->addWidget(deleteButton);

    QPushButton *back = new QPushButton("Back", deleteStudentPage);
    layout->addWidget(back);

    connect(deleteButton, &QPushButton::clicked, [this, comboBox, batchEdit, rollEdit, deleteStudentPage]()
    {
        QString classFolder =  dir + comboBox->currentText() + batchEdit->text();
        QString faceFolder = classFolder + "/faces";

        QString filename = classFolder + "/" + comboBox->currentText() + batchEdit->text()  + ".txt";

        QFile file(filename);

        int roll = rollEdit->value();

        if(roll >=1 && roll < 10)
        {
            rollEdit->setPrefix("0");
        }

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QStringList students;
            QTextStream stream(&file);
            while (!stream.atEnd())
            {
                students << stream.readLine();
            }

            for (int i = 0; i < students.size(); i++)
            {
                QStringList fields = students[i].split(" ");

                int j = 0;

                while(j < fields.size())
                {
                    if (fields[j] == rollEdit->text())
                    {
                        students.removeAt(i);
                        break;
                    }
                    else
                    {
                        j++;
                    }
                }

            }

            file.close();

            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream stream(&file);
                for (const QString &student : students) {
                    stream << student << "\n";
                }
            }
        }

        file.close();

        QDir dir(faceFolder);

        if(dir.exists())
        {
            QStringList filters;

            filters << rollEdit->text() + "*.jpg";

            dir.setNameFilters(filters);

            QStringList imageFiles = dir.entryList();

            for(int i=0; i < imageFiles.size(); i++)
            {
                QFile file(dir.path() + "/" + imageFiles.at(i));
                file.remove();
            }
            eigenFaceTrainer();

            deleteStudentPage->close();
            this->showMessage("Data Deleted Successfully");
        }
        else
        {
        deleteStudentPage->close();
        this->showMessage("Directory not available");
        }
    });

    connect(back, &QPushButton::clicked, [this, deleteStudentPage]()
    {
        this->hide();
        menu->show();
        deleteStudentPage->close();

    });

    deleteStudentPage->setLayout(layout);
    deleteStudentPage->show();
    menu->close();
}

void Home:: showMenu()
{
    menu = new QMainWindow;
    menu->setWindowTitle("Menu");
    menu->setFixedSize(339, 599);

    QLabel *bg = new QLabel(menu);
    bg->setPixmap(QPixmap("bg(2).png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    QPushButton *recognizeFace = new QPushButton("Attendance", menu);
    recognizeFace->setGeometry(110, 100, 120, 30);

    QPushButton *addStudent = new QPushButton("Add Student", menu);
    addStudent->setGeometry(110, 200, 120, 30);

    QPushButton *deleteStudent = new QPushButton("Delete Student", menu);
    deleteStudent->setGeometry(110, 300, 120, 30);

    QPushButton *back = new QPushButton("Back", menu);
    back->setGeometry(110, 400, 120, 30);

    connect(recognizeFace, &QPushButton::clicked, this, &Home::recognizeFaceButton);

    connect(back, &QPushButton::clicked, [this]()
    {
        this->show();
        menu->close();
    });

    connect(addStudent, &QPushButton::clicked, this, &Home::addStudentButton);

    connect(deleteStudent, &QPushButton::clicked, this, &Home::deleteStudentButton);

    this->hide();
    menu->show();
}

Home:: Home()
{
    setWindowTitle("Home");
    setFixedSize(339, 599);

    QLabel *bg = new QLabel(this);
    bg->setPixmap(QPixmap("bg.png"));
    bg->setScaledContents(true);
    bg->resize(339, 599);

    QPushButton *enter = new QPushButton("Enter", this);
    enter->setGeometry(120, 380, 100, 30);

    connect(enter, &QPushButton::clicked, this, &Home::showMenu);
}

Home::~Home()
{
}
