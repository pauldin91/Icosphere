#ifndef ICOSPHERE_H
#define ICOSPHERE_H
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
using namespace glm;
class IcoSphere{
	unsigned int subdivision;
	unsigned int from;
	unsigned int to;
	std::vector<unsigned int> offset;
	float radius;
	unsigned int uiPow(unsigned int,unsigned int);
	const float mpie = 3.141592f;
	std::vector<glm::vec3> icoCoords;
	std::vector<glm::vec3> subdivCoords;
	std::vector<glm::vec3> nwParCoords;
	std::vector<glm::vec3> pars;
	const unsigned int uParam = 5;
	unsigned int vParam,vPrev;
	void repeatStep(unsigned int &,unsigned int, unsigned int, unsigned int);
	void bottomSkip(unsigned int &, unsigned int, unsigned int, unsigned int &);
	void skipStep  (unsigned int &,unsigned int &, unsigned int);
	void getOffset(unsigned int);
	void bottomBand(unsigned int, unsigned int , unsigned int);
	void middleBand(unsigned int, unsigned int , unsigned int);
	void topBand(unsigned int, unsigned int , unsigned int);
	void mergeSubNw(unsigned int);
	void createSubDivision(int,int);
public:
	IcoSphere(float,unsigned int);
	void inceaseSubOrder(int,int);
	void genIndexBuffer(unsigned int*& );
	unsigned int getIndexBufferSize();
	void getTexCoords(float *& arr);
	void getCoords(float *& arr);
	unsigned int getListSize();
	float transform(float, float, float, float, float);
	~IcoSphere();




};




#endif //ICOSPHERE_H