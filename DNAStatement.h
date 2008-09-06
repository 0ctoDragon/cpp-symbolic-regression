#pragma once
#include "afx.h"



#include "StdAfx.h"

#define MAXARITY 15
#define DEFAULTMUTPROB 0.01f
#define DEFAULTCROSSOVERMAXDEPTH 10



class CFitnessClass;
class CDNAStatement :
    public CObject
{
       
protected:
        double MutationPbblty;
        int CrossOverMaxDepth;
        unsigned int arity; 

        GENEStatementType Type;
        vector<CDNAStatement*> SubStatements; 


	void copy(const CDNAStatement& S);  
        void destroy();
        void CompleteConstruction();
    
	void addBranch(unsigned int i, const CDNAStatement& S);
        bool removeBranch(unsigned int i);
        
            
        static int MemTrace(int t){
                static int Mem = 0;
                Mem += t;
                return Mem;
            }    
	int TreeDensity;
            
public:
	CFitnessClass* Fitness;

	//Set Methods
	void setCrossOverMaxDepth(int Depth);
	void setMutationProb(double Prob);
	bool replaceBranch(unsigned int i, const CDNAStatement& S);

	//Admins
	// DECLARE_SERIAL( CDNAStatement );
	CDNAStatement(GENEStatementType S = UNDEF, int=50);
	CDNAStatement(const CDNAStatement& S);
        const CDNAStatement& operator=(const CDNAStatement& S);
	virtual ~CDNAStatement(void);


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

        void grow(unsigned int MaxDepth);
        void growCreate(unsigned int Maxdepth);
        CString toString();
        
	
	CDNAStatement& operator*(CDNAStatement& C) ;
	void mutate();
	CDNAStatement* getBranchRandomType(COUNTER MaxDepth, FUNCTIONTYPECLASS BranchType);

	void simplify();
        F<double> Eval(F<double> val);
	void draw(COUNTER PointsNum, double Min, double Max);

};
