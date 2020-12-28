## To Run with python: cd to directory, python FileName.py. Takes history_mainset_modelnumber, outputs plotname_mainset_modelnumber

# V2: Changed inverter from int to boolean, changed the input file name, set marker color to red, fixed indentation issues
# V3: Me, changing program from HR_plotter to a period/GREKM/light curve plotter for non linear data 
###NEW PROGRAM

# V1: Made of off FullAmpChecker to make light curve plots automatically
###NEW PROGRAM

# V1: Templet for CMD_FileMakerV3.cpp. C program will use and edit this templet to make program that it will run. Also made change for Y-axis invertion

# V2: Adding Type and data location controls to make use easier (also to make gettting I,V-I easy)

# import libraries

import numpy as np
import matplotlib.pyplot as plt

### Controls ###

Mode = 'NinthO'

CMDdata_FileName = '_CMDdataClassical_Set' #Prefix to input CMD data file. Suffix is Set.dat (example: FU_CMDdata_Set as prefix and A.dat as suffix)

StarType = 'Classical' #Type of star being analyized. Will use this for title to plot and output file for plot.
xTitle = 'M_V - M_I (Mags)' #Label for xaxis. 'M_V - M_I (Mags)' usually
yTitle = 'M_I (Mags)' #Label for yaxis. 'M_V (Mags)' or 'M_I (Mags)'

DataLocationFor_x = 3 #Location in file for x value. Note, starts at 0. 
DataLocationFor_y = 2 #Location in file for y value. Note, starts at 0. 2 for I, 1 for V

# create array to iterate through convection sets

sets = ['C', 'D'] #'A', 'B', 'C', 'D'

# variable to keep y axis inverted

inverter = True #Made False for NonLinear analysis


for i in sets:
	
    # grab data from file

    data = np.loadtxt(Mode + CMDdata_FileName + i + '.dat', skiprows=1) 
	
	### CMD Stuff ###
	
	# This seems to go X then Y column, starts at 0
    plt.plot(data[:, DataLocationFor_x], data[:, DataLocationFor_y], 'r.', markersize=1)  #Plotting
    plt.xlabel(xTitle)
    plt.ylabel(yTitle)
    plt.suptitle(Mode + ' mode Set ' + i + ' CMD ' + StarType)

    # invert x axis once

    if inverter:

        # invert y axis

        plt.gca().invert_yaxis()
        #inverter = False

    # show the plot
    # plt.show()

    # save file

    plt.savefig(Mode + '_Set' + i + '_' + StarType + 'CMD.png')
    plt.clf()
	