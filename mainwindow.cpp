#include "mainwindow.h"
#include <cmath>
#include <string>
#include <thread>
#include <chrono>

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
}

void MainWindow::initializeGL(){
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    std::cout<<"commands: "<<std::endl
             <<"wasd - rotate figure"<<std::endl
             <<"arows - rotate light"<<std::endl
             <<"N, M - aproximize"<<std::endl
             <<"-, + - scale"<<std::endl
             <<"H - animation"<<std::endl
             <<"J - visible"<<std::endl
             <<"K - edges"<<std::endl
             <<"L - light"<<std::endl;
    create_mesh(aprox);
}

void MainWindow::resizeGL(int width, int height){
    if(height==0)
        height=1;

    int min = std::min(width, height);
    glViewport((width-min)/2, (height-min)/2, min, min);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::drawMesh(){
if(edges){
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_TRIANGLES);
    glColor4d(0.0, 0.8, 0.0, 1.0);
    for( const auto& pol : polygons)
        for(auto& v : pol)
        glVertex4dv(&vertexes[v][0]);
    glEnd();
}
    glLineWidth(5);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    glColor4d(0.0, 0.5, 0.0, 1.0);
    for( const auto& pol : polygons){
        for(const auto& v : pol){
            glVertex4dv(&vertexes[v][0]);
        }
    }
    glEnd();
}

void MainWindow::drawAxis(){
    glBegin(GL_LINES);
    glColor4d(1.0, 0.0, 0.0, 1.0);
        glVertex4d(0,0,0,1);
        glVertex4d(0.5, 0,0,1);
    glEnd();

    glBegin(GL_LINES);
    glColor4d(0.0, 1.0, 0.0, 1.0);
        glVertex4d(0,0,0,1);
        glVertex4d(0,0.5,0,1);
    glEnd();

    glBegin(GL_LINES);
    glColor4d(0.0, 0.0, 1.0, 1.0);
        glVertex4d(0,0,0,1);
        glVertex4d(0,0,0.5,1);
    glEnd();

}

void MainWindow::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(visible || edges)
        glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    if(light && visible && edges)
        glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    GLfloat  v3[4] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, v3);
    GLfloat  v2[4] = {0.9, 0.9, 0.9, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, v2);
    GLfloat  v1[4] = {0.2, 0.2, 0.2, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, v1);

    if(light && visible && edges){
       glEnable(GL_LIGHTING);
       glEnable(GL_LIGHT0);
    }
    GLfloat pos[4] = {2*std::sin(a)*std::cos(b),
                      2*std::sin(b),
                      2*std::cos(a)*std::cos(b), 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glScalef(scale, scale, scale);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);



    drawAxis();
    drawMesh();

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

}

void MainWindow::create_mesh(unsigned int steps){
    vertexes.clear();
    polygons.clear();
    if(steps<=2){
        vertexes.push_back({ 0,  0,  0.5, 1.0});
        vertexes.push_back({0,  0,  0.5, 1.0});
        vertexes.push_back({0, 0,  0.5, 1.0});
        vertexes.push_back({0, 0,  0.5, 1.0});

        vertexes.push_back({ 0.5,  0.5, 0, 1.0});
        vertexes.push_back({ 0.5, -0.5, 0, 1.0});
        vertexes.push_back({-0.5, -0.5, 0, 1.0});
        vertexes.push_back({-0.5,  0.5, 0, 1.0});

        polygons.push_back({0, 1, 2}); //front
        polygons.push_back({0, 2, 3});
        polygons.push_back({4, 5, 6}); //back
        polygons.push_back({4, 6, 7});
        polygons.push_back({5, 4, 0}); //right
        polygons.push_back({5, 0, 3});
        polygons.push_back({2, 1, 7}); // left
        polygons.push_back({2, 7, 6});
        polygons.push_back({3, 2, 6}); //bottom
        polygons.push_back({3, 6, 5});
        polygons.push_back({1, 0, 4}); //top
        polygons.push_back({1, 4, 7});
        return;

    }
     float PI= std::atan(1)*4;
     for(double i=0.0; i<PI/2+0.01; i+=PI/steps){
         for(double j=0.0; j<2*PI; j+=2*PI/steps)
             vertexes.push_back({
                                 std::cos(j)*std::cos(i)*0.5,
                                 std::sin(j)*std::cos(i)*0.5,
                                 std::sin(i)*0.5,
                                 1
                               });
     }
     vertexes.push_back({0, 0, 0, 1});

     for(int i=0; i<steps/2; ++i){
         for(int j=0; j<steps; ++j){
             if(i<steps/2-1)
             polygons.push_back({
                                    i*steps+j,
                                    (i+1)*steps + (j+1)%steps,
                                    (i+1)*steps + j
                                });
             polygons.push_back({
                                    i*steps+j,
                                    i*steps + (j+1)%steps,
                                    (i+1)*steps + (j+1)%steps
                                });
         }
     }

     for(int j=0; j<steps; ++j)
         polygons.push_back({ vertexes.size()-1, (j+1)%(steps),  j});
}

void MainWindow::animation(){
    for(int i=0; i<360; ++i){
        yRot+=1;
        updateGL();
        std::this_thread::sleep_for (std::chrono::seconds(1/24));
    }
}

void MainWindow::keyPressEvent(QKeyEvent* k){
    if(k->key()==Qt::Key_Up){
        b+=0.1;
    }
    if(k->key()==Qt::Key_Down){
        b-=0.1;
    }
    if(k->key()==Qt::Key_Right){
        a+=0.1;
    }
    if(k->key()==Qt::Key_Left){
        a-=0.1;
    }
    if(k->key()==Qt::Key_W){
        xRot+=4;
        if(xRot>180)
            xRot-=360;
    }
    if(k->key()==Qt::Key_S){
        xRot-=4;
        if(xRot<-180)
            xRot+=360;
    }
    if(k->key()==Qt::Key_D){
        yRot+=4;
        if(yRot>180)
            yRot-=360;

    }
    if(k->key()==Qt::Key_A){
        yRot-=4;
        if(yRot<-180)
            yRot+=360;
    }
    if(k->key() == Qt::Key_Plus){
        scale*=1.1;
    }
    if(k->key() == Qt::Key_Minus){
        scale/=1.1;
        if(scale<=0.00001)
            scale=1;
    }
    if(k->key() == Qt::Key_N){
        if(aprox>=2)
            aprox-=2;
        create_mesh(aprox);
    }

    if(k->key() == Qt::Key_M){
        aprox+=2;
        create_mesh(aprox);
    }

    if(k->key() == Qt::Key_J){
        visible=!visible;
       // std::cout<<"vis changed"<<std::endl;
    }
    if(k->key() == Qt::Key_K){
        edges=!edges;
        //std::cout<<"edges changed"<<std::endl;
    }
    if(k->key() == Qt::Key_L){
        light=!light;
        edges=true;
        visible = true;
        //std::cout<<"light changed"<<std::endl;
    }
    if(k->key() == Qt::Key_H){
        animation();
    }else{
       updateGL();
    }
}


MainWindow::~MainWindow()
{
}

