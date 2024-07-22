#ifndef SCENE_H
#define SCENE_H
#include "VertexArray.h"
#include "IcoSphere.h"

class Scene{
    IcoSphere * m_ico;
    VertexArray * m_vr;
    float * m_verts;
    unsigned int * m_inds;
    bool del = false;
    template<typename T> void resizeArray(T *& ,unsigned int);
    void setVertexArray(std::vector<unsigned int>,unsigned int );
  public:
    Scene(float,unsigned int,std::vector<unsigned int> layout);
    void renderScene(float radius,unsigned int & param,unsigned int & current,std::vector<unsigned int> layout);
    unsigned int getTriangles();
    ~Scene();



};

#endif