#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>	



#define GNUPLOT "gnuplot -persist"

int fd[2];

int debug=0;

ADC_simulator(){
	pid_t pid;
	int status, in=0, x=0;
	FILE *input;
	float buf=0;
	pipe(fd);

	pid = fork();
	if(pid==0){
		close(fd[0]);
		//	input = fdopen(fd[0],"w");
		while(1){
			buf = x++%150 + x/10.0;//sin(x++/1000.0) + x/100;
			write(fd[1], &buf, sizeof(buf));
		}



		/*child data here*/
		return 1;
	}else{ close(fd[1]); }
}
int main(int argc, char **argv)
{
	FILE *gp, *input;
	int i=0, x=0;
	float buf;

	ADC_simulator();

	gp = popen(GNUPLOT,"w"); /* 'gp' is the pipe descriptor */
	if (gp==NULL)
	{
		printf("Error opening pipe to GNU plot. Check if you have it! \n");
		exit(0);
	}

	input = fopen("./data.txt", "w" );



	int j;
		fprintf(gp, "plot 'data.txt'\n");
	fprintf(gp, "set xrange [0:10]\n");
	for(j=0;j<100; j++){
		fseek(input, 0, SEEK_SET);
		for(x=0;x<10;x++){

			read(fd[0], &buf, sizeof(buf));

			fprintf(input, "%i	%f\n", x, buf);

		}







	//	fprintf(gp, "clear\n");
		fprintf(gp, "plot 'data.txt'\n");
	fclose(gp);

		printf("%i\n", j);

	sleep(1);
	}

	fclose(gp);
	return 0;
}
