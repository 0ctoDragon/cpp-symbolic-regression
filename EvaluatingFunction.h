#pragma once
#include "afx.h"

#define FUNCNUM 3
class CDNAStatement;

class CEvaluatingFunction :
	public CObject
{
	
protected:
	vector<double> FunctionYP;
	vector<double> FunctionY;
	vector<double> FunctionX1;

	void destroyPoints();
	F<double> RangeMin;
	F<double> RangeMax;
	
	F<double> makeBehave(F<double> y);
	F<double> Eval(F<double> Xval);

	void PushValueSet(F<double> Val);
	COUNTER funChoice;
	
	
	



public:

	static CString FuncString(COUNTER i);
	void chooseFunction(COUNTER FunChoice);
	CEvaluatingFunction(double=-1.0f, double=1.0f,  COUNTER = 0);
	~CEvaluatingFunction(void);

	double EvaluateCDNA(CDNAStatement*);
	void generatePoints(COUNTER FitCaseNum);
	void draw();
	
		
};
