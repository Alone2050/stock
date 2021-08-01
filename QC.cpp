#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#include "quickselect.c"



// Recursion depth
#define DEFAULT 0
#define MEDIAN  1
#define FINAL   2


// default quality levels
double deflenlvl = 50000.0;
double defcurlvl = 300.0;
double defunilvl = 0.00;


// Number of quality level sets
int lvlcount = 0;

// quality levels arrays
double * lenlvlarr = NULL;
double * curlvlarr = NULL;
double * unilvlarr = NULL;


// Crit Curr Array
double * ic = NULL;
double * x  = NULL;
int n = 0;


// Output file
FILE * out;               // output file pointer
char * shortfilename;     // filename without pass





// Exit with "errmes" message, and wait while press eny key
void errexit(char * errmes)
{
	puts(errmes);
	puts("\n========================================\n");
	getch();
	exit(1);
}





// Recursion find acceptible parts
// deep = 0 (DEFAULT): original current/lenght specification
// deep = 1 (MEDIAN): Current level = (1-Uniformity) * Median, lenght >= Lenght level
// deep = 2 (FINAL) : Lenght == Lenght level (FCL Specs)

double findgood(double * cur, double * pos, int count, double curlvl, double lenlvl, double unilvl, int deep)
{
	int    good    = false; // good area flag
	double goodlen = 0.0;   // summ lengt of good area
	double min;             // minimum on current good area
	double med;             // median
	int    i0;              // index of beginning of good area
	
	
	
	for (int i = 0; i < count; i++)
	{
		// start of good area
		if (!good && cur[i] >= curlvl)
		{
			good = true;
			min  = cur[i];
			i0   = i;
		}
		
		
		// continue of good area
		if (good && cur[i] >= curlvl)
		{
			if (cur[i] < min) min = cur[i];
			
			// end of acceptible part
			if (deep >= FINAL && pos[i] - pos[i0] >= lenlvl)
			{
				good = false;
				med = quick_select(cur + i0, i - i0 +1); // (i0, i) including ends
				
				if ( (med - min)/med  <= unilvl )
				{
					printf("\t%5.1lf - %5.1lf = %5.1lfm   Uni: %4.1lf%%\n", pos[i0]/1000.0, pos[i]/1000.0, (pos[i]-pos[i0])/1000.0, 100.0*(med-min)/med);
					
					goodlen += pos[i] - pos[i0];
				}
				else 
				{
					i = i0+100;
					continue;
				}
			}
		}
		
		
		
		// end of good area
		if (good && ( cur[i] < curlvl || i+1 == count))
		{
			good = false;
			
			//if ( pos[i]-pos[i0] >= lenlvl )	
			if ( floor(pos[i])-ceil(pos[i0]) >= lenlvl )
			{
				med = quick_select(cur + i0, i - i0); // (i0, i-1), exclude cur[i]
				
				if ( unilvl == 0 ) 
				{
					goodlen += pos[i] - pos[i0]; // no uniform specs
					//printf("\t%5.1lf - %5.1lf = %5.1lfm\n", pos[i0]/1000.0, pos[i]/1000.0, (pos[i]-pos[i0])/1000.0);
					//printf(      "%.1lf\t%.1lf\t%.3lf - %.3lf\n", curlvl, (pos[i]-pos[i0])/1000.0, pos[i0]/1000.0, pos[i]/1000.0);
					printf(      "%.0lf\t%.0lf\t%.0lf\t%.0lf\n",  curlvl, floor(pos[i]/1000.0)-ceil(pos[i0]/1000.0), ceil(pos[i0]/1000.0), floor(pos[i]/1000.0));
					fprintf(out, "%.0lf\t%.0lf\t%.0lf\t%.0lf\n",  curlvl, floor(pos[i]/1000.0)-ceil(pos[i0]/1000.0), ceil(pos[i0]/1000.0), floor(pos[i]/1000.0));
					//fprintf(out, "%.0lf\t%.0lf\t%.0lf\t%.0lf\n", curlvl, (floor(pos[i])-ceil(pos[i0]))/1000.0, ceil(pos[i0])/1000.0, floor(pos[i])/1000.0);
				}
				else
				{
					if ( deep == DEFAULT )
						goodlen += findgood(cur+i0, pos+i0, i-i0, (1-unilvl)*med, lenlvl, unilvl, deep+1);
					
					if ( deep == MEDIAN )
					{
						goodlen += findgood(cur+i0, pos+i0, i-i0, curlvl, lenlvl, unilvl, deep+1);
						printf("\t>> %5.1lf - %5.1lf = %5.1lfm   Uni: %4.1lf%%\n\n", pos[i0]/1000.0, pos[i]/1000.0, (pos[i]-pos[i0])/1000.0, 100.0*(med-min)/med);
					}
					
					if ( deep == FINAL ) printf("ERROR: FINAL!\n");
				}
			}
		}
	}
	
	return goodlen;
}



// MAIN
int main(int argc, char ** filename)
{
	puts("========================================\n");
	puts("Quality Calculator\n");
	
	
	if (argc == 1) errexit("To use QCalc please add files");
	
	
	
	// Programm pass
	char defpass[1024], pass[1024];
	strcpy(defpass, filename[0]);
	* ( 1 + strrchr(defpass, '\\') ) = '\0';

	
	
	// Open Levels.txt file
	sprintf(pass, "%sLevels.txt", defpass);
	FILE * flvl = fopen(pass, "rt");
	if ( flvl != NULL ) 
	{
		lvlcount = 0;
		fscanf(flvl, "%*s%*s%*s");
		double cur, len, uni;
		
		while ( fscanf(flvl, "%lf%lf%lf", &cur, &len, &uni) != EOF)
		{
			lvlcount ++;
			
			curlvlarr = (double *) realloc (curlvlarr, sizeof(double) * lvlcount );
			lenlvlarr = (double *) realloc (lenlvlarr, sizeof(double) * lvlcount );
			unilvlarr = (double *) realloc (unilvlarr, sizeof(double) * lvlcount );
			
			curlvlarr[lvlcount-1] = cur;
			lenlvlarr[lvlcount-1] = len * 1000.0;
			unilvlarr[lvlcount-1] = uni / 100.0;
		}
		
		fclose(flvl);
	}
	
	// if file doesn't exists, or empty, use default levels
	if ( lvlcount == 0 )
	{
		lvlcount = 1;
		
		curlvlarr = (double *) malloc ( sizeof(double) * 1 );
		lenlvlarr = (double *) malloc ( sizeof(double) * 1 );
		unilvlarr = (double *) malloc ( sizeof(double) * 1 );
		
		curlvlarr[0] = defcurlvl;
		lenlvlarr[0] = deflenlvl;
		unilvlarr[0] = defunilvl;
	}	
	

	
	
	
	// Calculation parameters
	FILE * f;                 // f - input file, out - output file
	
	double cur;               // current current
	double pos;               // current pos, pos0 - beginning of good, start - beginning of sample
	double len, goodlen;      // len - whole smaple lenght, goodlen - summ good lenght
	

	int i0;                   // start of good area
	int good;                 // boolean

	//char * shortfilename;     // filename without pass
	char outfilename[1024];   // output filename: "QC_filename"
	
	#define curlvl curlvlarr[lvl]
	#define lenlvl lenlvlarr[lvl]
	#define unilvl unilvlarr[lvl]
	
	double med;               // Median
	double min;               // Minimum
	
	
	//FILE * out2 = fopen("list.dat", "wt"); // list
	//fprintf(out2, "Name\tCur\tLen\tUni\tYield\tPCS\n");
	
	// main cycle for every input file
	for (int filen = 1; filen < argc; filen++)
	{
		// open input file
		f = fopen (filename[filen], "rt");
		if (f == NULL)
		{
			printf("Can't open %s\n", filename[filen]);
			continue;
		}
		
		
		// short filename
		shortfilename = strrchr( filename[filen], '\\' );
		if ( shortfilename == NULL ) shortfilename = filename[filen];
		else shortfilename ++; 
		printf("----------------------------------------\n");
		printf("%d.%s\n\n", filen, shortfilename);
		
		
		
		// read file to array
		n = 0;
		while ( fscanf(f, "%lf%lf", &pos, &cur) != EOF)
		{
			n ++;
			ic = (double *) realloc( ic, sizeof(double) * n );
			x  = (double *) realloc( x,  sizeof(double) * n );
			
			ic[n-1] = cur;
			x [n-1] = pos;
		}
		
		
		// close input file
		fclose(f);
		
		
		// open output file
		sprintf(outfilename, "QC_%s", shortfilename); 
		
		out = fopen(outfilename, "wt");
		if (out == NULL)
		{
			printf("Can't create output file\n");
			continue;
		}
		
		
		
		
		
		// Calculations
		// for every level set
		fprintf(out, "Current\tLength\tStart\tEnd\n");
		double yield;
		for (int lvl = 0; lvl < lvlcount; lvl++)
		{
			printf("%.0lfA x %.0lfm : %.0lf%%\n", curlvl, lenlvl/1000.0, unilvl*100.0); // actual quality levels
			
			goodlen = findgood(ic, x, n, curlvl, lenlvl, unilvl, DEFAULT);
			len = x[n-1] - x[0];
			
			//printf("\n\tGOOD\t%.1lf / %.1lf = %.1lf%%", goodlen/1000.0, len/1000.0, 100.0*goodlen/len);
			fprintf(out, "\n");
			if (unilvl == 0) printf("\n");
			else 
			{
				printf("  (%d pcs)\n", (int)(goodlen/lenlvl));
			}
			puts("");
		}
		
		fclose(out);
	}
	
	
	//fclose(out2); // list
	puts("========================================\n");
	puts("Press any key to Exit\n");
	getch();

	
	return 0;
}
