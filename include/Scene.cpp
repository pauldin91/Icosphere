#include "Scene.h"

Scene::Scene(float radius,unsigned int sub,std::vector<unsigned int> layout){
    m_ico   = new IcoSphere(radius,sub);
    unsigned int sum = 0;
    for(auto & i : layout){
        sum += i;
    }
    m_verts = new float[sum*m_ico->getListSize()];
    m_inds  = new unsigned int[m_ico->getIndexBufferSize()];
    m_ico->getCoords     (m_verts);
    m_ico->genIndexBuffer(m_inds);
    
    setVertexArray(layout,sum);
    del=true;
}
void Scene::setVertexArray(std::vector<unsigned int> layout,unsigned int sum){
    if(del){
        delete m_vr;
    }

    m_vr    = new VertexArray(m_verts,sum*m_ico->getListSize());
    for(auto & i : layout){
        m_vr->addAttribute(i);
    }
    m_vr->pushLayout();
    m_vr->addIndexBuffer(m_inds,m_ico->getIndexBufferSize());
    m_vr->bind();

}

void Scene::renderScene(float radius,unsigned int & param,unsigned int & current,std::vector<unsigned int> layout){
    
    if(param!=current){
        unsigned int sum = 0;
        for(auto & i : layout){
            sum += i;
        }
        if(param<current){
            m_ico->inceaseSubOrder(param,current);
        }else{
            delete m_ico;
            m_ico = new IcoSphere(radius,current);
            
        }param = current;
        resizeArray(m_verts,sum*m_ico->getListSize());
        resizeArray(m_inds,m_ico->getIndexBufferSize());
        m_ico->getCoords(m_verts);
        m_ico->genIndexBuffer(m_inds);
        setVertexArray(layout,sum);
        
    }else{
        m_vr->bind();
    }

}
unsigned int Scene::getTriangles(){
    return m_ico->getIndexBufferSize();
}
template<typename T>
void Scene::resizeArray( T*& arr,unsigned int siz){
    delete [] arr;
    arr=new T[siz];
}

Scene::~Scene(){
    delete m_ico;
    delete m_vr;
    delete [] m_verts;
    delete [] m_inds;
}

