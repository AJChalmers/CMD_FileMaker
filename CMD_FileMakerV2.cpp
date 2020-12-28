/////Program designed to use output from InstabilityStrip_V12.cpp along with histroy files to get CMD data. 

//V1: Have program read InstabilityStrip_V12.cpp output for positive growth, then use model numbers in there to get history file. Then fill arrays with history file columns. Output Mv, Mv-Mi for use in python program. 
	//Working program when NumOfSets is set to 1. Only does one set at a time.

//V2: Add ability to do all 4 sets at once. Working program!!

# include <iostream>
# include <fstream>
# include <math.h>
# include <iomanip>
# include <cmath>
# include <stdlib.h>
# include <cstdlib>
//# include <fstream.h>
# include <string.h>
# include <string>
//# include <dos.h> //For Sleep() 


using namespace std;
int main() {
	
	//Controls//

	int NumOfSets = 4; //Number of sets, usually 4 for A, B, C, D. Max is 4 sets currently
	int NumOfModels = 20412; //Total possible number of models. Will be used for largest array possible
	
	char log_directory_prefix_SetA[50] = "LINA_reruns/SetA/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where history file should be
	char log_directory_prefix_SetB[50] = "LINA_reruns/SetB/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where history file should be
	char log_directory_prefix_SetC[50] = "LINA_reruns/SetC/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where history file should be
	char log_directory_prefix_SetD[50] = "LINA_reruns/SetD/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where history file should be
	
	char HistoryFileName[50] = "history.data"; //Normally should be "history.data"
	char inputFileName_ForPositiveGrowthModels[50] = "PostiveGrowth"; //input File with model parameters that have positve growth rates. Will have prefix for mode that is defined below. Must be in working directory. Suffix is "_Set.dat"
	char outputFileName[50] = "CMDdata"; //output file with CMD data in it. Will have prefix for mode defined below as well as the set suffix.
	
	char FU_FilePrefix[10] = "FU_"; //Prefix to input/output files that contain data for FU mode
	char FO_FilePrefix[10] = "FO_"; //Prefix to input/output files that contain data for FO mode
	char SO_FilePrefix[10] = "2O_"; //Prefix to input/output files that contain data for 2O mode
	char ThirdO_FilePrefix[10] = "ThirdO_"; //Prefix to input/output files that contain data for ThirdO mode
	char ForthO_FilePrefix[10] = "ForthO_"; //Prefix to input/output files that contain data for ForthO mode
	char FifthO_FilePrefix[10] = "FifthO_"; //Prefix to input/output files that contain data for FifthO mode
	char SixthO_FilePrefix[10] = "SixthO_"; //Prefix to input/output files that contain data for SixthO mode
	char SeventhO_FilePrefix[10] = "SeventhO_"; //Prefix to input/output files that contain data for SeventhO mode
	char EighthO_FilePrefix[10] = "EighthO_"; //Prefix to input/output files that contain data for EighthO mode
	char NinethO_FilePrefix[10] = "NinethO_"; //Prefix to input/output files that contain data for NinethO mode
	
	char SetA_Suffix[10] = "_SetA.dat"; //Suffix to input/output file for SetA
	char SetB_Suffix[10] = "_SetB.dat"; //Suffix to input/output file for SetB
	char SetC_Suffix[10] = "_SetC.dat"; //Suffix to input/output file for SetC
	char SetD_Suffix[10] = "_SetD.dat"; //Suffix to input/output file for SetD
	
	///////////////////////////////////////
	
	//Variables//
	
	const int nArray = NumOfModels + 1;
	
	char FU_FileName[100];
	char FO_FileName[100];
	char SO_FileName[100];
	char ThirdO_FileName[100];
	char ForthO_FileName[100];
	char FifthO_FileName[100];
	char SixthO_FileName[100];
	char SeventhO_FileName[100];
	char EighthO_FileName[100];
	char NinethO_FileName[100];
	
	char FU_HistoryFilePath[100];
	char FO_HistoryFilePath[100];
	char SO_HistoryFilePath[100];
	char ThirdO_HistoryFilePath[100];
	char ForthO_HistoryFilePath[100];
	char FifthO_HistoryFilePath[100];
	char SixthO_HistoryFilePath[100];
	char SeventhO_HistoryFilePath[100];
	char EighthO_HistoryFilePath[100];
	char NinethO_HistoryFilePath[100];
	
	char FU_FullOutputFileName[100];
	char FO_FullOutputFileName[100];
	char SO_FullOutputFileName[100];
	char ThirdO_FullOutputFileName[100];
	char ForthO_FullOutputFileName[100];
	char FifthO_FullOutputFileName[100];
	char SixthO_FullOutputFileName[100];
	char SeventhO_FullOutputFileName[100];
	char EighthO_FullOutputFileName[100];
	char NinethO_FullOutputFileName[100];
	
	double Model[nArray];
	double z[nArray];
	double x[nArray];
	double M[nArray];
	double L[nArray];
	double T[nArray];
	double Lin_Period[nArray];
	double Growth_rate[nArray];
	double logT[nArray];
	double logP[nArray];
	double logM[nArray];
	
	double abs_mag_V[nArray];
	double abs_mag_I[nArray];
	
	int NumOfLines_FU_SetA = 0;
	int NumOfLines_FU_SetB = 0;
	int NumOfLines_FU_SetC = 0;
	int NumOfLines_FU_SetD = 0;
	
	string readout;
	string input_header;
	string Model_string;
	
	string history_header1;
	string history_header2;
	string history_header3;
	string history_header4;
	string history_header5;
	string history_header6;
	
	ifstream inFileFU;
	ifstream inFileFO;
	ifstream inFileSO;
	ifstream inFileThirdO;
	ifstream inFileForthO;
	ifstream inFileFifthO;
	ifstream inFileSixthO;
	ifstream inFileSeventhO;
	ifstream inFileEighthO;
	ifstream inFileNinethO;
	
	ifstream inFileHistoryFU;
	ifstream inFileHistoryFO;
	ifstream inFileHistorySO;
	ifstream inFileHistoryThirdO;
	ifstream inFileHistoryForthO;
	ifstream inFileHistoryFifthO;
	ifstream inFileHistorySixthO;
	ifstream inFileHistorySeventhO;
	ifstream inFileHistoryEighthO;
	ifstream inFileHistoryNinethO;
	
	ofstream outFileCMD_FU;
	ofstream outFileCMD_FO;
	ofstream outFileCMD_SO;
	ofstream outFileCMD_ThirdO;
	ofstream outFileCMD_ForthO;
	ofstream outFileCMD_FifthO;
	ofstream outFileCMD_SixthO;
	ofstream outFileCMD_SenventhO;
	ofstream outFileCMD_EighthO;
	ofstream outFileCMD_NinethO;
	
	double dummy1[nArray];
	double dummy2[nArray];
	double dummy3[nArray];
	double dummy4[nArray];
	double dummy5[nArray];
	double dummy6[nArray];
	double dummy7[nArray];
	double dummy8[nArray];
	double dummy9[nArray];
	double dummy10[nArray];
	double dummy11[nArray];
	double dummy12[nArray];
	double dummy13[nArray];
	double dummy14[nArray];
	double dummy15[nArray];
	double dummy16[nArray];
	double dummy17[nArray];
	double dummy18[nArray];
	double dummy19[nArray];
	double dummy20[nArray];
	double dummy21[nArray];
	double dummy22[nArray];
	double dummy23[nArray];
	double dummy24[nArray];
	double dummy25[nArray];
	double dummy26[nArray];
	double dummy27[nArray];
	double dummy28[nArray];
	double dummy29[nArray];
	double dummy30[nArray];
	double dummy31[nArray];
	double dummy32[nArray];
	double dummy33[nArray];
	double dummy34[nArray];
	double dummy35[nArray];
	double dummy36[nArray];
	double dummy37[nArray];
	double dummy38[nArray];
	double dummy39[nArray];
	double dummy40[nArray];
	
	
	//////////////////////////////////////
	
	
	//Main loop for FU mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(FU_FileName, FU_FilePrefix);
			strcat(FU_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FU_FileName, SetA_Suffix);
			
			inFileFU.open(FU_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFU, readout)){
				NumOfLines_FU_SetA = NumOfLines_FU_SetA + 1;
			}
			
			cout<<"Number of Lines in FU SetA file: "<<NumOfLines_FU_SetA<<endl;
			
			inFileFU.close();
			
			inFileFU.open(FU_FileName, ios::in);
			getline(inFileFU, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FU_SetA; j++){
				
				inFileFU>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FU_HistoryFilePath, log_directory_prefix_SetA);
				strcat(FU_HistoryFilePath, Model_char);
				strcat(FU_HistoryFilePath, "/");
				strcat(FU_HistoryFilePath, HistoryFileName);
				cout<<"FU History file path: "<<FU_HistoryFilePath<<endl;
				
				inFileHistoryFU.open(FU_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFU, history_header1);
				getline(inFileHistoryFU, history_header2);
				getline(inFileHistoryFU, history_header3);
				getline(inFileHistoryFU, history_header4);
				getline(inFileHistoryFU, history_header5);
				getline(inFileHistoryFU, history_header6);
			
				inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
				//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  

				inFileHistoryFU.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFU.close();
			
			//Make outputfile name 
			strcpy(FU_FullOutputFileName, FU_FilePrefix);
			strcat(FU_FullOutputFileName, outputFileName);
			strcat(FU_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_FU.open(FU_FullOutputFileName, ios::out);
			outFileCMD_FU<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FU_SetA; j++){
			
				outFileCMD_FU<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FU.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(FU_FileName, FU_FilePrefix);
			strcat(FU_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FU_FileName, SetB_Suffix);
			
			inFileFU.open(FU_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFU, readout)){
				NumOfLines_FU_SetB = NumOfLines_FU_SetB + 1;
			}
			
			cout<<"Number of Lines in FU SetB file: "<<NumOfLines_FU_SetB<<endl;
			
			inFileFU.close();
			
			inFileFU.open(FU_FileName, ios::in);
			getline(inFileFU, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FU_SetB; j++){
				
				inFileFU>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FU_HistoryFilePath, log_directory_prefix_SetB);
				strcat(FU_HistoryFilePath, Model_char);
				strcat(FU_HistoryFilePath, "/");
				strcat(FU_HistoryFilePath, HistoryFileName);
				cout<<"FU History file path: "<<FU_HistoryFilePath<<endl;
				
				inFileHistoryFU.open(FU_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFU, history_header1);
				getline(inFileHistoryFU, history_header2);
				getline(inFileHistoryFU, history_header3);
				getline(inFileHistoryFU, history_header4);
				getline(inFileHistoryFU, history_header5);
				getline(inFileHistoryFU, history_header6);
			
				inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
				//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  

				inFileHistoryFU.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFU.close();
			
			//Make outputfile name 
			strcpy(FU_FullOutputFileName, FU_FilePrefix);
			strcat(FU_FullOutputFileName, outputFileName);
			strcat(FU_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_FU.open(FU_FullOutputFileName, ios::out);
			outFileCMD_FU<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FU_SetB; j++){
			
				outFileCMD_FU<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FU.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(FU_FileName, FU_FilePrefix);
			strcat(FU_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FU_FileName, SetC_Suffix);
			
			inFileFU.open(FU_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFU, readout)){
				NumOfLines_FU_SetC = NumOfLines_FU_SetC + 1;
			}
			
			cout<<"Number of Lines in FU SetC file: "<<NumOfLines_FU_SetC<<endl;
			
			inFileFU.close();
			
			inFileFU.open(FU_FileName, ios::in);
			getline(inFileFU, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FU_SetC; j++){
				
				inFileFU>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FU_HistoryFilePath, log_directory_prefix_SetC);
				strcat(FU_HistoryFilePath, Model_char);
				strcat(FU_HistoryFilePath, "/");
				strcat(FU_HistoryFilePath, HistoryFileName);
				cout<<"FU History file path: "<<FU_HistoryFilePath<<endl;
				
				inFileHistoryFU.open(FU_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFU, history_header1);
				getline(inFileHistoryFU, history_header2);
				getline(inFileHistoryFU, history_header3);
				getline(inFileHistoryFU, history_header4);
				getline(inFileHistoryFU, history_header5);
				getline(inFileHistoryFU, history_header6);
			
				inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
				//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  

				inFileHistoryFU.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFU.close();
			
			//Make outputfile name 
			strcpy(FU_FullOutputFileName, FU_FilePrefix);
			strcat(FU_FullOutputFileName, outputFileName);
			strcat(FU_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_FU.open(FU_FullOutputFileName, ios::out);
			outFileCMD_FU<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FU_SetC; j++){
			
				outFileCMD_FU<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FU.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(FU_FileName, FU_FilePrefix);
			strcat(FU_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FU_FileName, SetD_Suffix);
			
			inFileFU.open(FU_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFU, readout)){
				NumOfLines_FU_SetD = NumOfLines_FU_SetD + 1;
			}
			
			cout<<"Number of Lines in FU SetD file: "<<NumOfLines_FU_SetD<<endl;
			
			inFileFU.close();
			
			inFileFU.open(FU_FileName, ios::in);
			getline(inFileFU, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FU_SetD; j++){
				
				inFileFU>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FU_HistoryFilePath, log_directory_prefix_SetD);
				strcat(FU_HistoryFilePath, Model_char);
				strcat(FU_HistoryFilePath, "/");
				strcat(FU_HistoryFilePath, HistoryFileName);
				cout<<"FU History file path: "<<FU_HistoryFilePath<<endl;
				
				inFileHistoryFU.open(FU_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFU, history_header1);
				getline(inFileHistoryFU, history_header2);
				getline(inFileHistoryFU, history_header3);
				getline(inFileHistoryFU, history_header4);
				getline(inFileHistoryFU, history_header5);
				getline(inFileHistoryFU, history_header6);
			
				inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
				//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  

				inFileHistoryFU.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFU.close();
			
			//Make outputfile name 
			strcpy(FU_FullOutputFileName, FU_FilePrefix);
			strcat(FU_FullOutputFileName, outputFileName);
			strcat(FU_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_FU.open(FU_FullOutputFileName, ios::out);
			outFileCMD_FU<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FU_SetD; j++){
			
				outFileCMD_FU<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FU.close();
		
		}
		
	}
	
	//////////////////////////////////////////////////
	///This is where other modes will/can be added////
	//////////////////////////////////////////////////
	
	
	
	return 0;
}