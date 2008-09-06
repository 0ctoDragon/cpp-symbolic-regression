// SymbolRegressDoc.h : interface of the CSymbolRegressDoc class
//
#pragma once


class CDNAStatement;
class CEvaluatingFunction;
class GraphView;

class CSymbolRegressDoc : public CDocument
{
protected: // create from serialization only
	CSymbolRegressDoc(COUNTER Popsize = 300, COUNTER Selsize=60, 
					COUNTER maxDepth = 10, COUNTER Cmadept = 5, double MPro = 0.2f, COUNTER = 50);
	DECLARE_DYNCREATE(CSymbolRegressDoc)

	
	
	COUNTER m_CaseCount;
	void makeEvaluatingFunction();
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
};


