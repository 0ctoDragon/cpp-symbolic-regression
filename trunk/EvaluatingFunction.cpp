#include "StdAfx.h"
#include "DNAstatement.h"
#include ".\evaluatingfunction.h"



CString CEvaluatingFunction::FuncString(COUNTER i){


	switch(i){
		case 0:
			return (CString) _T("-1.0+2.0x^2+0.5x^3");
		case 1:
			return (CString) _T("-4.0+2.0x+3x^2");
		case 2:
			return (CString) _T("1.0+2.0x^2+3.0x^4");
		case 3:
			return (CString) _T("1.0+2.0x+3.0x^4");
		default:
			return (CString) _T("-2.0+2.0x+3.0x^3");
	}	
};



CEvaluatingFunction::CEvaluatingFunction(double Rmin, double Rmax, COUNTER FunChoice):
RangeMin(Rmin), RangeMax(Rmax), funChoice(FunChoice){

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
	FunctionYP.clear();
}


/*******************************
Evaluation Methods
*******************************/
void CEvaluatingFunction::chooseFunction(COUNTER FunChoice){
	funChoice = FunChoice;
}

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
		F<double> y;
		switch(funChoice){

			case 0:
				y = (F<double>)-1.0 + (F<double>)(2.0)* Xval*Xval + (F<double>)(0.5)*Xval*Xval*Xval;
				break;
			case 1:
				y = (F<double>)-4.0 + (F<double>)(2.0)* Xval + (F<double>)(3.0)*Xval*Xval;
				break;

			case 2:
				y = (F<double>)1.0 + (F<double>)(2.0)* Xval*Xval + (F<double>)(3.0)*Xval*Xval*Xval*Xval;
				break;
			
			case 3:
				y = (F<double>)1.0 + (F<double>)(2.0)*Xval + (F<double>)(3.0)*Xval*Xval*Xval*Xval;
				break;

			case 4: 
				y = sin(Xval);
				break;

			case 5: 
				y = cos(Xval);// + (F<double>)(3.0)*Xval*Xval;
				break;
			
			case 6:
				y = (F<double>)1.0 - (F<double>)(2.0)*Xval - (F<double>)(1.5)*Xval*Xval*Xval;
				break;


			case 7:
				y = (F<double>)1.0 - (F<double>)(2.0)*Xval - (F<double>)(1.5)*Xval*Xval*Xval*Xval;
				break;

			default:
				y = (F<double>)-2.0 + (F<double>)(2.0)* Xval  + (F<double>)(1.5)*Xval*Xval - (F<double>)(3.0)*Xval*Xval*Xval;


		}
		return  makeBehave(y);
	}
	catch(CString Exc){
		CString Mssg;
		Mssg.Format("CEvaluatingFunction::Eval produced an exception on value [%f]\n\r", Xval);
		Mssg+= Exc;
		throw Mssg;
	}

}

void CEvaluatingFunction::PushValueSet(F<double> Val){

	FunctionX1.push_back(Val.x());
	Val.diff(0,1);
	F<double> FVal = Eval(Val);
	FunctionY.push_back(FVal.x());
	FunctionYP.push_back(FVal.d(0));
}

void CEvaluatingFunction::generatePoints(COUNTER FitCaseNum){

		
		F<double> IntervalSize = (RangeMax-RangeMin)/(double)FitCaseNum;
		destroyPoints();
		try{
			PushValueSet(RangeMin);
			for(COUNTER i=1; i<FitCaseNum-1; i++){
			
				F<double> x = RangeMin + ((F<double>)i*IntervalSize);	//Pick a point 
				int somewhere = rand()%100;		//in the interval,
				if (somewhere) x+= ((F<double>)1.0f/(F<double>)(somewhere))*IntervalSize;	//somewhere in the interval.
				PushValueSet(x);
			}
			PushValueSet(RangeMax);
		}
		catch(CString Exc){
			throw Exc;
		}
}

double CEvaluatingFunction::EvaluateCDNA(CDNAStatement* Stat){
	
	double Grade = 0.0f;
	double diff;
	Stat->setFitness(0.0f);
	try{
		for(COUNTER i =0; i<this->FunctionX1.size();i++){

			F<double> x = this->FunctionX1[i];
			x.diff(0,1);
			F<double> FVal = Stat->Eval(x);
	
			diff = fabs(FVal.x()-this->FunctionY[i]);
			Grade += diff;
			diff = fabs(FVal.d(0)-this->FunctionYP[i]);
			Grade += diff/2.0f;
		}
	}
	catch(CString Mess){
		if(Mess == CString(_T("UNDEF"))){
			Grade = INFINITY_GRADE;
		}
		else{
			Mess += _T("\r\n -->At CEvaluatingFunction::EvaluateCDNA");
			throw Mess;
		}
	}
	Stat->setFitness(1.0f/((Grade)+1));
	
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
			
				glVertex3f(FunctionX1[i], FunctionY[i], 0.0f);
				glVertex3f(FunctionX1[i+1], FunctionY[i+1], 0.0f);

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










