// SymbolRegressDoc.h : interface of the CSymbolRegressDoc class
//
#pragma once


class CDNAStatement;
class CEvaluatingFunction;
class GraphView;

class CSymbolRegressDoc : public CDocument
{
protected: // create from serialization only
	CSymbolRegressDoc(COUNTER Popsize = 500, COUNTER Selsize=100, 
					COUNTER maxDepth = 7, COUNTER Cmadept = 4, 
					double MPro = 0.4f, COUNTER treeDensity= 80,
					COUNTER m_FuncNum=1);
	DECLARE_DYNCREATE(CSymbolRegressDoc)

	double getTotalFitness();
	void normalizeFitness();
	
	COUNTER m_CaseCount;

	
	void makeEvaluatingFunction(int EvalNum);
	double grade(CDNAStatement* Stat);
	void EvaluateAll();

	COUNTER SelectionSize;
	void Select();


	COUNTER m_FullPopulationSize;
	COUNTER CrossMaxDepth;
	double MutProb;
	void makePopulation();
	void destroyPopulation();
	void Breed();
	
	COUNTER MaxDepth;
	COUNTER TreeDensity;
	COUNTER m_funcNum;
	

	COUNTER generationCount;
	bool running;
	void UpdateOnRunIteration();
	void StopEvolutionRun();

public:
	vector<CDNAStatement*> m_Population;
	COUNTER m_CurrentIndividual;
	COUNTER m_BestIndex;
	CEvaluatingFunction* EvalFunc; 	
	double RangeMin;
	double RangeMax;

	GraphView* m_Graph;
	void UpdateGraph();

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSymbolRegressDoc();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void makeTreeDialog();
	afx_msg void onGo();
	afx_msg void OnStop();
	afx_msg void OnSettings();
	afx_msg void makeEvaluatingFunction0(){makeEvaluatingFunction(0);};
	afx_msg void makeEvaluatingFunction1(){makeEvaluatingFunction(1);};
	afx_msg void makeEvaluatingFunction2(){makeEvaluatingFunction(2);};
	afx_msg void makeEvaluatingFunction3(){makeEvaluatingFunction(3);};
	afx_msg void makeEvaluatingFunction4(){makeEvaluatingFunction(4);};
	afx_msg void makeEvaluatingFunction5(){makeEvaluatingFunction(5);};
	afx_msg void makeEvaluatingFunction6(){makeEvaluatingFunction(6);};
	afx_msg void makeEvaluatingFunction7(){makeEvaluatingFunction(7);};
	afx_msg void makeEvaluatingFunction8(){makeEvaluatingFunction(8);};
};


