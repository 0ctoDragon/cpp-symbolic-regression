#pragma once
#include "afx.h"



#include "StdAfx.h"

#define MAXARITY 15
#define DEFAULTREEDENSITY 80
#define DEFAULTMUTPROB 0.01f
#define DEFAULTMAXDEPTH	10
#define DEFAULTCROSSOVERMAXDEPTH 5

class StatementParameters{
protected:
		int Maxdepth;
		int TreeDensity;
		double MutationPbblty;
public:
	StatementParameters(int a, int b, double c){
		this->Maxdepth=a;
		this->TreeDensity=b;
		this->MutationPbblty=c;
		};
	void setTreeDensity(int TD);
	int getTreeDensity() const;
	void setMaxDepth(int MD);
	int getMaxDepth() const;
	void setMutationProb(double Prob);
	double getMutationProb( )const;
	
};

class CDNAStatement :
    public CObject
{
    
	StatementParameters* Params;

private:
	void grow();
	static int MemTrace(int t){
                static int Mem = 0;
                Mem += t;
                return Mem;
            } 

protected:
        int Maxdepth;
		int TreeDensity;
		double MutationPbblty;
        COUNTER arity; 
		GENEStatementType Type;
		double Fitness;
       

		COUNTER CrossOverMaxDepth;
        vector<CDNAStatement*> SubStatements; 


		virtual void copy(const CDNAStatement& S);  
        virtual void destroy();
        virtual void CompleteConstruction();
	
    
		void addBranch(unsigned int i, const CDNAStatement& S);
        bool removeBranch(unsigned int i);


public:


	//Set Methods
	void setCrossOverMaxDepth(int Depth);
	bool replaceBranch(unsigned int i, const CDNAStatement& S);

	//Admins
	// DECLARE_SERIAL( CDNAStatement );
	CDNAStatement(GENEStatementType = UNDEF,  
		COUNTER = DEFAULTCROSSOVERMAXDEPTH);

	void setTreeDensity(int TD);
	int getTreeDensity() const;
	void setMaxDepth(int MD);
	int getMaxDepth() const;
	void setMutationProb(double Prob);
	double getMutationProb( )const;



	CDNAStatement(const CDNAStatement& S);
    const CDNAStatement& operator=(const CDNAStatement& S);
	virtual ~CDNAStatement(void);


	
	double getFitness( )const;
	void setFitness( double );


        //get Methods
        static int getMem(){return MemTrace(0);};
        unsigned int getSize()  const;   //Number of nodes in the tree
        unsigned int getDepth() const;   //Length of longer branch
        unsigned int getArity() const { return arity;};
        GENEStatementType getRoot() const {return Type;};

        bool operator == (const CDNAStatement& S) const;
        const CDNAStatement& operator[] (unsigned int i);
        friend bool operator!(const CDNAStatement& S) {
            return (S.Type == UNDEF);
        };

	F<double> FromConst(GENEStatementType C);
	void toTreeCtrl(CTreeCtrl* Tctrl, HTREEITEM branch);

     
    void growCreate();
        CString toString();
        
	
	CDNAStatement& operator*(CDNAStatement& C) ;
	void mutate();
	CDNAStatement* getBranchRandomType(COUNTER MaxDepth, FUNCTIONTYPECLASS BranchType);

	void simplify();
        
	F<double> Eval(F<double> val);
	void draw(COUNTER PointsNum, double Min, double Max);

};
