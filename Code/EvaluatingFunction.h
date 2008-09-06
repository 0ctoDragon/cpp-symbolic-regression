#pragma once
#include "afx.h"

class CDNAStatement;

class CEvaluatingFunction :
	public CObject
{
	
protected:
	vector< F<double> > FunctionY;
	vector< F<double> > FunctionX1;

	void destroyPoints();
	F<double> RangeMin;
	F<double> RangeMax;
	
	F<double> makeBehave(F<double> y);
	F<double> Eval(F<double> Xval);
	
	
public:


	
	CEvaluatingFunction(double=-1.0f, double=1.0f);
	~CEvaluatingFunction(void);

	double EvaluateCDNA(CDNAStatement*);
	void generatePoints(COUNTER FitCaseNum);
	void draw();
	
		
};
