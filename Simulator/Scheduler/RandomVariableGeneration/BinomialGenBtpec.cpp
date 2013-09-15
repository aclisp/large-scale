//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%398ED01B005C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%398ED01B005C.cm

//## begin module%398ED01B005C.cp preserve=no
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: RandomVariableGeneration
//
//	File Name: BinomialGenBtpec.cpp
//
//	Description:
//
//	Author:
//
//	Creation Date:
//
//	Visual Source Safe $Revision: $
//
//	Last check in $Date: $
//
//	********************************************************************************
//## end module%398ED01B005C.cp

//## Module: BinomialGenBtpec%398ED01B005C; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\BinomialGenBtpec.cpp

//## begin module%398ED01B005C.additionalIncludes preserve=no
//## end module%398ED01B005C.additionalIncludes

//## begin module%398ED01B005C.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%398ED01B005C.includes

// BinomialGenBtpec
#include "BinomialGenBtpec.h"
//## begin module%398ED01B005C.additionalDeclarations preserve=yes
namespace sch {
//## end module%398ED01B005C.additionalDeclarations


// Class BinomialGenBtpec 



//## Operation: BinomialGenBtpec%398ED01B0060
//	********************************************************************************
//
//	Name: BinomialGen(Ulong seed, Ulong numSamples, Float probability)
//
//	Description: Non-default constructor - requires the initial seed, the number of samples and the probability of occurrence of each
//	sample to generate the random variables.
//
//	Input parameters: Ulong seed;        //seed to begin the random number generation
//	                  Ulong numSamples;  //number of samples
//	                  Float probability; //probability of occurrence of each sample
//
//	Returns: none
//
//	Remarks: must invoke the DiscreteDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; the probability value must be: 0 < probability < 1;  and numSamples > 0.
//
//	********************************************************************************
BinomialGenBtpec::BinomialGenBtpec (Ulong seed, Ulong numSamples, Float probability)
  //## begin BinomialGenBtpec::BinomialGenBtpec%398ED01B0060.hasinit preserve=no
      : psave(probability),
        nsave(numSamples)
  //## end BinomialGenBtpec::BinomialGenBtpec%398ED01B0060.hasinit
  //## begin BinomialGenBtpec::BinomialGenBtpec%398ED01B0060.initialization preserve=yes
    , DiscreteDistribution (seed)
  //## end BinomialGenBtpec::BinomialGenBtpec%398ED01B0060.initialization
{
  //## begin BinomialGenBtpec::BinomialGenBtpec%398ED01B0060.body preserve=yes

	// In this first implementation, it is not expected that p or n will stay constant for any call, 
	// we therefore do not calculate anything in advance

  //## end BinomialGenBtpec::BinomialGenBtpec%398ED01B0060.body
}



//## Other Operations (implementation)
//## Operation: nextValue%398ED01B0069
//	********************************************************************************
//
//	Name: RetCode nextValue(Int *pValue)
//
//	Description: Returns the next value of the Binomial distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode BinomialGenBtpec::nextValue (Int *pValue)
{
  //## begin BinomialGenBtpec::nextValue%398ED01B0069.body preserve=yes
	return -1;
  //## end BinomialGenBtpec::nextValue%398ED01B0069.body
}

//## Operation: redefine%398ED01B006B
//	********************************************************************************
//
//	Name: RetCode redefine( Ulong numSamples, Float probability)
//
//	Description: Redefines the parameters suplied at construction time. As the  constructor,  requires the number of samples and the
//	probability of occurrence of each sample to generate the random variables.
//
//	Input parameters: Ulong numSamples;  //number of samples
//	                  Float probability; //probability of occurrence of each sample
//
//	Returns:SCH_SUCCESS
//	                error code in case of error
//
//	Remarks: the probability value must be: 0 < probability < 1;  and numSamples > 0.
//
//	********************************************************************************
RetCode BinomialGenBtpec::redefine (Ulong numSamples, Float probability)
{
  //## begin BinomialGenBtpec::redefine%398ED01B006B.body preserve=yes
	return -1;
  //## end BinomialGenBtpec::redefine%398ED01B006B.body
}

//## Operation: redefineAndNextValue%398ED12E004D
//	********************************************************************************
//
//	Name: RetCode nextValue(Int *pValue)
//
//	Description: Returns the next value of the Binomial distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode BinomialGenBtpec::redefineAndNextValue (Int *pValue, Ulong n, Float pp)
{
  //## begin BinomialGenBtpec::redefineAndNextValue%398ED12E004D.body preserve=yes
Long ignbin,i,ix,ix1,k,m,mp,T1;
Float al,alv,amaxp,c,f,f1,f2,ffm,fm,g,p,p1,p2,p3,p4,q,qn,r,u,v,w,w2,x,x1,x2,xl,xll,xlr,xm,xnp,xnpq,xr,ynorm,z,z2;
RetCode rc = SCH_SUCCESS;

	//-------------------------------------------------
	// we assume that the values have changed for now
	//-------------------------------------------------
    //if(pp != psave) goto S10;
    //if(n != nsave) goto S20;
    //if(xnp < 30.0) goto S150;
    //goto S30;
S10:
/*
*****SETUP, PERFORM ONLY WHEN PARAMETERS CHANGE
*/
    psave = pp;
    p = (psave<1.0-psave) ? psave : 1.0-psave;
    q = 1.0-p;
S20:
    xnp = n*p;
    nsave = n;
    if(xnp < 30.0) goto S140;
    ffm = xnp+p;
    m = ffm;
    fm = m;
    xnpq = xnp*q;
    p1 = (long) (2.195*sqrt(xnpq)-4.6*q)+0.5;
    xm = fm+0.5;
    xl = xm-p1;
    xr = xm+p1;
    c = 0.134+20.5/(15.3+fm);
    al = (ffm-xl)/(ffm-xl*p);
    xll = al*(1.0+0.5*al);
    al = (xr-ffm)/(xr*q);
    xlr = al*(1.0+0.5*al);
    p2 = p1*(1.0+c+c);
    p3 = p2+c/xll;
    p4 = p3+c/xlr;
S30:
/*
*****GENERATE VARIATE
*/
	rc = getNextValue (&u);
	rc = getNextValue (&v);
    u = u*p4;
    //u = ranf()*p4;
    //v = ranf();
/*
     TRIANGULAR REGION
*/
    if(u > p1) goto S40;
    ix = xm-p1*v+u;
    goto S170;
S40:
/*
     PARALLELOGRAM REGION
*/
    if(u > p2) goto S50;
    x = xl+(u-p1)/c;
    v = v*c+1.0-fabs(xm-x)/p1;
    if(v > 1.0 || v <= 0.0) goto S30;
    ix = x;
    goto S70;
S50:
/*
     LEFT TAIL
*/
    if(u > p3) goto S60;
    ix = xl+log(v)/xll;
    if(ix < 0) goto S30;
    v *= ((u-p2)*xll);
    goto S70;
S60:
/*
     RIGHT TAIL
*/
    ix = xr-log(v)/xlr;
    if(ix > n) goto S30;
    v *= ((u-p3)*xlr);
S70:
/*
*****DETERMINE APPROPRIATE WAY TO PERFORM ACCEPT/REJECT TEST
*/
    k = abs(ix-m);
    if(k > 20 && k < xnpq/2-1) goto S130;
/*
     EXPLICIT EVALUATION
*/
    f = 1.0;
    r = p/q;
    g = (n+1)*r;
    T1 = m-ix;
    if(T1 < 0) goto S80;
    else if(T1 == 0) goto S120;
    else  goto S100;
S80:
    mp = m+1;
    for(i=mp; i<=ix; i++) f *= (g/i-r);
    goto S120;
S100:
    ix1 = ix+1;
    for(i=ix1; i<=m; i++) f /= (g/i-r);
S120:
    if(v <= f) goto S170;
    goto S30;
S130:
/*
     SQUEEZING USING UPPER AND LOWER BOUNDS ON ALOG(F(X))
*/
    amaxp = k/xnpq*((k*(k/3.0+0.625)+0.1666666666666)/xnpq+0.5);
    ynorm = -(k*k/(2.0*xnpq));
    alv = log(v);
    if(alv < ynorm-amaxp) goto S170;
    if(alv > ynorm+amaxp) goto S30;
/*
     STIRLING'S FORMULA TO MACHINE ACCURACY FOR
     THE FINAL ACCEPTANCE/REJECTION TEST
*/
    x1 = ix+1.0;
    f1 = fm+1.0;
    z = n+1.0-fm;
    w = n-ix+1.0;
    z2 = z*z;
    x2 = x1*x1;
    f2 = f1*f1;
    w2 = w*w;
    if(alv <= xm*log(f1/x1)+(n-m+0.5)*log(z/w)+(ix-m)*log(w*p/(x1*q))+(13860.0-
      (462.0-(132.0-(99.0-140.0/f2)/f2)/f2)/f2)/f1/166320.0+(13860.0-(462.0-
      (132.0-(99.0-140.0/z2)/z2)/z2)/z2)/z/166320.0+(13860.0-(462.0-(132.0-
      (99.0-140.0/x2)/x2)/x2)/x2)/x1/166320.0+(13860.0-(462.0-(132.0-(99.0
      -140.0/w2)/w2)/w2)/w2)/w/166320.0) goto S170;
    goto S30;
S140:
/*
     INVERSE CDF LOGIC FOR MEAN LESS THAN 30
*/
    qn = pow(q,(double)n);
    r = p/q;
    g = r*(n+1);
S150:
    ix = 0;
    f = qn;
	rc = getNextValue (&u);
    //u = ranf();
S160:
    if(u < f) goto S170;
    if(ix > 110) goto S150;
    u -= f;
    ix += 1;
    f *= (g/ix-r);
    goto S160;
S170:
    if(psave > 0.5) ix = n-ix;
    ignbin = ix;
	*pValue = ignbin;
    return (SCH_SUCCESS);
  //## end BinomialGenBtpec::redefineAndNextValue%398ED12E004D.body
}

// Additional Declarations
  //## begin BinomialGenBtpec%398ED01B005C.declarations preserve=yes
  //## end BinomialGenBtpec%398ED01B005C.declarations

//## begin module%398ED01B005C.epilog preserve=yes
} //namespace sch
//## end module%398ED01B005C.epilog
