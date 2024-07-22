#include "IcoSphere.h"

IcoSphere::IcoSphere(float r, unsigned int subdiv):
	radius(r),subdivision(subdiv),from(1),to(subdiv) {
	unsigned int i, j;
	float rad_i, rad_j;
	
	for (j = 0; j <= 3; ++j) {
		rad_j = - mpie/2.0 + (float)(mpie*j/(3.0));
		    for (i = 0; i < 5; ++i) {
				rad_i = -mpie + ((float)2 * mpie*i) / (5.0)  + rad_j/2.0 ;	
				icoCoords.push_back(vec3(
						radius*cos(rad_j)*cos(rad_i),
						radius*sin(rad_j),
						radius*cos(rad_j)*sin(rad_i)));

			}
	}
	createSubDivision(from,to);

}

void IcoSphere::inceaseSubOrder(int from,int to){
	this->from=subdivision;
	this->to=to;
	subdivision=this->to;
	createSubDivision(this->from,this->to);

}
void IcoSphere::getCoords(float *& arr){
   unsigned int i=0;
   for (vec3 elem : icoCoords) {
		arr[3 * i + 0] = elem[0];
		arr[3 * i + 1] = elem[1];
		arr[3 * i + 2] = elem[2];
		++i;
	}
}
unsigned int IcoSphere::getListSize(){
	return icoCoords.size();
}
unsigned int IcoSphere::getIndexBufferSize() {
	return (1<< 2* (subdivision - 1)) * 20 * 3;
}

void IcoSphere::getOffset(unsigned int sub) {
	unsigned int i, j,k;
	for (i = 1; i <= sub; ++i) {
		unsigned int zpow=(1<<(i-1));
		pars.push_back(glm::vec3(zpow, zpow + 1, zpow));
	}
	vParam = pars[sub - 1][0] + pars[sub - 1][1] + pars[sub - 1][2];
	if (sub > 1) {
		vPrev = pars[sub - 2][0] + pars[sub - 2][1] + pars[sub - 2][2];
		unsigned int up, mid;
		up  = pars[sub - 1][0];
		mid = pars[sub - 1][1] + up;
		offset.push_back(5); 
		j = 1;
		for (i = 1; i < up;++i) {
			k = 5 * j; ++j;
			offset.push_back(offset[i-1]+k);
		}
		k = 5 * j;
		for (i = up; i <mid ; ++i) {
			offset.push_back(offset[i - 1] + k);
		}
		
		for (i = mid; i < vParam-1; ++i) { 				
			--j; k = 5 * j;
			offset.push_back(offset[i - 1] + k);
			
		}
		offset.insert(offset.begin(), 0);
	}
	else {
		vPrev = 2;
		vParam = 4;
		offset.push_back(  0);
		offset.push_back(  5);
		offset.push_back( 10);
		offset.push_back( 15);
	
	}
	
}

void IcoSphere::topBand(unsigned int sub, unsigned int up, unsigned int mid)
{
	unsigned int i, j, low, high, next;
	up = pars[sub - 1][0];
	mid = up + pars[sub - 1][1];
	//start
	for (i = 0; i < 5; ++i) {
		subdivCoords.push_back(icoCoords[i]);
		nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[i + 5]));
	}
	//up		
	for (j = 1; j < up; ++j) {
		low = offset[j];
		high = offset[j + 1];
		for (i = low; i < high; ++i) {
			subdivCoords.push_back(icoCoords[i]);
			if (i + 1 != high) {
				subdivCoords.push_back(normalize(icoCoords[i] + icoCoords[i + 1]));
			}
			else {
				subdivCoords.push_back(normalize(icoCoords[i] + icoCoords[low]));
			}
		}
	}
	for (j = 1; j < up; ++j) {
		low   = offset[j];
		high  = offset[j + 1];
		next  = offset[j + 1] + 1;
		nwParCoords.push_back(normalize(icoCoords[low] + icoCoords[high]));
		nwParCoords.push_back(normalize(icoCoords[low] + icoCoords[high + 1]));
		for (i = low + 1; i < high; ++i) {
			nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[next]));
			 ++next;
			nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[next]));
			if ((i-low)%j==0) {
					++next;
					nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[next]));
			}
		}
		low = offset[j];
		next = offset[j + 2];
		nwParCoords.push_back(normalize(icoCoords[low] + icoCoords[next - 1]));

	}
}
void IcoSphere::bottomBand(unsigned int sub,unsigned int up, unsigned int mid)
{
	unsigned int i, j, low, high,next,clock;
	up = pars[sub - 1][0];
	mid = up + pars[sub - 1][1];

	//bottom
	for (j = mid; j < vParam - 1; ++j) {
		low = offset[j];
		high = offset[j + 1];
		for (i = low; i < high; ++i) {
			subdivCoords.push_back(icoCoords[i]);
			if (i + 1 != high) {
				subdivCoords.push_back(normalize(icoCoords[i] + icoCoords[i + 1]));
			}
			else {
				subdivCoords.push_back(normalize(icoCoords[i] + icoCoords[low]));
			}
		}
	}

	clock = vParam - mid;
	for (j = mid-1 ; j < vParam - 2; ++j) {
		low = offset[j];
		high = offset[j+1];
		next = offset[j+1];
		
		nwParCoords.push_back(normalize(icoCoords[next] + icoCoords[low]));
		for (i = low+1; i < high ; ++i) {
			
			nwParCoords.push_back(normalize(icoCoords[i]+icoCoords[next]));
			if ((i-low)%clock!=0) {
				++next;
				if(next!=offset[j+2]){
					nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[next]));
				}else{
					nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[high]));
				}
			}
		}
		--clock;
	}
	//end
	for (i = icoCoords.size() - 5; i < icoCoords.size(); ++i) {
		subdivCoords.push_back(icoCoords[i]);
		nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[i - 5]));
	}

}

void IcoSphere::middleBand(unsigned int sub,unsigned int up,unsigned int mid)
{

	unsigned int i, j, low, high, next;
	
	//middle
	for (j = up; j < mid; ++j) {
		low = offset[j];
		high = offset[j + 1];
		for (i = low; i < high; ++i) {

			subdivCoords.push_back(icoCoords[i]);
			if (i + 1 != high) {
				subdivCoords.push_back(normalize(icoCoords[i] + icoCoords[i + 1]));
			}
			else {
				subdivCoords.push_back(normalize(icoCoords[i] + icoCoords[low]));
			}
		}
	}
	for (j = up; j < mid -1; ++j) {
		low = offset[j];
		high = offset[j + 1];
		next = offset[j + 1];
		for (i = low; i < high; ++i) {
			
			nwParCoords.push_back(normalize(icoCoords[i] + icoCoords[next]));
			if (i + 1 != high) {
				nwParCoords.push_back(normalize(icoCoords[i + 1] + icoCoords[next]));
			}else{
				
			}++next;
		}
		low=offset[j];
		next=offset[j+2]-1;
		nwParCoords.push_back(normalize(icoCoords[low] + icoCoords[next]));
	}

}


void IcoSphere::mergeSubNw(unsigned int sub){
	unsigned int i,j,l,k,high,low;
    bool subStart=true;
	l=0;k=0;
	unsigned int tmp=vParam;
	offset.clear();
	pars.clear();
	getOffset(sub+1);
	for(j=0;j<vParam-1;++j){
		low=offset[j];
		high=offset[j+1];
		for(i=0;i<high-low;++i){
			if(subStart){
				icoCoords.push_back(subdivCoords[k]);++k;
			}else{
				icoCoords.push_back(nwParCoords[l]);++l;
			}
		}subStart=(!subStart);
	}
	for (i = subdivCoords.size() - 5; i < subdivCoords.size();++i) {
		icoCoords.push_back(subdivCoords[i]);
	}

}
void IcoSphere::createSubDivision(int from,int to){
	unsigned int sub,up ,mid;
	for (sub = from; sub < to;++sub) {
		
		getOffset(sub);
		up = pars[sub - 1][0];
		mid = up + pars[sub - 1][1];
		topBand(sub,up,mid);
		middleBand(sub,up, mid);
		bottomBand(sub, up, mid);
		icoCoords.clear();
		mergeSubNw(sub);
		 
		subdivCoords.clear();
		nwParCoords.clear();
		pars.clear();
		offset.clear();	
	}
}

void IcoSphere::genIndexBuffer(unsigned int *& indices){
	unsigned int i,j,low,high,prev,next,up,mid,clock,l1,l2,k=0;
	getOffset(subdivision);
	up = pars[subdivision-1][0];
	mid = up + pars[subdivision-1][1];
	//up
	for (j = 1; j < up + 1; ++j) {
		low = offset[j];
		high = offset[j + 1];
		prev = offset[j - 1];
		next = (j == up) ? offset[j + 1] : offset[j + 1] + 1;
		l1 = 0;
		for (i = low; i < high; ++i) {
			indices[6 * k + 0] = i;
			repeatStep(prev, j, i, low);
			indices[6 * k + 1] = prev;
			indices[6 * k + 2] = i + 1;
			indices[6 * k + 3] = i;
			indices[6 * k + 4] = next;
			if (j != up) {
				skipStep(next, j, high);
			}
			else { ++next; }
			indices[6 * k + 5] = i + 1;
			if (i + 1 == high) {
				indices[6 * k + 1] = offset[j - 1];
				indices[6 * k + 2] = low;
				indices[6 * k + 5] = low;
			}

			++k;
		}
	}
	//middle
	for (j = up+1 ; j < mid - 1;++j) {
		low = offset[j];
		high = offset[j + 1];
		prev = offset[j - 1] + 1;
		next = offset[j + 1];
		for (i = low; i < high;++i) {
			indices[6 * k + 0] = i;
			indices[6 * k + 1] = prev; 
			++prev;
			indices[6 * k + 2] = i+1;
			indices[6 * k + 3] = i;
			indices[6 * k + 4] = next; 
			++next;
			indices[6 * k + 5] = i+1;
			if (i+1==high) {
				
				indices[6 * k + 1] = offset[j-1];
				indices[6 * k + 2] = low;
				indices[6 * k + 5] = low;
			}++k;
		}
	}
	
	//bottom
	clock = vParam  - mid;
	for (j = mid-1; j < vParam-1; ++j) {
		low = offset[j];
		high = offset[j + 1];
		prev = offset[j - 1] + 1;
		next = offset[j + 1];
		for (i = low; i < high; ++i) {
			indices[6 * k + 0] = i;

			indices[6 * k + 1] = prev;
			if (j!=mid-1) {
				bottomSkip(prev,clock,i,low);
			}
			else {
				++prev;
			}

			indices[6 * k + 2] = i + 1;
			indices[6 * k + 3] = i;

			if (clock!=1) {
				repeatStep(next, clock, i, low);
			}
			indices[6 * k + 4] = next;
			if (clock==1) {
				++next;
			}
			indices[6 * k + 5] = i + 1;
			if (i + 1 == high) {
				if (j == mid - 1) {
					indices[6 * k + 1] = offset[j - 1];
				}
				indices[6 * k + 2] = low;
				indices[6 * k + 4] = offset[j + 1] ;
				indices[6 * k + 5] = low;
			}
			++k;
			}
		--clock;
	}
	
	offset.clear();
	pars.clear();
}



void IcoSphere::getTexCoords(float *& arr){
	
	unsigned int sub,up,mid,i,j,low,high,l1,k=0;
	float radi,radj,l;

	up = pars[subdivision-1][0];
	mid = up + pars[subdivision-1][1];
	unsigned int tmp=mid+up;
	unsigned int start=1;
	
	l=0.0f;
	//up
	for(j=0;j<up;++j){
		radj=1.0-(float)(j/(vParam-1.0));
		low=offset[j];
		high=offset[j+1];
		l1=1;
		l=1/11.0-(float)(j/(11.0*(1<<(subdivision-1))));
		for(i=0;i<high-low;++i){
			radi = l>1.0f?l-1.0f:l;
			arr[2*k]   = radi;
			arr[2*k+1] = radj;
			++k;
			if(j!=0){
				if(i==0||i%j!=0){
					l+=((float)2/(11*(1<<(subdivision-1))));
				}else{
					l1+=2;
					l=l1/11.0-(float)(j/(11*(1<<(subdivision-1))));
				}
			}else{
				l+=(2/11.0);
			}
		}
	}
	sub=subdivision;
	//middle
	l1=0;
	for(j=up;j<mid;++j){
		radj=1.0f - (float)(j/(vParam-1.0));
		low=offset[j];
		l1=(j-up);
		high=offset[j+1];
		for(i=0;i<high-low;++i){
			radi=(float)(i+l1)/((11*(1<<(sub-1))));
			arr[2*k]  =radi;
			arr[2*k+1]=radj;
			++k;++l1;
		}

	}
	offset.push_back(offset[vParam-1]+6);
	
	//bottom
	for(j=mid;j<tmp;++j){
		radj=1.0-(float)(j/(vParam-1.0));
		low=offset[j];
		high=offset[j+1];
		l1=(j==tmp-1)?2:1;
		l=1/11.0+(float)((j-mid+1)/(11.0*(1<<(subdivision-1))));
		for(i=0;i<high-low;++i){
			radi = l>1.0f?l-1.0f:l;
			arr[2*k]=radi;
			arr[2*k+1]=radj;
			++k;
			if(j!=tmp-1){
				if(i==0 || i%(tmp-j-1)!=0){
					l+=((float)2/(11*(1<<(subdivision-1))));
				}else{
					l1+=2;
					l=l1/11.0+(float)((j-mid+1)/(11*(1<<(subdivision-1))));
				}
			}else{
				l+=(2/11.0);
			}
		}
	}
	offset.pop_back();
	offset.clear();


}


float IcoSphere::transform(float t, float a, float b, float c, float d) {
	return c + (d - c) * (t - a) / (b - a);
}

void IcoSphere::skipStep(unsigned int & next, unsigned int & j, unsigned int high)
{
	if ((next - high + 1) % (j + 1) == 0) {
		next += 2;
	}
	else {
		++next;
	}
}


void IcoSphere::repeatStep(unsigned int & prev, unsigned int j,  unsigned int i, unsigned int low) {
	if (j==1) {
		++prev;
	}
	else {
		if ((i-low)%j!=0) {
			++prev;
		}
		
	}
	
}
void IcoSphere::bottomSkip(unsigned int & prev, unsigned int j,unsigned int i, unsigned int & low) {
	if ((low - prev -1) % (j+1) == 0) {
		prev += 2;
	}
	else {
		++prev;
	}
}

IcoSphere::~IcoSphere(){
	icoCoords.clear();
	subdivCoords.clear();
}
