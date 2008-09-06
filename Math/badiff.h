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
//                         COPYRIGHT NOTICE
// ***************************************************************

#ifndef _BADIFF_H
#define _BADIFF_H

#include <math.h>
#include <iostream>

#include "fadbad.h"

template <class T>
class BackwardNode
{
/* This class represents the BackwardNode of the tree formed during the forward
** sweep. It stores a value, v. During the differentiation phase a vector
** for storing the partial derivatives, g is allocated and this vector
** ceases to excist when all its elements have been propagated. The length
** of g is stored in gsize and the number of times the BackwardNode is referenced
** is stored in the resource counter, rc.
*/

//public: // Since access otherwise becomes too complicated.
private:
	T m_v;
	T *m_g;
	int m_gsize;
	int m_rc;
public:
	BackwardNode();
	BackwardNode(const T& x);
	~BackwardNode();
	void touchg(int n);
	T& value(){ return m_v; }
	T& deriv(const int idx){ return m_g[idx]; }
	int size() const { return m_gsize; }
	int rc() const { return m_rc; }
	int inc_rc() { return ++m_rc; }
	int dec_rc() { return --m_rc; }
};

template <class T>
class BackwardOp
{
/* This class is the baseclass of all operation met during the forward
** sweep. It contains a pointer to a BackwardNode where the result of the operation
** is stored, val.
*/

public:
	BackwardNode<T> *val;

protected:
	BackwardOp();
	BackwardOp(const T &x);
	BackwardOp(const BackwardOp<T> &x);
public:
	virtual ~BackwardOp();
	void inc_rc() const;
	virtual void dec_rc();
	/* Dummy virtual functions */
	virtual BackwardOp<T>* copy() const;
	virtual void propagate();
	virtual void propagateop();
};

template <class T>
class BINBackwardOp: public BackwardOp<T>
{
/* This class represents binary operators. It has two branches, one for each
** of the expresions (operators) it operates on. The pointer o1 refers to
** the left operand and o2 to the right operand.
*/

public:
	BackwardOp<T> *o1,*o2;

protected:
	BINBackwardOp();
	BINBackwardOp(const T& x);
	BINBackwardOp(const T& x, BackwardOp<T>* a, BackwardOp<T>* b);
	BINBackwardOp(const BINBackwardOp<T>& x);
public:
	virtual ~BINBackwardOp();
	virtual void propagate();
};

template <class T>
class UNBackwardOp: public BackwardOp<T>
{
/* This class represents unary operators. It has one branch for the
** expresion (operator) it operates on. The pointer o1 refers to this.
*/

public:
	BackwardOp<T> *o1;

protected:
	UNBackwardOp();
	UNBackwardOp(const T& x);
	UNBackwardOp(const T& x, BackwardOp<T>* a);
	UNBackwardOp(const UNBackwardOp<T>& x);
public:
	virtual ~UNBackwardOp();
	virtual void propagate();
};

template <class T>
class BTypeName : public UNBackwardOp<T>
{
/* This class is used as a substitute for variables. No (sub-)expression
** can be referenced directly more than once unless it is done by objects
** of this class. This class does often not contain its "own" BackwardNode but
** just refers to a BackwardNode from an underlying operator.
*/

public:
	BTypeName();
	BTypeName(const T &x);
	BTypeName(const BTypeName<T> &x);
	BTypeName(const BackwardOp<T> &x);
#ifdef BaseType
	BTypeName(const BaseType &x);
#endif

	virtual ~BTypeName();
	virtual void dec_rc();
	virtual void propagate();
	void diff(int idx, int n);
	virtual BackwardOp<T>* copy() const;
	T& x();
	T& d(int i);

  /* This is all overloadings of the = operator */
	BTypeName& operator = (const T& x);
	BTypeName& operator = (const BTypeName<T>& x);
	BTypeName& operator = (const BackwardOp<T>& x);
#ifdef BaseType
	BTypeName& operator = (const BaseType& x);
#endif

  /* Declaration of overloading of (?)= */
	BTypeName<T>& operator += (const BackwardOp<T>& x);
	BTypeName<T>& operator -= (const BackwardOp<T>& x);
	BTypeName<T>& operator *= (const BackwardOp<T>& x);
	BTypeName<T>& operator /= (const BackwardOp<T>& x);
	BTypeName<T>& operator += (const BTypeName<T>& x);
	BTypeName<T>& operator -= (const BTypeName<T>& x);
	BTypeName<T>& operator *= (const BTypeName<T>& x);
	BTypeName<T>& operator /= (const BTypeName<T>& x);
	BTypeName<T>& operator += (const T& x);
	BTypeName<T>& operator -= (const T& x);
	BTypeName<T>& operator *= (const T& x);
	BTypeName<T>& operator /= (const T& x);
#ifdef BaseType
	BTypeName<T>& operator += (const BaseType& x);
	BTypeName<T>& operator -= (const BaseType& x);
	BTypeName<T>& operator *= (const BaseType& x);
	BTypeName<T>& operator /= (const BaseType& x);
#endif

};

#ifdef BaseType
template <>
class BTypeName<BaseType> : public UNBackwardOp<BaseType>
{
/* This class is used as a substitute for variables. No (sub-)expression
** can be referenced directly more than once unless it is done by objects
** of this class. This class does often not contain its "own" BackwardNode but
** just refers to a BackwardNode from an underlying operator.
*/

public:
	BTypeName();
	BTypeName(const BaseType &x);
	BTypeName(const BTypeName<BaseType> &x);
	BTypeName(const BackwardOp<BaseType> &x);
	virtual ~BTypeName();
	virtual void dec_rc();
	virtual void propagate();
	void diff(int idx, int n);
	virtual BackwardOp<BaseType>* copy() const;
	BaseType& x();
	BaseType& d(int i);

  /* This is all overloadings of the = operator */
	BTypeName<BaseType>& operator = (const BaseType& x);
	BTypeName<BaseType>& operator = (const BTypeName<BaseType>& x);
	BTypeName<BaseType>& operator = (const BackwardOp<BaseType>& x);

  /* Declaration of overloading of (?)= */
	BTypeName<BaseType>& operator += (const BackwardOp<BaseType>& x);
	BTypeName<BaseType>& operator -= (const BackwardOp<BaseType>& x);
	BTypeName<BaseType>& operator *= (const BackwardOp<BaseType>& x);
	BTypeName<BaseType>& operator /= (const BackwardOp<BaseType>& x);
	BTypeName<BaseType>& operator += (const BTypeName<BaseType>& x);
	BTypeName<BaseType>& operator -= (const BTypeName<BaseType>& x);
	BTypeName<BaseType>& operator *= (const BTypeName<BaseType>& x);
	BTypeName<BaseType>& operator /= (const BTypeName<BaseType>& x);

};
#endif

template <class T>
std::ostream& operator << (std::ostream& os, const BTypeName<T>& o);

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION ADDITION FORMULAS               */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op + op --------------------------------- */

template <class T>
class ADDBackwardOp: public BINBackwardOp<T>
{
public:
	ADDBackwardOp();
	ADDBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b);
	virtual ~ADDBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#ifdef BaseType

 /* --------------------------- BaseType + op ------------------------------ */

template <class T>
class ADDBackwardOp3: public UNBackwardOp<T>
{
	BaseType ax;
public:
	ADDBackwardOp3();
	ADDBackwardOp3(const BaseType& a, const BackwardOp<T>& b);
	virtual ~ADDBackwardOp3();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------- op + BaseType ------------------------------ */

template <class T>
class ADDBackwardOp4: public UNBackwardOp<T>
{
	BaseType bx;
public:
	ADDBackwardOp4();
	ADDBackwardOp4(const BackwardOp<T>& a, const BaseType& b);
	virtual ~ADDBackwardOp4();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            BACKWARD DIFFERENTIATION SUBTRACTION FORMULAS              */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op - op --------------------------------- */

template <class T>
class SUBBackwardOp: public BINBackwardOp<T>
{
public:
	SUBBackwardOp();
	SUBBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b);
	virtual ~SUBBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#ifdef BaseType

/* --------------------------- BaseType - op ------------------------------ */

template <class T>
class SUBBackwardOp3: public UNBackwardOp<T>
{
	BaseType ax;
public:
	SUBBackwardOp3();
	SUBBackwardOp3(const BaseType& a, const BackwardOp<T>& b);
	virtual ~SUBBackwardOp3();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------- op - BaseType ------------------------------ */

template <class T>
class SUBBackwardOp4: public UNBackwardOp<T>
{
	BaseType bx;
public:
	SUBBackwardOp4();
	SUBBackwardOp4(const BackwardOp<T>& a, const BaseType& b);
	virtual ~SUBBackwardOp4();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*          BACKWARD DIFFERENTIATION MULTIPLICATION FORMULAS             */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op * op --------------------------------- */

template <class T>
class MULBackwardOp: public BINBackwardOp<T>
{
public:
	MULBackwardOp();
	MULBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b);
	virtual ~MULBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#ifdef BaseType

/* --------------------------- BaseType * op ------------------------------ */

template <class T>
class MULBackwardOp3: public UNBackwardOp<T>
{
	BaseType ax;
public:
	MULBackwardOp3();
	MULBackwardOp3(const BaseType& a, const BackwardOp<T>& b);
	virtual ~MULBackwardOp3();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------- op * BaseType ------------------------------ */

template <class T>
class MULBackwardOp4: public UNBackwardOp<T>
{
	BaseType bx;
public:
	MULBackwardOp4();
	MULBackwardOp4(const BackwardOp<T>& a, const BaseType& b);
	virtual ~MULBackwardOp4();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION DIVISION FORMULAS               */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op / op --------------------------------- */

template <class T>
class DIVBackwardOp: public BINBackwardOp<T>
{
public:
	DIVBackwardOp();
	DIVBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b);
	virtual ~DIVBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#ifdef BaseType

/* --------------------------- BaseType / op ------------------------------ */

template <class T>
class DIVBackwardOp3: public UNBackwardOp<T>
{
	BaseType ax;
public:
	DIVBackwardOp3();
	DIVBackwardOp3(const BaseType& a, const BackwardOp<T>& b);
	virtual ~DIVBackwardOp3();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------- op / BaseType ------------------------------ */

template <class T>
class DIVBackwardOp4: public UNBackwardOp<T>
{
	BaseType bx;
public:
	DIVBackwardOp4();
	DIVBackwardOp4(const BackwardOp<T>& a, const BaseType& b);
	virtual ~DIVBackwardOp4();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                BACKWARD DIFFERENTIATION POWER FORMULAS                */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* -------------------------- pow(op,BackwardOp) ------------------------------- */

template <class T>
class POWBackwardOp: public BINBackwardOp<T>
{
public:
	POWBackwardOp();
	POWBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b);
	virtual ~POWBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#ifdef BaseType

/* ----------------------- pow(BaseType,BackwardOp) ---------------------------- */

template <class T>
class POWBackwardOp3: public UNBackwardOp<T>
{
	BaseType ax;
public:
	POWBackwardOp3();
	POWBackwardOp3(const BaseType& a, const BackwardOp<T>& b);
	virtual ~POWBackwardOp3();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* ------------------------ pow(op,BaseType) --------------------------- */

template <class T>
class POWBackwardOp4: public UNBackwardOp<T>
{
	BaseType bx;
public:
	POWBackwardOp4();
	POWBackwardOp4(const BackwardOp<T>& a, const BaseType& b);
	virtual ~POWBackwardOp4();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            BACKWARD DIFFERENTIATION UNARY + FORMULA                   */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class UADDBackwardOp: public UNBackwardOp<T>
{
public:
	UADDBackwardOp();
	UADDBackwardOp(const BackwardOp<T>& a);
	virtual ~UADDBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            BACKWARD DIFFERENTIATION UNARY - FORMULA                   */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class USUBBackwardOp: public UNBackwardOp<T>
{
public:
	USUBBackwardOp();
	USUBBackwardOp(const BackwardOp<T>& a);
	virtual ~USUBBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*          BACKWARD DIFFERENTIATION pow(op,int) FORMULA                 */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class POWopN: public UNBackwardOp<T>
{
	int b;
public:
	POWopN();
	POWopN(const BackwardOp<T>& a, int ib);
	virtual ~POWopN();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION sqr FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class SQRBackwardOp: public UNBackwardOp<T>
{
public:
	SQRBackwardOp();
	SQRBackwardOp(const BackwardOp<T>& a);
	virtual ~SQRBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION exp FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class EXPBackwardOp: public UNBackwardOp<T>
{
public:
	EXPBackwardOp();
	EXPBackwardOp(const BackwardOp<T>& a);
	virtual ~EXPBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION log FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class LOGBackwardOp: public UNBackwardOp<T>
{
public:
	LOGBackwardOp();
	LOGBackwardOp(const BackwardOp<T>& a);
	virtual ~LOGBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION sqrt FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class SQRTBackwardOp: public UNBackwardOp<T>
{
public:
	SQRTBackwardOp();
	SQRTBackwardOp(const BackwardOp<T>& a);
	virtual ~SQRTBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION sin FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class SINBackwardOp: public UNBackwardOp<T>
{
public:
	SINBackwardOp();
	SINBackwardOp(const BackwardOp<T>& a);
	virtual ~SINBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION cos FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class COSBackwardOp: public UNBackwardOp<T>
{
public:
	COSBackwardOp();
	COSBackwardOp(const BackwardOp<T>& a);
	virtual ~COSBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION tan FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class TANBackwardOp: public UNBackwardOp<T>
{
public:
	TANBackwardOp();
	TANBackwardOp(const BackwardOp<T>& a);
	virtual ~TANBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION asin FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class ASINBackwardOp: public UNBackwardOp<T>
{
public:
	ASINBackwardOp();
	ASINBackwardOp(const BackwardOp<T>& a);
	virtual ~ASINBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION acos FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class ACOSBackwardOp: public UNBackwardOp<T>
{
public:
	ACOSBackwardOp();
	ACOSBackwardOp(const BackwardOp<T>& a);
	virtual ~ACOSBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION atan FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
class ATANBackwardOp: public UNBackwardOp<T>
{
public:
	ATANBackwardOp();
	ATANBackwardOp(const BackwardOp<T>& a);
	virtual ~ATANBackwardOp();
	virtual BackwardOp<T>* copy() const;
	virtual void propagateop();
};

template <class T>
INLINE1 BackwardNode<T>::BackwardNode():m_g(0),m_rc(1),m_gsize(0)
{
}

template <class T>
INLINE1 BackwardNode<T>::BackwardNode(const T& x):m_v(x),m_g(0),m_rc(1),m_gsize(0)
{
}

template <class T>
INLINE1 BackwardNode<T>::~BackwardNode()
{
	ASSERT(m_gsize?m_g!=0:m_g==0)
	if (m_g) delArray(T,m_g);
	m_g=0;
}

template <class T>
INLINE2 void BackwardNode<T>::touchg(int n)
{
/* This routine creates the vector for storage of g and initializes
** all elements to zero.
*/
	if (!m_g && n>0)
	{
		m_g=newArray(T,n);
		m_gsize=n;
		for(int i=0;i<n;m_g[i++]=T(0));
	}
}

template <class T>
INLINE1 BackwardOp<T>::BackwardOp():val(0)
{
}

template <class T>
INLINE1 BackwardOp<T>::BackwardOp(const T &x):val(newCopy(BackwardNode<T>,x))
{
}

template <class T>
INLINE1 BackwardOp<T>::BackwardOp(const BackwardOp<T> &x):val(x.val)
{
	if (val) inc_rc();
}

template <class T>
INLINE1 BackwardOp<T>::~BackwardOp()
{
	dec_rc();
}

template <class T>
INLINE1 void BackwardOp<T>::inc_rc() const
{
/* Increments the resource counter of the underlying BackwardNode. */
	ASSERT(val);
	val->inc_rc();
}

template <class T>
INLINE2 void BackwardOp<T>::dec_rc()
{
/* Decrements the resource counter of the underlying BackwardNode. */
	if (val && val->dec_rc()==0)
	{
	/* When no operator refers to a BackwardNode it can be deallocated. */
		delScalar(BackwardNode<T>,val);
		val=0;
	}
}

/* Dummy functions */
template <class T>
INLINE1 BackwardOp<T>* BackwardOp<T>::copy() const 
{ 
	return NULL;
}

template <class T>
INLINE1 void BackwardOp<T>::propagate()
{
}

template <class T>
INLINE1 void BackwardOp<T>::propagateop()
{
}

#ifdef HASEQ
template <class T>
INLINE1 bool operator == (const BackwardOp<T> &a, const BackwardOp<T> &b)
{
	ASSERT(a.val && b.val);
	return a.val->value()==b.val->value();
};

#ifdef BaseType
template <class T>
INLINE1 bool operator == (const BackwardOp<T> &a, const BaseType &b)
{
	ASSERT(a.val);
	return a.val->value()==b;
}

template <class T>
INLINE1 bool operator == (const BaseType &a, const BackwardOp<T> &b)
{
	ASSERT(b.val);
	return a==b.val->value();
};
#endif
#endif

#ifdef HASNEQ
template <class T>
INLINE1 bool operator != (const BackwardOp<T> &a, const BackwardOp<T> &b)
{
	ASSERT(a.val && b.val);
	return (a.val->value()!=b.val->value());
}

#ifdef BaseType
template <class T>
INLINE1 bool operator != (const BackwardOp<T> &a, const BaseType &b)
{
	ASSERT(a.val);
	return (a.val->value()!=b);
}

template <class T>
INLINE1 bool operator != (const BaseType &a, const BackwardOp<T> &b)
{
	ASSERT(b.val);
	return (a!=b.val->value());
}
#endif
#endif

#ifdef HASGT
template <class T>
INLINE1 bool operator > (const BackwardOp<T> &a, const BackwardOp<T> &b)
{
	ASSERT(a.val && b.val);
	return (a.val->value()>b.val->value());
}

#ifdef BaseType
template <class T>
INLINE1 bool operator > (const BackwardOp<T> &a, const BaseType &b)
{
	ASSERT(a.val);
	return (a.val->value()>b);
}

template <class T>
INLINE1 bool operator > (const BaseType &a, const BackwardOp<T> &b)
{
	ASSERT(b.val);
	return (a>b.val->value());
}
#endif
#endif

#ifdef HASGEQ
template <class T>
INLINE1 bool operator >= (const BackwardOp<T> &a, const BackwardOp<T> &b)
{
	ASSERT(a.val && b.val);
	return (a.val->value()>=b.val->value());
}

#ifdef BaseType
template <class T>
INLINE1 bool operator >= (const BackwardOp<T> &a, const BaseType &b)
{
	ASSERT(a.val);
	return (a.val->value()>=b);
}

template <class T>
INLINE1 bool operator >= (const BaseType &a, const BackwardOp<T> &b)
{
	ASSERT(b.val);
	return (a>=b.val->value());
}
#endif
#endif

#ifdef HASLT
template <class T>
INLINE1 bool operator < (const BackwardOp<T> &a, const BackwardOp<T> &b)
{
	ASSERT(a.val && b.val);
	return (a.val->value()<b.val->value());
}

#ifdef BaseType
template <class T>
INLINE1 bool operator < (const BackwardOp<T> &a, const BaseType &b)
{
	ASSERT(a.val);
	return (a.val->value()<b);
}

template <class T>
INLINE1 bool operator < (const BaseType &a, const BackwardOp<T> &b)
{
	ASSERT(b.val);
	return (a<b.val->value());
}
#endif
#endif

#ifdef HASLEQ
template <class T>
INLINE1 bool operator <= (const BackwardOp<T> &a, const BackwardOp<T> &b)
{
	ASSERT(a.val && b.val);
	return (a.val->value()<=b.val->value());
}

#ifdef BaseType
template <class T>
INLINE1 bool operator <= (const BackwardOp<T> &a, const BaseType &b)
{
	ASSERT(a.val);
	return (a.val->value()<=b);
}

template <class T>
INLINE1 bool operator <= (const BaseType &a, const BackwardOp<T> &b)
{
	ASSERT(b.val);
	return (a<=b.val->value());
}
#endif
#endif

template <class T>
INLINE1 BINBackwardOp<T>::BINBackwardOp():o1(0),o2(0)
{
}

template <class T>
INLINE1 BINBackwardOp<T>::BINBackwardOp(const T& x):BackwardOp<T>(x),o1(0),o2(0)
{
}

template <class T>
INLINE1 BINBackwardOp<T>::BINBackwardOp(const T& x,BackwardOp<T>* a,BackwardOp<T>* b):BackwardOp<T>(x),o1(a),o2(b)
{
}

template <class T>
INLINE1 BINBackwardOp<T>::BINBackwardOp(const BINBackwardOp<T>& x):BackwardOp<T>(x),o1(x.o1),o2(x.o2)
{
}

template <class T>
INLINE2 BINBackwardOp<T>::~BINBackwardOp()
{
	if (val && val->rc()==1)
	{
		if (o1) {o1->propagate(); delScalar(BackwardOp<T>,o1); o1=0;}
		if (o2) {o2->propagate(); delScalar(BackwardOp<T>,o2); o2=0;}
	}
}

template <class T>
INLINE2 void BINBackwardOp<T>::propagate()
{
/* This function propagates the partial derivatives. The propagation
** does not take place until all incomming partial derivatives have
** been received.
*/
	if (val && val->rc()==1)
	{
		propagateop();
		if (o1) {o1->propagate(); delScalar(BackwardOp<T>,o1); o1=0;}
		if (o2) {o2->propagate(); delScalar(BackwardOp<T>,o2); o2=0;}
	}
	dec_rc();
}

template <class T>
INLINE1 UNBackwardOp<T>::UNBackwardOp():o1(0)
{
}

template <class T>
INLINE1 UNBackwardOp<T>::UNBackwardOp(const T& x):BackwardOp<T>(x),o1(0)
{
}

template <class T>
INLINE1 UNBackwardOp<T>::UNBackwardOp(const T& x,BackwardOp<T>* a):BackwardOp<T>(x),o1(a)
{
}

template <class T>
INLINE1 UNBackwardOp<T>::UNBackwardOp(const UNBackwardOp<T>& x):BackwardOp<T>(x),o1(x.o1)
{
}

template <class T>
INLINE2 UNBackwardOp<T>::~UNBackwardOp()
{
	if (val && val->rc()==1)
	{
		if (o1)
		{
			o1->propagate();
			delScalar(BackwardOp<T>,o1);
			o1=0;
		}
	}
}

template <class T>
INLINE2 void UNBackwardOp<T>::propagate()
{
/* This function propagates the partial derivatives. The propagation
** does not take place until all incomming partial derivatives have
** been received.
*/
	if (val)
	{
		ASSERT(val->rc()==1);
		propagateop();
		if (o1)
		{
			o1->propagate();
			delScalar(BackwardOp<T>,o1);
			o1=0;
		}
	}
	dec_rc();
}

template <class T>
INLINE1 BTypeName<T>::BTypeName()
{
}

template <class T>
INLINE1 BTypeName<T>::BTypeName(const T &x):UNBackwardOp<T>(x)
{
}

template <class T>
INLINE1 BTypeName<T>::BTypeName(const BTypeName<T> &x):UNBackwardOp<T>(x)
{
}

template <class T>
INLINE2 BTypeName<T>::BTypeName(const BackwardOp<T> &x)
{
	val=x.val;
	o1=x.copy();
	inc_rc();
}

#ifdef BaseType
template <class T>
INLINE1 BTypeName<T>::BTypeName(const BaseType &x):UNBackwardOp<T>(T(x))
{
}
#endif

template <class T>
INLINE2 BTypeName<T>::~BTypeName()
{
	if (val && val->rc()==2 && o1)
	{
	/* If object is the last BTypeName encapsulating some operator then
	** underlying operator should also be deleted.
	*/
		dec_rc(); // When called again in ~op, BackwardNode is allready deleted.
	}
}

template <class T>
INLINE2 void BTypeName<T>::dec_rc()
{
/* If this is the last BTypeName encapsulating some operator then operator
** can be deleted (and this will delete the operators BackwardNode).
*/
	if (o1)
	{
		ASSERT(val);
		if (val->dec_rc()==1)
		{
			delScalar(BackwardOp<T>,o1);
			o1=0;
			val=0; // Since deleted from o1.
		}
	} else UNBackwardOp<T>::dec_rc();
}

template <class T>
INLINE2 void BTypeName<T>::propagate()
{
/* This function propagates the partial derivatives. The propagation
** does not take place until all incomming partial derivatives have
** been received. If object is the last object encapsulating some
** operator then propagation has to be passed on to this.
*/
	ASSERT( val || o1==0 );
	if (o1 && val->rc()==2)
	{
		UNBackwardOp<T>::dec_rc();
		o1->propagate();
		delScalar(BackwardOp<T>,o1);
	} else
		UNBackwardOp<T>::dec_rc();
	o1=0;
	val=0;
}

template <class T>
INLINE2 void BTypeName<T>::diff(int idx, int n)
{
/* Initiates the propagation of derivatives, ie. the backward sweep.
** idx is the index of the variable this objects represent of all
** the n "dependent" variables.
*/
	ASSERT(val);
	val->touchg(n);
	val->deriv(idx)=T(1);
	BTypeName<T> newthis(val->value());
	propagate();
	(*this)=newthis;
	o1 = 0;
}

template <class T>
INLINE2 BackwardOp<T>* BTypeName<T>::copy() const
{
/* Returns a copy of the object which still links to the same copy
** of the internal BackwardNode.
*/
	BTypeName<T>* o=newScalar(BTypeName<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 T& BTypeName<T>::x()
{
	if (val)
		return val->value();
	else
	{
		static T zero(0);
		zero=T(0);
		return zero;
	}
}

template <class T>
INLINE2 T& BTypeName<T>::d(int i)
{
	if (val && val->size())
	{
		ASSERT(i<val->size());
		return val->deriv(i);
	}
	else
	{
		static T zero(0);
		zero=T(0);
		return zero;
	}
}

  /* This is all overloadings of the = operator */
template <class T>
INLINE2 BTypeName<T>& BTypeName<T>::operator = (const T& x)
{
	dec_rc();
	val=newCopy(BackwardNode<T>,x);
	o1=0;
	return *this;
}

template <class T>
INLINE2 BTypeName<T>& BTypeName<T>::operator = (const BTypeName<T>& x)
{
	dec_rc();
	val=x.val;
	if (val) inc_rc();
	o1=x.o1;
	return *this;
}

template <class T>
INLINE2 BTypeName<T>& BTypeName<T>::operator = (const BackwardOp<T>& x)
{
	dec_rc();
	val=x.val;
	inc_rc();
	o1=x.copy();
	return *this;
}

#ifdef BaseType
template <class T>
INLINE2 BTypeName<T>& BTypeName<T>::operator = (const BaseType& x)
{
	dec_rc();
	val=newCopy(BackwardNode<T>,x);
	o1=NULL;
	return *this;
}
#endif


#ifdef BaseType

INLINE1 BTypeName<BaseType>::BTypeName()
{
}

INLINE1 BTypeName<BaseType>::BTypeName(const BaseType &x):UNBackwardOp<BaseType>(x)
{
}

INLINE1 BTypeName<BaseType>::BTypeName(const BTypeName<BaseType> &x):UNBackwardOp<BaseType>(x)
{
}

INLINE2 BTypeName<BaseType>::BTypeName(const BackwardOp<BaseType> &x)
{
	val=x.val;
	o1=x.copy();
	inc_rc();
}

INLINE2 BTypeName<BaseType>::~BTypeName()
{
	if (val && val->rc()==2 && o1)
	{
	/* If object is the last BTypeName encapsulating some operator then
	** underlying operator should also be deleted.
	*/
		dec_rc(); // When called again in ~op, BackwardNode is allready deleted.
	}
}

INLINE2 void BTypeName<BaseType>::dec_rc()
{
/* If this is the last BTypeName encapsulating some operator then operator
** can be deleted (and this will delete the operators BackwardNode).
*/
	if (o1)
	{
		ASSERT(val);
		if (val->dec_rc()==1)
		{
			delScalar(BackwardOp<BaseType>,o1);
			o1=0;
			val=0; // Since deleted from o1.
		}
	} else UNBackwardOp<BaseType>::dec_rc();
}

INLINE2 void BTypeName<BaseType>::propagate()
{
/* This function propagates the partial derivatives. The propagation
** does not take place until all incomming partial derivatives have
** been received. If object is the last object encapsulating some
** operator then propagation has to be passed on to this.
*/
	ASSERT( val || o1==0 );
	if (o1 && val->rc()==2)
	{
		UNBackwardOp<BaseType>::dec_rc();
		o1->propagate();
		delScalar(BackwardOp<BaseType>,o1);
	} else
		UNBackwardOp<BaseType>::dec_rc();
	o1=0;
	val=0;
}

INLINE2 void BTypeName<BaseType>::diff(int idx, int n)
{
/* Initiates the propagation of derivatives, ie. the backward sweep.
** idx is the index of the variable this objects represent of all
** the n "dependent" variables.
*/
	ASSERT(val);
	val->touchg(n);
	val->deriv(idx)=BaseType(1);
	BTypeName<BaseType> newthis(val->value());
	propagate();
	(*this)=newthis;
	o1 = 0;
}

INLINE2 BackwardOp<BaseType>* BTypeName<BaseType>::copy() const
{
/* Returns a copy of the object which still links to the same copy
** of the internal BackwardNode.
*/
	BTypeName<BaseType>* o=newScalar(BTypeName<BaseType>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

INLINE2 BaseType& BTypeName<BaseType>::x()
{
	if (val)
		return val->value();
	else 
	{
		static BaseType zero(0);
		zero=BaseType(0);
		return zero;
	}
}

INLINE2 BaseType& BTypeName<BaseType>::d(int i)
{
	if (val && val->size())
	{
		ASSERT(i<val->size());
		return val->deriv(i);
	}
	else
	{
		static BaseType zero(0);
		zero=BaseType(0);
		return zero;
	}
}

/* This is all overloadings of the = operator */
INLINE2 BTypeName<BaseType>& BTypeName<BaseType>::operator = (const BaseType& x)
{
	dec_rc();
	val=newCopy(BackwardNode<BaseType>,x);
	o1=0;
	return *this;
}

INLINE2 BTypeName<BaseType>& BTypeName<BaseType>::operator = (const BTypeName<BaseType>& x)
{
	dec_rc();
	val=x.val;
	if (val) inc_rc();
	o1=x.o1;
	return *this;
}

INLINE2 BTypeName<BaseType>& BTypeName<BaseType>::operator = (const BackwardOp<BaseType>& x)
{
	dec_rc();
	val=x.val;
	inc_rc();
	o1=x.copy();
	return *this;
}
#endif

template <class T>
INLINE2 std::ostream& operator << (std::ostream& os, const BTypeName<T>& o)
{
	os << "{";
	if (o.val)
	{
		os << o.val->value() << " >" << o.val->rc << "<";
		if (o.val->g)
		{
			os << "[ ";
			for (int i=0;i<o.val->gsize;i++)
				os << o.val->g[i] << " ";
			os << "]";
		}
	}
	if (o.o1) os << " d";
	os << "}";
	return os;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION ADDITION FORMULAS               */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op + op --------------------------------- */


template <class T>
INLINE1 ADDBackwardOp<T>::ADDBackwardOp()
{
}

template <class T>
INLINE1 ADDBackwardOp<T>::ADDBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b):
	BINBackwardOp<T>(a.val->value()+b.val->value(),a.copy(),b.copy())
{
}

template <class T>
INLINE1 ADDBackwardOp<T>::~ADDBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* ADDBackwardOp<T>::copy() const
{
	ADDBackwardOp<T>* o=newScalar(ADDBackwardOp<T>);
	o->o1=o1;
	o->o2=o2;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void ADDBackwardOp<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	(o2->val)->touchg(val->size());
	for (i=0;i<val->size();i++)
	{
		(o1->val)->deriv(i) += val->deriv(i);
		(o2->val)->deriv(i) += val->deriv(i);
	}
}

#ifdef BaseType

/* --------------------------- BaseType + op ------------------------------ */

template <class T>
INLINE1 ADDBackwardOp3<T>::ADDBackwardOp3()
{
}

template <class T>
INLINE1 ADDBackwardOp3<T>::ADDBackwardOp3(const BaseType& a, const BackwardOp<T>& b):
	UNBackwardOp<T>(a+b.val->value())
{
	o1=b.copy();
	ax=a;
}

template <class T>
INLINE1 ADDBackwardOp3<T>::~ADDBackwardOp3()
{
}

template <class T>
INLINE2 BackwardOp<T>* ADDBackwardOp3<T>::copy() const
{
	ADDBackwardOp3<T>* o=newScalar(ADDBackwardOp3<T>);
	o->o1=o1;
	o->val=val;
	o->ax=ax;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void ADDBackwardOp3<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i);
}

/* --------------------------- op + BaseType ------------------------------ */


template <class T>
INLINE1 ADDBackwardOp4<T>::ADDBackwardOp4()
{
}

template <class T>
INLINE1 ADDBackwardOp4<T>::ADDBackwardOp4(const BackwardOp<T>& a, const BaseType& b):
	UNBackwardOp<T>(a.val->value()+b)
{
	o1=a.copy();
	bx=b;
}

template <class T>
INLINE1 ADDBackwardOp4<T>::~ADDBackwardOp4()
{
}

template <class T>
INLINE2 BackwardOp<T>* ADDBackwardOp4<T>::copy() const
{
	ADDBackwardOp4<T>* o=newScalar(ADDBackwardOp4);
	o->o1=o1;
	o->val=val;
	o->bx=bx;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void ADDBackwardOp4<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i);
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            BACKWARD DIFFERENTIATION SUBTRACTION FORMULAS              */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op - op --------------------------------- */

template <class T>
INLINE1 SUBBackwardOp<T>::SUBBackwardOp()
{
}

template <class T>
INLINE1 SUBBackwardOp<T>::SUBBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b):
	BINBackwardOp<T>(a.val->value()-b.val->value(),a.copy(),b.copy())
{
}

template <class T>
INLINE1 SUBBackwardOp<T>::~SUBBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* SUBBackwardOp<T>::copy() const
{
	SUBBackwardOp<T>* o=newScalar(SUBBackwardOp<T>);
	o->o1=o1;
	o->o2=o2;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void SUBBackwardOp<T>::propagateop()
{
	(o1->val)->touchg(val->size());
	(o2->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
	{
		(o1->val)->deriv(i) += val->deriv(i);
		(o2->val)->deriv(i) -= val->deriv(i);
	}
}

#ifdef BaseType

/* --------------------------- BaseType - op ------------------------------ */

template <class T>
INLINE1 SUBBackwardOp3<T>::SUBBackwardOp3()
{
}

template <class T>
INLINE1 SUBBackwardOp3<T>::SUBBackwardOp3(const BaseType& a, const BackwardOp<T>& b):
	UNBackwardOp<T>(a-b.val->value())
{
	o1=b.copy();
	ax=a;
}

template <class T>
INLINE1 SUBBackwardOp3<T>::~SUBBackwardOp3()
{
}

template <class T>
INLINE2 BackwardOp<T>* SUBBackwardOp3<T>::copy() const
{
	SUBBackwardOp3<T>* o=newScalar(SUBBackwardOp3<T>);
	o->o1=o1;
	o->val=val;
	o->ax=ax;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void SUBBackwardOp3<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) -= val->deriv(i);
}

/* --------------------------- op - BaseType ------------------------------ */

template <class T>
INLINE1 SUBBackwardOp4<T>::SUBBackwardOp4()
{
}

template <class T>
INLINE1 SUBBackwardOp4<T>::SUBBackwardOp4(const BackwardOp<T>& a, const BaseType& b):
	UNBackwardOp<T>(a.val->value()-b)
{
	o1=a.copy();
	bx=b;
}

template <class T>
INLINE1 SUBBackwardOp4<T>::~SUBBackwardOp4()
{
}

template <class T>
INLINE2 BackwardOp<T>* SUBBackwardOp4<T>::copy() const
{
	SUBBackwardOp4<T>* o=newScalar(SUBBackwardOp4<T>);
	o->o1=o1;
	o->val=val;
	o->bx=bx;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void SUBBackwardOp4<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i);
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*          BACKWARD DIFFERENTIATION MULTIPLICATION FORMULAS             */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op * op --------------------------------- */

template <class T>
INLINE1 MULBackwardOp<T>::MULBackwardOp()
{
}

template <class T>
INLINE1 MULBackwardOp<T>::MULBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b):
	BINBackwardOp<T>(a.val->value()*b.val->value(),a.copy(),b.copy())
{
}

template <class T>
INLINE1 MULBackwardOp<T>::~MULBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* MULBackwardOp<T>::copy() const
{
	MULBackwardOp<T>* o=newScalar(MULBackwardOp<T>);
	o->o1=o1;
	o->o2=o2;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void MULBackwardOp<T>::propagateop()
{
	(o1->val)->touchg(val->size());
	(o2->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
	{
		(o1->val)->deriv(i) += val->deriv(i) * (o2->val)->value();
		(o2->val)->deriv(i) += val->deriv(i) * (o1->val)->value();
	}
}

#ifdef BaseType

/* --------------------------- BaseType * op ------------------------------ */

template <class T>
INLINE1 MULBackwardOp3<T>::MULBackwardOp3()
{
}

template <class T>
INLINE1 MULBackwardOp3<T>::MULBackwardOp3(const BaseType& a, const BackwardOp<T>& b):
	UNBackwardOp<T>(a*b.val->value())
{
	o1=b.copy();
	ax=a;
}

template <class T>
INLINE1 MULBackwardOp3<T>::~MULBackwardOp3()
{
}

template <class T>
INLINE2 BackwardOp<T>* MULBackwardOp3<T>::copy() const
{
	MULBackwardOp3<T>* o=newScalar(MULBackwardOp3<T>);
	o->o1=o1;
	o->val=val;
	o->ax=ax;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void MULBackwardOp3<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i) * ax;
}

/* --------------------------- op * BaseType ------------------------------ */

template <class T>
INLINE1 MULBackwardOp4<T>::MULBackwardOp4()
{
}

template <class T>
INLINE1 MULBackwardOp4<T>::MULBackwardOp4(const BackwardOp<T>& a, const BaseType& b):
	UNBackwardOp<T>(a.val->value()*b)
{
	o1=a.copy();
	bx=b;
}

template <class T>
INLINE1 MULBackwardOp4<T>::~MULBackwardOp4()
{
}

template <class T>
INLINE2 BackwardOp<T>* MULBackwardOp4<T>::copy() const
{
	MULBackwardOp4<T>* o=newScalar(MULBackwardOp4<T>);
	o->o1=o1;
	o->val=val;
	o->bx=bx;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void MULBackwardOp4<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i) * bx;
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION DIVISION FORMULAS               */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* --------------------------- op / op --------------------------------- */


template <class T>
INLINE1 DIVBackwardOp<T>::DIVBackwardOp()
{
}

template <class T>
INLINE1 DIVBackwardOp<T>::DIVBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b):
	BINBackwardOp<T>(a.val->value()/b.val->value(),a.copy(),b.copy())
{
}

template <class T>
INLINE1 DIVBackwardOp<T>::~DIVBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* DIVBackwardOp<T>::copy() const
{
	DIVBackwardOp<T>* o=newScalar(DIVBackwardOp<T>);
	o->o1=o1;
	o->o2=o2;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void DIVBackwardOp<T>::propagateop()
{
	T tmp;
	(o1->val)->touchg(val->size());
	(o2->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
	{
		tmp=val->deriv(i)/(o2->val)->value();
		(o1->val)->deriv(i) += tmp;
		(o2->val)->deriv(i) -= tmp * val->value();
	}
}

#ifdef BaseType

/* --------------------------- BaseType / op ------------------------------ */

template <class T>
INLINE1 DIVBackwardOp3<T>::DIVBackwardOp3()
{
}

template <class T>
INLINE1 DIVBackwardOp3<T>::DIVBackwardOp3(const BaseType& a, const BackwardOp<T>& b):
	UNBackwardOp<T>(a/b.val->value())
{
	o1=b.copy();
	ax=a;
}

template <class T>
INLINE1 DIVBackwardOp3<T>::~DIVBackwardOp3()
{
}

template <class T>
INLINE2 BackwardOp<T>* DIVBackwardOp3<T>::copy() const
{
	DIVBackwardOp3<T>* o=newScalar(DIVBackwardOp3<T>);
	o->o1=o1;
	o->val=val;
	o->ax=ax;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void DIVBackwardOp3<T>::propagateop()
{
	int i;
	T tmp(val->value() / (o1->val)->value());
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) -= val->deriv(i) * tmp;
}

/* --------------------------- op / BaseType ------------------------------ */

template <class T>
INLINE1 DIVBackwardOp4<T>::DIVBackwardOp4()
{
}

template <class T>
INLINE1 DIVBackwardOp4<T>::DIVBackwardOp4(const BackwardOp<T>& a, const BaseType& b):
	UNBackwardOp<T>(a.val->value()/b)
{
	o1=a.copy();
	bx=b;
}

template <class T>
INLINE1 DIVBackwardOp4<T>::~DIVBackwardOp4()
{
}

template <class T>
INLINE2 BackwardOp<T>* DIVBackwardOp4<T>::copy() const
{
	DIVBackwardOp4<T>* o=newScalar(DIVBackwardOp4<T>);
	o->o1=o1;
	o->val=val;
	o->bx=bx;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void DIVBackwardOp4<T>::propagateop()
{
	int i;
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i) / bx;
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                BACKWARD DIFFERENTIATION POWER FORMULAS                */
/*                                                                       */
/* --------------------------------------------------------------------- */

/* -------------------------- pow(op,BackwardOp) ------------------------------- */

template <class T>
INLINE1 POWBackwardOp<T>::POWBackwardOp()
{
}

template <class T>
INLINE1 POWBackwardOp<T>::POWBackwardOp(const BackwardOp<T>& a, const BackwardOp<T>& b):
	BINBackwardOp<T>(pow(a.val->value(),b.val->value()),a.copy(),b.copy())
{
}

template <class T>
INLINE1 POWBackwardOp<T>::~POWBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* POWBackwardOp<T>::copy() const
{
	POWBackwardOp<T>* o=newScalar(POWBackwardOp<T>);
	o->o1=o1;
	o->o2=o2;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void POWBackwardOp<T>::propagateop()
{
	T tmp1((o2->val)->v *  pow(o1->val->value(),o2->val->value()-1));
	T tmp2(val->value() * log(o1->val->value()));
	(o1->val)->touchg(val->size());
	(o2->val)->touchg(val->size());
	for (i=0;i<val->size();i++)
	{
		(o1->val)->deriv(i) += val->deriv(i) * tmp1;
		(o2->val)->deriv(i) += val->deriv(i) * tmp2;
	}
}

#ifdef BaseType

/* ----------------------- pow(BaseType,BackwardOp) ---------------------------- */

template <class T>
INLINE1 POWBackwardOp3<T>::POWBackwardOp3()
{
}

template <class T>
INLINE1 POWBackwardOp3<T>::POWBackwardOp3(const BaseType& a, const BackwardOp<T>& b):
	UNBackwardOp<T>(pow(a,b.val->value()))
{
	o1=b.copy();
	ax=a;
}

template <class T>
INLINE1 POWBackwardOp3<T>::~POWBackwardOp3()
{
}

template <class T>
INLINE2 BackwardOp<T>* POWBackwardOp3<T>::copy() const
{
	POWBackwardOp3<T>* o=newScalar(POWBackwardOp3<T>);
	o->o1=o1;
	o->val=val;
	o->ax=ax;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void POWBackwardOp3<T>::propagateop()
{
	int i;
	T tmp(val->value() * log(ax));
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i) * tmp;
}

/* ------------------------ pow(op,BaseType) --------------------------- */

template <class T>
INLINE1 POWBackwardOp4<T>::POWBackwardOp4()
{
}

template <class T>
INLINE1 POWBackwardOp4<T>::POWBackwardOp4(const BackwardOp<T>& a, const BaseType& b):
	UNBackwardOp<T>(pow(a.val->value(),b))
{
	o1=a.copy();
	bx=b;
}

template <class T>
INLINE1 POWBackwardOp4<T>::~POWBackwardOp4()
{
}

template <class T>
INLINE2 BackwardOp<T>* POWBackwardOp4<T>::copy() const
{
	POWBackwardOp4<T>* o=newScalar(POWBackwardOp4<T>);
	o->o1=o1;
	o->val=val;
	o->bx=bx;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void POWBackwardOp4<T>::propagateop()
{
	int i;
	T tmp(bx * pow(o1->val->value(),bx-1));
	(o1->val)->touchg(val->size());
	for (i=0;i<val->size();i++) (o1->val)->deriv(i) += val->deriv(i) * tmp;
}

#endif

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            BACKWARD DIFFERENTIATION UNARY + FORMULA                   */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
INLINE1 UADDBackwardOp<T>::UADDBackwardOp()
{
}

template <class T>
INLINE1 UADDBackwardOp<T>::UADDBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(a.val->value(),a.copy())
{
}

template <class T>
INLINE1 UADDBackwardOp<T>::~UADDBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* UADDBackwardOp<T>::copy() const
{
	UADDBackwardOp<T>* o=newScalar(UADDBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void UADDBackwardOp<T>::propagateop()
{
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*            BACKWARD DIFFERENTIATION UNARY - FORMULA                   */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
INLINE1 USUBBackwardOp<T>::USUBBackwardOp()
{
}

template <class T>
INLINE1 USUBBackwardOp<T>::USUBBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(-a.val->value(),a.copy())
{
}

template <class T>
INLINE1 USUBBackwardOp<T>::~USUBBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* USUBBackwardOp<T>::copy() const
{
	USUBBackwardOp<T>* o=newScalar(USUBBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void USUBBackwardOp<T>::propagateop()
{
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) -= val->deriv(i);
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*          BACKWARD DIFFERENTIATION pow(op,int) FORMULA                 */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 POWopN<T>::POWopN()
{
}

template <class T>
INLINE1 POWopN<T>::POWopN(const BackwardOp<T>& a, int ib):
	UNBackwardOp<T>(pow(a.val->value(),ib),a.copy()),b(ib)
{
}

template <class T>
INLINE1 POWopN<T>::~POWopN()
{
}

template <class T>
INLINE2 BackwardOp<T>* POWopN<T>::copy() const
{
	POWopN<T>* o=newScalar(POWopN<T>);
	o->o1=o1;
	o->val=val;
	o->b=b;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void POWopN<T>::propagateop()
{
	T tmp(b*pow(o1->val->value(),b-1));
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) * tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION sqr FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 SQRBackwardOp<T>::SQRBackwardOp()
{
}

template <class T>
INLINE1 SQRBackwardOp<T>::SQRBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(_sqr(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 SQRBackwardOp<T>::~SQRBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* SQRBackwardOp<T>::copy() const
{
	SQRBackwardOp<T>* o=newScalar(SQRBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void SQRBackwardOp<T>::propagateop()
{
	T tmp(2*(o1->val)->value());
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) * tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION exp FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
INLINE1 EXPBackwardOp<T>::EXPBackwardOp()
{
}

template <class T>
INLINE1 EXPBackwardOp<T>::EXPBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(exp(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 EXPBackwardOp<T>::~EXPBackwardOp()
{
}

template <class T>
BackwardOp<T>* EXPBackwardOp<T>::copy() const
{
	EXPBackwardOp<T>* o=newScalar(EXPBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
void EXPBackwardOp<T>::propagateop()
{
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) * val->value();
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION log FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
INLINE1 LOGBackwardOp<T>::LOGBackwardOp()
{
}

template <class T>
INLINE1 LOGBackwardOp<T>::LOGBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(log(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 LOGBackwardOp<T>::~LOGBackwardOp()
{
}

template <class T>
BackwardOp<T>* LOGBackwardOp<T>::copy() const
{
	LOGBackwardOp<T>* o=newScalar(LOGBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
void LOGBackwardOp<T>::propagateop()
{
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) / (o1->val)->value();
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION sqrt FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 SQRTBackwardOp<T>::SQRTBackwardOp()
{
}

template <class T>
INLINE1 SQRTBackwardOp<T>::SQRTBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(sqrt(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 SQRTBackwardOp<T>::~SQRTBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* SQRTBackwardOp<T>::copy() const
{
	SQRTBackwardOp<T>* o=newScalar(SQRTBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void SQRTBackwardOp<T>::propagateop()
{
	T tmp(val->value()*2);
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) / tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION sin FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 SINBackwardOp<T>::SINBackwardOp()
{
}

template <class T>
INLINE1 SINBackwardOp<T>::SINBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(sin(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 SINBackwardOp<T>::~SINBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* SINBackwardOp<T>::copy() const
{
	SINBackwardOp<T>* o=newScalar(SINBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void SINBackwardOp<T>::propagateop()
{
	T tmp(cos((o1->val)->value()));
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) * tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION cos FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 COSBackwardOp<T>::COSBackwardOp()
{
}

template <class T>
INLINE1 COSBackwardOp<T>::COSBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(cos(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 COSBackwardOp<T>::~COSBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* COSBackwardOp<T>::copy() const
{
	COSBackwardOp<T>* o=newScalar(COSBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void COSBackwardOp<T>::propagateop()
{
	T tmp(sin((o1->val)->value()));
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) -= val->deriv(i) * tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION tan FORMULA                     */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 TANBackwardOp<T>::TANBackwardOp()
{
}

template <class T>
INLINE1 TANBackwardOp<T>::TANBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(tan(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 TANBackwardOp<T>::~TANBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* TANBackwardOp<T>::copy() const
{
	TANBackwardOp<T>* o=newScalar(TANBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void TANBackwardOp<T>::propagateop()
{
	T tmp(_sqr(val->value())+1);
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) * tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION asin FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 ASINBackwardOp<T>::ASINBackwardOp()
{
}

template <class T>
INLINE1 ASINBackwardOp<T>::ASINBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(asin(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 ASINBackwardOp<T>::~ASINBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* ASINBackwardOp<T>::copy() const
{
	ASINBackwardOp<T>* o=newScalar(ASINBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void ASINBackwardOp<T>::propagateop()
{
	T tmp(sqrt(1-_sqr(o1->val->value())));
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) / tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION acos FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */


template <class T>
INLINE1 ACOSBackwardOp<T>::ACOSBackwardOp()
{
}

template <class T>
INLINE1 ACOSBackwardOp<T>::ACOSBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(acos(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 ACOSBackwardOp<T>::~ACOSBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* ACOSBackwardOp<T>::copy() const
{
	ACOSBackwardOp<T>* o=newScalar(ACOSBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void ACOSBackwardOp<T>::propagateop()
{
	T tmp(sqrt(1-_sqr(o1->val->value())));
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) -= val->deriv(i) / tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*              BACKWARD DIFFERENTIATION atan FORMULA                    */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
ATANBackwardOp<T>::ATANBackwardOp()
{
}

template <class T>
INLINE1 ATANBackwardOp<T>::ATANBackwardOp(const BackwardOp<T>& a):
	UNBackwardOp<T>(atan(a.val->value()),a.copy())
{
}

template <class T>
INLINE1 ATANBackwardOp<T>::~ATANBackwardOp()
{
}

template <class T>
INLINE2 BackwardOp<T>* ATANBackwardOp<T>::copy() const
{
	ATANBackwardOp<T>* o=newScalar(ATANBackwardOp<T>);
	o->o1=o1;
	o->val=val;
	inc_rc();
	return o;
}

template <class T>
INLINE2 void ATANBackwardOp<T>::propagateop()
{
	T tmp(_sqr(o1->val->value())+1);
	(o1->val)->touchg(val->size());
	for (int i=0;i<val->size();i++)
		(o1->val)->deriv(i) += val->deriv(i) / tmp;
}

/* --------------------------------------------------------------------- */
/*                                                                       */
/*                        DEFINITION OF OPERATORS                        */
/*                                                                       */
/* --------------------------------------------------------------------- */

template <class T>
INLINE1 ADDBackwardOp<T> operator+ (const BackwardOp<T>& a, const BackwardOp<T>& b)
{
	return ADDBackwardOp<T>(a,b);
}

template <class T>
INLINE1 ADDBackwardOp<T> operator+ (const BackwardOp<T>& a, const BTypeName<T>& b)
{
	return ADDBackwardOp<T>(a,b);
}

template <class T>
INLINE1 ADDBackwardOp<T> operator+ (const BTypeName<T>& a, const BackwardOp<T>& b)
{
	return ADDBackwardOp<T>(a,b);
}

template <class T>
INLINE1 ADDBackwardOp<T> operator+ (const BTypeName<T>& a, const BTypeName<T>& b)
{
	return ADDBackwardOp<T>(a,b);
}

#ifdef BaseType
template <class T>
INLINE1 ADDBackwardOp3<T> operator+ (const BaseType& a, const BackwardOp<T>& b)
{
	return ADDBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 ADDBackwardOp3<T> operator+ (const BaseType& a, const BTypeName<T>& b)
{
	return ADDBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 ADDBackwardOp4<T> operator+ (const BackwardOp<T>& a, const BaseType& b)
{
	return ADDBackwardOp4<T>(a,b);
}

template <class T>
INLINE1 ADDBackwardOp4<T> operator+ (const BTypeName<T>& a, const BaseType& b)
{
	return ADDBackwardOp4<T>(a,b);
}
#endif

template <class T>
INLINE1 SUBBackwardOp<T> operator- (const BackwardOp<T>& a, const BackwardOp<T>& b)
{
	return SUBBackwardOp<T>(a,b);
}

template <class T>
INLINE1 SUBBackwardOp<T> operator- (const BackwardOp<T>& a, const BTypeName<T>& b)
{
	return SUBBackwardOp<T>(a,b);
}

template <class T>
INLINE1 SUBBackwardOp<T> operator- (const BTypeName<T>& a, const BackwardOp<T>& b)
{
	return SUBBackwardOp<T>(a,b);
}

template <class T>
INLINE1 SUBBackwardOp<T> operator- (const BTypeName<T>& a, const BTypeName<T>& b)
{
	return SUBBackwardOp<T>(a,b);
}

#ifdef BaseType
template <class T>
INLINE1 SUBBackwardOp3<T> operator- (const BaseType& a, const BackwardOp<T>& b)
{
	return SUBBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 SUBBackwardOp3<T> operator- (const BaseType& a, const BTypeName<T>& b)
{
	return SUBBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 SUBBackwardOp4<T> operator- (const BackwardOp<T>& a, const BaseType& b)
{
	return SUBBackwardOp4<T>(a,b);
}

template <class T>
INLINE1 SUBBackwardOp4<T> operator- (const BTypeName<T>& a, const BaseType& b)
{
	return SUBBackwardOp4<T>(a,b);
}
#endif

template <class T>
INLINE1 MULBackwardOp<T> operator* (const BackwardOp<T>& a, const BackwardOp<T>& b)
{
	return MULBackwardOp<T>(a,b);
}

template <class T>
INLINE1 MULBackwardOp<T> operator* (const BackwardOp<T>& a, const BTypeName<T>& b)
{
	return MULBackwardOp<T>(a,b);
}

template <class T>
INLINE1 MULBackwardOp<T> operator* (const BTypeName<T>& a, const BackwardOp<T>& b)
{
	return MULBackwardOp<T>(a,b);
}

template <class T>
INLINE1 MULBackwardOp<T> operator* (const BTypeName<T>& a, const BTypeName<T>& b)
{
	return MULBackwardOp<T>(a,b);
}

#ifdef BaseType
template <class T>
INLINE1 MULBackwardOp3<T> operator* (const BaseType& a, const BackwardOp<T>& b)
{
	return MULBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 MULBackwardOp3<T> operator* (const BaseType& a, const BTypeName<T>& b)
{
	return MULBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 MULBackwardOp4<T> operator* (const BackwardOp<T>& a, const BaseType& b)
{
	return MULBackwardOp4<T>(a,b);
}

template <class T>
INLINE1 MULBackwardOp4<T> operator* (const BTypeName<T>& a, const BaseType& b)
{
	return MULBackwardOp4<T>(a,b);
}
#endif

template <class T>
INLINE1 DIVBackwardOp<T> operator/ (const BackwardOp<T>& a, const BackwardOp<T>& b)
{
	return DIVBackwardOp<T>(a,b);
}

template <class T>
INLINE1 DIVBackwardOp<T> operator/ (const BackwardOp<T>& a, const BTypeName<T>& b)
{
	return DIVBackwardOp<T>(a,b);
}

template <class T>
INLINE1 DIVBackwardOp<T> operator/ (const BTypeName<T>& a, const BackwardOp<T>& b)
{
	return DIVBackwardOp<T>(a,b);
}

template <class T>
INLINE1 DIVBackwardOp<T> operator/ (const BTypeName<T>& a, const BTypeName<T>& b)
{
	return DIVBackwardOp<T>(a,b);
}

#ifdef BaseType
template <class T>
INLINE1 DIVBackwardOp3<T> operator/ (const BaseType& a, const BackwardOp<T>& b)
{
	return DIVBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 DIVBackwardOp3<T> operator/ (const BaseType& a, const BTypeName<T>& b)
{
	return DIVBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 DIVBackwardOp4<T> operator/ (const BackwardOp<T>& a, const BaseType& b)
{
	return DIVBackwardOp4<T>(a,b);
}

template <class T>
INLINE1 DIVBackwardOp4<T> operator/ (const BTypeName<T>& a, const BaseType& b)
{
	return DIVBackwardOp4<T>(a,b);
}
#endif

template <class T>
INLINE1 POWBackwardOp<T> pow (const BackwardOp<T>& a, const BackwardOp<T>& b)
{
	return POWBackwardOp<T>(a,b);
}

template <class T>
INLINE1 POWBackwardOp<T> pow (const BackwardOp<T>& a, const BTypeName<T>& b)
{
	return POWBackwardOp<T>(a,b);
}

template <class T>
INLINE1 POWBackwardOp<T> pow (const BTypeName<T>& a, const BackwardOp<T>& b)
{
	return POWBackwardOp<T>(a,b);
}

template <class T>
INLINE1 POWBackwardOp<T> pow (const BTypeName<T>& a, const BTypeName<T>& b)
{
	return POWBackwardOp<T>(a,b);
}

#ifdef BaseType
template <class T>
INLINE1 POWBackwardOp3<T> pow (const BaseType& a, const BackwardOp<T>& b)
{
	return POWBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 POWBackwardOp3<T> pow (const BaseType& a, const BTypeName<T>& b)
{
	return POWBackwardOp3<T>(a,b);
}

template <class T>
INLINE1 POWBackwardOp4<T> pow (const BackwardOp<T>& a, const BaseType& b)
{
	return POWBackwardOp4<T>(a,b);
}

template <class T>
INLINE1 POWBackwardOp4<T> pow (const BTypeName<T>& a, const BaseType& b)
{
	return POWBackwardOp4<T>(a,b);
}
#endif

template <class T>
INLINE1 UADDBackwardOp<T> operator+ (const BackwardOp<T>& a)
{
	return UADDBackwardOp<T>(a);
}

template <class T>
INLINE1 USUBBackwardOp<T> operator- (const BackwardOp<T>& a)
{
	return USUBBackwardOp<T>(a);
}

template <class T>
INLINE1 POWopN<T> pow (const BackwardOp<T>& a, int ib)
{
	return POWopN<T>(a,ib);
}

template <class T>
INLINE1 POWopN<T> pow (const BTypeName<T>& a, int ib)
{
	return POWopN<T>(a,ib);
}

template <class T>
INLINE1 SQRBackwardOp<T> sqr (const BackwardOp<T>& a)
{
	return SQRBackwardOp<T>(a);
}

template <class T>
INLINE1 EXPBackwardOp<T> exp (const BackwardOp<T>& a)
{
	return EXPBackwardOp<T>(a);
}

template <class T>
INLINE1 LOGBackwardOp<T> log (const BackwardOp<T>& a)
{
	return LOGBackwardOp<T>(a);
}

template <class T>
INLINE1 SQRTBackwardOp<T> sqrt (const BackwardOp<T>& a)
{
	return SQRTBackwardOp<T>(a);
}

template <class T>
INLINE1 SINBackwardOp<T> sin (const BackwardOp<T>& a)
{
	return SINBackwardOp<T>(a);
}

template <class T>
INLINE1 COSBackwardOp<T> cos (const BackwardOp<T>& a)
{
	return COSBackwardOp<T>(a);
}

template <class T>
INLINE1 TANBackwardOp<T> tan (const BackwardOp<T>& a)
{
	return TANBackwardOp<T>(a);
}

template <class T>
INLINE1 ASINBackwardOp<T> asin (const BackwardOp<T>& a)
{
	return ASINBackwardOp<T>(a);
}

template <class T>
INLINE1 ACOSBackwardOp<T> acos (const BackwardOp<T>& a)
{
	return ACOSBackwardOp<T>(a);
}

template <class T>
INLINE1 ATANBackwardOp<T> atan (const BackwardOp<T>& a)
{
	return ATANBackwardOp<T>(a);
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator += (const BackwardOp<T>& x)
{
	return *this = (*this)+x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator -= (const BackwardOp<T>& x)
{
	return *this = (*this)-x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator *= (const BackwardOp<T>& x)
{
	return *this = (*this)*x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator /= (const BackwardOp<T>& x)
{
	return *this = (*this)/x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator += (const BTypeName<T>& x)
{
	return *this = (*this)+x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator -= (const BTypeName<T>& x)
{
	return *this = (*this)-x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator *= (const BTypeName<T>& x)
{
	return *this = (*this)*x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator /= (const BTypeName<T>& x)
{
	return *this = (*this)/x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator += (const T& x)
{
	return *this = (*this)+x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator -= (const T& x)
{
	return *this = (*this)-x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator *= (const T& x)
{
	return *this = (*this)*x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator /= (const T& x)
{
	return *this = (*this)/x;
}

#ifdef BaseType
template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator += (const BaseType& x)
{
	return *this = (*this)+x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator -= (const BaseType& x)
{
	return *this = (*this)-x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator *= (const BaseType& x)
{
	return *this = (*this)*x;
}

template <class T>
INLINE1 BTypeName<T>& BTypeName<T>::operator /= (const BaseType& x)
{
	return *this = (*this)/x;
}

#endif

#ifdef BaseType

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator += (const BackwardOp<BaseType>& x)
{
	return *this = (*this)+x;
}

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator -= (const BackwardOp<BaseType>& x)
{
	return *this = (*this)-x;
}

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator *= (const BackwardOp<BaseType>& x)
{
	return *this = (*this)*x;
}

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator /= (const BackwardOp<BaseType>& x)
{
	return *this = (*this)/x;
}

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator += (const BTypeName<BaseType>& x)
{
	return *this = (*this)+x;
}

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator -= (const BTypeName<BaseType>& x)
{
	return *this = (*this)-x;
}

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator *= (const BTypeName<BaseType>& x)
{
	return *this = (*this)*x;
}

INLINE0 BTypeName<BaseType>& BTypeName<BaseType>::operator /= (const BTypeName<BaseType>& x)
{
	return *this = (*this)/x;
}

#endif


#endif
