// Copyright (C) 1996-2003 Claus Bendtsen and Ole Stauning (ole.st@uning.dk)
// All rights reserved.

// This code is provided "as is", without any warranty of any kind,
// either expressed or implied, including but not limited to, any implied
// warranty of merchantibility or fitness for any purpose. In no event
// will any party who distributed the code be liable for damages or for
// any claim(s) by any other party, including but not limited to, any
// lost profits, lost monies, lost data or data rendered inaccurate,
// losses sustained by third parties, or any other special, incidental or
// consequential damages arising out of the use or inability to use the
// program, even if the possibility of such damages has been advised
// against. The entire risk as to the quality, the performance, and the
// fitness of the program for any particular purpose lies with the party
// using the code.

// This code, and any derivative of this code, may not be used in a
// commercial package without the prior explicit written permission of
// the authors. Verbatim copies of this code may be made and distributed
// in any medium, provided that this copyright notice is not removed or
// altered in any way. No fees may be charged for distribution of the
// codes, other than a fee to cover the cost of the media and a
// reasonable handling fee.

// ***************************************************************
// ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS OF THE
// COPYRIGHT NOTICE
// ***************************************************************

#ifndef _TADIFF_H
#define _TADIFF_H

#include <math.h>
#include <iostream>

#ifndef MaxLength
#define MaxLength 40
#endif

#ifndef CONVINT
#define CONVINT U
#endif

#include "fadbad.h"

template <class U>
class TaylorNode
{
public: // Since access otherwise becomes too complicated.

	U *v;   // The coefficients
	int n;  // The number of known taylor coefficients.
	int rc; // Resource counter

	TaylorNode();
	TaylorNode(const U& x);
	U& operator[](const int i);

	virtual ~TaylorNode();
};

template <class U>
std::ostream& operator << (std::ostream& os, const TaylorNode<U>& o);

template <class U>
class TaylorOp
{
/* This class is the baseclass of all operation met during the forward
 sweep. It contains a pointer to a TaylorNode where the result of the operation
 is stored, val. */

public:
	TaylorNode<U> *val;

protected:
	TaylorOp();
	TaylorOp(const U &x);
	TaylorOp(const TaylorOp<U> &x);
public:
	virtual ~TaylorOp();
	void inc_rc() const;
	virtual void dec_rc();
	/* Dummy virtual functions */
	virtual TaylorOp<U>* copy() const { return NULL; };
	virtual void eval(int) {};
	virtual void reset() {};
	const U& operator[](const int i) const;
	U& operator[](const int i);
};

template <class U>
class BINTaylorOp: public TaylorOp<U>
{
/* This class represents binary operators. It has two branches, one for each
 of the expresions (operators) it operates on. The pointer o1 refers to
 the left operand and o2 to the right operand. */

public:
	TaylorOp<U> *o1,*o2;

protected:
	BINTaylorOp();
	BINTaylorOp(const U &x);
	BINTaylorOp(const BINTaylorOp<U> &x);
public:
	~BINTaylorOp();
	virtual void eval(int i);
	virtual void reset();
};

template <class U>
class UNTaylorOp: public TaylorOp<U>{
/* This class represents unary operators. It has one branch for the
 expresion (operator) it operates on. The pointer o1 refers to this. */

public:
	TaylorOp<U> *o1;

protected:
	UNTaylorOp();
	UNTaylorOp(const U &x);
	UNTaylorOp(const UNTaylorOp<U> &x);
	UNTaylorOp(const TaylorOp<U> &x);
public:
	~UNTaylorOp();
	virtual void eval(int i);
	virtual void reset();
};

template <class U>
class TTypeName: public UNTaylorOp<U>
{
/* This class is used as a substitute for variables. No (sub-)expression
 can be referenced directly more than once unless it is done by objects
 of this class. This class does often not contain its "own" TaylorNode but
 just refers to a TaylorNode from an underlying operator. */

public:
	TTypeName();

	TTypeName(const U& x);
#ifdef BaseType
	TTypeName(const BaseType& x);
#endif
	TTypeName(const TTypeName<U> &x);
	TTypeName(const TaylorOp<U> &x);
	~TTypeName();
	virtual void dec_rc();
	virtual TaylorOp<U>* copy() const;
	int length();
	virtual void eval(int i);
	virtual void reset();
	U root() const;

	/* This is all overloadings of the = operator */
	TTypeName<U>& operator = (const U& x);
	TTypeName<U>& operator = (const TTypeName<U>& x);
	TTypeName<U>& operator = (const TaylorOp<U>& x);
#ifdef BaseType
	TTypeName<U>& operator = (const BaseType& x);
#endif

	/* Declaration of overloading of (?)= */
	TTypeName<U>& operator += (const TaylorOp<U>& x);
	TTypeName<U>& operator -= (const TaylorOp<U>& x);
	TTypeName<U>& operator *= (const TaylorOp<U>& x);
	TTypeName<U>& operator /= (const TaylorOp<U>& x);
	TTypeName<U>& operator += (const TTypeName<U>& x);
	TTypeName<U>& operator -= (const TTypeName<U>& x);
	TTypeName<U>& operator *= (const TTypeName<U>& x);
	TTypeName<U>& operator /= (const TTypeName<U>& x);
	TTypeName<U>& operator += (const U& x);
	TTypeName<U>& operator -= (const U& x);
	TTypeName<U>& operator *= (const U& x);
	TTypeName<U>& operator /= (const U& x);
#ifdef BaseType
	TTypeName<U>& operator += (const BaseType& x);
	TTypeName<U>& operator -= (const BaseType& x);
	TTypeName<U>& operator *= (const BaseType& x);
	TTypeName<U>& operator /= (const BaseType& x);
#endif

};

template <class U>
std::ostream& operator << (std::ostream& os, const TTypeName<U>& o);

#ifdef BaseType
template <>
class TTypeName<BaseType>: public UNTaylorOp<BaseType>
{
/* This class is used as a substitute for variables. No (sub-)expression
 can be referenced directly more than once unless it is done by objects
 of this class. This class does often not contain its "own" TaylorNode but
 just refers to a TaylorNode from an underlying operator. */

public:
	TTypeName();

	TTypeName(const BaseType& x);
	TTypeName(const TTypeName<BaseType> &x);
	TTypeName(const TaylorOp<BaseType> &x);
	~TTypeName();
	virtual void dec_rc();
	virtual TaylorOp<BaseType>* copy() const;
	int length();
	virtual void eval(int i);
	virtual void reset();
	BaseType root() const;

	/* This is all overloadings of the = operator */
	TTypeName<BaseType>& operator = (const BaseType& x);
	TTypeName<BaseType>& operator = (const TTypeName<BaseType>& x);
	TTypeName<BaseType>& operator = (const TaylorOp<BaseType>& x);

	/* Declaration of overloading of (?)= and actual code found below. */
	TTypeName<BaseType>& operator += (const TaylorOp<BaseType>& x);
	TTypeName<BaseType>& operator -= (const TaylorOp<BaseType>& x);
	TTypeName<BaseType>& operator *= (const TaylorOp<BaseType>& x);
	TTypeName<BaseType>& operator /= (const TaylorOp<BaseType>& x);
	TTypeName<BaseType>& operator += (const TTypeName<BaseType>& x);
	TTypeName<BaseType>& operator -= (const TTypeName<BaseType>& x);
	TTypeName<BaseType>& operator *= (const TTypeName<BaseType>& x);
	TTypeName<BaseType>& operator /= (const TTypeName<BaseType>& x);
	TTypeName<BaseType>& operator += (const BaseType& x);
	TTypeName<BaseType>& operator -= (const BaseType& x);
	TTypeName<BaseType>& operator *= (const BaseType& x);
	TTypeName<BaseType>& operator /= (const BaseType& x);

};
template <class U>
std::ostream& operator << (std::ostream& os, const TTypeName<U>& o);

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                       TAYLOR ADDITION FORMULAS                        */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op + op --------------------------------- */

template <class U>
class ADDTaylorOp: public BINTaylorOp<U>
{
public:
	ADDTaylorOp();
	ADDTaylorOp(const ADDTaylorOp<U>& x);
	ADDTaylorOp(const TaylorOp<U>& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
ADDTaylorOp<U> operator+ (const TaylorOp<U>& a, const TaylorOp<U>& b);
template <class U>
ADDTaylorOp<U> operator+ (const TaylorOp<U>& a, const TTypeName<U>& b);
template <class U>
ADDTaylorOp<U> operator+ (const TTypeName<U>& a, const TaylorOp<U>& b);
template <class U>
ADDTaylorOp<U> operator+ (const TTypeName<U>& a, const TTypeName<U>& b);

#ifdef BaseType

/* --------------------------- BaseType + op ------------------------------ */

template <class U>
class ADDTaylorOp3: public UNTaylorOp<U>
{
	BaseType ax;
public:
	ADDTaylorOp3();
	ADDTaylorOp3(const ADDTaylorOp3<U>& x);
	ADDTaylorOp3(const BaseType& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
ADDTaylorOp3<U> operator+ (const BaseType& a, const TaylorOp<U>& b);
template <class U>
ADDTaylorOp3<U> operator+ (const BaseType& a, const TTypeName<U>& b);

/* --------------------------- op + BaseType ------------------------------ */

template <class U>
class ADDTaylorOp4: public UNTaylorOp<U>
{
	BaseType bx;
public:
	ADDTaylorOp4();
	ADDTaylorOp4(const ADDTaylorOp4<U>& x);
	ADDTaylorOp4(const TaylorOp<U>& a, const BaseType& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
ADDTaylorOp4<U> operator+ (const TaylorOp<U>& a, const BaseType& b);
template <class U>
ADDTaylorOp4<U> operator+ (const TTypeName<U>& a, const BaseType& b);

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                     TAYLOR SUBTRACTION FORMULAS                       */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op - op --------------------------------- */

template <class U>
class SUBTaylorOp: public BINTaylorOp<U>
{
public:
	SUBTaylorOp();
	SUBTaylorOp(const SUBTaylorOp<U>& x);
	SUBTaylorOp(const TaylorOp<U>& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
SUBTaylorOp<U> operator- (const TaylorOp<U>& a, const TaylorOp<U>& b);
template <class U>
SUBTaylorOp<U> operator- (const TaylorOp<U>& a, const TTypeName<U>& b);
template <class U>
SUBTaylorOp<U> operator- (const TTypeName<U>& a, const TaylorOp<U>& b);
template <class U>
SUBTaylorOp<U> operator- (const TTypeName<U>& a, const TTypeName<U>& b);

#ifdef BaseType

/* --------------------------- BaseType - op ------------------------------ */

template <class U>
class SUBTaylorOp3: public UNTaylorOp<U>
{
	BaseType ax;
public:
	SUBTaylorOp3();
	SUBTaylorOp3(const SUBTaylorOp3<U>& x);
	SUBTaylorOp3(const BaseType& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
SUBTaylorOp3<U> operator- (const BaseType& a, const TaylorOp<U>& b);
template <class U>
SUBTaylorOp3<U> operator- (const BaseType& a, const TTypeName<U>& b);

/* --------------------------- op - BaseType ------------------------------ */

template <class U>
class SUBTaylorOp4: public UNTaylorOp<U>
{
	BaseType bx;
public:
	SUBTaylorOp4();
	SUBTaylorOp4(const SUBTaylorOp4<U>& x);
	SUBTaylorOp4(const TaylorOp<U>& a, const BaseType& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
SUBTaylorOp4<U> operator- (const TaylorOp<U>& a, const BaseType& b);
template <class U>
SUBTaylorOp4<U> operator- (const TTypeName<U>& a, const BaseType& b);

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                     TAYLOR MULTIPLICATION FORMULAS                    */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op * op --------------------------------- */

template <class U>
class MULTaylorOp: public BINTaylorOp<U>
{
public:
	MULTaylorOp();
	MULTaylorOp(const MULTaylorOp<U>& x);
	MULTaylorOp(const TaylorOp<U>& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
MULTaylorOp<U> operator* (const TaylorOp<U>& a, const TaylorOp<U>& b);
template <class U>
MULTaylorOp<U> operator* (const TaylorOp<U>& a, const TTypeName<U>& b);
template <class U>
MULTaylorOp<U> operator* (const TTypeName<U>& a, const TaylorOp<U>& b);
template <class U>
MULTaylorOp<U> operator* (const TTypeName<U>& a, const TTypeName<U>& b);

#ifdef BaseType

/* --------------------------- BaseType * op ------------------------------ */

template <class U>
class MULTaylorOp3: public UNTaylorOp<U>
{
	BaseType ax;
public:
	MULTaylorOp3();
	MULTaylorOp3(const MULTaylorOp3<U>& x);
	MULTaylorOp3(const BaseType& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
MULTaylorOp3<U> operator* (const BaseType& a, const TaylorOp<U>& b);
template <class U>
MULTaylorOp3<U> operator* (const BaseType& a, const TTypeName<U>& b);

/* --------------------------- op * BaseType ------------------------------ */

template <class U>
class MULTaylorOp4: public UNTaylorOp<U>
{
	BaseType bx;
public:
	MULTaylorOp4();
	MULTaylorOp4(const MULTaylorOp4<U>& x);
	MULTaylorOp4(const TaylorOp<U>& a, const BaseType& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
MULTaylorOp4<U> operator* (const TaylorOp<U>& a, const BaseType& b);
template <class U>
MULTaylorOp4<U> operator* (const TTypeName<U>& a, const BaseType& b);

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                      TAYLOR DIVISION FORMULAS                         */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op / op --------------------------------- */

template <class U>
class DIVTaylorOp: public BINTaylorOp<U>
{
public:
	DIVTaylorOp();
	DIVTaylorOp(const DIVTaylorOp<U>& x);
	DIVTaylorOp(const TaylorOp<U>& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
DIVTaylorOp<U> operator/ (const TaylorOp<U>& a, const TaylorOp<U>& b);
template <class U>
DIVTaylorOp<U> operator/ (const TaylorOp<U>& a, const TTypeName<U>& b);
template <class U>
DIVTaylorOp<U> operator/ (const TTypeName<U>& a, const TaylorOp<U>& b);
template <class U>
DIVTaylorOp<U> operator/ (const TTypeName<U>& a, const TTypeName<U>& b);

#ifdef BaseType

/* --------------------------- BaseType / op ------------------------------ */

template <class U>
class DIVTaylorOp3: public UNTaylorOp<U>
{
	BaseType ax;
public:
	DIVTaylorOp3();
	DIVTaylorOp3(const DIVTaylorOp3<U>& x);
	DIVTaylorOp3(const BaseType& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
DIVTaylorOp3<U> operator/ (const BaseType& a, const TaylorOp<U>& b);
template <class U>
DIVTaylorOp3<U> operator/ (const BaseType& a, const TTypeName<U>& b);

/* --------------------------- op / BaseType ------------------------------ */

template <class U>
class DIVTaylorOp4: public UNTaylorOp<U>
{
	BaseType bx;
public:
	DIVTaylorOp4();
	DIVTaylorOp4(const DIVTaylorOp4<U>& x);
	DIVTaylorOp4(const TaylorOp<U>& a, const BaseType& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
DIVTaylorOp4<U> operator/ (const TaylorOp<U>& a, const BaseType& b);
template <class U>
DIVTaylorOp4<U> operator/ (const TTypeName<U>& a, const BaseType& b);

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            TAYLOR power FORMULA (still to be implemented)             */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* -------------------------- pow(op,TaylorOp) ------------------------------- */

template <class U>
class POWTaylorOp: public BINTaylorOp<U>
{
public:
	POWTaylorOp();
	POWTaylorOp(const POWTaylorOp<U>& x);
	POWTaylorOp(const TaylorOp<U>& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
POWTaylorOp<U> pow (const TaylorOp<U>& a, const TaylorOp<U>& b);
template <class U>
POWTaylorOp<U> pow (const TaylorOp<U>& a, const TTypeName<U>& b);
template <class U>
POWTaylorOp<U> pow (const TTypeName<U>& a, const TaylorOp<U>& b);
template <class U>
POWTaylorOp<U> pow (const TTypeName<U>& a, const TTypeName<U>& b);

#ifdef BaseType

/* ------------------------ pow(BaseType,TaylorOp) -------------------------- */

template <class U>
class POWTaylorOp3: public UNTaylorOp<U>
{
	BaseType ax;
public:
	POWTaylorOp3();
	POWTaylorOp3(const POWTaylorOp3<U>& x);
	POWTaylorOp3(const BaseType& a, const TaylorOp<U>& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
POWTaylorOp3<U> pow (const BaseType& a, const TaylorOp<U>& b);
template <class U>
POWTaylorOp3<U> pow (const BaseType& a, const TTypeName<U>& b);

/* ------------------------ pow(op,BaseType) --------------------------- */

template <class U>
class POWTaylorOp4: public UNTaylorOp<U>
{
	BaseType bx;
public:
	POWTaylorOp4();
	POWTaylorOp4(const POWTaylorOp4<U>& x);
	POWTaylorOp4(const TaylorOp<U>& a, const BaseType& b);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
POWTaylorOp4<U> pow (const TaylorOp<U>& a, const BaseType& b);
template <class U>
POWTaylorOp4<U> pow (const TTypeName<U>& a, const BaseType& b);

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                    TAYLOR UNARY + FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class UADDTaylorOp: public UNTaylorOp<U>
{
public:
	UADDTaylorOp();
	UADDTaylorOp(const UADDTaylorOp<U>& x);
	UADDTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
UADDTaylorOp<U> operator+ (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                      TAYLOR UNARY - FORMULA                           */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class USUBTaylorOp: public UNTaylorOp<U>
{
public:
	USUBTaylorOp();
	USUBTaylorOp(const USUBTaylorOp<U>& x);
	USUBTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
USUBTaylorOp<U> operator- (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                   TAYLOR pow(op,int) FORMULA                          */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class POWTaylorOpN: public UNTaylorOp<U>
{
	int b;
public:
	POWTaylorOpN();
	POWTaylorOpN(const POWTaylorOpN<U>& x);
	POWTaylorOpN(const TaylorOp<U>& a, int ib);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
POWTaylorOpN<U> pow (const TaylorOp<U>& a, int b);
template <class U>
POWTaylorOpN<U> pow (const TTypeName<U>& a, int b);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR sqr FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class SQRTaylorOp: public UNTaylorOp<U>
{
public:
	SQRTaylorOp();
	SQRTaylorOp(const SQRTaylorOp<U>& x);
	SQRTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
SQRTaylorOp<U> sqr (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR exp FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class EXPTaylorOp: public UNTaylorOp<U>
{
public:
	EXPTaylorOp();
	EXPTaylorOp(const EXPTaylorOp<U>& x);
	EXPTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
EXPTaylorOp<U> exp (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                         TAYLOR log FORMULA                            */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class LOGTaylorOp: public UNTaylorOp<U>
{
public:
	LOGTaylorOp();
	LOGTaylorOp(const LOGTaylorOp<U>& x);
	LOGTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
LOGTaylorOp<U> log (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR sqrt FORMULA                            */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class SQRTTaylorOp: public UNTaylorOp<U>
{
public:
	SQRTTaylorOp();
	SQRTTaylorOp(const SQRTTaylorOp<U>& x);
	SQRTTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
SQRTTaylorOp<U> sqrt (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                          TAYLOR sin FORMULA                           */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class SINTaylorOp: public UNTaylorOp<U>
{
	U *COS; // The coefficients for corresponding cos
public:
	SINTaylorOp();
	SINTaylorOp(const SINTaylorOp<U>& x);
	SINTaylorOp(const TaylorOp<U>& a);
	~SINTaylorOp();
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
SINTaylorOp<U> sin (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                           TAYLOR cos FORMULA                          */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class COSTaylorOp: public UNTaylorOp<U>
{
	U *SIN; // The coefficients for corresponding SIN
public:
	COSTaylorOp();
	COSTaylorOp(const COSTaylorOp<U>& x);
	COSTaylorOp(const TaylorOp<U>& a);
	~COSTaylorOp();
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
COSTaylorOp<U> cos (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR tan FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class TANTaylorOp: public UNTaylorOp<U>
{
	TTypeName<U> SQRCOS; // The coefficients for corresponding pow(cos(a),2)
public:
	TANTaylorOp();
	TANTaylorOp(const TANTaylorOp<U>& x);
	TANTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
	virtual void reset();
};

template <class U>
TANTaylorOp<U> tan (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                       TAYLOR asin FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class ASINTaylorOp: public UNTaylorOp<U>
{
	TTypeName<U> SQRT; // The coefficients for sqrt(1-pow(a,2))
public:
	ASINTaylorOp();
	ASINTaylorOp(const ASINTaylorOp<U>& x);
	ASINTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
	virtual void reset();
};

template <class U>
ASINTaylorOp<U> asin (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                       TAYLOR acos FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class ACOSTaylorOp: public UNTaylorOp<U>
{
	TTypeName<U> SQRT; // The coefficients for sqrt(1-pow(a,2))
public:
	ACOSTaylorOp();
	ACOSTaylorOp(const ACOSTaylorOp<U>& x);
	ACOSTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
	virtual void reset();
};

template <class U>
ACOSTaylorOp<U> acos (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                         TAYLOR atan FORMULA                           */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class ATANTaylorOp: public UNTaylorOp<U>
{
	TTypeName<U> IPSQR; // The coefficients for 1+pow(a,2)
public:
	ATANTaylorOp();
	ATANTaylorOp(const ATANTaylorOp<U>& x);
	ATANTaylorOp(const TaylorOp<U>& a);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
	virtual void reset();
};

template <class U>
ATANTaylorOp<U> atan (const TaylorOp<U>& a);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                      TAYLOR diff(op,int) FORMULA                      */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
class DIFFTaylorOpN: public UNTaylorOp<U>
{
	int b;
public:
	DIFFTaylorOpN();
	DIFFTaylorOpN(const DIFFTaylorOpN<U>& x);
	DIFFTaylorOpN(const TaylorOp<U>& a, int ib);
	virtual TaylorOp<U>* copy() const;
	virtual void eval(int i);
};

template <class U>
DIFFTaylorOpN<U> diff (const TaylorOp<U>& a, int b);

template <class U>
DIFFTaylorOpN<U> diff (const TTypeName<U>& a, int b);

template <class U>
INLINE1 TaylorNode<U>::TaylorNode<U>():rc(1),n(0),v(new U[MaxLength])
{
	for (int i=1;i<MaxLength;i++) v[i]=U(0);
}

template <class U>
INLINE2 TaylorNode<U>::TaylorNode<U>(const U& x):rc(1),n(MaxLength),v(new U[MaxLength])
{
	v[0]=x;
	for (int i=1;i<MaxLength;i++) v[i]=U(0);
}

/* --------- destructor --------- */

template <class U>
INLINE1 TaylorNode<U>::~TaylorNode<U>()
{
	delete[] v;
}

/* --------- index operators --------- */

template <class U>
INLINE2 U& TaylorNode<U>::operator[](const int i)
{
	USER_ASSERT(i>=0&&i<MaxLength,"Index out of bounds in operator[]");
	if (i>=n) n=i+1;
	return v[i];
}

/* --------- std::ostream operator --------- */

template <class U>
INLINE2 std::ostream& operator << (std::ostream& os, const TaylorNode<U>& o)
{
	os << "[ ";
	for (int i=0;i<o.n;i++) os << o.v[i] << " ";
	if (o.n<MaxLength) os << "... ";
	os << "]";
	return os;
}

/* --------- END Class: TaylorNode --------- */

/* --------- BEGIN Class: op --------- */

template <class U>
INLINE1 TaylorOp<U>::TaylorOp<U>():val(new TaylorNode<U>())
{
}

template <class U>
INLINE1 TaylorOp<U>::TaylorOp<U>(const U &x):val(new TaylorNode<U>(x))
{
}

template <class U>
INLINE1 TaylorOp<U>::TaylorOp<U>(const TaylorOp<U> &x):val(x.val)
{
	inc_rc();
}

template <class U>
INLINE1 TaylorOp<U>::~TaylorOp<U>()
{
	dec_rc();
}

template <class U>
INLINE1 void TaylorOp<U>::inc_rc() const
{
	/* Increments the resource counter of the underlying TaylorNode. */
	ASSERT(val)
	val->rc++;
}

template <class U>
INLINE2 void TaylorOp<U>::dec_rc()
{
	/* Decrements the resource counter of the underlying TaylorNode. */
	ASSERT(val)
	if (--val->rc==0)
	{
		/* When no operator refers to a TaylorNode it can be deallocated. */
		delete val;
		val=0;
	}
}

template <class U>
INLINE1 const U& TaylorOp<U>::operator[](const int i) const
{
	return (*val)[i];
}

template <class U>
INLINE1 U& TaylorOp<U>::operator[](const int i)
{
	return (*val)[i];
}

/* --------- END Class: op --------- */

/* --------- BEGIN Class: BINop --------- */

template <class U>
INLINE1 BINTaylorOp<U>::BINTaylorOp<U>()
{
	o1=o2=NULL;
}

template <class U>
INLINE1 BINTaylorOp<U>::BINTaylorOp<U>(const U &x):TaylorOp<U>(x)
{
	o1=o2=NULL;
}

template <class U>
INLINE2 BINTaylorOp<U>::BINTaylorOp<U>(const BINTaylorOp<U> &x):TaylorOp<U>(x)
{
	o1=x.o1;
	o2=x.o2;
}

template <class U>
INLINE2 BINTaylorOp<U>::~BINTaylorOp<U>()
{
	ASSERT(val)
	if (val->rc==1)
	{
		if (o1) {delete o1; o1=NULL;}
		if (o2) {delete o2; o2=NULL;}
	}
}

template <class U>
INLINE2 void BINTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1&&o2)
	o1->eval(i);
	o2->eval(i);
}

template <class U>
INLINE2 void BINTaylorOp<U>::reset()
{
	ASSERT(val&&o1&&o2)
	o1->reset();
	o2->reset();
	val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
}

/* --------- END Class: BINop --------- */

/* --------- BEGIN Class: UNop --------- */

template <class U>
INLINE1 UNTaylorOp<U>::UNTaylorOp<U>()
{
	o1=NULL;
}

template <class U>
INLINE1 UNTaylorOp<U>::UNTaylorOp<U>(const U &x):TaylorOp<U>(x)
{
	o1=NULL;
}

template <class U>
INLINE1 UNTaylorOp<U>::UNTaylorOp<U>(const UNTaylorOp<U> &x):TaylorOp<U>(x)
{
	o1=x.o1;
}

template <class U>
INLINE1 UNTaylorOp<U>::UNTaylorOp<U>(const TaylorOp<U> &x):TaylorOp<U>(x)
{
	o1=x.copy();
}

template <class U>
INLINE2 UNTaylorOp<U>::~UNTaylorOp<U>()
{
	ASSERT(val)
	if (val->rc==1)
	{
		if (o1) {delete o1; o1=NULL;}
	}
}

template <class U>
INLINE1 void UNTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	o1->eval(i);
}

template <class U>
INLINE2 void UNTaylorOp<U>::reset()
{
	ASSERT(val&&o1)
	o1->reset();
	val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
}

/* --------- END Class: UNop --------- */

/* --------- BEGIN Class: tad-variable --------- */

template <class U>
INLINE1 TTypeName<U>::TTypeName<U>()
{
}

template <class U>
INLINE1 TTypeName<U>::TTypeName<U>(const U& x):UNTaylorOp<U>(x)
{
}

#ifdef BaseType
template <class U>
INLINE1 TTypeName<U>::TTypeName<U>(const BaseType& x):UNTaylorOp<U>(U(x))
{
}
#endif

template <class U>
INLINE1 TTypeName<U>::TTypeName<U>(const TTypeName<U> &x):UNTaylorOp<U>(x)
{
}

template <class U>
INLINE1 TTypeName<U>::TTypeName<U>(const TaylorOp<U> &x):UNTaylorOp<U>(x)
{
//	o1=x.copy();
}

template <class U>
INLINE2 TTypeName<U>::~TTypeName<U>()
{
	ASSERT(val)
	if ((val->rc==2)&&(o1))
	{
		/* If object is the last tad-variable encapsulating some operator then
		   underlying operator should also be deleted. */
		dec_rc(); // When called again in ~op, TaylorNode is allready deleted.
	}
}

template <class U>
INLINE2 void TTypeName<U>::dec_rc()
{
	/* If this is the last tad-variable encapsulating some operator then operator
	   can be deleted (and this will delete the operators TaylorNode). */
	ASSERT(val)
	if (o1)
	{
		if (--val->rc==1)
		{
			delete o1;
			o1=NULL;
//			val=NULL; // Since deleted from o1.
		}
	} else UNTaylorOp<U>::dec_rc();
}

template <class U>
INLINE1 TaylorOp<U>* TTypeName<U>::copy() const
{
	return new TTypeName<U>(*this);
}

template <class U>
INLINE1 int TTypeName<U>::length()
{
	ASSERT(val)
	return val->n;
}

template <class U>
INLINE1 void TTypeName<U>::eval(int i)
{
	ASSERT(val)
	if (o1) o1->eval(i);
}

template <class U>
INLINE2 void TTypeName<U>::reset()
{
	ASSERT(val)
	if (o1)
	{
		o1->reset();
//		val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
	}
	val->n=0;
}

template <class U>
INLINE1 U TTypeName<U>::root() const
{
	return val->v[0];
}

/* This is all overloadings of the = operator */
template <class U>
INLINE2 TTypeName<U>& TTypeName<U>::operator = (const U& x)
{
	dec_rc();
	val=new TaylorNode<U>(x);
	o1=NULL;
	return *this;
}

template <class U>
INLINE2 TTypeName<U>& TTypeName<U>::operator = (const TTypeName<U>& x)
{
	dec_rc();
	val=x.val;
	inc_rc();
	o1=x.o1;
	return *this;
}

template <class U>
INLINE2 TTypeName<U>& TTypeName<U>::operator = (const TaylorOp<U>& x)
{
	dec_rc();
	val=x.val;
	inc_rc();
	o1=x.copy();
	return *this;
}

#ifdef BaseType
template <class U>
INLINE2 TTypeName<U>& TTypeName<U>::operator = (const BaseType& x)
{
	dec_rc();
	val=new TaylorNode<U>(x);
	o1=NULL;
	return *this;
}
#endif

#ifdef BaseType

INLINE0 TTypeName<BaseType>::TTypeName<BaseType>()
{
}

INLINE0 TTypeName<BaseType>::TTypeName<BaseType>(const BaseType& x):UNTaylorOp<BaseType>(x)
{
}

INLINE0 TTypeName<BaseType>::TTypeName<BaseType>(const TTypeName<BaseType> &x):UNTaylorOp<BaseType>(x)
{
}

INLINE0 TTypeName<BaseType>::TTypeName<BaseType>(const TaylorOp<BaseType> &x):UNTaylorOp<BaseType>(x)
{
}

INLINE0 TTypeName<BaseType>::~TTypeName<BaseType>()
{
	ASSERT(val)
	if ((val->rc==2)&&(o1))
	{
		/* If object is the last tad-variable encapsulating some operator then
		   underlying operator should also be deleted. */
		dec_rc(); // When called again in ~op, TaylorNode is allready deleted.
	}
}

INLINE0 void TTypeName<BaseType>::dec_rc()
{
	/* If this is the last tad-variable encapsulating some operator then operator
	   can be deleted (and this will delete the operators TaylorNode). */
	ASSERT(val)
	if (o1)
	{
		if (--val->rc==1)
		{
			delete o1;
			o1=NULL;
//			val=NULL; // Since deleted from o1.
		}
	} else UNTaylorOp<BaseType>::dec_rc();
}

INLINE0 TaylorOp<BaseType>* TTypeName<BaseType>::copy() const
{
	/* Returns a copy of the object which still links to the same copy
	   of the internal TaylorNode. */
	TTypeName<BaseType>* o=new TTypeName<BaseType>(*this);
	return o;
}

INLINE0 int TTypeName<BaseType>::length()
{
	ASSERT(val)
	return val->n;
}

INLINE0 void TTypeName<BaseType>::eval(int i)
{
	ASSERT(val)
	if (o1) o1->eval(i);
}

INLINE0 void TTypeName<BaseType>::reset()
{
	ASSERT(val)
	if (o1)
	{
		o1->reset();
//		val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
	}
	val->n=0;
}

INLINE0 BaseType TTypeName<BaseType>::root() const
{
	return val->v[0];
}

INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator = (const TTypeName<BaseType>& x)
{
	dec_rc();
	val=x.val;
	inc_rc();
	o1=x.o1;
	return *this;
}

INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator = (const TaylorOp<BaseType>& x)
{
	dec_rc();
	val=x.val;
	inc_rc();
	o1=x.copy();
	return *this;
}

INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator = (const BaseType& x)
{
	dec_rc();
	val=new TaylorNode<BaseType>(x);
	o1=NULL;
	return *this;
}

#endif

/* --------- END Class: tad-variable --------- */

/* Overloading of standard << so that class can be directly displayed
   using <<. */

template <class U>
INLINE2 std::ostream& operator << (std::ostream& os, const TTypeName<U>& o)
{
	if (o.val) os << (*o.val);
	if (o.o1) os << "d";
	return os;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                     TAYLOR ADDITION FORMULAS                          */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- TaylorOp + TaylorOp --------------------------------- */

template <class U>
INLINE1 ADDTaylorOp<U>::ADDTaylorOp<U>()
{
}

template <class U>
INLINE1 ADDTaylorOp<U>::ADDTaylorOp<U>(const ADDTaylorOp<U>& x):BINTaylorOp<U>(x)
{
}

template <class U>
INLINE2 ADDTaylorOp<U>::ADDTaylorOp<U>(const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	if (a.val->n>0 && b.val->n>0) (*val)[0]=a[0]+b[0];
	o1=a.copy();
	o2=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* ADDTaylorOp<U>::copy() const
{
	return new ADDTaylorOp<U>(*this);
}

template <class U>
INLINE2 void ADDTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1&&o2)
	if (val->n<=i)
	{
		o1->eval(i);o2->eval(i);
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k]+(*o2)[k];
	}
}

template <class U>
INLINE1 ADDTaylorOp<U> operator+ (const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	return ADDTaylorOp<U>(a,b);
}

template <class U>
INLINE1 ADDTaylorOp<U> operator+ (const TaylorOp<U>& a, const TTypeName<U>& b)
{
	return ADDTaylorOp<U>(a,b);
}

template <class U>
INLINE1 ADDTaylorOp<U> operator+ (const TTypeName<U>& a, const TaylorOp<U>& b)
{
	return ADDTaylorOp<U>(a,b);
}

template <class U>
INLINE1 ADDTaylorOp<U> operator+ (const TTypeName<U>& a, const TTypeName<U>& b)
{
	return ADDTaylorOp<U>(a,b);
}


#ifdef BaseType

/* --------------------------- BaseType + TaylorOp ------------------------------ */

template <class U>
INLINE1 ADDTaylorOp3<U>::ADDTaylorOp3<U>()
{
}

template <class U>
INLINE1 ADDTaylorOp3<U>::ADDTaylorOp3<U>(const ADDTaylorOp3<U>& x):UNTaylorOp<U>(x),ax(x.ax)
{
}

template <class U>
INLINE2 ADDTaylorOp3<U>::ADDTaylorOp3<U>(const BaseType& a, const TaylorOp<U>& b):ax(a)
{
	if (b.val->n>0) (*val)[0]=a+b[0];
	o1=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* ADDTaylorOp3<U>::copy() const
{
	return new ADDTaylorOp3<U>(*this);
}

template <class U>
INLINE2 void ADDTaylorOp3<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=ax+(*o1)[0];
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k];
	}
}

template <class U>
INLINE1 ADDTaylorOp3<U> operator+ (const BaseType& a, const TaylorOp<U>& b)
{
	return ADDTaylorOp3<U>(a,b);
}

template <class U>
INLINE1 ADDTaylorOp3<U> operator+ (const BaseType& a, const TTypeName<U>& b)
{
	return ADDTaylorOp3<U>(a,b);
}

/* --------------------------- TaylorOp + BaseType ------------------------------ */

template <class U>
INLINE1 ADDTaylorOp4<U>::ADDTaylorOp4<U>()
{
}

template <class U>
INLINE1 ADDTaylorOp4<U>::ADDTaylorOp4<U>(const ADDTaylorOp4<U>& x):UNTaylorOp<U>(x),bx(x.bx)
{
}

template <class U>
INLINE2 ADDTaylorOp4<U>::ADDTaylorOp4<U>(const TaylorOp<U>& a, const BaseType& b):bx(b)
{
	if (a.val->n>0) (*val)[0]=a[0]+b;
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* ADDTaylorOp4<U>::copy() const
{
	return new ADDTaylorOp4<U>(*this);
}

template <class U>
INLINE2 void ADDTaylorOp4<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=(*o1)[0]+bx;
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k];
	}
}

template <class U>
INLINE1 ADDTaylorOp4<U> operator+ (const TaylorOp<U>& a, const BaseType& b)
{
	return ADDTaylorOp4<U>(a,b);
}

template <class U>
INLINE1 ADDTaylorOp4<U> operator+ (const TTypeName<U>& a, const BaseType& b)
{
	return ADDTaylorOp4<U>(a,b);
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                     TAYLOR SUBTRACTION FORMULAS                       */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- TaylorOp - TaylorOp --------------------------------- */

template <class U>
INLINE1 SUBTaylorOp<U>::SUBTaylorOp<U>()
{
}

template <class U>
INLINE1 SUBTaylorOp<U>::SUBTaylorOp<U>(const SUBTaylorOp<U>& x):BINTaylorOp<U>(x)
{
}

template <class U>
INLINE2 SUBTaylorOp<U>::SUBTaylorOp<U>(const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	if (a.val->n>0 && b.val->n>0) (*val)[0]=a[0]-b[0];
	o1=a.copy();
	o2=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* SUBTaylorOp<U>::copy() const
{
	return new SUBTaylorOp<U>(*this);
}

template <class U>
INLINE2 void SUBTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1&&o2)
	if (val->n<=i)
	{
		o1->eval(i);o2->eval(i);
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k]-(*o2)[k];
	}
}

/* All combinations of operator- operations. Global definitions. */

template <class U>
INLINE1 SUBTaylorOp<U> operator- (const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	return SUBTaylorOp<U>(a,b);
}

template <class U>
INLINE1 SUBTaylorOp<U> operator- (const TaylorOp<U>& a, const TTypeName<U>& b)
{
	return SUBTaylorOp<U>(a,b);
}

template <class U>
INLINE1 SUBTaylorOp<U> operator- (const TTypeName<U>& a, const TaylorOp<U>& b)
{
	return SUBTaylorOp<U>(a,b);
}

template <class U>
INLINE1 SUBTaylorOp<U> operator- (const TTypeName<U>& a, const TTypeName<U>& b)
{
	return SUBTaylorOp<U>(a,b);
}

#ifdef BaseType

/* --------------------------- BaseType - TaylorOp ------------------------------ */

template <class U>
INLINE1 SUBTaylorOp3<U>::SUBTaylorOp3<U>()
{
}

template <class U>
INLINE1 SUBTaylorOp3<U>::SUBTaylorOp3<U>(const SUBTaylorOp3<U>& x):UNTaylorOp<U>(x),ax(x.ax)
{
}

template <class U>
INLINE2 SUBTaylorOp3<U>::SUBTaylorOp3<U>(const BaseType& a, const TaylorOp<U>& b):ax(a)
{
	if (b.val->n>0) (*val)[0]=a-b[0];
	o1=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* SUBTaylorOp3<U>::copy() const
{
	return new SUBTaylorOp3<U>(*this);
}

template <class U>
INLINE2 void SUBTaylorOp3<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=ax-(*o1)[0];
		for(int k=val->n;k<=i;k++) (*val)[k]=-(*o1)[k];
	}
}

template <class U>
INLINE1 SUBTaylorOp3<U> operator- (const BaseType& a, const TaylorOp<U>& b)
{
	return SUBTaylorOp3<U>(a,b);
}

template <class U>
INLINE1 SUBTaylorOp3<U> operator- (const BaseType& a, const TTypeName<U>& b)
{
	return SUBTaylorOp3<U>(a,b);
}

/* --------------------------- TaylorOp - BaseType ------------------------------ */

template <class U>
INLINE1 SUBTaylorOp4<U>::SUBTaylorOp4<U>()
{
}

template <class U>
INLINE1 SUBTaylorOp4<U>::SUBTaylorOp4<U>(const SUBTaylorOp4<U>& x):UNTaylorOp<U>(x),bx(x.bx)
{
}

template <class U>
INLINE2 SUBTaylorOp4<U>::SUBTaylorOp4<U>(const TaylorOp<U>& a, const BaseType& b):bx(b)
{
	if (a.val->n>0) (*val)[0]=a[0]-b;
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* SUBTaylorOp4<U>::copy() const
{
	return new SUBTaylorOp4<U>(*this);
}

template <class U>
INLINE2 void SUBTaylorOp4<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=(*o1)[0]-bx;
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k];
	}
}

template <class U>
INLINE1 SUBTaylorOp4<U> operator- (const TaylorOp<U>& a, const BaseType& b)
{
	return SUBTaylorOp4<U>(a,b);
}

template <class U>
INLINE1 SUBTaylorOp4<U> operator- (const TTypeName<U>& a, const BaseType& b)
{
	return SUBTaylorOp4<U>(a,b);
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                  TAYLOR MULTIPLICATION FORMULAS                       */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- TaylorOp * TaylorOp --------------------------------- */

template <class U>
INLINE1 MULTaylorOp<U>::MULTaylorOp<U>()
{
}

template <class U>
INLINE1 MULTaylorOp<U>::MULTaylorOp<U>(const MULTaylorOp<U>& x):BINTaylorOp<U>(x)
{
}

template <class U>
INLINE2 MULTaylorOp<U>::MULTaylorOp<U>(const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	if (a.val->n>0 && b.val->n>0) (*val)[0]=a[0]*b[0];
	o1=a.copy();
	o2=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* MULTaylorOp<U>::copy() const
{
	return new MULTaylorOp<U>(*this);
}

template <class U>
INLINE2 void MULTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1&&o2)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);o2->eval(i);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=0;j<=k;j++) (*val)[k]+=(*o1)[j]*(*o2)[k-j];
		}
	}
}

/* All combinations of operator* operations. Global definitions. */

template <class U>
INLINE1 MULTaylorOp<U> operator* (const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	return MULTaylorOp<U>(a,b);
}

template <class U>
INLINE1 MULTaylorOp<U> operator* (const TaylorOp<U>& a, const TTypeName<U>& b)
{
	return MULTaylorOp<U>(a,b);
}

template <class U>
INLINE1 MULTaylorOp<U> operator* (const TTypeName<U>& a, const TaylorOp<U>& b)
{
	return MULTaylorOp<U>(a,b);
}

template <class U>
INLINE1 MULTaylorOp<U> operator* (const TTypeName<U>& a, const TTypeName<U>& b)
{
	return MULTaylorOp<U>(a,b);
}

#ifdef BaseType

/* --------------------------- BaseType * TaylorOp ------------------------------ */

template <class U>
INLINE1 MULTaylorOp3<U>::MULTaylorOp3<U>()
{
}

template <class U>
INLINE1 MULTaylorOp3<U>::MULTaylorOp3<U>(const MULTaylorOp3<U>& x):UNTaylorOp<U>(x),ax(x.ax)
{
}

template <class U>
INLINE2 MULTaylorOp3<U>::MULTaylorOp3<U>(const BaseType& a, const TaylorOp<U>& b):ax(a)
{
	if (b.val->n>0) (*val)[0]=a*b[0];
	o1=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* MULTaylorOp3<U>::copy() const
{
	return new MULTaylorOp3<U>(*this);
}

template <class U>
INLINE2 void MULTaylorOp3<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		for(int k=val->n;k<=i;k++) (*val)[k]=ax*(*o1)[k];
	}
}

template <class U>
INLINE1 MULTaylorOp3<U> operator* (const BaseType& a, const TaylorOp<U>& b)
{
	return MULTaylorOp3<U>(a,b);
}

template <class U>
INLINE1 MULTaylorOp3<U> operator* (const BaseType& a, const TTypeName<U>& b)
{
	return MULTaylorOp3<U>(a,b);
}

/* --------------------------- TaylorOp * BaseType ------------------------------ */

template <class U>
INLINE1 MULTaylorOp4<U>::MULTaylorOp4<U>()
{
}

template <class U>
INLINE1 MULTaylorOp4<U>::MULTaylorOp4<U>(const MULTaylorOp4<U>& x):UNTaylorOp<U>(x),bx(x.bx)
{
}

template <class U>
INLINE2 MULTaylorOp4<U>::MULTaylorOp4<U>(const TaylorOp<U>& a, const BaseType& b):bx(b)
{
	if (a.val->n>0) (*val)[0]=a[0]*b;
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* MULTaylorOp4<U>::copy() const
{
	return new MULTaylorOp4<U>(*this);
}

template <class U>
INLINE2 void MULTaylorOp4<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k]*bx;
	}
}

template <class U>
INLINE1 MULTaylorOp4<U> operator* (const TaylorOp<U>& a, const BaseType& b)
{
	return MULTaylorOp4<U>(a,b);
}

template <class U>
INLINE1 MULTaylorOp4<U> operator* (const TTypeName<U>& a, const BaseType& b)
{
	return MULTaylorOp4<U>(a,b);
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                    TAYLOR DIVISION FORMULAS                           */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- TaylorOp / TaylorOp --------------------------------- */

template <class U>
INLINE1 DIVTaylorOp<U>::DIVTaylorOp<U>()
{
}

template <class U>
INLINE1 DIVTaylorOp<U>::DIVTaylorOp<U>(const DIVTaylorOp<U>& x):BINTaylorOp<U>(x)
{
}

template <class U>
INLINE2 DIVTaylorOp<U>::DIVTaylorOp<U>(const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	if (a.val->n>0 && b.val->n>0) (*val)[0]=a[0]/b[0];
	o1=a.copy();
	o2=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* DIVTaylorOp<U>::copy() const
{
	return new DIVTaylorOp<U>(*this);
}

template <class U>
INLINE2 void DIVTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1&&o2)
	if (val->n<=i)
	{
		o1->eval(i);o2->eval(i);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=(*o1)[k];
			for(int j=1;j<=k;j++) (*val)[k]-=(*o2)[j]*(*val)[k-j];
			(*val)[k]/=(*o2)[0];
		}
	}
}

/* All combinations of operator/ operations. Global definitions. */

template <class U>
INLINE1 DIVTaylorOp<U> operator/ (const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	return DIVTaylorOp<U>(a,b);
}

template <class U>
INLINE1 DIVTaylorOp<U> operator/ (const TaylorOp<U>& a, const TTypeName<U>& b)
{
	return DIVTaylorOp<U>(a,b);
}

template <class U>
INLINE1 DIVTaylorOp<U> operator/ (const TTypeName<U>& a, const TaylorOp<U>& b)
{
	return DIVTaylorOp<U>(a,b);
}

template <class U>
INLINE1 DIVTaylorOp<U> operator/ (const TTypeName<U>& a, const TTypeName<U>& b)
{
	return DIVTaylorOp<U>(a,b);
}

#ifdef BaseType

/* --------------------------- BaseType / TaylorOp ------------------------------ */

template <class U>
INLINE1 DIVTaylorOp3<U>::DIVTaylorOp3<U>()
{
}

template <class U>
INLINE1 DIVTaylorOp3<U>::DIVTaylorOp3<U>(const DIVTaylorOp3<U>& x):UNTaylorOp<U>(x),ax(x.ax)
{
}

template <class U>
INLINE2 DIVTaylorOp3<U>::DIVTaylorOp3<U>(const BaseType& a, const TaylorOp<U>& b):ax(a)
{
	if (b.val->n>0) (*val)[0]=a/b[0];
	o1=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* DIVTaylorOp3<U>::copy() const
{
	return new DIVTaylorOp3<U>(*this);
}

template <class U>
INLINE2 void DIVTaylorOp3<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=ax/(*o1)[0];
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=BaseType(0);
			for(j=1;j<=k;j++)
				(*val)[k]-=(*o1)[j]*(*val)[k-j];
			(*val)[k]/=(*o1)[0];
		}
	}
}

template <class U>
INLINE1 DIVTaylorOp3<U> operator/ (const BaseType& a, const TaylorOp<U>& b)
{
	return DIVTaylorOp3<U>(a,b);
}

template <class U>
INLINE1 DIVTaylorOp3<U> operator/ (const BaseType& a, const TTypeName<U>& b)
{
	return DIVTaylorOp3<U>(a,b);
}

/* --------------------------- TaylorOp / BaseType ------------------------------ */

template <class U>
INLINE1 DIVTaylorOp4<U>::DIVTaylorOp4<U>()
{
}

template <class U>
INLINE1 DIVTaylorOp4<U>::DIVTaylorOp4<U>(const DIVTaylorOp4<U>& x):UNTaylorOp<U>(x),bx(x.bx)
{
}

template <class U>
INLINE2 DIVTaylorOp4<U>::DIVTaylorOp4<U>(const TaylorOp<U>& a, const BaseType& b):bx(b)
{
	if (a.val->n>0) (*val)[0]=a[0]/b;
	o1=a.copy();
}

template <class U>
INLINE2 TaylorOp<U>* DIVTaylorOp4<U>::copy() const
{
	DIVTaylorOp4<U>* o=new DIVTaylorOp4<U>(*this);
	return o;
}

template <class U>
INLINE2 void DIVTaylorOp4<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k]/bx;
	}
}

template <class U>
INLINE1 DIVTaylorOp4<U> operator/ (const TaylorOp<U>& a, const BaseType& b)
{
	return DIVTaylorOp4<U>(a,b);
}

template <class U>
INLINE1 DIVTaylorOp4<U> operator/ (const TTypeName<U>& a, const BaseType& b)
{
	return DIVTaylorOp4<U>(a,b);
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            TAYLOR power FORMULA  (still to be implemented)            */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* -------------------------- pow(TaylorOp,TaylorOp) ------------------------------- */

template <class U>
INLINE1 POWTaylorOp<U>::POWTaylorOp<U>()
{
}

template <class U>
INLINE1 POWTaylorOp<U>::POWTaylorOp<U>(const POWTaylorOp<U>& x):BINTaylorOp<U>(x)
{
}

template <class U>
INLINE2 POWTaylorOp<U>::POWTaylorOp<U>(const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	if (a.val->n>0 && b.val->n>0) (*val)[0]=pow(a[0],b[0]);
	o1=a.copy();
	o2=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* POWTaylorOp<U>::copy() const
{
	return new POWTaylorOp<U>(*this);
}

template <class U>
INLINE1 void POWTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1&&o2)
	cerr << "pow(op,TaylorOp) not implemented yet" << std::endl;
}

/* All combinations of pow operations. Global definitions. */

template <class U>
INLINE1 POWTaylorOp<U> pow (const TaylorOp<U>& a, const TaylorOp<U>& b)
{
	return POWTaylorOp<U>(a,b);
}

template <class U>
INLINE1 POWTaylorOp<U> pow (const TaylorOp<U>& a, const TTypeName<U>& b)
{
	return POWTaylorOp<U>(a,b);
}

template <class U>
INLINE1 POWTaylorOp<U> pow (const TTypeName<U>& a, const TaylorOp<U>& b)
{
	return POWTaylorOp<U>(a,b);
}

template <class U>
INLINE1 POWTaylorOp<U> pow (const TTypeName<U>& a, const TTypeName<U>& b)
{
	return POWTaylorOp<U>(a,b);
}

#ifdef BaseType

/* ------------------------ pow(BaseType,TaylorOp) -------------------------- */

template <class U>
INLINE1 POWTaylorOp3<U>::POWTaylorOp3<U>()
{
}

template <class U>
INLINE1 POWTaylorOp3<U>::POWTaylorOp3<U>(const POWTaylorOp3<U>& x):UNTaylorOp<U>(x),ax(x.ax)
{
}

template <class U>
INLINE2 POWTaylorOp3<U>::POWTaylorOp3<U>(const BaseType& a, const TaylorOp<U>& b):ax(a)
{
	if (b.val->n>0) (*val)[0]=pow(a,b[0]);
	o1=b.copy();
}

template <class U>
INLINE1 TaylorOp<U>* POWTaylorOp3<U>::copy() const
{
	return new POWTaylorOp3<U>(*this);
}

template <class U>
INLINE1 void POWTaylorOp3<U>::eval(int i)
{
	ASSERT(val&&o1)
	cerr << "pow(BaseType,TaylorOp) not implemented yet" << std::endl;
}

template <class U>
INLINE1 POWTaylorOp3<U> pow (const BaseType& a, const TaylorOp<U>& b)
{
	return POWTaylorOp3<U>(a,b);
}

template <class U>
INLINE1 POWTaylorOp3<U> pow (const BaseType& a, const TTypeName<U>& b)
{
	return POWTaylorOp3<U>(a,b);
}

/* ------------------------ pow(TaylorOp,BaseType) --------------------------- */

template <class U>
INLINE1 POWTaylorOp4<U>::POWTaylorOp4<U>()
{
}

template <class U>
INLINE1 POWTaylorOp4<U>::POWTaylorOp4<U>(const POWTaylorOp4<U>& x):UNTaylorOp<U>(x),bx(x.bx)
{
}

template <class U>
INLINE2 POWTaylorOp4<U>::POWTaylorOp4<U>(const TaylorOp<U>& a, const BaseType& b):bx(b)
{
	if (a.val->n>0) (*val)[0]=pow(a[0],b);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* POWTaylorOp4<U>::copy() const
{
	return new POWTaylorOp4<U>(*this);
}

template <class U>
INLINE1 void POWTaylorOp4<U>::eval(int i)
{
	ASSERT(val&&o1)
	cerr << "pow(op,BaseType) not implemented yet" << std::endl;
}

template <class U>
INLINE1 POWTaylorOp4<U> pow (const TaylorOp<U>& a, const BaseType& b)
{
	return POWTaylorOp4<U>(a,b);
}

template <class U>
INLINE1 POWTaylorOp4<U> pow (const TTypeName<U>& a, const BaseType& b)
{
	return POWTaylorOp4<U>(a,b);
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                     TAYLOR UNARY + FORMULA                            */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class U>
INLINE1 UADDTaylorOp<U>::UADDTaylorOp<U>()
{
}

template <class U>
INLINE1 UADDTaylorOp<U>::UADDTaylorOp<U>(const UADDTaylorOp<U>& x):UNTaylorOp<U>(x)
{
}

template <class U>
INLINE2 UADDTaylorOp<U>::UADDTaylorOp<U>(const TaylorOp<U>& a)
{
	if (a.val->n>0) (*val)[0]=+a[0];
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* UADDTaylorOp<U>::copy() const
{
	return new UADDTaylorOp<U>(*this);
}

template <class U>
INLINE2 void UADDTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		for(int k=val->n;k<=i;k++) (*val)[k]=(*o1)[k];
	}
}

/* Global definition of operator+ operation. */

template <class U>
INLINE1 UADDTaylorOp<U> operator+ (const TaylorOp<U>& a)
{
	return UADDTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                      TAYLOR UNARY - FORMULA                           */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class U>
INLINE1 USUBTaylorOp<U>::USUBTaylorOp<U>()
{
}

template <class U>
INLINE1 USUBTaylorOp<U>::USUBTaylorOp<U>(const USUBTaylorOp<U>& x):UNTaylorOp<U>(x)
{
}

template <class U>
INLINE2 USUBTaylorOp<U>::USUBTaylorOp<U>(const TaylorOp<U>& a)
{
	if (a.val->n>0) (*val)[0]=-a[0];
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* USUBTaylorOp<U>::copy() const
{
	return new USUBTaylorOp<U>(*this);
}

template <class U>
INLINE2 void USUBTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	if (val->n<=i)
	{
		o1->eval(i);
		for(int k=val->n;k<=i;k++) (*val)[k]=-(*o1)[k];
	}
}

/* Global definition of operator- operation. */

template <class U>
INLINE1 USUBTaylorOp<U> operator- (const TaylorOp<U>& a)
{
	return USUBTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                    TAYLOR pow(TaylorOp,int) FORMULA                        */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
INLINE1 POWTaylorOpN<U>::POWTaylorOpN<U>()
{
}

template <class U>
INLINE1 POWTaylorOpN<U>::POWTaylorOpN<U>(const POWTaylorOpN<U>& x):UNTaylorOp<U>(x),b(x.b)
{
}

template <class U>
INLINE2 POWTaylorOpN<U>::POWTaylorOpN<U>(const TaylorOp<U>& a,int ib):b(ib)
{
	if (a.val->n>0) (*val)[0]=_pow(a[0],ib);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* POWTaylorOpN<U>::copy() const
{
	return new POWTaylorOpN<U>(*this);
}

template <class U>
INLINE2 void POWTaylorOpN<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=_pow((*o1)[0],b);

		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=0;j<k;j++)
				(*val)[k]+=CONVINT(b*(k-j)-j)*(*o1)[k-j]*(*val)[j];
			(*val)[k]/=CONVINT(k)*(*o1)[0];
		}
	}
}

/* Global definition of pow-operation. */
template <class U>
INLINE1 POWTaylorOpN<U> pow (const TaylorOp<U>& a, int b)
{
	return POWTaylorOpN<U>(a,b);
}

template <class U>
INLINE1 POWTaylorOpN<U> pow (const TTypeName<U>& a, int b)
{
	return POWTaylorOpN<U>(a,b);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR sqr FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
INLINE1 SQRTaylorOp<U>::SQRTaylorOp<U>()
{
}

template <class U>
INLINE1 SQRTaylorOp<U>::SQRTaylorOp<U>(const SQRTaylorOp<U>& x):UNTaylorOp<U>(x)
{
}

template <class U>
INLINE2 SQRTaylorOp<U>::SQRTaylorOp<U>(const TaylorOp<U>& a)
{
	if (a.val->n>0) (*val)[0]=_sqr(a[0]);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* SQRTaylorOp<U>::copy() const
{
	return new SQRTaylorOp<U>(*this);
}

template <class U>
INLINE2 void SQRTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j,m;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=_sqr((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			m=(k+1)/2;
			for(j=0;j<m;j++)
				(*val)[k]+=(*o1)[k-j]*(*o1)[j];
			(*val)[k]*=CONVINT(2);
			if (k%2==0) (*val)[k]+=_sqr((*o1)[m]);
		}
	}
}

/* Global definition of SQR operation. */

template <class U>
INLINE1 SQRTaylorOp<U> sqr (const TaylorOp<U>& a)
{
	return SQRTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                         TAYLOR exp FORMULA                            */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class U>
INLINE1 EXPTaylorOp<U>::EXPTaylorOp<U>()
{
}

template <class U>
INLINE1 EXPTaylorOp<U>::EXPTaylorOp<U>(const EXPTaylorOp<U>& x):UNTaylorOp<U>(x)
{
}

template <class U>
INLINE2 EXPTaylorOp<U>::EXPTaylorOp<U>(const TaylorOp<U>& a)
{
	if (a.val->n>0) (*val)[0]=exp(a[0]);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* EXPTaylorOp<U>::copy() const
{
	return new EXPTaylorOp<U>(*this);
}

template <class U>
INLINE2 void EXPTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=exp((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=0;j<k;j++)
				(*val)[k]+=(CONVINT(1)-CONVINT(j)/CONVINT(k))*(*o1)[k-j]*(*val)[j];
		}
	}
}

/* Global definition of exp operation. */

template <class U>
INLINE1 EXPTaylorOp<U> exp (const TaylorOp<U>& a)
{
	return EXPTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                         TAYLOR log FORMULA                            */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class U>
INLINE1 LOGTaylorOp<U>::LOGTaylorOp<U>()
{
}

template <class U>
INLINE1 LOGTaylorOp<U>::LOGTaylorOp<U>(const LOGTaylorOp<U>& x):UNTaylorOp<U>(x)
{
}

template <class U>
INLINE2 LOGTaylorOp<U>::LOGTaylorOp<U>(const TaylorOp<U>& a)
{
	if (a.val->n>0) (*val)[0]=log(a[0]);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* LOGTaylorOp<U>::copy() const
{
	return new LOGTaylorOp<U>(*this);
}

template <class U>
INLINE2 void LOGTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=log((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=(*o1)[k];
			for(j=1;j<k;j++)
				(*val)[k]-=(CONVINT(1)-CONVINT(j)/CONVINT(k))*(*o1)[j]*(*val)[k-j];
			(*val)[k]/=(*o1)[0];
		}
	}
}

/* Global definition of log operation. */

template <class U>
INLINE1 LOGTaylorOp<U> log (const TaylorOp<U>& a)
{
	return LOGTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR sqrt FORMULA                            */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class U>
INLINE1 SQRTTaylorOp<U>::SQRTTaylorOp<U>()
{
}

template <class U>
INLINE1 SQRTTaylorOp<U>::SQRTTaylorOp<U>(const SQRTTaylorOp<U>& x):UNTaylorOp<U>(x)
{
}

template <class U>
INLINE2 SQRTTaylorOp<U>::SQRTTaylorOp<U>(const TaylorOp<U>& a)
{
	if (a.val->n>0) (*val)[0]=sqrt(a[0]);
	o1=a.copy();
}

template <class U>
INLINE2 TaylorOp<U>* SQRTTaylorOp<U>::copy() const
{
	return new SQRTTaylorOp<U>(*this);
}

template <class U>
INLINE2 void SQRTTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j,m;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0) (*val)[0]=sqrt((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			m=(k+1)/2;
			(*val)[k]=U(0);
			for(j=1;j<m;j++)
				(*val)[k]+=(*val)[k-j]*(*val)[j];
			(*val)[k]*=CONVINT(2);
			if (k%2==0) (*val)[k]+=_pow((*val)[m],2);
			(*val)[k]=((*o1)[k]-(*val)[k])/(CONVINT(2)*(*val)[0]);
		}
	}
}

/* Global definition of sqrt operation. */

template <class U>
INLINE1 SQRTTaylorOp<U> sqrt (const TaylorOp<U>& a)
{
	return SQRTTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR sin FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class U>
INLINE1 SINTaylorOp<U>::SINTaylorOp<U>()
{
}

template <class U>
INLINE1 SINTaylorOp<U>::SINTaylorOp<U>(const SINTaylorOp<U>& x):UNTaylorOp<U>(x),COS(x.COS)
{
}

template <class U>
INLINE2 SINTaylorOp<U>::SINTaylorOp<U>(const TaylorOp<U>& a)
{
	COS=new U[MaxLength];
	if (a.val->n>0)
	{
		(*val)[0]=sin(a[0]);
		COS[0]=cos(a[0]);
	}
	o1=a.copy();
}

template <class U>
INLINE1 SINTaylorOp<U>::~SINTaylorOp<U>()
{
	if ((val)&&(val->rc==1)&&(COS)) delete[] COS;
}

template <class U>
INLINE1 TaylorOp<U>* SINTaylorOp<U>::copy() const
{
	return new SINTaylorOp<U>(*this);
}

template <class U>
INLINE2 void SINTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0)
		{
			(*val)[0]=sin((*o1)[0]);
			COS[0]=cos((*o1)[0]);
		}
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=0;j<k;j++)
				(*val)[k]+=CONVINT(j+1)*COS[k-1-j]*(*o1)[j+1];
			(*val)[k]/=CONVINT(k);
			COS[k]=U(0);
			for(j=0;j<k;j++)
				COS[k]-=CONVINT(j+1)*(*val)[k-1-j]*(*o1)[j+1];
			COS[k]/=CONVINT(k);
		}
	}
}

/* Global definition of sin operation. */

template <class U>
INLINE1 SINTaylorOp<U> sin (const TaylorOp<U>& a)
{
	return SINTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                          TAYLOR cos FORMULA                           */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class U>
INLINE1 COSTaylorOp<U>::COSTaylorOp<U>()
{
}

template <class U>
INLINE1 COSTaylorOp<U>::COSTaylorOp<U>(const COSTaylorOp<U>& x):UNTaylorOp<U>(x),SIN(x.SIN)
{
}

template <class U>
INLINE2 COSTaylorOp<U>::COSTaylorOp<U>(const TaylorOp<U>& a)
{
	SIN=new U[MaxLength];
	if (a.val->n>0)
	{
		(*val)[0]=cos(a[0]);
		 SIN[0]=sin(a[0]);
	 }
	o1=a.copy();
}

template <class U>
INLINE1 COSTaylorOp<U>::~COSTaylorOp<U>()
{
	if ((val)&&(val->rc==1)&&(SIN)) delete[] SIN;
}

template <class U>
INLINE2 TaylorOp<U>* COSTaylorOp<U>::copy() const
{
	COSTaylorOp<U>* o=new COSTaylorOp<U>(*this);
	return o;
}

template <class U>
INLINE2 void COSTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		if (val->n==0)
		{
			(*val)[0]=cos((*o1)[0]);
			SIN[0]=sin((*o1)[0]);
		}
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=0;j<k;j++)
				(*val)[k]-=CONVINT(j+1)*SIN[k-1-j]*(*o1)[j+1];
			(*val)[k]/=CONVINT(k);
			SIN[k]=U(0);
			for(j=0;j<k;j++)
				SIN[k]+=CONVINT(j+1)*(*val)[k-1-j]*(*o1)[j+1];
			SIN[k]/=CONVINT(k);
		}
	}
}

/* Global definition of cos operation. */

template <class U>
INLINE1 COSTaylorOp<U> cos (const TaylorOp<U>& a)
{
	return COSTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        TAYLOR tan FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
INLINE1 TANTaylorOp<U>::TANTaylorOp<U>()
{
}

template <class U>
INLINE1 TANTaylorOp<U>::TANTaylorOp<U>(const TANTaylorOp<U>& x):UNTaylorOp<U>(x),SQRCOS(x.SQRCOS)
{
}

template <class U>
INLINE2 TANTaylorOp<U>::TANTaylorOp<U>(const TaylorOp<U>& a):SQRCOS(pow(cos(a),2))
{
	if (a.val->n>0) (*val)[0]=tan(a[0]);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* TANTaylorOp<U>::copy() const
{
	return new TANTaylorOp<U>(*this);
}

template <class U>
INLINE2 void TANTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i) {
		o1->eval(i);
		SQRCOS.eval(i);
		if (val->n==0) (*val)[0]=tan((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=1;j<k;j++)
				(*val)[k]+=CONVINT(j)*(*val)[j]*SQRCOS[k-j];
			(*val)[k]=((*o1)[k]-(*val)[k]/CONVINT(k))/SQRCOS[0];
		}
	}
}

template <class U>
INLINE2 void TANTaylorOp<U>::reset()
{
	ASSERT(val)
	SQRCOS.reset();
	if (o1) o1->reset();
	val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
}

/* Global definition of tan operation. */

template <class U>
INLINE1 TANTaylorOp<U> tan (const TaylorOp<U>& a)
{
	return TANTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                      TAYLOR asin FORMULA                              */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
INLINE1 ASINTaylorOp<U>::ASINTaylorOp<U>()
{
}

template <class U>
INLINE1 ASINTaylorOp<U>::ASINTaylorOp<U>(const ASINTaylorOp<U>& x):UNTaylorOp<U>(x),SQRT(x.SQRT)
{
}

template <class U>
INLINE2 ASINTaylorOp<U>::ASINTaylorOp<U>(const TaylorOp<U>& a):SQRT(sqrt(CONVINT(1)-pow(a,2)))
{
	if (a.val->n>0) (*val)[0]=asin(a[0]);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* ASINTaylorOp<U>::copy() const
{
	return new ASINTaylorOp<U>(*this);
}

template <class U>
INLINE2 void ASINTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		SQRT.eval(i);
		if (val->n==0) (*val)[0]=asin((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=1;j<k;j++)
				(*val)[k]+=CONVINT(j)*(*val)[j]*SQRT[k-j];
			(*val)[k]=((*o1)[k]-(*val)[k]/CONVINT(k))/SQRT[0];
		}
	}
}

template <class U>
INLINE2 void ASINTaylorOp<U>::reset()
{
	ASSERT(val)
	SQRT.reset();
	if (o1) o1->reset();
	val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
}

/* Global definition of tan operation. */

template <class U>
INLINE1 ASINTaylorOp<U> asin (const TaylorOp<U>& a)
{
	return ASINTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                       TAYLOR acos FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
INLINE1 ACOSTaylorOp<U>::ACOSTaylorOp<U>()
{
}

template <class U>
INLINE1 ACOSTaylorOp<U>::ACOSTaylorOp<U>(const ACOSTaylorOp<U>& x):UNTaylorOp<U>(x),SQRT(x.SQRT)
{
}

template <class U>
INLINE2 ACOSTaylorOp<U>::ACOSTaylorOp<U>(const TaylorOp<U>& a):SQRT(sqrt(CONVINT(1)-pow(a,2)))
{
	if (a.val->n>0) (*val)[0]=acos(a[0]);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* ACOSTaylorOp<U>::copy() const
{
	return new ACOSTaylorOp<U>(*this);
}

template <class U>
INLINE2 void ACOSTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		SQRT.eval(i);
		if (val->n==0) (*val)[0]=acos((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=1;j<k;j++)
				(*val)[k]+=CONVINT(j)*(*val)[j]*SQRT[k-j];
			(*val)[k]=-((*o1)[k]+(*val)[k]/CONVINT(k))/SQRT[0];
		}
	}
}

template <class U>
INLINE2 void ACOSTaylorOp<U>::reset()
{
	ASSERT(val)
	SQRT.reset();
	if (o1) o1->reset();
	val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
}

/* Global definition of tan operation. */

template <class U>
INLINE1 ACOSTaylorOp<U> acos (const TaylorOp<U>& a)
{
	return ACOSTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                       TAYLOR atan FORMULA                             */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
INLINE1 ATANTaylorOp<U>::ATANTaylorOp<U>()
{
}

template <class U>
INLINE1 ATANTaylorOp<U>::ATANTaylorOp<U>(const ATANTaylorOp<U>& x):UNTaylorOp<U>(x),IPSQR(x.IPSQR)
{
}

template <class U>
INLINE2 ATANTaylorOp<U>::ATANTaylorOp<U>(const TaylorOp<U>& a):IPSQR(CONVINT(1)+pow(a,2))
{
	if (a.val->n>0) (*val)[0]=atan(a[0]);
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* ATANTaylorOp<U>::copy() const
{
	return new ATANTaylorOp<U>(*this);
}

template <class U>
INLINE2 void ATANTaylorOp<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
		o1->eval(i);
		IPSQR.eval(i);
		if (val->n==0) (*val)[0]=atan((*o1)[0]);
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=U(0);
			for(j=1;j<k;j++)
				(*val)[k]+=CONVINT(j)*(*val)[j]*IPSQR[k-j];
			(*val)[k]=((*o1)[k]-(*val)[k]/CONVINT(k))/IPSQR[0];
		}
	}
}

template <class U>
INLINE2 void ATANTaylorOp<U>::reset()
{
	ASSERT(val)
	IPSQR.reset();
	if (o1) o1->reset();
	val->n=0; //Can't be done before subtrees as TaylorNode might be used by them
}

/* Global definition of tan operation. */

template <class U>
INLINE1 ATANTaylorOp<U> atan (const TaylorOp<U>& a)
{
	return ATANTaylorOp<U>(a);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                   COMPOUND ASSIGNMENT OPERATORS                       */
/*                                                                       */
/* --------------------------------------------------------------------- */


/* Code for tad-variables += operator. Uses overloaded definition of + */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator += (const TaylorOp<U>& x)
{
	return *this = (*this)+x;
}

/* Code for tad-variables -= operator. Uses overloaded definition of - */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator -= (const TaylorOp<U>& x)
{
	return *this = *this-x;
}

/* Code for tad-variables *= operator. Uses overloaded definition of * */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator *= (const TaylorOp<U>& x)
{
	return *this = *this*x;
}

/* Code for tad-variables /= operator. Uses overloaded definition of / */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator /= (const TaylorOp<U>& x)
{
	return *this = *this/x;
}

/* Code for tad-variables += operator. Uses overloaded definition of + */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator += (const TTypeName<U>& x)
{
	return *this = (*this)+x;
}

/* Code for tad-variables -= operator. Uses overloaded definition of - */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator -= (const TTypeName<U>& x)
{
	return *this = *this-x;
}

/* Code for tad-variables *= operator. Uses overloaded definition of * */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator *= (const TTypeName<U>& x)
{
	*this = *this*x;
	return *this;
}

/* Code for tad-variables /= operator. Uses overloaded definition of / */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator /= (const TTypeName<U>& x)
{
	return *this = *this/x;
}

/* Code for tad-variables += operator. Uses overloaded definition of + */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator += (const U& x)
{
	return *this = (*this)+x;
}

/* Code for tad-variables -= operator. Uses overloaded definition of - */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator -= (const U& x)
{
	return *this = *this-x;
}

/* Code for tad-variables *= operator. Uses overloaded definition of * */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator *= (const U& x)
{
	return *this = *this*x;
}

/* Code for tad-variables /= operator. Uses overloaded definition of / */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator /= (const U& x)
{
	return *this = *this/x;
}

#ifdef BaseType
/* Code for tad-variables += operator. Uses overloaded definition of + */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator += (const BaseType& x)
{
	return *this = (*this)+x;
}

/* Code for tad-variables -= operator. Uses overloaded definition of - */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator -= (const BaseType& x)
{
	return *this = *this-x;
}

/* Code for tad-variables *= operator. Uses overloaded definition of * */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator *= (const BaseType& x)
{
	return *this = *this*x;
}

/* Code for tad-variables /= operator. Uses overloaded definition of / */
template <class U>
INLINE1 TTypeName<U>& TTypeName<U>::operator /= (const BaseType& x)
{
	return *this = *this/x;
}
#endif

#ifdef BaseType

INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator += (const TaylorOp<BaseType>& x)
{
	return *this = (*this)+x;
}

/* Code for tad-variables -= operator. Uses overloaded definition of - */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator -= (const TaylorOp<BaseType>& x)
{
	return *this = *this-x;
}

/* Code for tad-variables *= operator. Uses overloaded definition of * */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator *= (const TaylorOp<BaseType>& x)
{
	return *this = *this*x;
}

/* Code for tad-variables /= operator. Uses overloaded definition of / */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator /= (const TaylorOp<BaseType>& x)
{
	return *this = *this/x;
}

/* Code for tad-variables += operator. Uses overloaded definition of + */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator += (const TTypeName<BaseType>& x)
{
	return *this = (*this)+x;
}

/* Code for tad-variables -= operator. Uses overloaded definition of - */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator -= (const TTypeName<BaseType>& x)
{
	return *this = *this-x;
}

/* Code for tad-variables *= operator. Uses overloaded definition of * */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator *= (const TTypeName<BaseType>& x)
{
	return *this = *this*x;
}

/* Code for tad-variables /= operator. Uses overloaded definition of / */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator /= (const TTypeName<BaseType>& x)
{
	return *this = *this/x;
}

/* Code for tad-variables += operator. Uses overloaded definition of + */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator += (const BaseType& x)
{
	return *this = (*this)+x;
}

/* Code for tad-variables -= operator. Uses overloaded definition of - */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator -= (const BaseType& x)
{
	return *this = *this-x;
}

/* Code for tad-variables *= operator. Uses overloaded definition of * */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator *= (const BaseType& x)
{
	return *this = *this*x;
}

/* Code for tad-variables /= operator. Uses overloaded definition of / */
INLINE0 TTypeName<BaseType>& TTypeName<BaseType>::operator /= (const BaseType& x)
{
	return *this = *this/x;
}
#endif

#ifdef HASEQ
template <class U>
INLINE0 bool operator == (const TTypeName<U> &a, const TTypeName<U> &b)
{
	return (a.root()==b.root());
}
#ifdef BaseType
template <class U>
INLINE0 bool operator == (const TTypeName<U> &a, const BaseType &b)
{
	return (a.root()==b);
}
template <class U>
INLINE0 bool operator == (const BaseType &a, const TTypeName<U> &b)
{
	return (a==b.root());
}
#endif
#endif
#ifdef HASNEQ
template <class U>
INLINE0 bool operator != (const TTypeName<U> &a, const TTypeName<U> &b)
{
	return (a.root()!=b.root());
}
#ifdef BaseType
template <class U>
INLINE0 bool operator != (const TTypeName<U> &a, const BaseType &b)
{
	return (a.root()!=b);
}
template <class U>
INLINE0 bool operator != (const BaseType &a, const TTypeName<U> &b)
{
	return (a!=b.root());
}
#endif
#endif
#ifdef HASGT
template <class U>
INLINE0 bool operator > (const TTypeName<U> &a, const TTypeName<U> &b)
{
	return (a.root()>b.root());
}
#ifdef BaseType
template <class U>
INLINE0 bool operator > (const TTypeName<U> &a, const BaseType &b)
{
	return (a.root()>b);
}
template <class U>
INLINE0 bool operator > (const BaseType &a, const TTypeName<U> &b)
{
	return (a>b.root());
}
#endif
#endif
#ifdef HASGEQ
template <class U>
INLINE0 bool operator >= (const TTypeName<U> &a, const TTypeName<U> &b)
{
	return (a.root()>=b.root());
}
#ifdef BaseType
template <class U>
INLINE0 bool operator >= (const TTypeName<U> &a, const BaseType &b)
{
	return (a.root()>=b);
}
template <class U>
INLINE0 bool operator >= (const BaseType &a, const TTypeName<U> &b)
{
	return (a>=b.root());
}
#endif
#endif
#ifdef HASLT
template <class U>
INLINE0 bool operator < (const TTypeName<U> &a, const TTypeName<U> &b)
{
	return (a.root()<b.root());
}
#ifdef BaseType
template <class U>
INLINE0 bool operator < (const TTypeName<U> &a, const BaseType &b)
{
	return (a.root()<b);
}
template <class U>
INLINE0 bool operator < (const BaseType &a, const TTypeName<U> &b)
{
	return (a<b.root());
}
#endif
#endif
#ifdef HASLEQ
template <class U>
INLINE0 bool operator <= (const TTypeName<U> &a, const TTypeName<U> &b)
{
	return (a.root()<=b.root());
}
#ifdef BaseType
template <class U>
INLINE0 bool operator <= (const TTypeName<U> &a, const BaseType &b)
{
	return (a.root()<=b);
}
template <class U>
INLINE0 bool operator <= (const BaseType &a, const TTypeName<U> &b)
{
	return (a<=b.root());
}
#endif
#endif

#ifdef HASEQ
INLINE0 bool operator == (const TTypeName<BaseType> &a, const TTypeName<BaseType> &b)
{
	return (a.root()==b.root());
}
INLINE0 bool operator == (const TTypeName<BaseType> &a, const BaseType &b)
{
	return (a.root()==b);
}
INLINE0 bool operator == (const BaseType &a, const TTypeName<BaseType> &b)
{
	return (a==b.root());
}
#endif
#ifdef HASNEQ
INLINE0 bool operator != (const TTypeName<BaseType> &a, const TTypeName<BaseType> &b)
{
	return (a.root()!=b.root());
}
INLINE0 bool operator != (const TTypeName<BaseType> &a, const BaseType &b)
{
	return (a.root()!=b);
}
INLINE0 bool operator != (const BaseType &a, const TTypeName<BaseType> &b)
{
	return (a!=b.root());
}
#endif
#ifdef HASGT
INLINE0 bool operator > (const TTypeName<BaseType> &a, const TTypeName<BaseType> &b)
{
	return (a.root()>b.root());
}
INLINE0 bool operator > (const TTypeName<BaseType> &a, const BaseType &b)
{
	return (a.root()>b);
}
INLINE0 bool operator > (const BaseType &a, const TTypeName<BaseType> &b)
{
	return (a>b.root());
}
#endif
#ifdef HASGEQ
INLINE0 bool operator >= (const TTypeName<BaseType> &a, const TTypeName<BaseType> &b)
{
	return (a.root()>=b.root());
}
INLINE0 bool operator >= (const TTypeName<BaseType> &a, const BaseType &b)
{
	return (a.root()>=b);
}
INLINE0 bool operator >= (const BaseType &a, const TTypeName<BaseType> &b)
{
	return (a>=b.root());
}
#endif
#ifdef HASLT
INLINE0 bool operator < (const TTypeName<BaseType> &a, const TTypeName<BaseType> &b)
{
	return (a.root()<b.root());
}
INLINE0 bool operator < (const TTypeName<BaseType> &a, const BaseType &b)
{
	return (a.root()<b);
}
INLINE0 bool operator < (const BaseType &a, const TTypeName<BaseType> &b)
{
	return (a<b.root());
}
#endif
#ifdef HASLEQ
INLINE0 bool operator <= (const TTypeName<BaseType> &a, const TTypeName<BaseType> &b)
{
	return (a.root()<=b.root());
}
INLINE0 bool operator <= (const TTypeName<BaseType> &a, const BaseType &b)
{
	return (a.root()<=b);
}
INLINE0 bool operator <= (const BaseType &a, const TTypeName<BaseType> &b)
{
	return (a<=b.root());
}
#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                    TAYLOR diff(TaylorOp,int) FORMULA                       */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class U>
INLINE1 DIFFTaylorOpN<U>::DIFFTaylorOpN<U>()
{
}

template <class U>
INLINE1 DIFFTaylorOpN<U>::DIFFTaylorOpN<U>(const DIFFTaylorOpN<U>& x):UNTaylorOp<U>(x),b(x.b)
{
}

template <class U>
INLINE2 DIFFTaylorOpN<U>::DIFFTaylorOpN<U>(const TaylorOp<U>& a,int ib):b(ib)
{
// IF THE ARGUMENT TAYLOR EXPANSION TaylorOp HAS BEEN EVALUATED TO
// DEGREE ib THEN WE CAN EVALUATE THE ZERO ORDER VALUE OF
// diff(TaylorOp,ib).
// THIS FUNCTION EVALUATES THE 0.ORDER COEFFICIENT
	if (a.val->n>ib)
	{
		(*val)[0]=a[ib];
		for(int fact=ib;fact>1;fact--)(*val)[0]*=fact;
	}
	o1=a.copy();
}

template <class U>
INLINE1 TaylorOp<U>* DIFFTaylorOpN<U>::copy() const
{
	return new DIFFTaylorOpN<U>(*this);
}

template <class U>
INLINE2 void DIFFTaylorOpN<U>::eval(int i)
{
	ASSERT(val&&o1)
	int j;
	if (val->n<=i)
	{
// IN ORDER TO COMPUTE i'th ORDER COEFFICIENTS OF diff(o1,b)
// WE NEED (i+b)'th ORDER COEFFICIENTS OF TaylorOp.
		o1->eval(i+b);
// NOW WE SHOULD HAVE o1 EXPANDED TO DEGREE (i+b)'th ORDER,
// WE CAN PROCEED TO COMPUTE UP TO i'TH ORDER COEFFICIENTS
// OF diff(o1,b).
		for(int k=val->n;k<=i;k++)
		{
			(*val)[k]=(*o1)[k+b];
			for(int fact=(k+b);fact>k;fact--)(*val)[k]*=fact;
		}
	}
}

/* Global definition of diff-operation. */
template <class U>
INLINE1 DIFFTaylorOpN<U> diff (const TaylorOp<U>& a, int b)
{
	return DIFFTaylorOpN<U>(a,b);
}

template <class U>
INLINE1 DIFFTaylorOpN<U> diff (const TTypeName<U>& a, int b)
{
	return DIFFTaylorOpN<U>(a,b);
}

#endif
