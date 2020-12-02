#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGLWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QKeyEvent>

#include <iostream>
#include <vector>

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void drawMesh();
    void drawAxis();
   // void scene();
    void keyPressEvent(QKeyEvent* p);
    void create_mesh(unsigned int ui);
    void animation();
    ~MainWindow();

private:
    std::vector<std::vector<double>> vertexes;
    std::vector<std::vector<unsigned int>> polygons;
    float xRot=0, yRot=0, zRot=0, a=0, b=0;
    float scale=1;
    bool visible=true, edges=true, light=false;
    unsigned int aprox=6;
    QPoint MousePos{0, 0};

};
#endif // MAINWINDOW_H
