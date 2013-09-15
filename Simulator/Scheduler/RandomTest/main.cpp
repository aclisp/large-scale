#include "MemLeak.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include "RandomGen.h"
#include "SchException.h"
#include "StatsCalc.h"

using namespace sch;

//#define  TEST_MEM_LEAK		// activate the memory leak verification


const char *WA_TRACE_FORMAT = "#samples = %02d, min = %02f, max = %02f, sum = %02f, mean = %02f, wa = %02f";
const char *DS_TRACE_FORMAT = "#samples = %02d, min = %02f, max = %02f, sum = %02f, mean = %02f, variance = %02f, sd = %02f";
const char *PQ_TRACE_FORMAT = "#samples = %02d, threshold = %02.02f, p-quantile = %02.02f";


enum distribution { Beta = 1, Binomial, Cauchy, Discrete, Draw, Erlang, Gamma, Geometric, 
					LogNormal, NegExp, Normal, Pareto, Poisson, RandInt, Rayleigh, 
					TruncatedCauchy, TruncatedGeometric, TruncatedLogNormal, TruncatedPareto, TruncatedWeibull, Uniform, 
					Weibull};


	RetCode               rc     = SCH_SUCCESS;

	WindowedAverage      *pWA    = NULL;
	DescriptiveStatistic *pDS    = NULL;
	DiscreteHistogram    *pDH    = NULL;
	Quantile             *pPQ    = NULL;
	ContinuousHistogram  *pCH    = NULL;
	CumulativeHistogram  *pAH	 = NULL;

	BooleanDistribution *draw = NULL;

	DiscreteDistribution *discrete = NULL;
	DiscreteDistribution *randInt = NULL;
	DiscreteDistribution *poisson = NULL;
	DiscreteDistribution *binomial = NULL;
	DiscreteDistribution *geometric = NULL;
	DiscreteDistribution *truncatedGeometric = NULL;

	ContinuousDistribution *beta = NULL;
	ContinuousDistribution *cauchy = NULL;
	ContinuousDistribution *erlang = NULL;
	ContinuousDistribution *gamma = NULL;
	ContinuousDistribution *logNormal = NULL;
	ContinuousDistribution *negExp = NULL;
	ContinuousDistribution *normal = NULL;
	ContinuousDistribution *pareto = NULL;
	ContinuousDistribution *rayleigh = NULL;
	ContinuousDistribution *truncCauchy = NULL;
	ContinuousDistribution *truncLogNormal = NULL;
	ContinuousDistribution *truncPareto = NULL;
	ContinuousDistribution *truncWeibull = NULL;
	ContinuousDistribution *uniform = NULL;
	ContinuousDistribution *weibull = NULL;


	bool  value;
	bool   success;		
	char  tmpMsg[200];
	Int   contFalse;
	Int   contTrue;
	Int   continuousIntervals;
	Int	  cumulativeIntervals;
	Int   iA;
	Int   intervals;
	Int   iNum;
	Int   iResult;
	Int   iXMin;
	Int   iXMax;
	Int   maxVal;
	Int   minVal;
	Int   optionDistr;
	Int   output;
	Int   separator;
	Int   seqLength;
	Int   whatSee;
	Int   winSize;
	Ulong calcDS = 2;
	Ulong calcWA = 2;
	Ulong calcPQ = 2;
	Ulong calcDH = 2;
	Ulong calcCH = 2;
	Ulong calcAH = 2;
    Ulong i;
	Ulong kValue;
	Ulong numberOfSamples;
	Ulong seed;
	Ulong uMaxVal;
	Ulong uMinVal;
	Ulong uStdDev;
	Float alfa;
	Float probValue;
	Float a;
	Float alfa1;
	Float alfa2;
	Float b;
	Float bValue;
	Float cValue;
	Float exponent;
	Float fMaxVal;
	Float fMinVal;
	Float maxContinuousVal;
	Float minContinuousVal;
	Float fXMax;
	Float fXMin;
	Float mean;
	Float num;
	Float fStdDev;
	Float fResult;
	Float threshold;
	Float kParetoValue;
	Float lambda;
	Float p;
	Float pVal;
	Float *probability;
	Float *pTmpProb;
	Float pX;
	Float pY;
	string sNumber;
	string fileName;
	ofstream *pf = NULL;




//	Handle several math errors caused by abnormal operation in
//  Function Evaluator package. When this happens, _matherr only 
//  reports the error and follows the normal processing.

Int _matherr( struct _exception *except )
{
	cout << endl << "ERROR IN:" << 	except->name << " with " << except->arg1 << endl;

    return 0;    /*  use the default actions */    
}




bool VerifyInteger (string sNum)
{
	Uint pos;
	
	pos = sNum.find (".");
	if (pos != -1)
	{
		return false;
	}

	pos = sNum.find (",");
	if (pos != -1)
	{
		return false;
	}

	return true;
}

bool VerifyNegative (string sNum)
{
	Uint pos;
	
	pos = sNum.find ("-");
	if (pos != -1)
	{
		return false;
	}
	return true;
}



void calcBinomial (bool redefine)
{
	cout << "\nNew BinomialGen" << endl;
	cout << "What is the Binomial number of samples?" << endl;
	cin >> sNumber;
	if (!(VerifyInteger (sNumber)))
	{
		cout << "Invalid value" << endl;
		calcDS = 2;
		calcWA = 2;
		calcPQ = 2;
		calcDH = 2;
		calcCH = 2;
		exit(1);
	}
	else
	{
		numberOfSamples = atoi (sNumber.c_str());
	}
	cout << "What is the probability value?" << endl;
	cin  >> probValue;


	if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
	{
	*pf << endl << endl;
	*pf << "BinomialGen: Number of samples = " << numberOfSamples << " ProbValue = " << probValue << endl << endl;

	}

	if (redefine)
		((BinomialGen *)binomial)->redefine(numberOfSamples, probValue);
	else
		binomial = new BinomialGen (seed, numberOfSamples, probValue);

	if (binomial != NULL)
	{
		cout << "\nBinomial Distribution";
		for (i=0; i<seqLength; i++)
		{
			rc = binomial->nextValue(&iNum);
			if (rc == SCH_SUCCESS)
			{
				if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
				{
					cout << "\nnum = " << iNum;
				}
				if (calcDS == 1)
				{
					pDS->nextSample (iNum);
				}
				if (calcWA == 1)
				{
					pWA->nextSample (iNum);
				}
				if (calcDH == 1)
				{
					pDH->nextSample (iNum);
				}
				if (calcPQ == 1)
				{
					pPQ->nextSample (iNum);
				}
				if (calcCH == 1)
				{
					pCH->nextSample (iNum);
				}
				if (calcAH == 1)
				{
					pAH->nextSample (iNum);
				}
				if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
				{
					if (separator == 1) // blank separator
					{
						*pf << iNum << " ";
					}
					else	// CR separator
					{
						*pf << iNum << endl;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
}


void calcDiscrete(bool redefine)
{

				cout << "\nNew DiscreteGen" << endl;


				if (!redefine)  { // when redefining the parameters we do not need the array size => It is fix
					cout << "What is the probability array size ?" << endl;
					cin >> sNumber;
					if (!(VerifyInteger (sNumber)))
					{
						cout << "Invalid value" << endl;
						calcDS = 2;
						calcWA = 2;
						calcPQ = 2;
						calcDH = 2;
						calcCH = 2;
						exit(1);
					}
					else
					{
						numberOfSamples = atoi (sNumber.c_str());
					}
				}

				cout << "Entry with each value of the probability separating with ENTER" << endl;
				probability = new Float [numberOfSamples];
				if (probability != NULL)
				{
					pTmpProb = probability;

					if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
					{
						*pf << "\n\nDiscreteGen. Number of samples = " << numberOfSamples << endl;
					}

					for (i=0; i<numberOfSamples; i++, pTmpProb++)
					{
						cin >> *pTmpProb;
						if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
						{
							*pf << *pTmpProb << " ";
						}
					}
					cout << "What is the a value? " << endl;
					cin >> sNumber;
					if (!(VerifyInteger (sNumber)))
					{
						cout << "Invalid value" << endl;
						calcDS = 2;
						calcWA = 2;
						calcPQ = 2;
						calcDH = 2;
						calcCH = 2;
						exit(1);

					}
					else
					{
						iA = atoi (sNumber.c_str());
					}
					if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
					{
						*pf << "a = " << iA << endl << endl;
					}

					if (redefine)
						((DiscreteGen *)discrete)->redefine(probability, iA);
					else   
						discrete = new DiscreteGen (seed, probability, numberOfSamples, iA);
					
					if (discrete != NULL)
					{
		 				cout << "\nDiscrete Distribution";
						for (i=0; i<seqLength; i++)
						{
							rc = discrete->nextValue(&iNum);
							if (rc == SCH_SUCCESS)
							{
								if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
								{
									cout << "\nnum = " << iNum;
								}
								if (calcDS == 1)
								{
									pDS->nextSample (iNum);
								}
								if (calcWA == 1)
								{
									pWA->nextSample (iNum);
								}
								if (calcDH == 1)
								{
									pDH->nextSample (iNum);
								}
								if (calcPQ == 1)
								{
									pPQ->nextSample (iNum);
								}
								if (calcCH == 1)
								{
									pCH->nextSample (iNum);
								}
								if (calcAH == 1)
								{
									pAH->nextSample (iNum);
								}

								if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
								{
									if (separator == 1) // blank separator
									{
										*pf << iNum << " ";
									}
									else	// CR separator
									{
										*pf << iNum << endl;
									}
								}
							}
							else
							{
								break;
							}
						}
					}
				}

				delete [] probability;
}




void calcDraw( bool redefine)
{
				cout << "\nNew DrawGen" << endl;
				cout << "What is the threshold value?" << endl;
				cin >> threshold;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "\n\nDrawGen. Threshold = " << threshold << endl << endl;
				}

				if (redefine)
					((DrawGen *)draw)->redefine(threshold);
				else
					draw = new DrawGen (seed, threshold);

				contTrue = 0;
				contFalse = 0;
				if (draw != NULL)
				{
					cout << "\nDraw Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = draw->nextValue(&value);
						if (rc == SCH_SUCCESS)
						{
							if (value == 1)
							{
								contTrue ++;
							}
							else
							{
								contFalse ++;
							}
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nvalue = " << value;
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << value << " ";
								}
								else	// CR separator
								{
									*pf << value << endl;
								}
							}
						}
						else
						{
							break;
						}
					}
					if (rc == SCH_SUCCESS)
					{
						cout << endl << "True = " << contTrue << " False = " << contFalse << endl;

						if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
						{
							*pf << endl << "True = " << contTrue << " False = " << contFalse << endl << endl;
						}
					}					
				}


}


void calcGeometric (bool redefine)
{

 				cout << "\nNew GeometricGen" << endl;
				cout << "What is the p value? " << endl;
				cin  >> pVal;
				
				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "\n\nGeometricGen. p Value = " << pVal << endl << endl;
				}

				if (redefine)
					((GeometricGen *)geometric)->redefine( pVal);
				else
					geometric = new GeometricGen (seed, pVal);

				if (geometric != NULL)
				{
					cout << "\nGeometric Distribution";
				}

				for (i=0; i<seqLength; i++)
				{
					rc = geometric->nextValue(&iNum);
					if (rc == SCH_SUCCESS)
					{
						if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
						{
							cout << "\nnum = " << iNum;
						}
						if (calcDS == 1)
						{
							pDS->nextSample (iNum);
						}
						if (calcWA == 1)
						{
							pWA->nextSample (iNum);
						}
						if (calcDH == 1)
						{
							pDH->nextSample (iNum);
						}
						if (calcPQ == 1)
						{
							pPQ->nextSample (iNum);
						}
						if (calcCH == 1)
						{
							pCH->nextSample (iNum);
						}
						if (calcAH == 1)
						{
							pAH->nextSample (iNum);
						}
						if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
						{
							if (separator == 1) // blank separator
							{
								*pf << iNum << " ";
							}
							else	// CR separator
							{
								*pf << iNum << endl;
							}
						}
					}
					else
					{
						break;
					}
				}

}


void calcPoisson (bool redefine)
{
				cout << "\nNew PoissonGen" << endl;
				cout << "What is the mean value? " << endl;
				cin >> mean;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "\n\nPoissonGen. Mean = " << mean << endl << endl;
				}

				if (redefine)
					((PoissonGen *)poisson)->redefine(mean);
				else
					poisson = new PoissonGen (seed, mean);

				if (poisson != NULL)
				{
					cout << "\nPoisson Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = poisson->nextValue(&iNum);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << iNum;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (iNum);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (iNum);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (iNum);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (iNum);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << iNum << " ";
								}
								else	// CR separator
								{
									*pf << iNum << endl;
								}
							}
						}
						else
						{
							break;
						}
					}
				}

}


void calcRandInt (bool redefine)
{
 				cout << "\nNew RandIntGen" << endl;
				cout << "What is the minimum value?" << endl;
				cin >> sNumber;
				if (!(VerifyInteger (sNumber)))
				{
					cout << "Invalid value" << endl;
					calcDS = 2;
					calcWA = 2;
					calcPQ = 2;
					calcDH = 2;
					calcCH = 2;
					exit(1);
				}
				else
				{
					uMinVal = atoi (sNumber.c_str());
				}
				cout << "What is the maximum value?" << endl;
				cin >> sNumber;
				if (!(VerifyInteger (sNumber)))
				{
					cout << "Invalid value" << endl;
					calcDS = 2;
					calcWA = 2;
					calcPQ = 2;
					calcDH = 2;
					calcCH = 2;
					exit(1);
				}
				else
				{
					uMaxVal = atoi (sNumber.c_str());
				}

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "\n\nRandIntGen. MinVal = " << uMinVal << " MaxVal = " << uMaxVal << endl;
				}

				if (redefine)
					((RandIntGen *)randInt)->redefine(uMinVal, uMaxVal);
				else
					randInt = new RandIntGen (seed, uMinVal, uMaxVal);

				if (randInt != NULL)
				{
					cout << "\nRandInt Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = randInt->nextValue(&iNum);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << iNum;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (iNum);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (iNum);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (iNum);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (iNum);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << iNum << " ";
								}
								else	// CR separator
								{
									*pf << iNum << endl;
								}
							}
						}
						else
						{
							break;
						}
					}
				}

}


void calcTruncatedGeometric (bool redefine)
{

 				cout << "\nNew TruncatedGeometricGen" << endl;
				cout << "What is the p value? " << endl;
				cin  >> pVal;
				cout << "What is the truncated minimum value? " << endl;
				cin >> iXMin;
				cout << "What is the truncated maximum value? " << endl;				
				cin >> iXMax;

				
				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << endl << endl << "TruncatedGeometricGen. p Value = " << pVal;
					*pf << " Minimum Value = " << iXMin;
					*pf << " Maximum Value = " << iXMax	<< endl << endl;
				}

				if (redefine)
					((TruncatedGeometricGen *)truncatedGeometric)->redefine(pVal, iXMin, iXMax);
				else
					truncatedGeometric = new TruncatedGeometricGen (seed, pVal, iXMin, iXMax);

				if (truncatedGeometric != NULL)
				{
					cout << "\n Truncated Geometric Distribution";
				}

				for (i=0; i<seqLength; i++)
				{
					rc = truncatedGeometric->nextValue(&iNum);
					if (rc == SCH_SUCCESS)
					{
						if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
						{
							cout << "\nnum = " << iNum;
						}
						if (calcDS == 1)
						{
							pDS->nextSample (iNum);
						}
						if (calcWA == 1)
						{
							pWA->nextSample (iNum);
						}
						if (calcDH == 1)
						{
							pDH->nextSample (iNum);
						}
						if (calcPQ == 1)
						{
							pPQ->nextSample (iNum);
						}
						if (calcCH == 1)
						{
							pCH->nextSample (iNum);
						}
						if (calcAH == 1)
						{
							pAH->nextSample (iNum);
						}
						if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
						{
							if (separator == 1) // blank separator
							{
								*pf << iNum << " ";
							}
							else	// CR separator
							{
								*pf << iNum << endl;
							}
						}
					}
					else
					{
						break;
					}
				}

}





void reportResults()
{
	//Report  Result
	if (success)				// do not occur errors
	{
		if (calcDS == 1)
		{
			cout <<"\nDescriptive Statistic results: " << endl;
			sprintf (tmpMsg, DS_TRACE_FORMAT, 			  
							 pDS->pMean->getNumSamples(),				  
							 pDS->pMean->getMin(), pDS->pMean->getMax(),  
							 pDS->pMean->getSum(), pDS->pMean->getMean(), 
							 pDS->getVariance(), pDS->getStdDev());	      
			cout << tmpMsg << endl; 
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
			{
				*pf << "Descriptive Statistic results: " << endl;
				*pf << tmpMsg << endl;
			}
		   pDS->reset();
		}

		if (calcWA == 1)
		{
			cout << endl << "Windowed Average results: " << endl;
			sprintf (tmpMsg, WA_TRACE_FORMAT, 
							 pWA->pMean->getNumSamples(),	
							 pWA->pMean->getMin(), pWA->pMean->getMax(),
							 pWA->pMean->getSum(), pWA->pMean->getMean(),
							 pWA->getWindowedAverage());
			cout << tmpMsg << endl;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
			{
				*pf << endl << "Windowed Average results: " << endl;
				*pf << tmpMsg << endl;
			}
			pWA->reset();
		}

		if (calcPQ == 1)
		{
			fResult = pPQ->getQuantile ();
			cout << endl << "pQuantile results: " << endl;
			sprintf (tmpMsg, PQ_TRACE_FORMAT, 
							pPQ->getNumSamples(),
							pPQ->getThreshold(),
							fResult);
			cout << tmpMsg << endl;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
			{
				*pf << endl << "pQuantile results: " << endl;
				*pf << tmpMsg << endl;
			}
			
			pPQ->reset();

		}

		if (calcDH == 1)
		{
			//
			// now loop thru histogram intervals and print results
			//

			cout << endl << "Discrete Histogram results: " << endl;

			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf << endl << "Discrete Histogram results: " << endl;

			for (Int i = 0; (i < pDH->getNumIntervals()) && (rc == SCH_SUCCESS); i++)
			{
				rc = pDH->getResult(i, &iResult);
				if (rc == SCH_SUCCESS)
				{
					sprintf (tmpMsg, "interval = %02d, result = %02d", i+1, iResult);
					cout << tmpMsg << endl;
					if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
					{
						*pf << tmpMsg << endl;
					}
				}
			}
			sprintf (tmpMsg, "\n#intervals = %02d, width = %d, numSamples = %d ", 
							 pDH->getNumIntervals(), pDH->getIntervalWidth(), 
							 pDH->getNumSamples());
			cout << tmpMsg << endl;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf << tmpMsg << endl;
			sprintf (tmpMsg, "#maxSample = %d, maxValue = %d, \n#minSample = %d, minValue = %d", 
							 pDH->getMaxSample(), pDH->getMaxValue(), 
							 pDH->getMinSample(), pDH->getMinValue());
			cout << tmpMsg << endl;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf << tmpMsg << endl;
			pDH->reset();

		}
		
		if (calcCH == 1)
		{
			//
			// now loop thru histogram intervals and print results

			cout << endl << "Continuous Histogram results: " << endl << endl;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf << endl << "Continuous Histogram results: " << endl;


			pX = minContinuousVal;
			for (Int i = 0; (i < pCH->getNumIntervals()) && (rc == SCH_SUCCESS); i++)
			{
				rc = pCH->getResult(i, &iResult);
				if (rc == SCH_SUCCESS)
				{
					cout << pX << '\t' << iResult << endl;
					if ((output == 1) &&  ((whatSee == 1) || (whatSee == 3)))
					{
						*pf << pX << '\t' << iResult << endl;
					}
				}
				pX += pCH->getIntervalWidth();
			}

			sprintf (tmpMsg, "\n#intervals = %d, width = %f, numSamples = %d \n", 
							 pCH->getNumIntervals(), pCH->getIntervalWidth(), 
							 pCH->getNumSamples());
			cout << tmpMsg;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf << tmpMsg;
			sprintf (tmpMsg, "#maxSample = %f, maxValue = %f, \n#minSample = %f, minValue = %f \n", 
							 pCH->getMaxSample(), pCH->getMaxValue(), 
							 pCH->getMinSample(), pCH->getMinValue());
			cout << tmpMsg;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf << tmpMsg << endl;
			pCH->reset();
		}
		
		if (calcAH == 1)
		{
			//
			// now loop thru histogram intervals and print results
			// Note that when the file flag is set only the intervals of the
			// histogram is written to the file. 
			//
			
			sprintf (tmpMsg, "\n\nCumulative Histogram with derived results; #intervals = %02d", 
							 pAH->getNumIntervals());

			cout << tmpMsg << endl;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf  << tmpMsg << endl;

			for (Int i = 0; (i < pAH->getNumIntervals()) && (rc == SCH_SUCCESS); i++)
			{
				rc = pAH->getDerivedResult (i, &pX, &pY);
				if (rc == SCH_SUCCESS)			
				{
					cout << pX << '\t' << pY << endl;
					if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
					{
						*pf <<  pX << '\t' << pY << endl;
					}
				}
			}

			sprintf (tmpMsg, "\n\nCumulative Histogram with cumulative results; #intervals = %02d", 
							 pAH->getNumIntervals());

			cout << tmpMsg << endl;
			if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				*pf  << tmpMsg << endl;

			for (Int i = 0; (i <= pAH->getNumIntervals()) && (rc == SCH_SUCCESS); i++)
			{
				rc = pAH->getCumulativeResult (i, &pX, &pY);
				if (rc == SCH_SUCCESS)			
				{
					cout << pX << '\t' << pY << endl;
					if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
					{
						*pf <<  pX << '\t' << pY << endl;
					}
				}
			}

			pAH->reset();

		}
	}



}


void main (void)
{
	// Ask which distribution the user wants generate
	 Ulong userResp;

	 DumpMemoryLeaks();

#if defined(_DEBUG) && defined(WIN32) && defined (TEST_MEM_LEAK)
	// set report mode for debugging purpose only (under Windows NT)
	_CrtMemState diffState, beginState, endState;
	HANDLE outfile;
	outfile = CreateFile("memleak.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_WARN, outfile );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ERROR, outfile );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ASSERT, outfile );

	// get memory at start of program;
	// include the following variables at your debug window:
	//   _crtBreakAlloc
	//   _lRequestCurr
	_CrtMemCheckpoint(&beginState);
#endif






	do
	{
		cout << endl;
		cout << " 1  - Beta "      << endl;
		cout << " 2  - Binomial "  << endl;
		cout << " 3  - Cauchy "    << endl;
		cout << " 4  - Discrete "  << endl;  
		cout << " 5  - Draw "      << endl;
		cout << " 6  - Erlang "    << endl;
		cout << " 7  - Gamma "     << endl;
		cout << " 8  - Geometric " << endl; 
		cout << " 9  - LogNormal " << endl;
		cout << " 10 - NegExp "    << endl;
		cout << " 11 - Normal "    << endl;
		cout << " 12 - Pareto "    << endl;
		cout << " 13 - Poisson "   << endl;
		cout << " 14 - RandInt "   << endl;
		cout << " 15 - Rayleigh "  << endl;
		cout << " 16 - Truncated Cauchy " << endl;
		cout << " 17 - Truncated Geometric " << endl;
		cout << " 18 - Truncated LogNormal " << endl;
		cout << " 19 - Truncated Pareto " << endl;
		cout << " 20 - Truncated Weibull " << endl;
		cout << " 21 - Uniform "   << endl;
		cout << " 22 - Weibull "   << endl;
		cout << " 23 - QUIT "      << endl;

		cout << "Which distribution do you want generate?" << endl;
		cin  >> optionDistr;
	}while ((optionDistr < 1) && (optionDistr > 23) );

	if (optionDistr == 23)
	{
		exit (0);
	}

	cout << "Choose the output: 1=Descriptive Statistics + RandomNumbers"<< endl;
	cout << "                   2=RandomNumbers " << endl;
	cout << "                   3=Descriptive Statistics: " << endl;
	do 
	{
		cout << endl;
		cin >> whatSee;
	} while ((whatSee != 1) && (whatSee != 2) && (whatSee != 3) );

	if ((whatSee == 1) || (whatSee == 2))
	{
		cout << "What is the separator between the numbers? (1=blank;2=CR)";
		do 
		{
			cout << endl;
			cin >> separator;
		} while ((separator != 1) && (separator != 2));
	}


	// Ask to put the output in a file
	cout << "\n\nThe results always go to the screen (default)." << endl;
	cout << "Do you also want put the output in a file? (1=Yes;2=No) ";
	cin >> output;
	if (output == 1)
	{
		cout << "What is the output file name?" << endl;
		cin >> fileName;
		if (fileName.length() == 0)
		{
			cout << "Invalid file name" << endl;
			exit (0);
		}
		pf = new ofstream (fileName.c_str());
		if (!pf)
		{
			cout << "Can not open the file" << endl;
			exit (0);
		}
		if ((whatSee == 1) || (whatSee == 3))
		{
			*pf << "\t\tDistribution = ";
			switch 	(optionDistr) {
			case Beta: *pf << "Beta"<< endl; break;
			case Binomial : *pf << "Binomial "  << endl;	 break;
			case Cauchy : *pf << "Cauchy "    << endl;	 break;
			case Discrete : *pf << "Discrete "  << endl;  break;
			case Draw : *pf << "Draw "      << endl;	 break;
			case Erlang : *pf << "Erlang "    << endl;	 break;
			case Gamma : *pf << "Gamma "     << endl;	 break;
			case Geometric : *pf << "Geometric " << endl;  break;
			case LogNormal : *pf << "LogNormal " << endl;	 break;
			case NegExp : *pf << "NegExp "    << endl;	 break;
			case Normal : *pf << "Normal "    << endl;	 break;
			case Pareto : *pf << "Pareto "    << endl;	 break;
			case Poisson : *pf << "Poisson "   << endl;	 break;
			case RandInt : *pf << "RandInt "   << endl;	 break;
			case Rayleigh : *pf << "Rayleigh "  << endl;	 break;
			case TruncatedCauchy : *pf << "Truncated Cauchy " << endl;	break;
			case TruncatedGeometric : *pf << "Truncated Geometric " << endl;	break;
			case TruncatedLogNormal : *pf << "Truncated LogNormal " << endl; break;
			case TruncatedPareto : *pf << "Truncated Pareto " << endl; break;
			case TruncatedWeibull : *pf << "Truncated Weibull " << endl; break;
			case Uniform : *pf << "Uniform "   << endl;	  break;
			case Weibull : *pf << "Weibull "   << endl;	  break;
			}

		}
	}

	cout << "What is the length of the sequence?" << endl;
	cin >> sNumber;
	if (!(VerifyInteger (sNumber)))
	{
		cout << "Invalid value" << endl;
		exit(0);
	}
	else
	{
		seqLength = atoi (sNumber.c_str());
	}
	if (seqLength <= 0)
	{
		cout << "\nInvalid sequence length" << endl;
		exit (0);
	}

	if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
	{
		*pf << "Sequence length = " << seqLength << endl;
	}

	cout << "What is the seed?" << endl;
	cin >> sNumber;

	if (!(VerifyInteger (sNumber)))
	{
		cout << "Invalid value" << endl;
		exit(0);
	}
	else
	{
		seed = atoi (sNumber.c_str());
	}

	if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
	{
		*pf << "Seed = " << seed << endl;
	}

	if (optionDistr != Draw)  
	{
		try
		{
			if ((whatSee == 1) || (whatSee == 3)) {
				calcDS = 1;
				pDS = new DescriptiveStatistic ();
				if (output == 1)
				{
					*pf << "Calculate Descriptive Statistics" << endl;
				}
			}

			cout << "Do you want to calculate Windowed Average? (1=Yes;2=No)";
			do 
			{
				cout << endl;
				cin >> calcWA;
			} while ((calcWA != 1) && (calcWA != 2));
			if (calcWA == 1)
			{
				cout << "What is the window size?";
				do
				{
					cout << endl;
					cin >> winSize;
				} while (winSize <= 0);
				pWA = new WindowedAverage (winSize);
				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "Calculate Windowed Average. WinSize = " << winSize << endl;
				}
			}

			if ((optionDistr == Binomial) || (optionDistr == Discrete) || 
				(optionDistr == Geometric) || (optionDistr == Poisson) || 
				(optionDistr == RandInt) || (optionDistr == TruncatedGeometric))
			{
				cout << "Do you want to calculate Discrete Histogram? (1=Yes;2=No)";
				do 
				{
					cout << endl;
					cin >> calcDH;
				} while ((calcDH != 1) && (calcDH != 2));
				if (calcDH == 1)
				{
					cout << "What is the minimum point?" << endl;
					cin >> minVal;
				
					cout << "What is the maximum point?" << endl;
					cin >> maxVal;

					cout << "What is the number of the intervals?" << endl;
					cin >> intervals;

					pDH = new DiscreteHistogram (minVal, maxVal, intervals);	
					if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
					{
						*pf << "Calculate Discrete Histogram. MinVal = " << minVal << " MaxVal = " << maxVal << " Intervals = " << intervals << endl;
					}
				}
			}
			else
			{

				cout << "Do you want to calculate Continuous Histogram? (1=Yes;2=No)";
				do 
				{
					cout << endl;
					cin >> calcCH;
				} while ((calcCH != 1) && (calcCH != 2));
				if (calcCH == 1) {
					cout << "What is the minimum point of the histogram?" << endl;
					cin >> minContinuousVal;
				
					cout << "What is the maximum point of the histogram?"  << endl;
					cin >> maxContinuousVal;

					cout << "What is the number of the intervals in the histogram?"  << endl;
					cin >> continuousIntervals;

					pCH = new ContinuousHistogram (minContinuousVal, maxContinuousVal, continuousIntervals);
				}

				cout << "Do you want calculate Cumulative Histogram? (1=Yes;2=No)";
				do 
				{
					cout << endl;
					cin >> calcAH;
				} while ((calcAH != 1) && (calcAH != 2));
				if (calcAH == 1) {
					cout << "What is the number of the intervals in the histogram? ";
					cin >> cumulativeIntervals;

					pAH = new CumulativeHistogram (cumulativeIntervals);	
				}		
			}

			cout << "\nDo you want to calculate Quantile? (1=Yes;2=No)";
			do 
			{
				cout << endl;
				cin >> calcPQ;
			} while ((calcPQ != 1) && (calcPQ != 2));
			if (calcPQ == 1)
			{
				cout << "What is the p value for Quantile Result?" << endl;
				cin >> p;

				pPQ = new Quantile (p);
				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "Calculate Quantile. p = " << p << endl;
				} 
			}
		}
		catch (SchException &e)
		{
			cout << "\nERROR = " << e.getError() << "\t";
			cerr << e.what();

	 		if (pDS != NULL)
			{
				delete pDS;
			}
			if (pWA != NULL)
			{
				delete pWA;
			}
			if (pDH != NULL)
			{
				delete pDH;
			}
			if (pCH != NULL)
			{
				delete pDH;
			}
			if (pAH != NULL)
			{
				delete pDH;
			}
			if (pPQ != NULL)
			{
				delete pPQ;
			}
			pf->close();
			exit (0);
		}
	}

	success = true;
	pf->precision(10);
	switch (optionDistr)
	{
		case Beta:
			try
			{
				cout << "\nNew BetaGen" << endl;
				cout << "What is the alfa1 value?" << endl;
				cin >> alfa1;
				cout << "What is the alfa2 value?" << endl;
				cin >> alfa2;
				cout << "What is the a value? " << endl;
				cin  >> a;
				cout << "What is the b value? " << endl;
				cin  >> b;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "BetaGen. Alfa1 = " << alfa1 << " Alfa2 = " << alfa2 << " a = " << a << " b = " << b << endl << endl;
				}

				beta = new BetaGen (seed, alfa1, alfa2, a, b);
				if (beta != NULL)
				{
					cout << "\nBeta Distribution" << endl;
					for (i=0; i<seqLength; i++)
					{
						rc = beta->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}
					delete beta;
					reportResults();
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;

		case Binomial:
			try
			{

				calcBinomial(false);

				reportResults();

				for ( ; ;)
				{
					cout << "\nDo you want redefine the initial parameters and execute another run ?(1=Yes; 2=No)";
					cout << endl;
					cin >> userResp;
					if (userResp != 1) break;
					calcBinomial(true);

					reportResults();
				}
				delete binomial;

			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Cauchy:
			try
			{
				cout << "\nNew CauchyGen" << endl;
				cout << "What is the a value?" << endl;
				cin  >> a;
				cout << "What is the b value?" << endl;				
				cin  >> b;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "CauchyGen. a = " << a << " b = " << b << endl << endl;
				}

				cauchy = new CauchyGen (seed, a, b);
				if (cauchy != NULL)
				{
					cout << "\nCauchy Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = cauchy->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete cauchy;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;

		case Discrete:
			try
			{

				calcDiscrete(false);

				reportResults();

				for (;;)
				{
					cout << "\nDo you want redefine the initial parameters and execute another run ?(1=Yes; 2=No)";
					cout << endl;
					cin >> userResp;
					if (userResp != 1) break;
					calcDiscrete(true);

					reportResults();
				}
				delete discrete;
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;

		case Draw:
			try
			{
				calcDraw(false);

				reportResults();

				for (;;)
				{
					cout << "\nDo you want redefine the initial parameters and execute another run ?(1=Yes; 2=No)";
					cout << endl;
					cin >> userResp;
					if (userResp != 1) break;
					calcDraw(true);

					reportResults();
				}

			   	delete draw;

			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Erlang:
			try {
				cout << "\nNew ErlangGen" << endl;
				cout << "What is the lambda value? " << endl;
				cin >> lambda;
				cout << "What is the k value?" << endl;
				cin >> sNumber;
				if (!(VerifyInteger (sNumber)))
				{
					cout << "Invalid value" << endl;
					calcDS = 2;
					calcWA = 2;
					calcPQ = 2;
					calcDH = 2;
					calcCH = 2;
					break;
				}
				else if (!(VerifyNegative (sNumber)))
				{
					cout << "Invalid value" << endl;
					calcDS = 2;
					calcWA = 2;
					calcPQ = 2;
					calcDH = 2;
					calcCH = 2;
					break;
				}
				else
				{
					kValue = atoi (sNumber.c_str());
				}

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "ErlangGen. Lambda = " << lambda << " k Value = " << kValue << endl << endl;
				}

				erlang = new ErlangGen (seed, lambda, kValue);
				if (erlang != NULL)
				{
					cout << "\nErlang Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = erlang->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete erlang;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Gamma:
			try
			{
				cout << "\nNew GammaGen" << endl;
				cout << "What is the alfa value? " << endl;
				cin >> a;
				cout << "What is the beta value? " << endl;
				cin >> b;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "GammaGen. Alfa = " << a << " Beta = " << b << endl << endl;
				}

				gamma = new GammaGen (seed, a, b);
				if (gamma != NULL)
				{
					cout << "\nGamma Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = gamma->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete gamma;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Geometric:
			try
			{
			 	calcGeometric(false);
				reportResults();

				for (;;)
				{
					cout << "\nDo you want redefine the initial parameters and execute another run ?(1=Yes; 2=No)";
					cout << endl;
					cin >> userResp;
					if (userResp != 1) break;
					calcGeometric(true);
					reportResults();
				}
			 	delete geometric;
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case LogNormal:
			try
			{
				cout << "\nNew LogNormalGen" << endl;
				cout << "What is the mean value for the Normal distribution associated? " << endl;
				cin  >> mean;
				cout << "What is the standard deviation value for the Normal distribution associated?" << endl;
				cin  >> fStdDev;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "LogNormalGen. Mean = " << mean << " StdDev = " << fStdDev << endl << endl;
				}

				logNormal = new LogNormalGen (seed, mean, fStdDev);
				if (logNormal != NULL)
				{
					cout << "\nLogNormal Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = logNormal->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete logNormal;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case NegExp:
			try
			{
				cout << "\nNew NegExpGen" << endl;
				cout << "What is the lambda value? " << endl;
				cin >> lambda;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "NegExpGen. Lambda = " << lambda << endl << endl;
				}

				negExp = new NegExpGen (seed, lambda);
				if (negExp != NULL)
				{
					cout << "\nNegExp Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = negExp->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete negExp;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Normal:
			try
			{
				cout << "\nNew NormalGen" << endl;
				cout << "What is the mean value?" << endl;
				cin  >> mean;
				cout << "What is the standard deviation value?" << endl;				
				cin  >> fStdDev;
				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "NormalGen. Mean = " << a << " StdDev = " << fStdDev << endl << endl;
				}
				normal = new NormalGen (seed, mean, fStdDev);
				if (normal != NULL)
				{
					cout << "\nNormal Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = normal->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete normal;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Pareto:
			try
			{
				cout << "\nNew ParetoGen" << endl;
				cout << "What is the alfa value? " << endl;
				cin  >> alfa;
				cout << "What is the k value? " << endl;
				cin  >> kParetoValue; 

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "ParetoGen. Alfa = " << alfa << " k Value = " << kParetoValue << endl << endl;
				}

				pareto = new ParetoGen (seed, alfa, kParetoValue);
				if (pareto != NULL)
				{
					cout << "\nPareto Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = pareto->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete pareto;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Poisson:
			try
			{
 			 	calcPoisson(false);
				reportResults();

				for (;;)
				{
					cout << "\nDo you want redefine the initial parameters and execute another run ?(1=Yes; 2=No)";
					cout << endl;
					cin >> userResp;
					if (userResp != 1) break;
					calcPoisson(true);
					reportResults();

				}

				delete poisson;
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case RandInt:
			try
			{
 			 	calcRandInt(false);

				reportResults();

				for (;;)
				{
					cout << "\nDo you want redefine the initial parameters and execute another run ?(1=Yes; 2=No)";
					cout << endl;
					cin >> userResp;
					if (userResp != 1) break;
					calcRandInt(true);

					reportResults();
				}
			 	delete randInt;
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Rayleigh:
			try
			{
				cout << "\nNew RayleighGen" << endl;
				cout << "What is the lambda value? " << endl;
				cin >> lambda;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "RayleighGen. Lambda = " << lambda << endl << endl;
				}

				rayleigh = new RayleighGen (seed, lambda);
				if (rayleigh != NULL)
				{
					cout << "\nRayleigh Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = rayleigh->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete rayleigh;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case TruncatedCauchy:
			try
			{
				cout << "\nNew TruncatedCauchyGen" << endl;
				cout << "What is the a value? " << endl;
				cin >> a;
				cout << "What is the b value? " << endl;				
				cin >> b;
				cout << "What is the truncated minimum value? " << endl;
				cin >> fXMin;
				cout << "What is the truncated maximum value? " << endl;				
				cin >> fXMax;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "TruncatedCauchyGen. a = " << a << " b = " << b << " XMin = " << fXMin << " XMax = " << fXMax << endl << endl;
				}

				truncCauchy = new TruncatedCauchyGen (seed, a, b, fXMin, fXMax);
				if (truncCauchy != NULL)
				{
					cout << "\nTruncatedCauchy Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = truncCauchy->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete truncCauchy;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case TruncatedGeometric:
			try
			{
			 	calcTruncatedGeometric(false);
				reportResults();

				for (;;)
				{
					cout << "\nDo you want redefine the initial parameters and execute another run ?(1=Yes; 2=No)";
					cout << endl;
					cin >> userResp;
					if (userResp != 1) break;
					calcTruncatedGeometric(true);
					reportResults();
				}
			 	delete truncatedGeometric;
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;

		
		case TruncatedLogNormal:
			try
			{
				cout << "\nNew TruncatedLogNormalGen" << endl;
				cout << "What is the mean value? " << endl;
				cin >> mean;
				cout << "What is the standard deviation ? " << endl;				
				cin >> fStdDev;
				cout << "What is the truncated minimum value? " << endl;
				cin >> fXMin;
				cout << "What is the truncated maximum value? " << endl;				
				cin >> fXMax;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "TruncatedLogNormalGen. Mean = " << mean << " Standard Deviation = " << fStdDev << " XMin = " << fXMin << " XMax = " << fXMax << endl << endl;
				}

				truncLogNormal = new TruncatedLogNormalGen (seed, mean, fStdDev, fXMin, fXMax);
				if (truncPareto != NULL)
				{
					cout << "\nTruncatedLogNormal Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = truncLogNormal->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete truncPareto;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;


		case TruncatedPareto:
			try
			{
				pf->precision(10);	
				cout << "\nNew TruncatedPareto" << endl;
				cout << "What is the alfa value? " << endl;
				cin >> alfa;
				cout << "What is the k Value? " << endl;				
				cin >> kParetoValue;
				cout << "What is the truncated minimum value? " << endl;
				cin >> fXMin;
				cout << "What is the truncated maximum value? " << endl;				
				cin >> fXMax;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "TruncatedPareto. Alfa = " << alfa << " k Value = " << kParetoValue << " XMin = " << fXMin << " XMax = " << fXMax << endl << endl;
				}

				truncPareto = new TruncatedParetoGen (seed, alfa, kParetoValue, fXMin, fXMax);
				if (truncPareto != NULL)
				{
					cout << "\nTruncatedPareto Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = truncPareto->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete truncPareto;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		
		
		
		
		
		case TruncatedWeibull:
			try
			{
				cout << "\nNew WeibullGen" << endl;
				cout << "What is the lambda value?" << endl;
				cin >> lambda;
				cout << "What is the b value?" << endl;
				cin >> bValue;
				cout << "What is the truncated minimum value? " << endl;
				cin >> fXMin;
				cout << "What is the truncated maximum value? " << endl;				
				cin >> fXMax;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "TruncatedWeibullGen. Lambda = " << lambda << " b Value = " << bValue << " XMin = " << fXMin << " XMax = " << fXMax << endl << endl;
				}

				truncWeibull = new TruncatedWeibullGen (seed, lambda, bValue, fXMin, fXMax);
				if (truncWeibull != NULL)
				{
					cout << "\nTruncated Weibull Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = truncWeibull->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete truncWeibull;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		
		
		
		
		
		
		
		
		case Uniform:
			try
			{
				cout << "\nNew UniformGen" << endl;
				cout << "What is the minimum value?" << endl;
				cin >> fMinVal;
				cout << "What is the maximum value?" << endl;
				cin >> fMaxVal;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "UniformGen. MinVal = " << fMinVal << " MaxVal = " << fMaxVal << endl << endl;
				}

				uniform = new UniformGen (seed, fMinVal, fMaxVal);
				if (uniform != NULL)
				{
					cout << "\nUniform Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = uniform->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " <<  num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete uniform;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		case Weibull:
			try
			{
				cout << "\nNew WeibullGen" << endl;
				cout << "What is the lambda value?" << endl;
				cin >> lambda;
				cout << "What is the b value?" << endl;
				cin >> bValue;

				if ((output == 1) && ((whatSee == 1) || (whatSee == 3)))
				{
					*pf << "WeibullGen. Lambda = " << lambda << " b Value = " << bValue << endl << endl;
				}

				weibull = new WeibullGen (seed, lambda, bValue);
				if (weibull != NULL)
				{
					cout << "\nWeibull Distribution";
					for (i=0; i<seqLength; i++)
					{
						rc = weibull->nextValue(&num);
						if (rc == SCH_SUCCESS)
						{
							if ((output == 2) && ((whatSee == 1) || (whatSee == 2)))
							{
								cout << "\nnum = " << num;
							}
							if (calcDS == 1)
							{
								pDS->nextSample (num);
							}
							if (calcWA == 1)
							{
								pWA->nextSample (num);
							}
							if (calcDH == 1)
							{
								pDH->nextSample (num);
							}
							if (calcPQ == 1)
							{
								pPQ->nextSample (num);
							}
							if (calcCH == 1)
							{
								pCH->nextSample (num);
							}
							if (calcAH == 1)
							{
								pAH->nextSample (num);
							}
							if ((output == 1) && ((whatSee == 1) || (whatSee == 2)))
							{
								if (separator == 1) // blank separator
								{
									*pf << num << " ";
								}
								else	// CR separator
								{
									*pf << num << endl;
								}
							}
						}
						else
						{
							break;
						}
					}

					reportResults();
					delete weibull;
				}
			}
			catch (SchException &e)
			{
				cout << "\nERROR = " << e.getError() << "\t";
				cerr << e.what();
				success = false;
			}
			break;
		default:
			break;
	}			// end switch


	// deletes all pointers
	if (pDS != NULL)
	{
		delete pDS;
	}
	if (pDH != NULL)
	{
		delete pDH;
	}
	if (pWA != NULL)
	{
		delete pWA;
	}
	if (pPQ != NULL)
	{
		delete pPQ;
	}
	if (pCH != NULL)
	{
		delete pCH;
	}
	if (pAH != NULL)
	{
		delete pAH;
	}
	
	if (pf != NULL) 
	{
		pf->close();
		delete pf;
	}



 #if defined(_DEBUG) && defined(WIN32) && defined (TEST_MEM_LEAK)
	// get memory at end of program for debugging purpose only (under Windows NT)
	_CrtMemCheckpoint(&endState);

	// dump information for memory leak check
	_CrtMemDifference(&diffState, &beginState, &endState);
	_CrtMemDumpStatistics(&diffState);
	_CrtMemDumpAllObjectsSince(&beginState);

	// close report file
	CloseHandle(outfile);
#endif





	exit (0);
}
