#include "StdAfx.h"
#include ".\fitnessclass.h"

/*******************************
Static Methods
*******************************/

double CFitnessClass::UpdateTotalStandardizedFitness(double change){
	static double Total = 0.0f;
	Total += change;
	return Total;
}

double CFitnessClass::UpdateTotalAdjustedFitness(double change){
	static double Total = 0.0f;
	Total += change;
	return Total;
}

double CFitnessClass::UpdateTotalNormalizedFitness(double change){
	static double Total = 0.0f;
	Total += change;
	return Total;
}


void CFitnessClass::reset(){
	
	UpdateTotalStandardizedFitness(-1.0f*standardizedFitness);
	UpdateTotalAdjustedFitness(-1.0f*adjustedFitness);
	UpdateTotalNormalizedFitness(-1.0f*normalizedFitness);

	standardizedFitness = 0.0f;
	adjustedFitness = 0.0f;
	normalizedFitness = 0.0f;
	hits = 0;
}
void CFitnessClass::copy(const CFitnessClass& S){
	reset();

	UpdateTotalStandardizedFitness(S.standardizedFitness);
	UpdateTotalAdjustedFitness(S.adjustedFitness);
	UpdateTotalNormalizedFitness(S.normalizedFitness);

	standardizedFitness = S.standardizedFitness;
	adjustedFitness = S.adjustedFitness;
	normalizedFitness = S.normalizedFitness;
	hits = S.hits;
}

CFitnessClass::CFitnessClass(void){
	standardizedFitness = 0.0f;
	adjustedFitness = 0.0f;
	normalizedFitness = 0.0f;
	hits = 0;
}
CFitnessClass::CFitnessClass(const CFitnessClass& S){
	standardizedFitness = 0.0f;
	adjustedFitness = 0.0f;
	normalizedFitness = 0.0f;
	hits = 0;
	copy(S);
}
const CFitnessClass& CFitnessClass::operator=(const CFitnessClass& S){
	copy(S);
	return *this;
}
CFitnessClass::~CFitnessClass(void){
	reset();	
}

/********************
Set Functions
*********************/

void CFitnessClass::setStandardizedFitness(double Fit){
	reset();
	this->standardizedFitness = Fit;	
	if(Fit >=0.0f){
		UpdateTotalStandardizedFitness(Fit);
		double AdjustFit = 1.0f / (standardizedFitness + 1.0f);
		UpdateTotalAdjustedFitness(AdjustFit);
		this->adjustedFitness = AdjustFit;
	}
	else{
		CString Msg;
		Msg.Format("\r\nProblem at CFitnessClass::setStandardizedFitness: Fitness = [%f]", Fit);
		throw Msg;
	}
}


void CFitnessClass::normalizeFitness(){

	if(getTotalAdjustedFitness() > 0.0f){
		double NormFit = ((10.0f)*adjustedFitness) / getTotalAdjustedFitness(); 
		UpdateTotalNormalizedFitness((-1.0f*normalizedFitness) + NormFit);	
		this->normalizedFitness = NormFit;
	}
}













