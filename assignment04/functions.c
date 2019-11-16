#include "mytar.h"
#include "header.h"

void extract(int readfile, char *name){
	char *namebuf;
	char *size;
	int amount = 0;
	int i, j;
	struct header head;

	/*read header first*/
	read(readfile, &head, sizeof(struct header));
	lseek(readfile, 12, SEEK_CUR);

	if(head.prefix[0] != 0){
		namebuf = strncpy(namebuf, head.prefix, 155);
		strcat(namebuf, "/");
		strcat(namebuf, head.name);
	}
	else{
		namebuf = strdup(head.name);
	}

	size = strdup(head.size);

	/*octtodec = turns oct to dec*/
	/*find out how much need to write*/
	amount = octtodec(atoi(size));

	if(name != NULL){
		/*while loop should check namebuf if it is the correct file or not*/
		/*if not correct one, move on to next file*/
		while(strcmp(name,namebuf) != 0){
			lseek(readfile, amount, SEEK_CUR);
			lseek(readfile, 512 - amount % 512, SEEK_CUR);

			/*move on to the next file*/
			read(readfile, &head, sizeof(struct header));
			lseek(readfile, 12, SEEK_CUR);

			if(head.prefix[0] != 0){
					namebuf = strncpy(namebuf, head.prefix, 155);
					strcat(namebuf, "/");
					strcat(namebuf, head.name);
			}
			else{
				namebuf = strdup(head.name);
			}
			size = strdup(head.size);
			amount = octtodec(atoi(size));

			/*printf("header: %s\n", buff);*/
			/*read name first*/
			/*for(i = 0; i< 100; i++){
				namebuf[i] = buff[i];
			}*/
			/*handle if name longer than 100 char*/
			/*if(namebuf[99]!= '\0'){
				j = 100;
				i = 345;
				while(buff[i] != '\0' && i< 500){
					namebuf[j] = buff[i];
					i++;
					j++;
				}
				namebuf[j] = '\0';
			}*/
			/*printf("%s\n",namebuf);*/
			/*printf("%s\n",name);*/
			/*for(i = 0, j = 124; i<12; i++, j++){
				size[i] = buff[j];
			}*/
		}
		createnewfile(readfile, namebuf, amount);


	}else{
		while(namebuf[0] != 0){
			/*writing contents of new file*/
			createnewfile(readfile, namebuf, amount);
			lseek(readfile, 512 - amount % 512, SEEK_CUR);

			/*getting new info*/
			/*get to new header first*/
			read(readfile, &head, sizeof(struct header));
			lseek(readfile, 12, SEEK_CUR);

			if(head.prefix[0] != 0){
					namebuf = strncpy(namebuf, head.prefix, 155);
					strcat(namebuf, "/");
					strcat(namebuf, head.name);
			}
			else{
				namebuf = strdup(head.name);
			}
			size = strdup(head.size);
			amount = octtodec(atoi(size));

			/*checks first time if namebuf is empty*/
			/*if namebuf is empty it checks the next block
			to see if that's empty too*/
			if(namebuf[0] == 0){
				read(readfile, &head, sizeof(struct header));
				lseek(readfile, 12, SEEK_CUR);

				if(head.prefix[0] != 0){
						namebuf = strncpy(namebuf, head.prefix, 155);
						strcat(namebuf, "/");
						strcat(namebuf, head.name);
				}
				else{
					namebuf = strdup(head.name);
				}
				size = strdup(head.size);
				amount = octtodec(atoi(size));
			}


			/*if(namebuf[0]!= 0){
				createnewfile(buff, namebuf, readfile, amount);
			}

			 numblocks = amount / 512;
			 remainder = amount % 512;

			if(remainder!=0){
				numblocks++;
			}

			memset(buff, 0 ,512);
			lseek(readfile, numblocks * 512-amount, SEEK_CUR);*/
		}
	}
}

/*makes new thing to write to*/
void createnewfile(char *namebuf, int readfile, int amount){
	int newfile, count;
	char ch;

	newfile = open(namebuf, O_WRONLY | O_CREAT | O_TRUNC);
	while(count < amount){
		read(readfile, ch, 1);
		write(newfile, ch, 1);
		count++;
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