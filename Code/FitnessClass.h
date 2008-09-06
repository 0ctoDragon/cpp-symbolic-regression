#pragma once

class CFitnessClass
{

	double standardizedFitness;
	double adjustedFitness;
	double normalizedFitness;
	int hits;

	void copy(const CFitnessClass& S);

	static double UpdateTotalStandardizedFitness(double);
	static double UpdateTotalAdjustedFitness(double);
	static double UpdateTotalNormalizedFitness(double);
	
	public:

		static double getTotalStandardizedFitness(){return UpdateTotalStandardizedFitness(0.0f);};
		static double getTotalAdjustedFitness(){return UpdateTotalAdjustedFitness(0.0f);};
		static double getTotalNormalizedFitness(){return UpdateTotalNormalizedFitness(0.0f);};
		
	int getHits(){return hits;};
		CFitnessClass(void);
		CFitnessClass(const CFitnessClass& S);
		const CFitnessClass& operator=(const CFitnessClass& S);
		~CFitnessClass(void);
		void reset();

		
		inline double getStandardizedFitness() const {return standardizedFitness;};
		inline double getAdjustedFitness() const {return adjustedFitness;};
		inline double getNormalizedFitness() const {return normalizedFitness;};
		

		void setStandardizedFitness(double);
		void normalizeFitness();
		void addHit(){hits++;};
};
