#include "home.h"
#include <fstream>
#include <sstream>
#include <QApplication>

CascadeClassifier face_cascade;
string cf;
string ff;
string eigenFile;
string studentFile;
string attendanceFile;
string name;
string videoPath;
string xmlPath = "/home/teja/OpenCV/opencv/data/haarcascades/";

int attendance = 0;

int filenumber = 10;

int array_hash[200] = {0};
bool isClosed = false;

void detectAndDisplay(Mat frame,  QSpinBox *rollEdit)
{
    vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    String folder;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    Rect roi_b;
    Rect roi_c;

    size_t ic = 0;
    size_t ib = 0;

    for (ic = 0; ic < faces.size(); ic++)
    {
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);

        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);

        crop = frame(roi_b);
        resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR);
        cvtColor(crop, gray, COLOR_BGR2GRAY);
        stringstream ssfn;

        folder = ff;

        String name = rollEdit->text().toStdString();

        ssfn << folder.c_str() << name << filenumber << ".jpg";

        folder = ssfn.str();
        imwrite(folder, res);

        filenumber++;

        if(filenumber > 99)
        {
            filenumber = 10;
            break;
        }

        Point pt1(faces[ic].x, faces[ic].y);
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 0, 255), 2, 8, 0);
    }

    if (!crop.empty())
    {
        imshow("Detected", crop);
    }
    else
    {
        destroyWindow("Detected");
    }
}

void addFace(int i,  QSpinBox *rollEdit, QString classFolder, QString path)
{
    QString faceFolder = classFolder + "faces/";
    QString vidpath = classFolder + "video/"+path;

    videoPath = vidpath.toStdString();

    cf = classFolder.toStdString();
    ff = faceFolder.toStdString();

    if (i == 1)
    {
        VideoCapture capture(videoPath);
        if (!capture.isOpened())
            return;

        if (!face_cascade.load(xmlPath + "haarcascade_frontalface_alt.xml"))
        {
            cout << "error" << endl;
            return;
        };

        int frame_counter = 0;

        Mat frame;

        while (frame_counter < 80)
        {
            capture >> frame;

            if (frame.empty())
            {
                break;
            }

            detectAndDisplay(frame, rollEdit);

            frame_counter++;
            sleep(0.1);
        }

        eigenFaceTrainer();
        return;
    }
    else
    {
        int i = 0;
        VideoCapture capture(0);

        if (!capture.isOpened())
        {
            return;
        }

        Mat frame;

        if (!face_cascade.load(xmlPath + "haarcascade_frontalface_alt.xml"))
        {
            cout << "error" << endl;
            return;
        }

        for (;;)
        {
            capture >> frame;

            detectAndDisplay(frame, rollEdit);
            i++;

            if (i == 80)
            {
                cout << "\nFace Added";
                break;
            }

            int c = waitKey(10);

            if (27 == char(c))
            {
                break;
            }
        }

        return;
    }

    faceFolder = classFolder + "faces";
    ff = faceFolder.toStdString();
}

static void dbread(vector<Mat> &images, vector<int> &labels)
{
    vector<cv::String> fn;

    glob(ff, fn, false);

    size_t count = fn.size();

    for (size_t i = 2; i < count; i++)
    {
        string itsname = "";
        char sep = '/';
        size_t j = fn[i].rfind(sep, fn[i].length());
        if (j != string::npos)
        {
            itsname = (fn[i].substr(j + 1, fn[i].length() - j - 6));
        }
        images.push_back(imread(fn[i], 0));
        labels.push_back(atoi(itsname.c_str()));
    }
}

void eigenFaceTrainer()
{
    vector<Mat> images;
    vector<int> labels;

    dbread(images, labels);

    if(images.size() <= 1)
    {
        return;
    }

    cout << "size of the images is " << images.size() << endl;
    cout << "size of the labels is " << labels.size() << endl;
    cout << "Training begins...." << endl;

    Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();

    model->train(images, labels);

    cout << cf;

    model->save(cf + "eigenface.yml");

    cout << "Training finished...." << endl;
    waitKey(10000);
}

void mark_attendance(QString file, Home *home, QString cf, QComboBox *comboBox, QSpinBox *batchEdit,int label){

    ifstream infile;
    ofstream outfile;
    string filename=(cf+"/"+comboBox->currentText()+batchEdit->text()+".txt").toStdString();
    infile.open(filename);
    outfile.open("temp.txt");
    string name, batch, branch;
    int roll_number,attendance;
    while (infile >> name >> roll_number >> batch >> branch >> attendance)
    {
        if (roll_number == label)
        {
            attendance++;
            outfile << name << " " << roll_number << " " << batch << " " << branch << "  " << attendance << "\n";
            attendance=0;
        }
        else
        {
            outfile << name << " " << roll_number << " " << batch << " " << branch << "  " << attendance << "\n";
        }
    }

    infile.close();
    outfile.close();

    remove(filename.c_str());
    rename("temp.txt", filename.c_str());

}

void faceRecognition( QString file, Home *home, QString cf, QComboBox *comboBox, QSpinBox *batchEdit, QString attendanceFile, QDateEdit *dateEdit)
{
    cout << "start recognizing..." << endl;
    Ptr<FaceRecognizer> model = FisherFaceRecognizer::create();

    eigenFile = file.toStdString();

    model->read(eigenFile);

    int img_width = 128;
    int img_height = 128;
    string window = "Capture - face detection";

    if (!face_cascade.load("/home/teja/OpenCV/opencv/data/haarcascades/haarcascade_frontalface_alt.xml"))
    {
        cout << " Error loading file" << endl;
        return;
    }

    VideoCapture cap(0);


    if (!cap.isOpened())
    {
        cout << "exit" << endl;
        return;
    }
    namedWindow(window,1);
    long count = 0;

    while(1)
    {
        vector<Rect> faces;
        Mat frame;
        Mat grayScaleFrame;
        Mat original;
        string temp;
        cap >> frame;
        count = count + 1;

        if (!frame.empty())
        {
            original = frame.clone();

            cvtColor(original, grayScaleFrame, COLOR_BGR2GRAY);

            face_cascade.detectMultiScale(grayScaleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));
            string frameset = to_string(count);
            string faceset = to_string(faces.size());

            for (int i = 0; i < faces.size(); i++)
            {
                Rect face_i = faces[i];
                Mat face = grayScaleFrame(face_i);

                Mat face_resized;
                cv::resize(face, face_resized, Size(img_width, img_height), 1.0, 1.0, INTER_CUBIC);

                int label = -1;
                double confidence = 0;

                model->predict(face_resized, label, confidence);
                label = label/10;

                array_hash[label]++;


                if (array_hash[label] <= 1)
                {
                    home->present=true;
                    mark_attendance(file,home,cf, comboBox, batchEdit,label);


                }
             cout << " confidence " << confidence << " Label: " << label << endl;


                // drawing green rectagle in recognize face
                rectangle(original, face_i, CV_RGB(0, 0, 255), 1);
                //string text = Pname;

                int pos_x = max(face_i.tl().x - 10, 0);
                int pos_y = max(face_i.tl().y - 10, 0);
                string text=to_string(label);
                // name the person who is in the image
                putText(original, text, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
            }

            putText(original, "Frames: " + frameset, Point(30, 60), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
            putText(original, "No. of Persons detected: " + to_string(faces.size()), Point(30, 90), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);

            cv::imshow(window, original);
        }
        if (waitKey(30) == 27)
        {
          // destroyWindow("Web Cam" );
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Home w;
    w.show();

    return a.exec();
}
