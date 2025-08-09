#pragma once

class MainWindow 
{
private:
    unsigned short width;
    unsigned short height;
public:
    MainWindow(unsigned short width, unsigned short height);
    ~MainWindow();
    void setup();
    void run();
};
