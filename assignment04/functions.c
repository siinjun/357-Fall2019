#include "header.h"

void extract(int readfile, char *name){
	char buff[512];
	char namebuf[256];
	char size[12];
	int remainder = 0;
	int amount = 0;
	int numblocks = 0;
	int i, j;

	memset(buff, 0 ,512);
	memset(namebuf, 0 , 256);

	if(name != NULL){
		/*read header first*/
		while(read(readfile, buff, 512)>0){
			/*printf("header: %s\n", buff);*/
			memset(namebuf, 0 ,256);

			/*read name first*/
			for(i = 0; i< 100; i++){
				namebuf[i] = buff[i];
			}

			/*handle if name longer than 100 char*/
			if(namebuf[99]!= '\0'){
				j = 100;
				i = 345;

				while(buff[i] != '\0' && i< 500){
					namebuf[j] = buff[i];
					i++;
					j++;
				}

				namebuf[j] = '\0';
			}
			/*printf("%s\n",namebuff);*/
			/*printf("%s\n",name);*/

			for(i = 0, j = 124; i<12; i++, j++){
				size[i] = buff[j];
			}

			/*octtodec = turns oct to dec*/
			/*find out how much need to write*/
			amount = octtodec(atoi(size));

			if(namebuf[0]!= 0 && strcmp(name,namebuf)== 0){
				/*printf("%s\n",namebuff);*/
				createnewfile(buff, namebuf, readfile, amount);
			}
			else{
				lseek(readfile, amount, SEEK_CUR);
			}

			 numblocks = amount / 512;
			 remainder = amount % 512;

			if(remainder!=0){
				numblocks++;
			}

			/*still need to read the rest of stuff if over 512*/
			memset(buff, 0 ,512);
			lseek(readfile, numblocks * 512 - amount, SEEK_CUR);
		}


	}else{
	while(read(readfile, buff, 512)>0){
		/*printf("header: %s\n", buff);*/
		memset(namebuf, 0 ,255);
		for(i = 0; i< 100; i++){
			namebuf[i] = buff[i];
		}
		if(namebuf[99]!= '\0'){
			i = 345;
			j = 100;
			while(buff[i] != '\0' && i< 500){
				namebuf[j] = buff[i];
				i++;
				j++;
			}
			
			namebuf[j] = '\0';
			
		}
		for(i = 0, j = 124; i<12; i++, j++){
			size[i] = buff[j];
		}
		amount = octtodec(atoi(size));
		if(namebuf[0]!= 0){
			createnewfile(buff, namebuf, readfile, amount);
		}
		 numblocks = amount / 512;
		 remainder = amount % 512;

		if(remainder!=0){
			numblocks++;
		}

		memset(buff, 0 ,512);
		lseek(readfile, numblocks * 512-amount, SEEK_CUR);
		}
	}
}

/*makes new thing to write to*/
void createnewfile(char buff[512], char namebuf[256],
                    int readfile, int amount){
	int newfile;
	char ch;
	int curr = 0;
	char mod[8];
	char *ptr = NULL;
	mode_t mode = 0;
	int i = 100;
	int j = 0;
	while(buff[i] != 0){
		mod[j] = buff[i];
		i++;
		j++;
	}
	mod[j] = 0;		
	mode = (mode_t)strtol(mod, &ptr, 8);
	newfile = open(namebuf, O_WRONLY | O_CREAT | O_TRUNC, mode);
	while(curr < amount){
		read(readfile, &ch, 1);
		write(newfile, &ch, 1);
		curr++;
	}
}

/*turns oct to dec for size*/
int octtodec(int n){
	int num = n;
	int dec = 0;
	int base = 1;
	int temp = num;
	int last = 0;
	while(temp){
		last = temp % 10;
		temp = temp / 10;
		dec += last * base;
		base = base * 8;
	}
	return dec;
}
