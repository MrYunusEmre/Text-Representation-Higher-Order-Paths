#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <dirent.h>
#include <math.h>

/*
	Yunus Emre Ertunç 
	150117064

*/

//Every dataset ,we have, works correctly but big dataset can take a little much time.


//This struct holds our MLL datas and txt datas.
struct terms{
	char term[40];
	int txt;
	int doc;
	int header;
	int goTxt;
	int occ[10][2];
	int occurance;
	int id;
	struct terms *fnext;
	struct terms *next;
};


typedef struct terms terms;
typedef terms *termPtr;

//This struct holds our word pairs 
struct pairs{
	char word1[40];
	char word2[40];
	int order;
	int id;
	struct pairs *next;	
};

typedef struct pairs pairs;
typedef pairs *pairPtr;


//This struct hols our term frequency info
struct tf{
	char term[40];
	int tf;
	double idftf;
	int doc;
	int n;
	int id;
	struct tf *next;
	
};

typedef struct tf tfs;
typedef tfs *tfPtr;


void createFileName(char fname[200], char name[40]);
void insert(termPtr *sPtr, char word[40],int doc,int txt,int header);
int checkList(termPtr currentPtr, char word[40]);
int checkFList(termPtr currentPtr, char word[40]);
void printMLL(termPtr currentPtr);
void printFOL(termPtr currentPtr, pairPtr *startPtr);
void printSOL(termPtr currentPtr,pairPtr *startPtr);
void printTOL(termPtr currentPtr,pairPtr *sPtr);
void printThird(termPtr startPtr ,termPtr head1,termPtr head2 ,char word1[40],pairPtr *sPtr);
void printthrd(termPtr head1,termPtr head3,char word[40],char word1[40],pairPtr *sPtr);
void printsec(termPtr head1,termPtr head2,char word[40],pairPtr *sPtr);
void printArtik(pairPtr sPtr);
void insertLastList(pairPtr *sPtr, char word1[40], char word2[40],int orderInfo);
int karsilastir(termPtr list1, termPtr list2,char word[40]);
int checkLastTime(pairPtr sPtr , char word1[40], char word2[40]);
void miniInsert(termPtr *sPtr,termPtr currentPtr);
void findPosition(termPtr currentPtr , termPtr *iter, int doc, int txt, int header);
int compareWords(char word1[40], char word2[40]);


int main(){
	
	setlocale(LC_ALL,"Turkish");
	
	char link[200] = "C:\\Users\\Yunus Emre\\Desktop\\small dataset2";
	
	//There are 2 structs from DIR library to read all documents no matter how many there are.
	
	struct dirent *de;
	struct dirent *dd;
	DIR *dr = opendir(link);
	DIR *doc;
	FILE *inpf;
	
	char word[40];
	
	
	termPtr startPtr = NULL;
	termPtr iter = NULL;
	
	
	if(dr == NULL){
		printf("Could not open current directory");
		return 0;
	}
	
	int docNum = 1;
	int txtNum = 0;
	int header = 0;
	int x =1;
	int k = 1;
	int txtInfo = 0;
	int docInfo = 0;
	
	while((de = readdir(dr)) ){
		
		if(!strcmp(de->d_name, "."))
		continue;
		if(!strcmp(de->d_name, ".."))
		continue;
		
		char fname[200];
		
		strcpy(fname,link);
	
		
		createFileName(fname,de->d_name);
		
		doc = opendir(fname);
		
		if(dr == NULL){
			printf("Could not open current directory.\n");
			return 0;	
		}
		
		txtNum = 0;
		while(dd = readdir(doc)){ // This while loop determines the documents
		
			
			if(!strcmp(dd->d_name, "."))
			continue;
			if(!strcmp(dd->d_name, ".."))
			continue;
			
			createFileName(fname,dd->d_name);

			inpf = fopen(fname,"r");   
			
			if(inpf == NULL){
				printf("Could not open current directory.\n");
				return 0;
			}
			
			txtNum++;
					
			while(fscanf(inpf,"%s",word) == 1){ //Each word is catched in this while loop and added into MLL 
			
				if(x == 1){
					header = 1;
				}
				else{
					header = 0;
				}
				
				if(checkList(startPtr,word) == 0){    // If word is already added into MLL 
				
				
					updateWord(&startPtr,word,docNum);
				
				
					termPtr temp = startPtr;
					
					if(checker(startPtr,txtNum,docNum) == 0 && header == 1){ //If word is located at the beginning of txt 
					termPtr newPtr = malloc(sizeof(terms));
					
					if(newPtr != NULL){
					
						strcpy(newPtr->term,word);
						newPtr->doc = docNum;
						newPtr->txt = txtNum;
						newPtr->goTxt = txtNum;
						newPtr->header = header;
						newPtr->fnext = NULL;
						newPtr->next = NULL;
						
						findPosition(startPtr, &iter, docNum, txtNum ,1);
						
						while(iter->fnext != NULL){
							iter = iter->fnext;
						}
												
						iter->fnext = newPtr;						
					}
					}				
					
					findPosition(startPtr,&iter,docNum,txtNum,1);
										
					if(checkFList(iter,word) != 0){
						
						termPtr newPtr = malloc(sizeof(terms));
						
						if(newPtr != NULL){
							strcpy(newPtr->term,word);
							newPtr->doc = docNum;
							newPtr->txt = txtNum;
							newPtr->goTxt = 0;
							newPtr->header = header;
							newPtr->fnext = NULL;
							newPtr->next = NULL;
							
							termPtr temp = iter;
							
							
							while(iter->fnext != NULL){
								iter = iter->fnext;
							}
														
							iter->fnext = newPtr;
							
									
						}
						
					}
					
					
					k = 0;
												
				}
				else{   //If word is not added yet into the MLL
					insert(&startPtr,word,docNum,txtNum,header);
					
					
					findPosition(startPtr,&iter,docNum,txtNum,1);
					
					if(checkFList(iter,word) != 0){
						
						termPtr newPtr = malloc(sizeof(terms));
						
						if(newPtr != NULL){
							strcpy(newPtr->term,word);
							newPtr->doc = docNum;
							newPtr->txt = txtNum;
							newPtr->goTxt = 0;
							newPtr->header = header;
							newPtr->fnext = NULL;
							newPtr->next = NULL;
							
							
							while(iter->fnext != NULL){
								iter = iter->fnext;
							}
							
							iter->fnext = newPtr;
							
						}						
					}					
				}
	
			if(k != 0)					
			x = 0;

			k = 1;
					
			}
		x = 1;
	
		}		
		closedir(doc);
		txtNum = 0;
		docNum++;
	}
	closedir(dr);	
		
	//RepairList and PurgeList functions are created for the situation , which the word is located at the beginning of the txt.
	repairList(&startPtr);
	purgeList(&startPtr);	


	pairPtr sPtr = NULL;

	//Relationship between the words are found in these three functions.
	printFOL(startPtr,&sPtr);

	printSOL(startPtr,&sPtr);

	printTOL(startPtr,&sPtr);	

	tfPtr tPtr = NULL;
	
	//The most frequent words are added into the tf struct
	tf(&startPtr,&tPtr);

	printLastTime(&sPtr,startPtr);
	
	printTfList(&tPtr,startPtr);
	
}





//MLL and other branchs are created in here
void insert(termPtr *sPtr, char word[40],int doc,int txt,int header){
	
	termPtr newPtr = malloc(sizeof(terms));
	
	if(newPtr != NULL){
		
		strcpy(newPtr->term,word);
		newPtr->doc = doc;
		newPtr->txt = txt;
		newPtr->occurance = 1;
		newPtr->id = 0;
		newPtr->header = header;
		
		int i = 0;
		int j = 0;
		
		for(i = 0; i <10; i++){
			for(j = 0; j< 2 ; j++){
				newPtr->occ[i][j] = 0;
			}
		}
	
		newPtr->occ[doc-1][0] = 1;
		newPtr->occ[doc-1][1] = 1;
		
		newPtr->fnext = NULL;
		newPtr->next = NULL;
		
		termPtr previousPtr = NULL;
		termPtr currentPtr = *sPtr;
		
		while(currentPtr != NULL) {
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}
		
		if(previousPtr == NULL){
			newPtr->next = *sPtr;
			*sPtr = newPtr;
		}
		else{
			previousPtr->next = newPtr;
			newPtr->next = currentPtr;
		}
		
		
		
	}
	else{
		printf("Node oluþturulamadý.\n");
		return;
	}	
	
}

void repairList(termPtr *sPtr){
	
	termPtr currentPtr = *sPtr;
	termPtr iter = currentPtr->next;
	termPtr temp = currentPtr;
	termPtr head1;
	termPtr head2;
	termPtr itertemp = iter;
	termPtr lastTemp;
	while(currentPtr != NULL){
		itertemp = iter;
		if(currentPtr->header == 1){
			head1 = currentPtr;
		}
		else{
			currentPtr = currentPtr->next;
			if(currentPtr != NULL)
			iter = currentPtr->next;
			continue;
		}
		
		while(head1 != NULL){
			
			if(head1->goTxt != 0){ 
				iter = currentPtr->next;
				
				while(iter != NULL){
					if(iter->header == 1 && head1->goTxt == iter->txt){
						head2 = iter;
						lastTemp = head2;
					}
					else{
						iter = iter->next;
						continue;
					}
					
					while(head2->fnext != NULL){
						
						head2 = head2->fnext;
					}
					
					termPtr newPtr = malloc(sizeof(terms));
					if(newPtr != NULL){
						strcpy(newPtr->term,head1->term);
						newPtr->txt = head1->goTxt;
						newPtr->doc = head1->doc;
						newPtr->goTxt = 0;
						newPtr->next = NULL;
						newPtr->fnext = NULL;
						
					}
					if(checkFList(lastTemp,head1->term) != 0)
					head2->fnext = newPtr;
					iter = iter->next;
				}
								
			}
			else{
				head1 = head1->fnext;
				continue;
			}
			head1 = head1->fnext;
		}
		
		currentPtr = currentPtr->next;
		if(currentPtr != NULL)
		iter = currentPtr->next;
	}
	
}

void purgeList(termPtr *sPtr){
	
	termPtr currentPtr = *sPtr;
	termPtr iter = currentPtr;
	termPtr head = NULL;
	termPtr temp = NULL;
	termPtr head1 = NULL;
	
	while(currentPtr != NULL){
		
		if(currentPtr->header == 1){
			head = currentPtr;
			iter = head;
			
			while(iter != NULL){
				
				if(iter->goTxt != 0 ){
					head = currentPtr;
					while(head->fnext != iter){
						head = head->fnext;
					}
					temp = iter;
					head->fnext = head->fnext->fnext;
					free(temp);
				}
				
				iter = iter->fnext;
			}
			
			
			
		}
		else{
			currentPtr = currentPtr->next;
			continue;
		}
		
		
		currentPtr = currentPtr->next;		
	}
	
	
	
}

//This function checks the list if the word is already added or not.
int checkList(termPtr currentPtr, char word[40]){
	
		while(currentPtr != NULL){
			if(strcmp(currentPtr->term,word) == 0){
				return 0; // which means they are same
			}
			else{
				currentPtr = currentPtr->next;
				continue;
			}
		}
	
	return 1;
	
}

int checkFList(termPtr currentPtr, char word[40]){
	
		while(currentPtr != NULL){
			if(strcmp(currentPtr->term,word) == 0){
				return 0; // which means they are same
			}
			else{
				currentPtr = currentPtr->fnext;
				continue;
			}
		}
	
	return 1;
	
}

void printMLL(termPtr currentPtr){
	
	if(currentPtr == NULL){
		puts("List is Empty.\n");
	}
	else{
		puts("The list is: ");
		
		while(currentPtr != NULL){
			printf("%s --> %d\n",currentPtr->term,currentPtr->txt);
			currentPtr = currentPtr->next;
		}
	}
	
	puts("NULL\n");
}

void printFOL(termPtr currentPtr, pairPtr *startPtr){
	
	termPtr iter = currentPtr->fnext;
	termPtr sPtr = currentPtr;
	
	while(currentPtr != NULL){
		
		if(currentPtr->header == 1){
			
			while(sPtr != NULL){
				while(iter != NULL){
			//		printf("{%s , %s} - ",sPtr->term,iter->term);
					insertLastList(&(*startPtr),sPtr->term,iter->term,1);
					iter = iter->fnext;
				}
			//	printf("\n");
				sPtr = sPtr->fnext;
				if(sPtr != NULL)
				iter = sPtr->fnext;
			}			
		}
				
	//	printf("\n\n");
		currentPtr = currentPtr->next;
		sPtr = currentPtr;
		if(currentPtr != NULL)
		iter = currentPtr->fnext;
	}
	
	
	
}

void printSOL(termPtr currentPtr,pairPtr *startPtr){
	
	
	termPtr iter = currentPtr->next;
	termPtr head1 = NULL;
	termPtr head2 = NULL;
	char word[40];
	
	while(currentPtr->next->next != NULL){
		
		if(currentPtr->fnext != NULL){
			head1 = currentPtr;
			
			while(iter != NULL){
			
				if(iter->fnext != NULL){
					head2 = iter;
					
					if(karsilastir(head1,head2,word) == 0){
						printsec(head1,head2,word,&(*startPtr));
					}
									
				}				
				iter = iter->next;			
			}	
								
		}
		
		currentPtr = currentPtr->next;
		iter = currentPtr->next;
			
	}

	
}

void printTOL(termPtr currentPtr,pairPtr *sPtr){
	
	termPtr iter = currentPtr->next;
	termPtr head1 = NULL;
	termPtr head2 = NULL;
	termPtr cPtr = currentPtr;
	char word[40];
	
	while(currentPtr->next->next != NULL){
		
		if(currentPtr->fnext != NULL){
			head1 = currentPtr;
			
			while(iter != NULL){
			
				if(iter->fnext != NULL){
					head2 = iter;
					
					if(karsilastir(head1,head2,word) == 0){
						printThird(cPtr,head1,head2,word,&(*sPtr));
					
					}
				
					
				}
				
				iter = iter->next;
			
			}	
					
			
		}
		

		currentPtr = currentPtr->next;
		iter = currentPtr->next;
	
		
	}
	
	

}

void printThird(termPtr startPtr ,termPtr head1,termPtr head2 ,char word1[40],pairPtr *sPtr){
	
	termPtr head3 = NULL;
	char word[40];
	
	while(startPtr != NULL){
		
		if(startPtr->fnext != NULL){
			
			if(strcmp(head1->term,startPtr->term) == 0 || strcmp(head2->term,startPtr->term) == 0){
				startPtr = startPtr->next;
				continue;
			}
			
			head3 = startPtr;
			
			if(karsilastir(head2,head3,word) == 0){
				printthrd(head1,head3,word,word1,&(*sPtr));
			}
						
			
		}
		
		startPtr = startPtr->next;
	}
	
	
}

void printthrd(termPtr head1,termPtr head3,char word[40],char word1[40],pairPtr *sPtr){
	
	termPtr temp = head3;
	
	while(head1 != NULL){
	
		if(strcmp(head1->term, word) != 0){
		head3 = temp;
		while(head3 != NULL){
			
			if(strcmp(head3->term,word) != 0 && strcmp(head1->term,word1) != 0){
		//	printf("\nOrtak Kelime 1 : %s, Ortak Kelime 2 : %s, head1->doc: %d Head1->txt : %d , head3->doc : %d head3->txt %d\n\n", word1,word, head1->doc,head1->txt,head3->doc,head3->txt);
			if(strcmp(head1->term,head3->term) != 0)	
	//		printf("{%s , %s} - ",head1->term,head3->term);
			insertLastList(&(*sPtr),head1->term,head3->term,3);
		}
			head3 = head3->fnext;
		}
		}
	//	printf("\n\n");
		head1 = head1->fnext;
	}
	
	
}

void printsec(termPtr head1,termPtr head2,char word[40],pairPtr *sPtr){
	
	termPtr temp = head2;
	
	while(head1 != NULL){
	
		if(strcmp(head1->term, word) != 0){
			head2 = temp;
		while(head2 != NULL){
			
			if(strcmp(head2->term,word) != 0){
		//	printf("\nOrtak Kelime : %s , head1->doc: %d Head1->txt : %d , head2->doc : %d head2->txt %d\n\n", word,head1->doc,head1->txt,head2->doc,head2->txt);

			insertLastList(&(*sPtr),head1->term,head2->term,2);
		}
			head2 = head2->fnext;
		}
		}
	//	printf("\n\n");
		head1 = head1->fnext;
	}
	
	
}

void printArtik(pairPtr sPtr){
	
	int k = 0;
	int l = 0;
	int m = 0;
	
	while(sPtr != NULL){
	
			if(sPtr->order == 1){
				if(k == 0)
				printf("\nFirst-Order List is : \n");
				
				k = 1;
			}
				
			if(sPtr->order == 2){
				if(l == 0)
				printf("\n\nSecond-Order List is : \n");
				
				l = 1;
			}
				
		if(sPtr->order == 3){
			if(m == 0)
			printf("\n\nThird-Order List is : \n\n");
			
			m = 1;
		}
		
		printf("{%s , %s} - ", sPtr->word1,sPtr->word2);
		
			
		sPtr = sPtr->next;	
	}
	
	
	
}

void insertLastList(pairPtr *sPtr, char word1[40], char word2[40],int orderInfo){
/*	
	if(checkLastTime(*sPtr,word1,word2) == 0){
		return;
	}
*/	
	pairPtr newPtr = malloc(sizeof(pairs));
	
	if(newPtr != NULL){
		
		strcpy(newPtr->word1,word1);
		strcpy(newPtr->word2,word2);
		newPtr->order = orderInfo;
		newPtr->id = 0;
		newPtr->next = NULL;
		
		pairPtr previousPtr = NULL;
		pairPtr currentPtr = *sPtr;
		
		while(currentPtr != NULL) {
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}
		if(previousPtr == NULL){
			newPtr->next = *sPtr;
			*sPtr = newPtr;
		}
		else{
			previousPtr->next = newPtr;
			newPtr->next = currentPtr;
		}
		
		
		
	}
	
	
}

//This method compares the txt files to find common word between them.
int karsilastir(termPtr list1, termPtr list2,char word[40]){ 
	
	termPtr temp = list2;
	

	
	while(list1 != NULL){
		
		while(list2 != NULL){


			if(strcmp(list1->term,list2->term) == 0){
				strcpy(word,list2->term);
				return 0;
			}
			
			list2 = list2->fnext;
			
		}
		list2 = temp;
		list1 = list1->fnext;
	}

	return 1;
	
	
}

int checkLastTime(pairPtr sPtr , char word1[40], char word2[40]){
	
	
	while(sPtr != NULL){
		
		if(strcmp(sPtr->word1,word1) == 0 && strcmp(sPtr->word2,word2) == 0){
			return 0;
		}
		if(strcmp(sPtr->word1,word2) == 0 && strcmp(sPtr->word2,word1) == 0){
			return 0;
		}
		
		sPtr = sPtr->next;
	}
	
	return 1;
	
}

void findPosition(termPtr currentPtr , termPtr *iter, int doc, int txt, int header){
	
	while(currentPtr != NULL){
		
		if(currentPtr->doc == doc && currentPtr->txt == txt && currentPtr->header == header){
			*iter = currentPtr;
			break;
		}
		currentPtr = currentPtr->next;
	}
	
}

int compareWords(char word1[40], char word2[40]){
	
	int i = 0;
	int k = 0;
	for(i = 0; i < 40; i++){
		if(word1[i] == word2[i]){
			continue;
		}
		else if(word1[i] < word2[i]){
			return 1;
		}
		else return 0;
	}
	
	
	
}

//Updates the file to change the location.
void createFileName(char fname[200], char name[40]){
	int i = 0;
	int d = 0;
	for(i = 0; i <= strlen(fname); i++);

	i = i-2;
	
	if(fname[i] == 't' && fname[i-1] == 'x' && fname[i-2] == 't'){
		fname[i] = '\0';
		fname[i-1] = '\0';
		fname[i-2] = '\0';
		fname[i-3] = '\0';
		fname[i-4] = '\0';
		fname[i-5] = '\0';
		fname[i-6] = '\0';
	}

	d = i;
	i = 0;
	
	for(i = 0; i <= strlen(fname); i++);
	i = i-2;
	fname[i+1] = '\\';
	fname[i+2] = '\\';
	
	d = i;
	
	for(i = 0; i < 40; i++){
		fname[d+3] = name[i];
		d++;	
	}
	

}

int checker(termPtr startPtr,int txt,int doc){
	
	while(startPtr != NULL){
		
		if(startPtr->txt == txt && startPtr->doc == doc){
			return 1;
		}
		else{
			startPtr = startPtr->next;
			continue;
		}
		
		
	}
	
	return 0;
	
}

//Determines the docs of the word.
void updateWord(termPtr *sPtr,char word[40], int doc){
	
	termPtr temp = *sPtr;
	
	while(temp != NULL){
		
		if(strcmp(temp->term,word) == 0){
			temp->occ[doc-1][0] = 1;
			temp->occ[doc-1][1] += 1;
		}
		
		temp = temp->next;
	}
	
	
}

void tf(termPtr *sPtr,tfPtr *tPtr){
	
	termPtr iter = *sPtr;
	char word[40];
	int i = 0;
	int j = 0;
	int max = 0;
	termPtr temp = *sPtr;
	
	for(i = 0 ; i < 10; i++){
		
		for(j = 0; j < 10; j++){ 
			while(iter != NULL){
				
			if(iter->occ[i][0] == 1){
				
				if(iter->occ[i][1] > max && iter->id != 1){

					if(checkTfList(*tPtr,iter->term,i+1) != 0){
						
						int n = 0;
						int a = 0;
						for( a = 0; a < 10; a++){
							n += iter->occ[a][0];
						}
						
						tfListEkle(&(*sPtr),&(*tPtr),iter->term,iter->occ[i][1],i+1,n);
					}		
				}
				
			}
			else{
				iter = iter->next;
				continue;
			}
			
			iter = iter->next;
		}
		
		iter = *sPtr;
	//	printf("%s;%d\n",word,max);
		max = 0;		
		}
		
		while(temp != NULL){
			temp->id = 0;
			temp = temp->next;
		}
		temp = *sPtr;
	//	printf("\n");
		iter = *sPtr;
	}
	
	
}

void tfListEkle(termPtr *sPtr,tfPtr *tPtr, char word[40],int tf,int doc,int n){
	
	tfPtr newPtr = malloc(sizeof(tfs));
	
	if(newPtr != NULL){
		
		strcpy(newPtr->term,word);
		newPtr->tf = tf;
		newPtr->doc = doc;
		newPtr->next = NULL;
		newPtr->n = n;
		newPtr->id = 0;
		newPtr->idftf = 0.0;
		tfPtr previousPtr = NULL;
		tfPtr currentPtr = *tPtr;
		
		while(currentPtr != NULL && currentPtr->tf > tf) {
			previousPtr = currentPtr;
			currentPtr = currentPtr->next;
		}
		
		if(previousPtr == NULL){
			newPtr->next = *tPtr;
			*tPtr = newPtr;
		}
		else{
			previousPtr->next = newPtr;
			newPtr->next = currentPtr;
		}
		
		termPtr temp = *sPtr;
		while(temp != NULL){
			if(strcmp(temp->term,word) == 0){
				temp->id = 1;
			}
			
			temp = temp->next;
		}
	
		}
		
	
}

int checkTfList(tfPtr tPtr,char word[40],int doc){
	
	if(tPtr == NULL) return 1;
	
	while(tPtr != NULL){
		if(strcmp(tPtr->term,word) == 0 && tPtr->doc == doc){
			return 0;
		} 
		tPtr = tPtr->next;
	}
	
	return 1;
}

void printTfList(tfPtr *tPtr,termPtr sPtr){
	
	int docNum;
	tfPtr iter = *tPtr;
	while(sPtr->next != NULL){
		sPtr = sPtr->next;
	}
	docNum = sPtr->doc;
	
	
	int counter = 1;
	int miktar = 0;
	double idf = 1.0;

	for(counter = 1; counter <= docNum; counter++){
		printf("%d . Doc's Words Are : \n",counter);
		while(iter != NULL){			
			if(iter->doc == counter){
				if(miktar != 5){
										
					idf = log(docNum / iter->n);
					iter->idftf= idf * iter->tf;				
					printf("%s;%d , ",iter->term,iter->tf);
					miktar++;
				}
				
			}
			iter = iter->next;
		}
		idf  =1 ;
		miktar = 0;
		printf("\n\n");
		iter = *tPtr;
	}
	
	double max = 0.0;
	char word[40];
	int doc;
	
	tfPtr temp;
	printf("\n\n");
	for(counter = 1; counter <= docNum; counter++){
		miktar = 0;
		printf("%d. Document's IDF Word's Are : \n ",counter);
		while(miktar != 5){
			while(iter != NULL){
				if(iter->doc == counter){
			
				if(iter->idftf > max && iter->id != 1){
					max = iter->idftf;
					strcpy(word,iter->term);
					doc = iter->doc;
					printf("%s ; %lf , ",iter->term,iter->idftf);
				}
				
			}
			max = 0.0;
			updateId(&(*tPtr),word,doc);
			iter = iter->next;
		}
	
		miktar++;
		iter = *tPtr;	
		}
		printf("\n\n");
			
	}
	
}

void updateId(tfPtr *tPtr, char word[40], int doc){
	
	tfPtr currentPtr = *tPtr;
	
	while(currentPtr != NULL){
		
		if(strcmp(currentPtr->term,word) == 0 && currentPtr->doc == doc){
			currentPtr->id = 1;
		}
		
		currentPtr = currentPtr->next;
	}
	
	
}


void printLastTime(pairPtr *sPtr,termPtr tPtr){
	
	
	int docNum;
	
	while(tPtr->next != NULL){
		tPtr = tPtr->next;
	}
	docNum = tPtr->doc;

	
	int order = 1;
	int counter = 0;
	int k = 0;
	int inc = 63;
	int stoper = 0;
	
	pairPtr currentPtr = *sPtr;

	for(counter = 0; counter < 3; counter++){ // order için
			currentPtr = *sPtr;
			inc = 63;
			stoper = 0;
		for(k = 0 ; k < 63; k++){
			while(currentPtr != NULL){
				if((currentPtr->word1[0]) == inc && currentPtr->id != 1 && currentPtr->order == order){
					if(order == 1 && stoper == 0)
					 printf("\nFirst Order is : \n");
					if(order == 2 && stoper == 0)
					 printf("\nSecond Order is : \n");
					if(order == 3 && stoper == 0)
					 printf("\nThird Order is : \n");
					 stoper = 1;
					printf("{%s , %s} ,",currentPtr->word1,currentPtr->word2);
					currentPtr->id = 1;
				}
				
				currentPtr = currentPtr->next;
			}
			currentPtr = *sPtr;
			inc++;
		}
		printf("\n\n");
		order = order + 1;
	}
		
	
}



