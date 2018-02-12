//includes
#include<stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include<time.h>
#include<bits/stdc++.h>

using namespace std;


//global variables
ofstream f;


double gcContent;
int gcCount;
int seqLen;

string bases[] = {"A", "C", "G", "T"};

string sequence;
string rcSequence; //reverse complement sequence


int lines = 0;

int l = 0;
//functions

//ask the user for input
void getInput()
{
	string temp;
	string temp1;

	cout << "What is your GC content? " <<endl;
	cin >> temp;
	gcContent = stod(temp);

	cout << "What is your sequence length? " <<endl;
	cin >> temp1;
	seqLen = stoi(temp1);

	gcCount = gcContent * seqLen /100;
}

double getGCcount(string sequence)
{
	int gcNum;
	double gcCont;

	for (int i = 0; i < sequence.length(); i++)
	{
		if (sequence.at(i) == 'C' || sequence.at(i) == 'G')
		{
			gcNum ++;
		}
	}
	gcCont = (gcNum * 100)/sequence.length();
	return gcNum;
}

	// generate the random sequence for the speicified GC content


string createSeq()
{
	srand(time(0));
	string seq;
	for (int i = 0; i < seqLen; i++)
	{
		seq += bases[rand()%4];
	}

	int gcNum = getGCcount(seq);
	if (gcNum > gcCount)
	{
//		cout << "it was more.." <<endl;

		//change sequence
		int temp = gcNum - gcCount;
		vector<int> gcPos; //put all the gc positions here and pick a random place in the for loop and change them??!!
		for (int x = 0; x < seq.length(); x++)
		{
			if (seq.at(x) == 'G' || seq.at(x) == 'C')
			{
				gcPos.push_back(x);
			}
		}

		for (int j = 0; j < temp; j++)
		{
			int pos = -1;
			do
			{
				pos = rand()%temp; //what if it got the same positions again??
			}while ((seq[gcPos[pos]] != 'C') && (seq[gcPos[pos]] != 'G'));
			if (pos%2 == 0)
			{
				seq[gcPos[pos]] = 'A';
			}
			else if(pos%2 == 1)
			{
				seq[gcPos[pos]] = 'T';
			}
			//change random CG

		}
	}
	else if (gcNum < gcCount)
	{
//		cout << "it was less.." <<endl;

		//change sequence
		int temp = gcCount - gcNum;
		vector<int> atPos; //put all the gc positions here and pick a random place in the for loop and change them??!!
		for (int x = 0; x < seq.length(); x++)
		{
			if (seq.at(x) == 'A' || seq.at(x) == 'T')
			{
				atPos.push_back(x);
			}
		}

		for (int j = 0; j < temp; j++)
		{
			int pos = -1;
			do
			{
				pos = rand()%temp; //what if it got the same positions again??
			}while ((seq[atPos[pos]] != 'A') && (seq[atPos[pos]] != 'T'));

			if (pos%2 == 0)
			{
				seq[atPos[pos]] = 'C';
			}
			else if (pos%2 == 1)
			{
				seq[atPos[pos]] = 'G';
			}
		}
	}

	return seq;
}


void getReverseComplement(string seq)
{
	for (int x = 0; x < seq.length(); x++)
	{
		if (seq.at(x) == 'A')
		{
			rcSequence += "T";
		}
		else if (seq.at(x) == 'T')
		{
			rcSequence += "A";
		}
		else if (seq.at(x) == 'C')
		{
			rcSequence += "G";
		}
		else if (seq.at(x) == 'G')
		{
			rcSequence += "C";
		}
	}
	reverse(rcSequence.begin(), rcSequence.end());
}

void findORF(string seq)
{
	vector<int> starts;
	vector<int> stops;

	//creating windows
	vector<string> windows;
	vector <int> startWind;

	int x = seq.length() - 2 ;
	for ( int i = 0; i < x; i+=3)
	{
		windows.push_back(seq.substr(i, 3));
	}

	//iterate over vector elements and look for a start codon, if found, look for a stop codon and then report it

	int start = -1; //place of the start codon

	int codonPosStart = 0; //to know the position of each codon in the sequence //1-indexed

	//looking for ORF...


	//getting all the start codon..
	for (int f = 0; f < windows.size(); f++)
	{
		if (windows[f] == "ATG")
		{
			startWind.push_back(f);
			starts.push_back(codonPosStart +l);
		}
		else if (windows[f] == "GTG")
		{
			startWind.push_back(f);
			starts.push_back(codonPosStart + l);
		}
		else if (windows[f] == "TTG")
		{
			startWind.push_back(f);
			starts.push_back(codonPosStart + l);
		}
		codonPosStart+=3;
	}


	for (int o = 0; o < starts.size(); o++)
	{
		int stop = -1; //place of the stop codon
		int codonPosStop = 0;

		for (int p = startWind[o]; p < windows.size(); p++)
		{
			if (stop < 0)
			{
				if (windows[p] == "TAA")
				{
					stop = codonPosStop + starts[o];
					if (stop < seq.length()){stops.push_back(stop);}
				}
				else if (windows[p] == "TAG")
				{
					stop = codonPosStop + starts[o];
					if (stop < seq.length()){stops.push_back(stop);}
				}
				else if (windows[p] == "TGA")
				{
					stop = codonPosStop + starts[o];
					if (stop < seq.length()){stops.push_back(stop);}
				}
			}
			codonPosStop +=3 ;
		}
	}

	if (stops.size() > 0 && starts.size() > 0)
	{
		for (int h = 0; h < stops.size(); h++)
		{
			if (stops[h] > 0 && stops[h] < seq.length())
			{
				cout << "Start: " << starts[h]-1 << "\t" << " Stop: " << stops[h]-1 <<endl;
				f << "Start: " << starts[h]-1 << "\t" << " Stop: " << stops[h] - 1 <<"\n";
				lines ++;
			}
		}
	}
}

void findORF_rc(string seq)
{
	vector<int> starts;
	vector<int> stops;

	//creating windows
	vector<string> windows;
	vector <int> startWind;

	int x = seq.length() - 2 ;
	for ( int i = 0; i < x; i+=3)
	{
		windows.push_back(seq.substr(i, 3));
	}

	//iterate over vector elements and look for a start codon, if found, look for a stop codon and then report it

	int start = -1; //place of the start codon

	int codonPosStart = 0; //to know the position of each codon in the sequence //1-indexed

	//looking for ORF...


	//getting all the start codon..
	for (int f = 0; f < windows.size(); f++)
	{
		if (windows[f] == "ATG")
		{
			startWind.push_back(f);
			starts.push_back(codonPosStart);
		}
		else if (windows[f] == "GTG")
		{
			startWind.push_back(f);
			starts.push_back(codonPosStart);
		}
		else if (windows[f] == "TTG")
		{
			startWind.push_back(f);
			starts.push_back(codonPosStart);
		}
		codonPosStart+=3;
	}


	for (int o = 0; o < starts.size(); o++)
	{
		int stop = -1; //place of the stop codon
		int codonPosStop = 0;

		for (int p = startWind[o]; p < windows.size(); p++)
		{
			if (stop < 0)
			{
				if (windows[p] == "TAA")
				{
					stop = codonPosStop + starts[o];
					if (stop < seq.length()){stops.push_back(stop);}
				}
				else if (windows[p] == "TAG")
				{
					stop = codonPosStop + starts[o];
					if (stop < seq.length()){stops.push_back(stop);}
				}
				else if (windows[p] == "TGA")
				{
					stop = codonPosStop + starts[o];
					if (stop < seq.length()){stops.push_back(stop);}
				}
			}
			codonPosStop +=3 ;
		}
	}

	for (int k = 0; k < stops.size(); k++)
	{
		starts[k] = seq.length() - starts[k] + 1;
		stops[k] = seq.length() - stops[k] +1;
	}

	if (stops.size() > 0 && starts.size() > 0)
	{
		for (int h = 0; h < stops.size() ; h++)
		{
			if (stops[h] > 1 && stops[h] < seq.length())
			{
				cout << "Start: " << starts[h]-1 << "\t" << " Stop: " << stops[h]-1 <<endl;
				f << "Start: " << starts[h]-1 << "\t" << " Stop: " << stops[h]-1 <<"\n";
				lines ++;
			}
		}
	}
}

//TODO: print the sequence to a file
//print the ORFs to the same file as well.



//main function
int main()
{


	getInput();
	sequence = createSeq();

	f.open("output.txt");
	f << "The generated random sequence: ";
	f << sequence <<"\n";


//	cout << sequence << endl;
    	getReverseComplement(sequence);
//	cout << rcSequence << endl;

	cout << "The Open Reading frames for one strand...\n" << endl;
	f << "The ORFs for the sequence are: " << "\n";
	l = 0;
	findORF(sequence);
	l = 1;
	findORF(sequence.substr(l));
	l = 2;
	findORF(sequence.substr(l));

	cout << "\n" << endl;

	cout << "The Open Reading Frames for the opposite strand... \n" << endl;
	l = 0;
	findORF_rc(rcSequence);
	l = 1;
	findORF_rc(rcSequence.substr(l));
	l = 2;
	findORF_rc(rcSequence.substr(l));

	cout << "\n The number of ORFs found:" << lines << "\n" << endl;
//	cout << getGCcontent("AACCGGTT") <<endl;


}
