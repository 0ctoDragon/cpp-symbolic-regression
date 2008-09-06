// SymbolRegressDoc.cpp : implementation of the CSymbolRegressDoc class
//

#include "stdafx.h"
#include "SymbolRegress.h"

#include "SymbolRegressDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DNAStatement.h"
#include "EvaluatingFunction.h"
#include "RegressTreeDlg.h"
#include "MainFrm.h"
#include "GraphView.h"
#include "SettingsDialog.h"


#include "FitnessClass.h" // Stephane

#ifdef _DEBUG

void CDNAMemPopup(CString M){
	CString F1;
	F1.Format(	"%d CDNAStatement Objects Currently Allocated\r\n Total ", CDNAStatement::getMem());

	CString F2;
	F2.Format(	"Total Standardized Fitness %f\r\n", CFitnessClass::getTotalStandardizedFitness());

	CString F3;
	F3.Format(	"Total Adjusted Fitness %f\r\n", CFitnessClass::getTotalAdjustedFitness());

	CString F4;
	F4.Format(	"Total Normalized Fitness %f\r\n", CFitnessClass::getTotalNormalizedFitness());

	M = M + F1 + F2 + F3 + F4;
	AfxMessageBox(M);
}
#endif



IMPLEMENT_DYNCREATE(CSymbolRegressDoc, CDocument)

BEGIN_MESSAGE_MAP(CSymbolRegressDoc, CDocument)
	 ON_COMMAND(ID_TREEVIEW, makeTreeDialog)
	 ON_COMMAND(ID_GO, onGo)
	 ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(ID_SETTINGS, OnSettings)
	//ID_FILE_SEND_MAIL
	ON_COMMAND(FUNC0, makeEvaluatingFunction0)
	ON_COMMAND(FUNC1, makeEvaluatingFunction1)
	ON_COMMAND(FUNC2, makeEvaluatingFunction2)
	ON_COMMAND(FUNC3, makeEvaluatingFunction3)
	ON_COMMAND(FUNC4, makeEvaluatingFunction4)
	ON_COMMAND(FUNC5, makeEvaluatingFunction5)
	ON_COMMAND(FUNC6, makeEvaluatingFunction6)
	ON_COMMAND(FUNC7, makeEvaluatingFunction7)
	ON_COMMAND(FUNC8, makeEvaluatingFunction8)
END_MESSAGE_MAP()


void CSymbolRegressDoc::UpdateGraph(){
		if(m_Graph)
			m_Graph->Invalidate();
}

// CSymbolRegressDoc construction/destruction

CSymbolRegressDoc::CSymbolRegressDoc(
				     COUNTER Popsize, COUNTER SelSize, 
				     COUNTER maxdeth, COUNTER CMaxDep,
				     double MProb, COUNTER treeDensity,
				     COUNTER m_FuncNum):
m_FullPopulationSize(Popsize), SelectionSize(SelSize), MaxDepth(maxdeth), 
CrossMaxDepth(CMaxDep), MutProb(MProb), TreeDensity(treeDensity),
m_CurrentIndividual(0) , generationCount(0), running(false), m_BestIndex(0),
EvalFunc(NULL), RangeMin(-1.0f), RangeMax(1.0f), m_CaseCount(60),
m_Graph(NULL), m_funcNum(){
	makeEvaluatingFunction(m_FuncNum);

}

CSymbolRegressDoc::~CSymbolRegressDoc(){
	destroyPopulation();
	if(EvalFunc) delete EvalFunc;
	
	#ifdef _DEBUG
		CDNAMemPopup(_T("At ~CSymbolRegressDoc()\r\n"));
	#endif

}

BOOL CSymbolRegressDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//for(COUNTER i=0; i<FUNCNUM; i++){
	
	//}
	return TRUE;
}




// CSymbolRegressDoc serialization
void CSymbolRegressDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar<<this->m_Population[this->m_BestIndex]->toString();
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSymbolRegressDoc diagnostics

#ifdef _DEBUG
void CSymbolRegressDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSymbolRegressDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/**********************************
Make Methods
**********************************/

void CSymbolRegressDoc::makeTreeDialog(){

	if(this->m_Population.size())
		if(this->m_CurrentIndividual < this->m_Population.size()){
			try{
				CRegressTreeDlg TreeDlg(NULL, this->m_Population[this->m_CurrentIndividual]);
				TreeDlg.DoModal();
				this->UpdateAllViews(NULL);
			}catch(CString Mssg){}
		}
		else
			throw CString(_T("Trying to display non-existing population member at CSymbolRegressDoc::MakeTreeDialog()"));
	else
		throw CString(_T("Population is empty at CSymbolRegressDoc::MakeTreeDialog()"));


	
}

/****************************************
Evaluation Methods
****************************************/

double CSymbolRegressDoc::getTotalFitness(){
	double Total = 0.0f;
	for(COUNTER i=0; i<this->m_Population.size();i++)
		if(m_Population[i])
			Total += m_Population[i]->getFitness();
	return Total;
}

void CSymbolRegressDoc::normalizeFitness(){
	double Total = getTotalFitness();
	for(COUNTER i=0; i<this->m_Population.size();i++)
		if(m_Population[i])
			m_Population[i]->setFitness(m_Population[i]->getFitness()/(double)Total);
}

void CSymbolRegressDoc::makeEvaluatingFunction(int EvalNum){
	try{
		if (EvalFunc) delete EvalFunc;
		CString R;
		R.Format("%d",EvalNum);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->BestGeneration.SetWindowText(_T("0"));//.InsertString(-1, R);
		
		EvalFunc = new CEvaluatingFunction(RangeMin, RangeMax, EvalNum);
		EvalFunc->generatePoints(m_CaseCount);
			makePopulation();
	}
	catch(CString Mssg){
		AfxMessageBox(Mssg);
		EvalFunc = NULL;
	}
}


double CSymbolRegressDoc::grade(CDNAStatement* Stat){
    
	MSG msg;
	while(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){

		if(!AfxGetApp()->PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	AfxGetApp()->OnIdle(0);
	AfxGetApp()->OnIdle(1);

	try{
		return EvalFunc->EvaluateCDNA(Stat);
	}
	catch(CString Mssg){
		throw Mssg;
	}
}

void CSymbolRegressDoc::EvaluateAll(){

	
    	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->Progress.SetPos(0);
	COUNTER i = 0;

	COUNTER olBest = m_BestIndex;
	m_BestIndex = 0;

	try{
		EvalFunc->generatePoints(m_CaseCount);
		while ((i < this->m_Population.size())&&(running)){
			if(i%10 == 0) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->Progress.StepIt();
			this->grade(this->m_Population[i]);
			i++;
		}
		//normalizeFitness();
		for(i=0; i<this->m_Population.size();i++){
			if(m_Population[i]->getFitness() > m_Population[m_BestIndex]->getFitness())
				m_BestIndex = i;
		}
		if(!(*m_Population[m_BestIndex] == *m_Population[olBest])){
			CString R;
			R.Format("%d",this->generationCount);
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->BestGeneration.SetWindowText(R);
		}
		
	}
	catch(CString Mssg){
		running = false;
		throw Mssg;		
	}
}

/***********************************
Selection Methods
******************************/

void CSymbolRegressDoc::Select(){

	if(SelectionSize > this->m_FullPopulationSize)
		throw CString(_T("The SelectionSize Facteur must be less than the size of the Population"));

	vector<CDNAStatement*> NewPopulation;       

	int TotFit = (int)getTotalFitness();
	if(TotFit < 1) TotFit=1;
	int ind = 0; 
	double acc;

	
	//Keep the best one
	NewPopulation.push_back(new CDNAStatement(*(this->m_Population[this->m_BestIndex])));

	while(NewPopulation.size() < SelectionSize){
		acc = (double)((rand()%TotFit) + 1);
		while ((acc - this->m_Population[ind]->getFitness() > 0.0)){
			acc -= this->m_Population[ind]->getFitness();
			ind = (ind+1)%this->m_Population.size();
		}
		NewPopulation.push_back(new CDNAStatement(*(this->m_Population[ind])));
		
	}

	//Clear out Population
	for(COUNTER i=0; i<this->m_Population.size();i++){
		if(this->m_Population[i]) {
			delete this->m_Population[i];
			this->m_Population[i] = NULL;
		}
	}

	for(COUNTER i=0; i<NewPopulation.size(); i++)
		this->m_Population[i] = NewPopulation[i];
}

/***********************************
Breeding Methods
***********************************/
void CSymbolRegressDoc::makePopulation(){

	destroyPopulation();
	try{

	
		for(COUNTER i=0; i<this->m_FullPopulationSize; i++){
			CDNAStatement* temp = new CDNAStatement(UNDEF);
			temp->growCreate();
			m_Population.push_back(temp);
			//(m_Population[i])->growCreate();
		}

		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->Progress.SetRange(0, (m_Population.size())/10);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->Progress.SetStep(1);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_GenerationEdit.SetWindowText(_T("100"));
	}
	catch(CString Msg){
		AfxMessageBox(Msg);
	}
	this->UpdateAllViews(NULL);
	#ifdef _DEBUG
		CDNAMemPopup(_T("At makePopulation()\r\n"));
	#endif
}

void CSymbolRegressDoc::destroyPopulation(){

	generationCount = 0;
	m_BestIndex = 0;
	for(COUNTER i=0; i<this->m_Population.size(); i++)
		if(this->m_Population[i])
			delete this->m_Population[i];

	this->m_Population.clear();
}


void CSymbolRegressDoc::Breed(){

	COUNTER indMom = 0; 
        COUNTER indPop = 0;
        double acc;
	int TotFit = (int)getTotalFitness();
        if(TotFit < 1) TotFit=1;


        COUNTER CurrentPopSize = 0;
        for(COUNTER i=0; i<this->m_Population.size(); i++)
		if(this->m_Population[i])
			CurrentPopSize++;
        
	COUNTER BegPopSize = CurrentPopSize;

        while(CurrentPopSize < this->m_FullPopulationSize){
		acc = (double)((rand()%TotFit)+1);

		while ((acc - this->m_Population[indMom]->getFitness() > 0.0f)){
			acc -= this->m_Population[indMom]->getFitness();
			indMom = rand()%BegPopSize;
                }
		
                
		m_Population[indMom]->setCrossOverMaxDepth(CrossMaxDepth);
                m_Population[indPop]->setCrossOverMaxDepth(CrossMaxDepth);
                
                this->m_Population[CurrentPopSize]
                        = &((*(this->m_Population[indPop]))*(*(this->m_Population[indMom])));
                
		indPop = (indPop+1)%BegPopSize;
                CurrentPopSize++;
        }
}

/***********************************
Steering Wheel methods
************************************/
void CSymbolRegressDoc::UpdateOnRunIteration(){
	CString t;
	t.Format("%d", generationCount);
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_GenerationEdit.SetWindowText(t);
	this->UpdateAllViews(NULL);
}

void CSymbolRegressDoc::StopEvolutionRun(){

	running = false;
	EvaluateAll();	
	CString Msg;
	Msg.Format("Evolution run finished at generation %d\r\nTotal Fitness %f", 
		generationCount, getTotalFitness());
	AfxMessageBox(Msg);
	this->UpdateAllViews(NULL);
}

void CSymbolRegressDoc::onGo(){
	if(running){
		CString g;
		g.Format("already running : current generation %d",  generationCount);
		AfxMessageBox(g);
	}
	else{
		CString t;
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_GenerationEdit.GetWindowText(t);
		stringstream trad1;
		trad1<<(LPCTSTR) t;
		COUNTER evolveTo;
		running = true;
		trad1>>evolveTo;
		t.Format("%d", generationCount);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_GenerationEdit.SetWindowText(t.GetBuffer());
		
		try{
			while((evolveTo > generationCount)&&(running)){
				EvaluateAll();
				if(running){
					Select();
					Breed();
					generationCount++;
					UpdateOnRunIteration();
				}
			}
			StopEvolutionRun();
		}
		catch (CString Mssg){
			AfxMessageBox(Mssg);
		}
	}
}


void CSymbolRegressDoc::OnStop(){
	running = false;
}


void CSymbolRegressDoc::OnSettings(){

	CSettingsDialog k(NULL, this->m_funcNum, this->m_FullPopulationSize, this->SelectionSize, 
		this->MutProb, this->MaxDepth,  this->CrossMaxDepth, 0, this->TreeDensity);
	k.DoModal();

	this->m_FullPopulationSize = k.PopCount;
	this->SelectionSize = k.SelectionSize;
	this->MutProb = k.MutRate;
	this->MaxDepth = k.Maxdepth;
	this->CrossMaxDepth = k.MaxdepthX;
	this->TreeDensity = k.TreeDensity;
	this->makePopulation();
	this->UpdateAllViews(NULL);

}


