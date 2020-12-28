/////Program designed to use output from InstabilityStrip_V12.cpp along with histroy files to get CMD data. 

//V1: Have program read InstabilityStrip_V12.cpp output for positive growth, then use model numbers in there to get history file. Then fill arrays with history file columns. Output Mv, Mv-Mi for use in python program. 
	//Working program when NumOfSets is set to 1. Only does one set at a time.

//V2: Add ability to do all 4 sets at once. Working program!!

//V3: Adding ablity to make the CMDs as well. Place this program along with python program and output from InstabilityStrip_V12.cpp in the same directory, one in which has a direct line to the LOGS (need history file)
	//Note: Working program! Make sure to edit the python program for any changes in set or mode!
	
//V4: Make program able to do all 10 modes

//V5: Adding more dummy variables and inFile lines in order to use on classical cep models (differently ordered history file)

//V6: Fixing magnetude problem, trying factors of Lsun (-2.5log(Lsun))to produce practical mags. Also added bool for clasical/typeII history file format

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
	int NumOfModels = 16281; //Total possible number of models. Will be used for largest array possible. Classical: 16281
	int NumOfModes = 10; //Number of modes investigated. Max is 10. Must have the positive growth files named accordingly with prefix and suffix below!
	
	char log_directory_prefix_SetA[50] = "LINA_classical/LOGS_A/LOGS_A/LOGS_A"; //Prefix to log_directory, suffix is model number. This is where history file should be
	char log_directory_prefix_SetB[50] = "LINA_classical/LOGS_B/LOGS_B/LOGS_B"; //Prefix to log_directory, suffix is model number. This is where history file should be
	char log_directory_prefix_SetC[50] = "LINA_classical/LOGS_C/LOGS_C/LOGS_C"; //Prefix to log_directory, suffix is model number. This is where history file should be
	char log_directory_prefix_SetD[50] = "LINA_classical/LOGS_D/LOGS_D/LOGS_D"; //Prefix to log_directory, suffix is model number. This is where history file should be
	
	char HistoryFileName[50] = "history.data"; //Normally should be "history.data"
	char inputFileName_ForPositiveGrowthModels[50] = "PostiveGrowthClassical"; //input File with model parameters that have positve growth rates. Will have prefix/suffix for mode that is defined below. Must be in working directory.
	char outputFileName[50] = "CMDdataClassicalREMAKE"; //output file with CMD data in it. Will have prefix for mode defined below as well as the set suffix.
	
	char FU_FilePrefix[10] = "FU_"; //Prefix to input/output files that contain data for FU mode
	char FO_FilePrefix[10] = "FO_"; //Prefix to input/output files that contain data for FO mode
	char SO_FilePrefix[10] = "2O_"; //Prefix to input/output files that contain data for 2O mode
	char ThirdO_FilePrefix[10] = "ThirdO_"; //Prefix to input/output files that contain data for ThirdO mode
	char ForthO_FilePrefix[10] = "ForthO_"; //Prefix to input/output files that contain data for ForthO mode
	char FifthO_FilePrefix[10] = "FifthO_"; //Prefix to input/output files that contain data for FifthO mode
	char SixthO_FilePrefix[10] = "SixthO_"; //Prefix to input/output files that contain data for SixthO mode
	char SeventhO_FilePrefix[10] = "SeventhO_"; //Prefix to input/output files that contain data for SeventhO mode
	char EighthO_FilePrefix[10] = "EighthO_"; //Prefix to input/output files that contain data for EighthO mode
	char NinethO_FilePrefix[10] = "NinthO_"; //Prefix to input/output files that contain data for NinethO mode
	
	char SetA_Suffix[10] = "_SetA.dat"; //Suffix to input/output file for SetA
	char SetB_Suffix[10] = "_SetB.dat"; //Suffix to input/output file for SetB
	char SetC_Suffix[10] = "_SetC.dat"; //Suffix to input/output file for SetC
	char SetD_Suffix[10] = "_SetD.dat"; //Suffix to input/output file for SetD
	
	bool MakeCMDsWithPython = false; //If true, will try to run CMD_plotter.py in order to make CMDs. Python program may need to be edited before running.
	bool ClassicalFormat = true; //If true, will use file formatting that Nick had in his classical models. False will use mine from the type II models.
	bool MakeMagCorrection = true; //If true, will account for abs mag bug in version 11701 - factor of Lsun problem. If false will use values from history file.
	
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
	double Lsun = 3.839e26;
	
	int NumOfLines_FU_SetA = 0;
	int NumOfLines_FU_SetB = 0;
	int NumOfLines_FU_SetC = 0;
	int NumOfLines_FU_SetD = 0;
	
	int NumOfLines_FO_SetA = 0;
	int NumOfLines_FO_SetB = 0;
	int NumOfLines_FO_SetC = 0;
	int NumOfLines_FO_SetD = 0;
	
	int NumOfLines_SO_SetA = 0;
	int NumOfLines_SO_SetB = 0;
	int NumOfLines_SO_SetC = 0;
	int NumOfLines_SO_SetD = 0;
	
	int NumOfLines_ThirdO_SetA = 0;
	int NumOfLines_ThirdO_SetB = 0;
	int NumOfLines_ThirdO_SetC = 0;
	int NumOfLines_ThirdO_SetD = 0;
	
	int NumOfLines_ForthO_SetA = 0;
	int NumOfLines_ForthO_SetB = 0;
	int NumOfLines_ForthO_SetC = 0;
	int NumOfLines_ForthO_SetD = 0;
	
	int NumOfLines_FifthO_SetA = 0;
	int NumOfLines_FifthO_SetB = 0;
	int NumOfLines_FifthO_SetC = 0;
	int NumOfLines_FifthO_SetD = 0;
	
	int NumOfLines_SixthO_SetA = 0;
	int NumOfLines_SixthO_SetB = 0;
	int NumOfLines_SixthO_SetC = 0;
	int NumOfLines_SixthO_SetD = 0;
	
	int NumOfLines_SeventhO_SetA = 0;
	int NumOfLines_SeventhO_SetB = 0;
	int NumOfLines_SeventhO_SetC = 0;
	int NumOfLines_SeventhO_SetD = 0;
	
	int NumOfLines_EighthO_SetA = 0;
	int NumOfLines_EighthO_SetB = 0;
	int NumOfLines_EighthO_SetC = 0;
	int NumOfLines_EighthO_SetD = 0;
	
	int NumOfLines_NinethO_SetA = 0;
	int NumOfLines_NinethO_SetB = 0;
	int NumOfLines_NinethO_SetC = 0;
	int NumOfLines_NinethO_SetD = 0;
	
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
	ofstream outFileCMD_SeventhO;
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
	double dummy41[nArray];
	double dummy42[nArray];
	double dummy43[nArray];
	double dummy44[nArray];
	double dummy45[nArray];
	double dummy46[nArray];
	double dummy47[nArray];
	double dummy48[nArray];
	double dummy49[nArray];
	double dummy50[nArray];
	double dummy51[nArray];
	double dummy52[nArray];
	double dummy53[nArray];
	double dummy54[nArray];
	double dummy55[nArray];
	double dummy56[nArray];
	double dummy57[nArray];
	double dummy58[nArray];
	double dummy59[nArray];
	double dummy60[nArray];
	double dummy61[nArray];
	double dummy62[nArray];
	
	
	
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
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
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
				
				if(MakeMagCorrection){
					abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
					abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
				}
			
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
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
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
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
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
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
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
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
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
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFU>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
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
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FU<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FU.close();
		
		}
		
	}
	
	
	//Main loop for FO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(FO_FileName, FO_FilePrefix);
			strcat(FO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FO_FileName, SetA_Suffix);
			
			inFileFO.open(FO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFO, readout)){
				NumOfLines_FO_SetA = NumOfLines_FO_SetA + 1;
			}
			
			cout<<"Number of Lines in FO SetA file: "<<NumOfLines_FO_SetA<<endl;
			
			inFileFO.close();
			
			inFileFO.open(FO_FileName, ios::in);
			getline(inFileFO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FO_SetA; j++){
				
				inFileFO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(FO_HistoryFilePath, Model_char);
				strcat(FO_HistoryFilePath, "/");
				strcat(FO_HistoryFilePath, HistoryFileName);
				cout<<"FO History file path: "<<FO_HistoryFilePath<<endl;
				
				inFileHistoryFO.open(FO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFO, history_header1);
				getline(inFileHistoryFO, history_header2);
				getline(inFileHistoryFO, history_header3);
				getline(inFileHistoryFO, history_header4);
				getline(inFileHistoryFO, history_header5);
				getline(inFileHistoryFO, history_header6);
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryFO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFO.close();
			
			//Make outputfile name 
			strcpy(FO_FullOutputFileName, FO_FilePrefix);
			strcat(FO_FullOutputFileName, outputFileName);
			strcat(FO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_FO.open(FO_FullOutputFileName, ios::out);
			outFileCMD_FO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(FO_FileName, FO_FilePrefix);
			strcat(FO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FO_FileName, SetB_Suffix);
			
			inFileFO.open(FO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFO, readout)){
				NumOfLines_FO_SetB = NumOfLines_FO_SetB + 1;
			}
			
			cout<<"Number of Lines in FO SetB file: "<<NumOfLines_FO_SetB<<endl;
			
			inFileFO.close();
			
			inFileFO.open(FO_FileName, ios::in);
			getline(inFileFO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FO_SetB; j++){
				
				inFileFO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(FO_HistoryFilePath, Model_char);
				strcat(FO_HistoryFilePath, "/");
				strcat(FO_HistoryFilePath, HistoryFileName);
				cout<<"FO History file path: "<<FO_HistoryFilePath<<endl;
				
				inFileHistoryFO.open(FO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFO, history_header1);
				getline(inFileHistoryFO, history_header2);
				getline(inFileHistoryFO, history_header3);
				getline(inFileHistoryFO, history_header4);
				getline(inFileHistoryFO, history_header5);
				getline(inFileHistoryFO, history_header6);
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				
				inFileHistoryFO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFO.close();
			
			//Make outputfile name 
			strcpy(FO_FullOutputFileName, FO_FilePrefix);
			strcat(FO_FullOutputFileName, outputFileName);
			strcat(FO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_FO.open(FO_FullOutputFileName, ios::out);
			outFileCMD_FO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(FO_FileName, FO_FilePrefix);
			strcat(FO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FO_FileName, SetC_Suffix);
			
			inFileFO.open(FO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFO, readout)){
				NumOfLines_FO_SetC = NumOfLines_FO_SetC + 1;
			}
			
			cout<<"Number of Lines in FO SetC file: "<<NumOfLines_FO_SetC<<endl;
			
			inFileFO.close();
			
			inFileFO.open(FO_FileName, ios::in);
			getline(inFileFO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FO_SetC; j++){
				
				inFileFO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(FO_HistoryFilePath, Model_char);
				strcat(FO_HistoryFilePath, "/");
				strcat(FO_HistoryFilePath, HistoryFileName);
				cout<<"FO History file path: "<<FO_HistoryFilePath<<endl;
				
				inFileHistoryFO.open(FO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFO, history_header1);
				getline(inFileHistoryFO, history_header2);
				getline(inFileHistoryFO, history_header3);
				getline(inFileHistoryFO, history_header4);
				getline(inFileHistoryFO, history_header5);
				getline(inFileHistoryFO, history_header6);
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				
				inFileHistoryFO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFO.close();
			
			//Make outputfile name 
			strcpy(FO_FullOutputFileName, FO_FilePrefix);
			strcat(FO_FullOutputFileName, outputFileName);
			strcat(FO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_FO.open(FO_FullOutputFileName, ios::out);
			outFileCMD_FO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(FO_FileName, FO_FilePrefix);
			strcat(FO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FO_FileName, SetD_Suffix);
			
			inFileFO.open(FO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFO, readout)){
				NumOfLines_FO_SetD = NumOfLines_FO_SetD + 1;
			}
			
			cout<<"Number of Lines in FO SetD file: "<<NumOfLines_FO_SetD<<endl;
			
			inFileFO.close();
			
			inFileFO.open(FO_FileName, ios::in);
			getline(inFileFO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FO_SetD; j++){
				
				inFileFO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(FO_HistoryFilePath, Model_char);
				strcat(FO_HistoryFilePath, "/");
				strcat(FO_HistoryFilePath, HistoryFileName);
				cout<<"FO History file path: "<<FO_HistoryFilePath<<endl;
				
				inFileHistoryFO.open(FO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFO, history_header1);
				getline(inFileHistoryFO, history_header2);
				getline(inFileHistoryFO, history_header3);
				getline(inFileHistoryFO, history_header4);
				getline(inFileHistoryFO, history_header5);
				getline(inFileHistoryFO, history_header6);
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				
				inFileHistoryFO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFO.close();
			
			//Make outputfile name 
			strcpy(FO_FullOutputFileName, FO_FilePrefix);
			strcat(FO_FullOutputFileName, outputFileName);
			strcat(FO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_FO.open(FO_FullOutputFileName, ios::out);
			outFileCMD_FO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FO.close();
		
		}
		
	}
	
	
	//Main loop for SO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(SO_FileName, SO_FilePrefix);
			strcat(SO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SO_FileName, SetA_Suffix);
			
			inFileSO.open(SO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSO, readout)){
				NumOfLines_SO_SetA = NumOfLines_SO_SetA + 1;
			}
			
			cout<<"Number of Lines in SO SetA file: "<<NumOfLines_SO_SetA<<endl;
			
			inFileSO.close();
			
			inFileSO.open(SO_FileName, ios::in);
			getline(inFileSO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SO_SetA; j++){
				
				inFileSO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(SO_HistoryFilePath, Model_char);
				strcat(SO_HistoryFilePath, "/");
				strcat(SO_HistoryFilePath, HistoryFileName);
				cout<<"SO History file path: "<<SO_HistoryFilePath<<endl;
				
				inFileHistorySO.open(SO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySO, history_header1);
				getline(inFileHistorySO, history_header2);
				getline(inFileHistorySO, history_header3);
				getline(inFileHistorySO, history_header4);
				getline(inFileHistorySO, history_header5);
				getline(inFileHistorySO, history_header6);
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				
				inFileHistorySO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSO.close();
			
			//Make outputfile name 
			strcpy(SO_FullOutputFileName, SO_FilePrefix);
			strcat(SO_FullOutputFileName, outputFileName);
			strcat(SO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_SO.open(SO_FullOutputFileName, ios::out);
			outFileCMD_SO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(SO_FileName, SO_FilePrefix);
			strcat(SO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SO_FileName, SetB_Suffix);
			
			inFileSO.open(SO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSO, readout)){
				NumOfLines_SO_SetB = NumOfLines_SO_SetB + 1;
			}
			
			cout<<"Number of Lines in SO SetB file: "<<NumOfLines_SO_SetB<<endl;
			
			inFileSO.close();
			
			inFileSO.open(SO_FileName, ios::in);
			getline(inFileSO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SO_SetB; j++){
				
				inFileSO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(SO_HistoryFilePath, Model_char);
				strcat(SO_HistoryFilePath, "/");
				strcat(SO_HistoryFilePath, HistoryFileName);
				cout<<"SO History file path: "<<SO_HistoryFilePath<<endl;
				
				inFileHistorySO.open(SO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySO, history_header1);
				getline(inFileHistorySO, history_header2);
				getline(inFileHistorySO, history_header3);
				getline(inFileHistorySO, history_header4);
				getline(inFileHistorySO, history_header5);
				getline(inFileHistorySO, history_header6);
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSO.close();
			
			//Make outputfile name 
			strcpy(SO_FullOutputFileName, SO_FilePrefix);
			strcat(SO_FullOutputFileName, outputFileName);
			strcat(SO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_SO.open(SO_FullOutputFileName, ios::out);
			outFileCMD_SO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(SO_FileName, SO_FilePrefix);
			strcat(SO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SO_FileName, SetC_Suffix);
			
			inFileSO.open(SO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSO, readout)){
				NumOfLines_SO_SetC = NumOfLines_SO_SetC + 1;
			}
			
			cout<<"Number of Lines in SO SetC file: "<<NumOfLines_SO_SetC<<endl;
			
			inFileSO.close();
			
			inFileSO.open(SO_FileName, ios::in);
			getline(inFileSO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SO_SetC; j++){
				
				inFileSO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(SO_HistoryFilePath, Model_char);
				strcat(SO_HistoryFilePath, "/");
				strcat(SO_HistoryFilePath, HistoryFileName);
				cout<<"SO History file path: "<<SO_HistoryFilePath<<endl;
				
				inFileHistorySO.open(SO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySO, history_header1);
				getline(inFileHistorySO, history_header2);
				getline(inFileHistorySO, history_header3);
				getline(inFileHistorySO, history_header4);
				getline(inFileHistorySO, history_header5);
				getline(inFileHistorySO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSO.close();
			
			//Make outputfile name 
			strcpy(SO_FullOutputFileName, SO_FilePrefix);
			strcat(SO_FullOutputFileName, outputFileName);
			strcat(SO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_SO.open(SO_FullOutputFileName, ios::out);
			outFileCMD_SO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(SO_FileName, SO_FilePrefix);
			strcat(SO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SO_FileName, SetD_Suffix);
			
			inFileSO.open(SO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSO, readout)){
				NumOfLines_SO_SetD = NumOfLines_SO_SetD + 1;
			}
			
			cout<<"Number of Lines in SO SetD file: "<<NumOfLines_SO_SetD<<endl;
			
			inFileSO.close();
			
			inFileSO.open(SO_FileName, ios::in);
			getline(inFileSO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SO_SetD; j++){
				
				inFileSO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(SO_HistoryFilePath, Model_char);
				strcat(SO_HistoryFilePath, "/");
				strcat(SO_HistoryFilePath, HistoryFileName);
				cout<<"SO History file path: "<<SO_HistoryFilePath<<endl;
				
				inFileHistorySO.open(SO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySO, history_header1);
				getline(inFileHistorySO, history_header2);
				getline(inFileHistorySO, history_header3);
				getline(inFileHistorySO, history_header4);
				getline(inFileHistorySO, history_header5);
				getline(inFileHistorySO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSO.close();
			
			//Make outputfile name 
			strcpy(SO_FullOutputFileName, SO_FilePrefix);
			strcat(SO_FullOutputFileName, outputFileName);
			strcat(SO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_SO.open(SO_FullOutputFileName, ios::out);
			outFileCMD_SO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SO.close();
		
		}
		
	}
	
	
	//Main loop for ThirdO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(ThirdO_FileName, ThirdO_FilePrefix);
			strcat(ThirdO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ThirdO_FileName, SetA_Suffix);
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileThirdO, readout)){
				NumOfLines_ThirdO_SetA = NumOfLines_ThirdO_SetA + 1;
			}
			
			cout<<"Number of Lines in ThirdO SetA file: "<<NumOfLines_ThirdO_SetA<<endl;
			
			inFileThirdO.close();
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			getline(inFileThirdO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetA; j++){
				
				inFileThirdO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ThirdO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(ThirdO_HistoryFilePath, Model_char);
				strcat(ThirdO_HistoryFilePath, "/");
				strcat(ThirdO_HistoryFilePath, HistoryFileName);
				cout<<"ThirdO History file path: "<<ThirdO_HistoryFilePath<<endl;
				
				inFileHistoryThirdO.open(ThirdO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryThirdO, history_header1);
				getline(inFileHistoryThirdO, history_header2);
				getline(inFileHistoryThirdO, history_header3);
				getline(inFileHistoryThirdO, history_header4);
				getline(inFileHistoryThirdO, history_header5);
				getline(inFileHistoryThirdO, history_header6);
				
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryThirdO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileThirdO.close();
			
			//Make outputfile name 
			strcpy(ThirdO_FullOutputFileName, ThirdO_FilePrefix);
			strcat(ThirdO_FullOutputFileName, outputFileName);
			strcat(ThirdO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_ThirdO.open(ThirdO_FullOutputFileName, ios::out);
			outFileCMD_ThirdO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ThirdO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ThirdO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(ThirdO_FileName, ThirdO_FilePrefix);
			strcat(ThirdO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ThirdO_FileName, SetB_Suffix);
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileThirdO, readout)){
				NumOfLines_ThirdO_SetB = NumOfLines_ThirdO_SetB + 1;
			}
			
			cout<<"Number of Lines in ThirdO SetB file: "<<NumOfLines_ThirdO_SetB<<endl;
			
			inFileThirdO.close();
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			getline(inFileThirdO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetB; j++){
				
				inFileThirdO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ThirdO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(ThirdO_HistoryFilePath, Model_char);
				strcat(ThirdO_HistoryFilePath, "/");
				strcat(ThirdO_HistoryFilePath, HistoryFileName);
				cout<<"ThirdO History file path: "<<ThirdO_HistoryFilePath<<endl;
				
				inFileHistoryThirdO.open(ThirdO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryThirdO, history_header1);
				getline(inFileHistoryThirdO, history_header2);
				getline(inFileHistoryThirdO, history_header3);
				getline(inFileHistoryThirdO, history_header4);
				getline(inFileHistoryThirdO, history_header5);
				getline(inFileHistoryThirdO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryThirdO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileThirdO.close();
			
			//Make outputfile name 
			strcpy(ThirdO_FullOutputFileName, ThirdO_FilePrefix);
			strcat(ThirdO_FullOutputFileName, outputFileName);
			strcat(ThirdO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_ThirdO.open(ThirdO_FullOutputFileName, ios::out);
			outFileCMD_ThirdO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ThirdO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ThirdO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(ThirdO_FileName, ThirdO_FilePrefix);
			strcat(ThirdO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ThirdO_FileName, SetC_Suffix);
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileThirdO, readout)){
				NumOfLines_ThirdO_SetC = NumOfLines_ThirdO_SetC + 1;
			}
			
			cout<<"Number of Lines in ThirdO SetC file: "<<NumOfLines_ThirdO_SetC<<endl;
			
			inFileThirdO.close();
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			getline(inFileThirdO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetC; j++){
				
				inFileThirdO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ThirdO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(ThirdO_HistoryFilePath, Model_char);
				strcat(ThirdO_HistoryFilePath, "/");
				strcat(ThirdO_HistoryFilePath, HistoryFileName);
				cout<<"ThirdO History file path: "<<ThirdO_HistoryFilePath<<endl;
				
				inFileHistoryThirdO.open(ThirdO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryThirdO, history_header1);
				getline(inFileHistoryThirdO, history_header2);
				getline(inFileHistoryThirdO, history_header3);
				getline(inFileHistoryThirdO, history_header4);
				getline(inFileHistoryThirdO, history_header5);
				getline(inFileHistoryThirdO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryThirdO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileThirdO.close();
			
			//Make outputfile name 
			strcpy(ThirdO_FullOutputFileName, ThirdO_FilePrefix);
			strcat(ThirdO_FullOutputFileName, outputFileName);
			strcat(ThirdO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_ThirdO.open(ThirdO_FullOutputFileName, ios::out);
			outFileCMD_ThirdO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ThirdO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ThirdO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(ThirdO_FileName, ThirdO_FilePrefix);
			strcat(ThirdO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ThirdO_FileName, SetD_Suffix);
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileThirdO, readout)){
				NumOfLines_ThirdO_SetD = NumOfLines_ThirdO_SetD + 1;
			}
			
			cout<<"Number of Lines in ThirdO SetD file: "<<NumOfLines_ThirdO_SetD<<endl;
			
			inFileThirdO.close();
			
			inFileThirdO.open(ThirdO_FileName, ios::in);
			getline(inFileThirdO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetD; j++){
				
				inFileThirdO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ThirdO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(ThirdO_HistoryFilePath, Model_char);
				strcat(ThirdO_HistoryFilePath, "/");
				strcat(ThirdO_HistoryFilePath, HistoryFileName);
				cout<<"ThirdO History file path: "<<ThirdO_HistoryFilePath<<endl;
				
				inFileHistoryThirdO.open(ThirdO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryThirdO, history_header1);
				getline(inFileHistoryThirdO, history_header2);
				getline(inFileHistoryThirdO, history_header3);
				getline(inFileHistoryThirdO, history_header4);
				getline(inFileHistoryThirdO, history_header5);
				getline(inFileHistoryThirdO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryThirdO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryThirdO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileThirdO.close();
			
			//Make outputfile name 
			strcpy(ThirdO_FullOutputFileName, ThirdO_FilePrefix);
			strcat(ThirdO_FullOutputFileName, outputFileName);
			strcat(ThirdO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_ThirdO.open(ThirdO_FullOutputFileName, ios::out);
			outFileCMD_ThirdO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ThirdO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ThirdO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ThirdO.close();
		
		}
		
	}
	
	
	//Main loop for ForthO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(ForthO_FileName, ForthO_FilePrefix);
			strcat(ForthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ForthO_FileName, SetA_Suffix);
			
			inFileForthO.open(ForthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileForthO, readout)){
				NumOfLines_ForthO_SetA = NumOfLines_ForthO_SetA + 1;
			}
			
			cout<<"Number of Lines in ForthO SetA file: "<<NumOfLines_ForthO_SetA<<endl;
			
			inFileForthO.close();
			
			inFileForthO.open(ForthO_FileName, ios::in);
			getline(inFileForthO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ForthO_SetA; j++){
				
				inFileForthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ForthO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(ForthO_HistoryFilePath, Model_char);
				strcat(ForthO_HistoryFilePath, "/");
				strcat(ForthO_HistoryFilePath, HistoryFileName);
				cout<<"ForthO History file path: "<<ForthO_HistoryFilePath<<endl;
				
				inFileHistoryForthO.open(ForthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryForthO, history_header1);
				getline(inFileHistoryForthO, history_header2);
				getline(inFileHistoryForthO, history_header3);
				getline(inFileHistoryForthO, history_header4);
				getline(inFileHistoryForthO, history_header5);
				getline(inFileHistoryForthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryForthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileForthO.close();
			
			//Make outputfile name 
			strcpy(ForthO_FullOutputFileName, ForthO_FilePrefix);
			strcat(ForthO_FullOutputFileName, outputFileName);
			strcat(ForthO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_ForthO.open(ForthO_FullOutputFileName, ios::out);
			outFileCMD_ForthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ForthO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ForthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ForthO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(ForthO_FileName, ForthO_FilePrefix);
			strcat(ForthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ForthO_FileName, SetB_Suffix);
			
			inFileForthO.open(ForthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileForthO, readout)){
				NumOfLines_ForthO_SetB = NumOfLines_ForthO_SetB + 1;
			}
			
			cout<<"Number of Lines in ForthO SetB file: "<<NumOfLines_ForthO_SetB<<endl;
			
			inFileForthO.close();
			
			inFileForthO.open(ForthO_FileName, ios::in);
			getline(inFileForthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ForthO_SetB; j++){
				
				inFileForthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ForthO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(ForthO_HistoryFilePath, Model_char);
				strcat(ForthO_HistoryFilePath, "/");
				strcat(ForthO_HistoryFilePath, HistoryFileName);
				cout<<"ForthO History file path: "<<ForthO_HistoryFilePath<<endl;
				
				inFileHistoryForthO.open(ForthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryForthO, history_header1);
				getline(inFileHistoryForthO, history_header2);
				getline(inFileHistoryForthO, history_header3);
				getline(inFileHistoryForthO, history_header4);
				getline(inFileHistoryForthO, history_header5);
				getline(inFileHistoryForthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryForthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileForthO.close();
			
			//Make outputfile name 
			strcpy(ForthO_FullOutputFileName, ForthO_FilePrefix);
			strcat(ForthO_FullOutputFileName, outputFileName);
			strcat(ForthO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_ForthO.open(ForthO_FullOutputFileName, ios::out);
			outFileCMD_ForthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ForthO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ForthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ForthO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(ForthO_FileName, ForthO_FilePrefix);
			strcat(ForthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ForthO_FileName, SetC_Suffix);
			
			inFileForthO.open(ForthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileForthO, readout)){
				NumOfLines_ForthO_SetC = NumOfLines_ForthO_SetC + 1;
			}
			
			cout<<"Number of Lines in ForthO SetC file: "<<NumOfLines_ForthO_SetC<<endl;
			
			inFileForthO.close();
			
			inFileForthO.open(ForthO_FileName, ios::in);
			getline(inFileForthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ForthO_SetC; j++){
				
				inFileForthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ForthO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(ForthO_HistoryFilePath, Model_char);
				strcat(ForthO_HistoryFilePath, "/");
				strcat(ForthO_HistoryFilePath, HistoryFileName);
				cout<<"ForthO History file path: "<<ForthO_HistoryFilePath<<endl;
				
				inFileHistoryForthO.open(ForthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryForthO, history_header1);
				getline(inFileHistoryForthO, history_header2);
				getline(inFileHistoryForthO, history_header3);
				getline(inFileHistoryForthO, history_header4);
				getline(inFileHistoryForthO, history_header5);
				getline(inFileHistoryForthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryForthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileForthO.close();
			
			//Make outputfile name 
			strcpy(ForthO_FullOutputFileName, ForthO_FilePrefix);
			strcat(ForthO_FullOutputFileName, outputFileName);
			strcat(ForthO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_ForthO.open(ForthO_FullOutputFileName, ios::out);
			outFileCMD_ForthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ForthO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ForthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ForthO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(ForthO_FileName, ForthO_FilePrefix);
			strcat(ForthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(ForthO_FileName, SetD_Suffix);
			
			inFileForthO.open(ForthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileForthO, readout)){
				NumOfLines_ForthO_SetD = NumOfLines_ForthO_SetD + 1;
			}
			
			cout<<"Number of Lines in ForthO SetD file: "<<NumOfLines_ForthO_SetD<<endl;
			
			inFileForthO.close();
			
			inFileForthO.open(ForthO_FileName, ios::in);
			getline(inFileForthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_ForthO_SetD; j++){
				
				inFileForthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(ForthO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(ForthO_HistoryFilePath, Model_char);
				strcat(ForthO_HistoryFilePath, "/");
				strcat(ForthO_HistoryFilePath, HistoryFileName);
				cout<<"ForthO History file path: "<<ForthO_HistoryFilePath<<endl;
				
				inFileHistoryForthO.open(ForthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryForthO, history_header1);
				getline(inFileHistoryForthO, history_header2);
				getline(inFileHistoryForthO, history_header3);
				getline(inFileHistoryForthO, history_header4);
				getline(inFileHistoryForthO, history_header5);
				getline(inFileHistoryForthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryForthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryForthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileForthO.close();
			
			//Make outputfile name 
			strcpy(ForthO_FullOutputFileName, ForthO_FilePrefix);
			strcat(ForthO_FullOutputFileName, outputFileName);
			strcat(ForthO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_ForthO.open(ForthO_FullOutputFileName, ios::out);
			outFileCMD_ForthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_ForthO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_ForthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_ForthO.close();
		
		}
		
	}
	
	
	//Main loop for FifthO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(FifthO_FileName, FifthO_FilePrefix);
			strcat(FifthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FifthO_FileName, SetA_Suffix);
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFifthO, readout)){
				NumOfLines_FifthO_SetA = NumOfLines_FifthO_SetA + 1;
			}
			
			cout<<"Number of Lines in FifthO SetA file: "<<NumOfLines_FifthO_SetA<<endl;
			
			inFileFifthO.close();
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			getline(inFileFifthO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FifthO_SetA; j++){
				
				inFileFifthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FifthO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(FifthO_HistoryFilePath, Model_char);
				strcat(FifthO_HistoryFilePath, "/");
				strcat(FifthO_HistoryFilePath, HistoryFileName);
				cout<<"FifthO History file path: "<<FifthO_HistoryFilePath<<endl;
				
				inFileHistoryFifthO.open(FifthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFifthO, history_header1);
				getline(inFileHistoryFifthO, history_header2);
				getline(inFileHistoryFifthO, history_header3);
				getline(inFileHistoryFifthO, history_header4);
				getline(inFileHistoryFifthO, history_header5);
				getline(inFileHistoryFifthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryFifthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFifthO.close();
			
			//Make outputfile name 
			strcpy(FifthO_FullOutputFileName, FifthO_FilePrefix);
			strcat(FifthO_FullOutputFileName, outputFileName);
			strcat(FifthO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_FifthO.open(FifthO_FullOutputFileName, ios::out);
			outFileCMD_FifthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FifthO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FifthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FifthO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(FifthO_FileName, FifthO_FilePrefix);
			strcat(FifthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FifthO_FileName, SetB_Suffix);
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFifthO, readout)){
				NumOfLines_FifthO_SetB = NumOfLines_FifthO_SetB + 1;
			}
			
			cout<<"Number of Lines in FifthO SetB file: "<<NumOfLines_FifthO_SetB<<endl;
			
			inFileFifthO.close();
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			getline(inFileFifthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FifthO_SetB; j++){
				
				inFileFifthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FifthO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(FifthO_HistoryFilePath, Model_char);
				strcat(FifthO_HistoryFilePath, "/");
				strcat(FifthO_HistoryFilePath, HistoryFileName);
				cout<<"FifthO History file path: "<<FifthO_HistoryFilePath<<endl;
				
				inFileHistoryFifthO.open(FifthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFifthO, history_header1);
				getline(inFileHistoryFifthO, history_header2);
				getline(inFileHistoryFifthO, history_header3);
				getline(inFileHistoryFifthO, history_header4);
				getline(inFileHistoryFifthO, history_header5);
				getline(inFileHistoryFifthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryFifthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFifthO.close();
			
			//Make outputfile name 
			strcpy(FifthO_FullOutputFileName, FifthO_FilePrefix);
			strcat(FifthO_FullOutputFileName, outputFileName);
			strcat(FifthO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_FifthO.open(FifthO_FullOutputFileName, ios::out);
			outFileCMD_FifthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FifthO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FifthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FifthO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(FifthO_FileName, FifthO_FilePrefix);
			strcat(FifthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FifthO_FileName, SetC_Suffix);
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFifthO, readout)){
				NumOfLines_FifthO_SetC = NumOfLines_FifthO_SetC + 1;
			}
			
			cout<<"Number of Lines in FifthO SetC file: "<<NumOfLines_FifthO_SetC<<endl;
			
			inFileFifthO.close();
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			getline(inFileFifthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FifthO_SetC; j++){
				
				inFileFifthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FifthO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(FifthO_HistoryFilePath, Model_char);
				strcat(FifthO_HistoryFilePath, "/");
				strcat(FifthO_HistoryFilePath, HistoryFileName);
				cout<<"FifthO History file path: "<<FifthO_HistoryFilePath<<endl;
				
				inFileHistoryFifthO.open(FifthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFifthO, history_header1);
				getline(inFileHistoryFifthO, history_header2);
				getline(inFileHistoryFifthO, history_header3);
				getline(inFileHistoryFifthO, history_header4);
				getline(inFileHistoryFifthO, history_header5);
				getline(inFileHistoryFifthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryFifthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFifthO.close();
			
			//Make outputfile name 
			strcpy(FifthO_FullOutputFileName, FifthO_FilePrefix);
			strcat(FifthO_FullOutputFileName, outputFileName);
			strcat(FifthO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_FifthO.open(FifthO_FullOutputFileName, ios::out);
			outFileCMD_FifthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FifthO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FifthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FifthO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(FifthO_FileName, FifthO_FilePrefix);
			strcat(FifthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(FifthO_FileName, SetD_Suffix);
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileFifthO, readout)){
				NumOfLines_FifthO_SetD = NumOfLines_FifthO_SetD + 1;
			}
			
			cout<<"Number of Lines in FifthO SetD file: "<<NumOfLines_FifthO_SetD<<endl;
			
			inFileFifthO.close();
			
			inFileFifthO.open(FifthO_FileName, ios::in);
			getline(inFileFifthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_FifthO_SetD; j++){
				
				inFileFifthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(FifthO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(FifthO_HistoryFilePath, Model_char);
				strcat(FifthO_HistoryFilePath, "/");
				strcat(FifthO_HistoryFilePath, HistoryFileName);
				cout<<"FifthO History file path: "<<FifthO_HistoryFilePath<<endl;
				
				inFileHistoryFifthO.open(FifthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryFifthO, history_header1);
				getline(inFileHistoryFifthO, history_header2);
				getline(inFileHistoryFifthO, history_header3);
				getline(inFileHistoryFifthO, history_header4);
				getline(inFileHistoryFifthO, history_header5);
				getline(inFileHistoryFifthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryFifthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryFifthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileFifthO.close();
			
			//Make outputfile name 
			strcpy(FifthO_FullOutputFileName, FifthO_FilePrefix);
			strcat(FifthO_FullOutputFileName, outputFileName);
			strcat(FifthO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_FifthO.open(FifthO_FullOutputFileName, ios::out);
			outFileCMD_FifthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_FifthO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_FifthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_FifthO.close();
		
		}
		
	}
	
	
	//Main loop for SixthO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(SixthO_FileName, SixthO_FilePrefix);
			strcat(SixthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SixthO_FileName, SetA_Suffix);
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSixthO, readout)){
				NumOfLines_SixthO_SetA = NumOfLines_SixthO_SetA + 1;
			}
			
			cout<<"Number of Lines in SixthO SetA file: "<<NumOfLines_SixthO_SetA<<endl;
			
			inFileSixthO.close();
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			getline(inFileSixthO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SixthO_SetA; j++){
				
				inFileSixthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SixthO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(SixthO_HistoryFilePath, Model_char);
				strcat(SixthO_HistoryFilePath, "/");
				strcat(SixthO_HistoryFilePath, HistoryFileName);
				cout<<"SixthO History file path: "<<SixthO_HistoryFilePath<<endl;
				
				inFileHistorySixthO.open(SixthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySixthO, history_header1);
				getline(inFileHistorySixthO, history_header2);
				getline(inFileHistorySixthO, history_header3);
				getline(inFileHistorySixthO, history_header4);
				getline(inFileHistorySixthO, history_header5);
				getline(inFileHistorySixthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySixthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSixthO.close();
			
			//Make outputfile name 
			strcpy(SixthO_FullOutputFileName, SixthO_FilePrefix);
			strcat(SixthO_FullOutputFileName, outputFileName);
			strcat(SixthO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_SixthO.open(SixthO_FullOutputFileName, ios::out);
			outFileCMD_SixthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SixthO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SixthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SixthO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(SixthO_FileName, SixthO_FilePrefix);
			strcat(SixthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SixthO_FileName, SetB_Suffix);
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSixthO, readout)){
				NumOfLines_SixthO_SetB = NumOfLines_SixthO_SetB + 1;
			}
			
			cout<<"Number of Lines in SixthO SetB file: "<<NumOfLines_SixthO_SetB<<endl;
			
			inFileSixthO.close();
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			getline(inFileSixthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SixthO_SetB; j++){
				
				inFileSixthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SixthO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(SixthO_HistoryFilePath, Model_char);
				strcat(SixthO_HistoryFilePath, "/");
				strcat(SixthO_HistoryFilePath, HistoryFileName);
				cout<<"SixthO History file path: "<<SixthO_HistoryFilePath<<endl;
				
				inFileHistorySixthO.open(SixthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySixthO, history_header1);
				getline(inFileHistorySixthO, history_header2);
				getline(inFileHistorySixthO, history_header3);
				getline(inFileHistorySixthO, history_header4);
				getline(inFileHistorySixthO, history_header5);
				getline(inFileHistorySixthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySixthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSixthO.close();
			
			//Make outputfile name 
			strcpy(SixthO_FullOutputFileName, SixthO_FilePrefix);
			strcat(SixthO_FullOutputFileName, outputFileName);
			strcat(SixthO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_SixthO.open(SixthO_FullOutputFileName, ios::out);
			outFileCMD_SixthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SixthO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SixthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SixthO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(SixthO_FileName, SixthO_FilePrefix);
			strcat(SixthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SixthO_FileName, SetC_Suffix);
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSixthO, readout)){
				NumOfLines_SixthO_SetC = NumOfLines_SixthO_SetC + 1;
			}
			
			cout<<"Number of Lines in SixthO SetC file: "<<NumOfLines_SixthO_SetC<<endl;
			
			inFileSixthO.close();
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			getline(inFileSixthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SixthO_SetC; j++){
				
				inFileSixthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SixthO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(SixthO_HistoryFilePath, Model_char);
				strcat(SixthO_HistoryFilePath, "/");
				strcat(SixthO_HistoryFilePath, HistoryFileName);
				cout<<"SixthO History file path: "<<SixthO_HistoryFilePath<<endl;
				
				inFileHistorySixthO.open(SixthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySixthO, history_header1);
				getline(inFileHistorySixthO, history_header2);
				getline(inFileHistorySixthO, history_header3);
				getline(inFileHistorySixthO, history_header4);
				getline(inFileHistorySixthO, history_header5);
				getline(inFileHistorySixthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySixthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSixthO.close();
			
			//Make outputfile name 
			strcpy(SixthO_FullOutputFileName, SixthO_FilePrefix);
			strcat(SixthO_FullOutputFileName, outputFileName);
			strcat(SixthO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_SixthO.open(SixthO_FullOutputFileName, ios::out);
			outFileCMD_SixthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SixthO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SixthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SixthO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(SixthO_FileName, SixthO_FilePrefix);
			strcat(SixthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SixthO_FileName, SetD_Suffix);
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSixthO, readout)){
				NumOfLines_SixthO_SetD = NumOfLines_SixthO_SetD + 1;
			}
			
			cout<<"Number of Lines in SixthO SetD file: "<<NumOfLines_SixthO_SetD<<endl;
			
			inFileSixthO.close();
			
			inFileSixthO.open(SixthO_FileName, ios::in);
			getline(inFileSixthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SixthO_SetD; j++){
				
				inFileSixthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SixthO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(SixthO_HistoryFilePath, Model_char);
				strcat(SixthO_HistoryFilePath, "/");
				strcat(SixthO_HistoryFilePath, HistoryFileName);
				cout<<"SixthO History file path: "<<SixthO_HistoryFilePath<<endl;
				
				inFileHistorySixthO.open(SixthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySixthO, history_header1);
				getline(inFileHistorySixthO, history_header2);
				getline(inFileHistorySixthO, history_header3);
				getline(inFileHistorySixthO, history_header4);
				getline(inFileHistorySixthO, history_header5);
				getline(inFileHistorySixthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySixthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySixthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSixthO.close();
			
			//Make outputfile name 
			strcpy(SixthO_FullOutputFileName, SixthO_FilePrefix);
			strcat(SixthO_FullOutputFileName, outputFileName);
			strcat(SixthO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_SixthO.open(SixthO_FullOutputFileName, ios::out);
			outFileCMD_SixthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SixthO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SixthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SixthO.close();
		
		}
		
	}
	
	
	//Main loop for SeventhO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(SeventhO_FileName, SeventhO_FilePrefix);
			strcat(SeventhO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SeventhO_FileName, SetA_Suffix);
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSeventhO, readout)){
				NumOfLines_SeventhO_SetA = NumOfLines_SeventhO_SetA + 1;
			}
			
			cout<<"Number of Lines in SeventhO SetA file: "<<NumOfLines_SeventhO_SetA<<endl;
			
			inFileSeventhO.close();
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			getline(inFileSeventhO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetA; j++){
				
				inFileSeventhO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SeventhO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(SeventhO_HistoryFilePath, Model_char);
				strcat(SeventhO_HistoryFilePath, "/");
				strcat(SeventhO_HistoryFilePath, HistoryFileName);
				cout<<"SeventhO History file path: "<<SeventhO_HistoryFilePath<<endl;
				
				inFileHistorySeventhO.open(SeventhO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySeventhO, history_header1);
				getline(inFileHistorySeventhO, history_header2);
				getline(inFileHistorySeventhO, history_header3);
				getline(inFileHistorySeventhO, history_header4);
				getline(inFileHistorySeventhO, history_header5);
				getline(inFileHistorySeventhO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySeventhO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSeventhO.close();
			
			//Make outputfile name 
			strcpy(SeventhO_FullOutputFileName, SeventhO_FilePrefix);
			strcat(SeventhO_FullOutputFileName, outputFileName);
			strcat(SeventhO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_SeventhO.open(SeventhO_FullOutputFileName, ios::out);
			outFileCMD_SeventhO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SeventhO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SeventhO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(SeventhO_FileName, SeventhO_FilePrefix);
			strcat(SeventhO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SeventhO_FileName, SetB_Suffix);
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSeventhO, readout)){
				NumOfLines_SeventhO_SetB = NumOfLines_SeventhO_SetB + 1;
			}
			
			cout<<"Number of Lines in SeventhO SetB file: "<<NumOfLines_SeventhO_SetB<<endl;
			
			inFileSeventhO.close();
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			getline(inFileSeventhO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetB; j++){
				
				inFileSeventhO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SeventhO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(SeventhO_HistoryFilePath, Model_char);
				strcat(SeventhO_HistoryFilePath, "/");
				strcat(SeventhO_HistoryFilePath, HistoryFileName);
				cout<<"SeventhO History file path: "<<SeventhO_HistoryFilePath<<endl;
				
				inFileHistorySeventhO.open(SeventhO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySeventhO, history_header1);
				getline(inFileHistorySeventhO, history_header2);
				getline(inFileHistorySeventhO, history_header3);
				getline(inFileHistorySeventhO, history_header4);
				getline(inFileHistorySeventhO, history_header5);
				getline(inFileHistorySeventhO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySeventhO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSeventhO.close();
			
			//Make outputfile name 
			strcpy(SeventhO_FullOutputFileName, SeventhO_FilePrefix);
			strcat(SeventhO_FullOutputFileName, outputFileName);
			strcat(SeventhO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_SeventhO.open(SeventhO_FullOutputFileName, ios::out);
			outFileCMD_SeventhO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SeventhO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SeventhO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(SeventhO_FileName, SeventhO_FilePrefix);
			strcat(SeventhO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SeventhO_FileName, SetC_Suffix);
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSeventhO, readout)){
				NumOfLines_SeventhO_SetC = NumOfLines_SeventhO_SetC + 1;
			}
			
			cout<<"Number of Lines in SeventhO SetC file: "<<NumOfLines_SeventhO_SetC<<endl;
			
			inFileSeventhO.close();
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			getline(inFileSeventhO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetC; j++){
				
				inFileSeventhO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SeventhO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(SeventhO_HistoryFilePath, Model_char);
				strcat(SeventhO_HistoryFilePath, "/");
				strcat(SeventhO_HistoryFilePath, HistoryFileName);
				cout<<"SeventhO History file path: "<<SeventhO_HistoryFilePath<<endl;
				
				inFileHistorySeventhO.open(SeventhO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySeventhO, history_header1);
				getline(inFileHistorySeventhO, history_header2);
				getline(inFileHistorySeventhO, history_header3);
				getline(inFileHistorySeventhO, history_header4);
				getline(inFileHistorySeventhO, history_header5);
				getline(inFileHistorySeventhO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySeventhO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSeventhO.close();
			
			//Make outputfile name 
			strcpy(SeventhO_FullOutputFileName, SeventhO_FilePrefix);
			strcat(SeventhO_FullOutputFileName, outputFileName);
			strcat(SeventhO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_SeventhO.open(SeventhO_FullOutputFileName, ios::out);
			outFileCMD_SeventhO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SeventhO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SeventhO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(SeventhO_FileName, SeventhO_FilePrefix);
			strcat(SeventhO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(SeventhO_FileName, SetD_Suffix);
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileSeventhO, readout)){
				NumOfLines_SeventhO_SetD = NumOfLines_SeventhO_SetD + 1;
			}
			
			cout<<"Number of Lines in SeventhO SetD file: "<<NumOfLines_SeventhO_SetD<<endl;
			
			inFileSeventhO.close();
			
			inFileSeventhO.open(SeventhO_FileName, ios::in);
			getline(inFileSeventhO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetD; j++){
				
				inFileSeventhO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(SeventhO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(SeventhO_HistoryFilePath, Model_char);
				strcat(SeventhO_HistoryFilePath, "/");
				strcat(SeventhO_HistoryFilePath, HistoryFileName);
				cout<<"SeventhO History file path: "<<SeventhO_HistoryFilePath<<endl;
				
				inFileHistorySeventhO.open(SeventhO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistorySeventhO, history_header1);
				getline(inFileHistorySeventhO, history_header2);
				getline(inFileHistorySeventhO, history_header3);
				getline(inFileHistorySeventhO, history_header4);
				getline(inFileHistorySeventhO, history_header5);
				getline(inFileHistorySeventhO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistorySeventhO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistorySeventhO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileSeventhO.close();
			
			//Make outputfile name 
			strcpy(SeventhO_FullOutputFileName, SeventhO_FilePrefix);
			strcat(SeventhO_FullOutputFileName, outputFileName);
			strcat(SeventhO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_SeventhO.open(SeventhO_FullOutputFileName, ios::out);
			outFileCMD_SeventhO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_SeventhO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_SeventhO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_SeventhO.close();
		
		}
		
	}
	
	
	//Main loop for EighthO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(EighthO_FileName, EighthO_FilePrefix);
			strcat(EighthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(EighthO_FileName, SetA_Suffix);
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileEighthO, readout)){
				NumOfLines_EighthO_SetA = NumOfLines_EighthO_SetA + 1;
			}
			
			cout<<"Number of Lines in EighthO SetA file: "<<NumOfLines_EighthO_SetA<<endl;
			
			inFileEighthO.close();
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			getline(inFileEighthO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_EighthO_SetA; j++){
				
				inFileEighthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(EighthO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(EighthO_HistoryFilePath, Model_char);
				strcat(EighthO_HistoryFilePath, "/");
				strcat(EighthO_HistoryFilePath, HistoryFileName);
				cout<<"EighthO History file path: "<<EighthO_HistoryFilePath<<endl;
				
				inFileHistoryEighthO.open(EighthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryEighthO, history_header1);
				getline(inFileHistoryEighthO, history_header2);
				getline(inFileHistoryEighthO, history_header3);
				getline(inFileHistoryEighthO, history_header4);
				getline(inFileHistoryEighthO, history_header5);
				getline(inFileHistoryEighthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryEighthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileEighthO.close();
			
			//Make outputfile name 
			strcpy(EighthO_FullOutputFileName, EighthO_FilePrefix);
			strcat(EighthO_FullOutputFileName, outputFileName);
			strcat(EighthO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_EighthO.open(EighthO_FullOutputFileName, ios::out);
			outFileCMD_EighthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_EighthO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_EighthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_EighthO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(EighthO_FileName, EighthO_FilePrefix);
			strcat(EighthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(EighthO_FileName, SetB_Suffix);
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileEighthO, readout)){
				NumOfLines_EighthO_SetB = NumOfLines_EighthO_SetB + 1;
			}
			
			cout<<"Number of Lines in EighthO SetB file: "<<NumOfLines_EighthO_SetB<<endl;
			
			inFileEighthO.close();
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			getline(inFileEighthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_EighthO_SetB; j++){
				
				inFileEighthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(EighthO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(EighthO_HistoryFilePath, Model_char);
				strcat(EighthO_HistoryFilePath, "/");
				strcat(EighthO_HistoryFilePath, HistoryFileName);
				cout<<"EighthO History file path: "<<EighthO_HistoryFilePath<<endl;
				
				inFileHistoryEighthO.open(EighthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryEighthO, history_header1);
				getline(inFileHistoryEighthO, history_header2);
				getline(inFileHistoryEighthO, history_header3);
				getline(inFileHistoryEighthO, history_header4);
				getline(inFileHistoryEighthO, history_header5);
				getline(inFileHistoryEighthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryEighthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileEighthO.close();
			
			//Make outputfile name 
			strcpy(EighthO_FullOutputFileName, EighthO_FilePrefix);
			strcat(EighthO_FullOutputFileName, outputFileName);
			strcat(EighthO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_EighthO.open(EighthO_FullOutputFileName, ios::out);
			outFileCMD_EighthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_EighthO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_EighthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_EighthO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(EighthO_FileName, EighthO_FilePrefix);
			strcat(EighthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(EighthO_FileName, SetC_Suffix);
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileEighthO, readout)){
				NumOfLines_EighthO_SetC = NumOfLines_EighthO_SetC + 1;
			}
			
			cout<<"Number of Lines in EighthO SetC file: "<<NumOfLines_EighthO_SetC<<endl;
			
			inFileEighthO.close();
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			getline(inFileEighthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_EighthO_SetC; j++){
				
				inFileEighthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(EighthO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(EighthO_HistoryFilePath, Model_char);
				strcat(EighthO_HistoryFilePath, "/");
				strcat(EighthO_HistoryFilePath, HistoryFileName);
				cout<<"EighthO History file path: "<<EighthO_HistoryFilePath<<endl;
				
				inFileHistoryEighthO.open(EighthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryEighthO, history_header1);
				getline(inFileHistoryEighthO, history_header2);
				getline(inFileHistoryEighthO, history_header3);
				getline(inFileHistoryEighthO, history_header4);
				getline(inFileHistoryEighthO, history_header5);
				getline(inFileHistoryEighthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryEighthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileEighthO.close();
			
			//Make outputfile name 
			strcpy(EighthO_FullOutputFileName, EighthO_FilePrefix);
			strcat(EighthO_FullOutputFileName, outputFileName);
			strcat(EighthO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_EighthO.open(EighthO_FullOutputFileName, ios::out);
			outFileCMD_EighthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_EighthO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_EighthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_EighthO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(EighthO_FileName, EighthO_FilePrefix);
			strcat(EighthO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(EighthO_FileName, SetD_Suffix);
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileEighthO, readout)){
				NumOfLines_EighthO_SetD = NumOfLines_EighthO_SetD + 1;
			}
			
			cout<<"Number of Lines in EighthO SetD file: "<<NumOfLines_EighthO_SetD<<endl;
			
			inFileEighthO.close();
			
			inFileEighthO.open(EighthO_FileName, ios::in);
			getline(inFileEighthO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_EighthO_SetD; j++){
				
				inFileEighthO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(EighthO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(EighthO_HistoryFilePath, Model_char);
				strcat(EighthO_HistoryFilePath, "/");
				strcat(EighthO_HistoryFilePath, HistoryFileName);
				cout<<"EighthO History file path: "<<EighthO_HistoryFilePath<<endl;
				
				inFileHistoryEighthO.open(EighthO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryEighthO, history_header1);
				getline(inFileHistoryEighthO, history_header2);
				getline(inFileHistoryEighthO, history_header3);
				getline(inFileHistoryEighthO, history_header4);
				getline(inFileHistoryEighthO, history_header5);
				getline(inFileHistoryEighthO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryEighthO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryEighthO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileEighthO.close();
			
			//Make outputfile name 
			strcpy(EighthO_FullOutputFileName, EighthO_FilePrefix);
			strcat(EighthO_FullOutputFileName, outputFileName);
			strcat(EighthO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_EighthO.open(EighthO_FullOutputFileName, ios::out);
			outFileCMD_EighthO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_EighthO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_EighthO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_EighthO.close();
		
		}
		
	}
	
	
	//Main loop for NinethO mode//
	for(int i = 0; i < NumOfSets; i++){
		
		//SetA
		if(i == 0){
			
			//Put together input file name
			strcpy(NinethO_FileName, NinethO_FilePrefix);
			strcat(NinethO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(NinethO_FileName, SetA_Suffix);
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileNinethO, readout)){
				NumOfLines_NinethO_SetA = NumOfLines_NinethO_SetA + 1;
			}
			
			cout<<"Number of Lines in NinethO SetA file: "<<NumOfLines_NinethO_SetA<<endl;
			
			inFileNinethO.close();
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			getline(inFileNinethO, input_header); //Get input header out of the way
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_NinethO_SetA; j++){
				
				inFileNinethO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(NinethO_HistoryFilePath, log_directory_prefix_SetA);
				strcat(NinethO_HistoryFilePath, Model_char);
				strcat(NinethO_HistoryFilePath, "/");
				strcat(NinethO_HistoryFilePath, HistoryFileName);
				cout<<"NinethO History file path: "<<NinethO_HistoryFilePath<<endl;
				
				inFileHistoryNinethO.open(NinethO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryNinethO, history_header1);
				getline(inFileHistoryNinethO, history_header2);
				getline(inFileHistoryNinethO, history_header3);
				getline(inFileHistoryNinethO, history_header4);
				getline(inFileHistoryNinethO, history_header5);
				getline(inFileHistoryNinethO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryNinethO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileNinethO.close();
			
			//Make outputfile name 
			strcpy(NinethO_FullOutputFileName, NinethO_FilePrefix);
			strcat(NinethO_FullOutputFileName, outputFileName);
			strcat(NinethO_FullOutputFileName, SetA_Suffix);
			
			outFileCMD_NinethO.open(NinethO_FullOutputFileName, ios::out);
			outFileCMD_NinethO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_NinethO_SetA; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_NinethO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_NinethO.close();
		
		}
		
		//SetB
		if(i == 1){
			
			//Put together input file name
			strcpy(NinethO_FileName, NinethO_FilePrefix);
			strcat(NinethO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(NinethO_FileName, SetB_Suffix);
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileNinethO, readout)){
				NumOfLines_NinethO_SetB = NumOfLines_NinethO_SetB + 1;
			}
			
			cout<<"Number of Lines in NinethO SetB file: "<<NumOfLines_NinethO_SetB<<endl;
			
			inFileNinethO.close();
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			getline(inFileNinethO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_NinethO_SetB; j++){
				
				inFileNinethO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(NinethO_HistoryFilePath, log_directory_prefix_SetB);
				strcat(NinethO_HistoryFilePath, Model_char);
				strcat(NinethO_HistoryFilePath, "/");
				strcat(NinethO_HistoryFilePath, HistoryFileName);
				cout<<"NinethO History file path: "<<NinethO_HistoryFilePath<<endl;
				
				inFileHistoryNinethO.open(NinethO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryNinethO, history_header1);
				getline(inFileHistoryNinethO, history_header2);
				getline(inFileHistoryNinethO, history_header3);
				getline(inFileHistoryNinethO, history_header4);
				getline(inFileHistoryNinethO, history_header5);
				getline(inFileHistoryNinethO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryNinethO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileNinethO.close();
			
			//Make outputfile name 
			strcpy(NinethO_FullOutputFileName, NinethO_FilePrefix);
			strcat(NinethO_FullOutputFileName, outputFileName);
			strcat(NinethO_FullOutputFileName, SetB_Suffix);
			
			outFileCMD_NinethO.open(NinethO_FullOutputFileName, ios::out);
			outFileCMD_NinethO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_NinethO_SetB; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_NinethO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_NinethO.close();
			
		}
		
		//SetC
		if(i == 2){
			
			//Put together input file name
			strcpy(NinethO_FileName, NinethO_FilePrefix);
			strcat(NinethO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(NinethO_FileName, SetC_Suffix);
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileNinethO, readout)){
				NumOfLines_NinethO_SetC = NumOfLines_NinethO_SetC + 1;
			}
			
			cout<<"Number of Lines in NinethO SetC file: "<<NumOfLines_NinethO_SetC<<endl;
			
			inFileNinethO.close();
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			getline(inFileNinethO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_NinethO_SetC; j++){
				
				inFileNinethO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(NinethO_HistoryFilePath, log_directory_prefix_SetC);
				strcat(NinethO_HistoryFilePath, Model_char);
				strcat(NinethO_HistoryFilePath, "/");
				strcat(NinethO_HistoryFilePath, HistoryFileName);
				cout<<"NinethO History file path: "<<NinethO_HistoryFilePath<<endl;
				
				inFileHistoryNinethO.open(NinethO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryNinethO, history_header1);
				getline(inFileHistoryNinethO, history_header2);
				getline(inFileHistoryNinethO, history_header3);
				getline(inFileHistoryNinethO, history_header4);
				getline(inFileHistoryNinethO, history_header5);
				getline(inFileHistoryNinethO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryNinethO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileNinethO.close();
			
			//Make outputfile name 
			strcpy(NinethO_FullOutputFileName, NinethO_FilePrefix);
			strcat(NinethO_FullOutputFileName, outputFileName);
			strcat(NinethO_FullOutputFileName, SetC_Suffix);
			
			outFileCMD_NinethO.open(NinethO_FullOutputFileName, ios::out);
			outFileCMD_NinethO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_NinethO_SetC; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_NinethO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_NinethO.close();
			
			
			
		}
		
		//SetD
		if(i == 3){
			
			//Put together input file name
			strcpy(NinethO_FileName, NinethO_FilePrefix);
			strcat(NinethO_FileName, inputFileName_ForPositiveGrowthModels);
			strcat(NinethO_FileName, SetD_Suffix);
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			
			//Getting number of lines in file
			while(getline(inFileNinethO, readout)){
				NumOfLines_NinethO_SetD = NumOfLines_NinethO_SetD + 1;
			}
			
			cout<<"Number of Lines in NinethO SetD file: "<<NumOfLines_NinethO_SetD<<endl;
			
			inFileNinethO.close();
			
			inFileNinethO.open(NinethO_FileName, ios::in);
			getline(inFileNinethO, input_header);
			
			//Main loop that fills arrays
			for(int j = 1; j < NumOfLines_NinethO_SetD; j++){
				
				inFileNinethO>>Model[j]>>z[j]>>x[j]>>M[j]>>L[j]>>T[j]>>Lin_Period[j]>>Growth_rate[j]>>logT[j]>>logP[j]>>logM[j];
				
				//cout<<setw(5)<<Model[j]<<setw(20)<<z[j]<<setw(20)<<x[j]<<setw(20)<<M[j]<<setw(20)<<L[j]<<setw(20)<<T[j]<<setw(20)<<Lin_Period[j]<<setw(20)<<Growth_rate[j]<<setw(20)<<logT[j]<<setw(20)<<logP[j]<<setw(20)<<logM[j]<<endl;
				
				//Getting model number into string, and then into char
				stringstream stream;
					stream<<Model[j];
					const char* Model_char = stream.str().c_str();	
				
				
				///Get history file and then input into arrays///
				strcpy(NinethO_HistoryFilePath, log_directory_prefix_SetD);
				strcat(NinethO_HistoryFilePath, Model_char);
				strcat(NinethO_HistoryFilePath, "/");
				strcat(NinethO_HistoryFilePath, HistoryFileName);
				cout<<"NinethO History file path: "<<NinethO_HistoryFilePath<<endl;
				
				inFileHistoryNinethO.open(NinethO_HistoryFilePath, ios::in);
				//Get headers out of the way
				getline(inFileHistoryNinethO, history_header1);
				getline(inFileHistoryNinethO, history_header2);
				getline(inFileHistoryNinethO, history_header3);
				getline(inFileHistoryNinethO, history_header4);
				getline(inFileHistoryNinethO, history_header5);
				getline(inFileHistoryNinethO, history_header6);
			
				//FOR TypeII order
				if(!ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy40[j];  
					//cout<<setw(5)<<dummy1[j]<<setw(10)<<dummy2[j]<<setw(10)<<dummy3[j]<<setw(10)<<dummy4[j]<<setw(10)<<dummy5[j]<<setw(10)<<dummy6[j]<<setw(10)<<dummy7[j]<<setw(10)<<dummy8[j]<<setw(10)<<dummy9[j]<<setw(10)<<dummy10[j]<<setw(10)<<dummy11[j]<<setw(10)<<dummy12[j]<<setw(10)<<dummy13[j]<<setw(10)<<dummy14[j]<<setw(10)<<dummy15[j]<<setw(10)<<dummy16[j]<<setw(10)<<dummy17[j]<<setw(10)<<dummy18[j]<<setw(10)<<dummy19[j]<<setw(10)<<dummy20[j]<<setw(10)<<dummy21[j]<<setw(10)<<dummy22[j]<<setw(10)<<dummy23[j]<<setw(10)<<dummy24[j]<<setw(10)<<dummy25[j]<<setw(10)<<dummy26[j]<<setw(10)<<dummy27[j]<<setw(10)<<dummy28[j]<<setw(10)<<dummy29[j]<<setw(10)<<dummy30[j]<<setw(10)<<dummy31[j]<<setw(10)<<dummy32[j]<<setw(10)<<dummy33[j]<<setw(10)<<dummy34[j]<<setw(10)<<dummy35[j]<<setw(10)<<dummy36[j]<<setw(10)<<dummy37[j]<<setw(10)<<abs_mag_V[j]<<setw(10)<<abs_mag_I[j]<<setw(10)<<dummy40[j];  
				}
				
				//FOR Classical Cep order
				if(ClassicalFormat){
					inFileHistoryNinethO>>dummy1[j]>>dummy2[j]>>dummy3[j]>>dummy4[j]>>dummy5[j]>>dummy6[j]>>dummy7[j]>>dummy8[j]>>dummy9[j]>>dummy10[j]>>dummy11[j]>>dummy12[j]>>dummy13[j]>>dummy14[j]>>dummy15[j]>>dummy16[j]>>dummy17[j]>>dummy18[j]>>dummy19[j]>>dummy20[j]>>dummy21[j]>>dummy22[j]>>dummy23[j]>>dummy24[j]>>dummy25[j]>>dummy26[j]>>dummy27[j]>>dummy28[j]>>dummy29[j]>>dummy30[j]>>dummy31[j]>>dummy32[j]>>dummy33[j]>>dummy34[j]>>dummy35[j]>>dummy36[j]>>dummy37[j]>>dummy38[j]>>dummy39[j]>>dummy40[j]>>dummy41[j]>>abs_mag_V[j]>>abs_mag_I[j]>>dummy44[j]>>dummy45[j]>>dummy46[j]>>dummy47[j]>>dummy48[j]>>dummy49[j]>>dummy50[j]>>dummy51[j]>>dummy52[j]>>dummy53[j]>>dummy54[j]>>dummy55[j]>>dummy56[j]>>dummy57[j]>>dummy58[j]>>dummy59[j]>>dummy60[j]>>dummy61[j]>>dummy62[j];  
				}
				
				inFileHistoryNinethO.close();
				//cout<<"abs_mag_V: "<<abs_mag_V[j]<<endl;
				//cout<<"abs_mag_I: "<<abs_mag_I[j]<<endl;
				
			}
			
			inFileNinethO.close();
			
			//Make outputfile name 
			strcpy(NinethO_FullOutputFileName, NinethO_FilePrefix);
			strcat(NinethO_FullOutputFileName, outputFileName);
			strcat(NinethO_FullOutputFileName, SetD_Suffix);
			
			outFileCMD_NinethO.open(NinethO_FullOutputFileName, ios::out);
			outFileCMD_NinethO<<setw(5)<<"Model"<<setw(34)<<"abs_mag_V"<<setw(40)<<"abs_mag_I"<<setw(46)<<"abs_mag_V - abs_mag_I"<<endl; //Header
			
			//Loop that outputs from the arrays
			for(int j = 1; j < NumOfLines_NinethO_SetD; j++){
				
				abs_mag_V[j] = (abs_mag_V[j] - 2.5*log10(Lsun));
				abs_mag_I[j] = (abs_mag_I[j] - 2.5*log10(Lsun));
			
				outFileCMD_NinethO<<setprecision(17)<<setw(5)<<Model[j]<<setw(40)<<abs_mag_V[j]<<setw(40)<<abs_mag_I[j]<<setw(40)<<(abs_mag_V[j] - abs_mag_I[j])<<endl; //Note that 17 is the precision of a history file. This will output all numbers
				
			}
			
			outFileCMD_NinethO.close();
		
		}
		
	}
	
	//////////////////////////////////////////////////
	///This is where other modes will/can be added////
	//////////////////////////////////////////////////
	
	
	
	/// Making the CMDs with a python program ///
	
	if(MakeCMDsWithPython){
		cout<<"Creating CMDs with python program..."<<endl;
		system("python CMD_plotter.py");	
	}
	
	return 0;
}