#include "StdAfx.h"
#include "FitnessClass.h"
#include "DNAstatement.h"
#include ".\evaluatingfunction.h"


CEvaluatingFunction::CEvaluatingFunction(double Rmin, double Rmax):
RangeMin(Rmin), RangeMax(Rmax){

	if(RangeMin >= RangeMax) throw CString(_T("Invalid range at CEvaluatingFunction construction\r\n"));
}

CEvaluatingFunction::~CEvaluatingFunction(void){

	destroyPoints();
}


/*******************************
Admin Functions
*******************************/
void CEvaluatingFunction::destroyPoints(){
	FunctionY.clear();
	FunctionX1.clear();
}


/*******************************
Evaluation Methods
*******************************/
F<double> CEvaluatingFunction::makeBehave(F<double> y){

	if(fabs(y.x()) < TOL_0)  return 0.0f;
	
	if (y.x() > OVERFLOW){
		CString Mssg;
		Mssg.Format("OVERFLOW Exception (Evaluates to %f)", y);
		throw(Mssg);
	}

	if (y.x() < UNDERFLOW){
		CString Mssg;
		Mssg.Format("UNDERFLOW Exception (Evaluates to %f)", y);
		throw Mssg;
	}

	return y;
}
F<double> CEvaluatingFunction::Eval(F<double> Xval){
	
	try{
		F<double> y = (F<double>)1.0 + (F<double>)(-2.0)* Xval + (F<double>)(2.0)*Xval*Xval*Xval;
		//= 1.5 + (-2.0)* Xval + (3.0)*Xval*Xval*Xval; //  
		//= 1.0 + (-2.0)* Xval + (3.0)*Xval*Xval*Xval*Xval; //Not too bad (pblms toward the 1.0 edge)
		//= 1.0 + (-2.0)* Xval + (3.0)*Xval*Xval*Xval;
		return  makeBehave(y);
	}
	catch(CString Exc){
		CString Mssg;
		Mssg.Format("CEvaluatingFunction::Eval produced an exception on value [%f]\n\r", Xval);
		Mssg+= Exc;
		throw Mssg;
	}

}

void CEvaluatingFunction::generatePoints(COUNTER FitCaseNum){

		F<double> IntervalSize = (RangeMax-RangeMin)/(double)FitCaseNum;
		destroyPoints();
		try{
			FunctionX1.push_back(RangeMin);
			FunctionY.push_back(Eval(RangeMin));
			for(COUNTER i=1; i<FitCaseNum-1; i++){
			
				F<double> x = RangeMin + ((F<double>)i*IntervalSize);	//Pick a point 
				int somewhere = rand()%100;		//in the interval,
				if (somewhere) x+= ((F<double>)1.0f/(F<double>)(somewhere))*IntervalSize;	//somewhere in the interval.
			
				FunctionX1.push_back(x);
				FunctionY.push_back(Eval(x));
			}
			FunctionX1.push_back(RangeMax);
			FunctionY.push_back(Eval(RangeMax));
		}
		catch(CString Exc){

			throw Exc;

		}
}

double CEvaluatingFunction::EvaluateCDNA(CDNAStatement* Stat){
	
	double Grade = 0.0f;
	double diff;
	Stat->Fitness->reset();

	try{
		for(COUNTER i =0; i<this->FunctionX1.size();i++){
			diff = fabs((this->FunctionY[i]-(Stat->Eval(this->FunctionX1[i]))).x());
			Grade += diff;
			if(diff <= TOL_0) Stat->Fitness->addHit();
		}
		
		Stat->Fitness->setStandardizedFitness(Grade);
	}
	catch(CString Mess){
		if(Mess == CString(_T("UNDEF"))){
			Grade = INFINITY_GRADE;
			Stat->Fitness->setStandardizedFitness(INFINITY_GRADE);
		}
		else{
			Mess += _T("\r\n -->At CEvaluatingFunction::EvaluateCDNA");
			throw Mess;
		}
	}
	
	return Grade;
}
/****************************
Drawing Routines
****************************/

void CEvaluatingFunction::draw(){

	glColor3f(1.0f, 0.0f, 0.0f);
	ASSERT(this->FunctionX1.size() > 2);
	for(COUNTER i=0; i<this->FunctionX1.size()-1;i++){
		glBegin(GL_LINES);
			
				glVertex3f(FunctionX1[i].x(), FunctionY[i].x(), 0.0f);
				glVertex3f(FunctionX1[i+1].x(), FunctionY[i+1].x(), 0.0f);

		glEnd();
	}

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
				glVertex3f(0.0f, 10.0f, 0.0f);
				glVertex3f(0.0f, -10.0f, 0.0f);
				glVertex3f(-10.0f, 0.0f, 0.0f);
				glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();



}










