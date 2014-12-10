#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>	
#include <strings.h>
#include <ctype.h>
#include <getopt.h>

#include "fft.c"

#define GNUPLOT "gnuplot -persist"

int fd[2];

int debug=0;

ADC_simulator(){
	pid_t pid;
	int status, in=0; 
	FILE *input;
	float buf=0, x=0;
	pipe(fd);

	pid = fork();
	if(pid==0){
		close(fd[0]);
		//	input = fdopen(fd[0],"w");
		while(1){
			x += 2;
			buf = ((int)x%3) + ((int)x%5);//sinf((float) (x++/(2000.0)));
			write(fd[1], &buf, sizeof(buf));
		}



		/*child data here*/
		return 1;
	}else{ close(fd[1]); }
}
int main(int argc, char **argv)
{
	FILE *gp, *input;
	int i=0, x=0, o;
	float buf;
	int j;
	double re[4096], im[4096];




	gp = popen(GNUPLOT,"w"); /* 'gp' is the pipe descriptor */
	if (gp==NULL)
	{
		printf("Error opening pipe to GNU plot. Check if you have it! \n");
		exit(0);
	}

	input = fopen("./data.txt", "w" );



	//	while((o = getopt(argc, argv, "tr") != -1))

	//		switch(o){

	//			case 't':

	printf("Test initialted!\n");
	ADC_simulator();

	//				break;
	//			default:
	//				break;

	//		}


	fprintf(gp, "plot 'data.txt'\n");
	fprintf(gp, "set xrange [0:4000]\n");
	for(j=0;j<1; j++){
		fseek(input, 0, SEEK_SET);
		for(x=0;x<4096;x++){

			read(fd[0], &buf, sizeof(buf));
			re[x] = (double) buf;
			im[x] = (double) 0;
			fprintf(input, "%i	%f\n", x, buf);


		}

		if(1){
			printf("FFT in\n");
			FFT(1, 12, re, im);

			printf("FFT out\n");

			fseek(input, 0, SEEK_SET);
			for(x=0;x<4096;x++){

				fprintf(input, "%i	%f\n", x, re[x]);

			}
		}



		//	fprintf(gp, "clear\n");
		fprintf(gp, "plot 'data.txt' linetype 5\n");

		printf("%i\n", j);


	}

	fclose(gp);
	return 0;
}
