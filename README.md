# Attendance Marker using Facial Recognition
A C++ program that uses facial recognition technology to mark attendance for events, classes, or meetings. It comes with a graphical user interface (GUI) for ease of use.

# Requirements
OpenCV library for facial recognition.

Qt for GUI design and development.

# Features
  Ability to add new faces to the database for recognition.
  
  Automatic recognition of registered faces.
  
  GUI for easy attendance marking and management.
  
  Records attendance in a Text file for later analysis.
# Installation
>Windows

    Install OpenCV and Qt on your system.
    
    Clone or download this repository to your local system.
    
    Open the project in Qt and build the project.
    
    Run the executable file to start the program.
>Linux

    Install OpenCV and Qt using the terminal by executing the following commands:
      sudo apt-get install libopencv-dev
      sudo apt-get install qt6-default
      
    Clone or download this repository to your local system.
    
    Open the project in Qt and build the project.
    
    Run the executable file to start the program.
# Usage
  Start the program and navigate to the "Add Student" tab.
  
  Add Student data to the database and then add face data by clicking the "Add face" button and capturing images of the individual by using any of two options present:
  1. WebCam 
  
  2. Video (In case of the this option video should be already predent in database folder and it's relative path should be entered when prompted)
  
  Navigate to the "Mark attendance" tab to start the facial recognition process.
  
  Registered faces will be automatically recognized and attendance will be marked in real-time.
  
  The attendance records can be viewed in the text file created in the folder of the batch entered automatically.
  
# Limitations
The program may not work as expected with low-quality images or poorly lit environments.

The program may not be able to recognize faces with significant changes (e.g. facial hair, glasses, masks, etc.).

# Team Members
This project was undertaken as a team assignment

Holesh Sharma

Shivam Litoria

M Satya Sai Teja

Ankit Kumar 


# Contributing
If you would like to contribute to the development of this program, please feel free to submit a pull request.


# License
This program is licensed under the MIT License.
