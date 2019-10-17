/*Nome: Julia Diniz Ferreira
  NUSP: 9364865*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>

typedef struct{
	int index;
	float value;	
}copy_c;

typedef struct{
	float real;
	float imaginaria;	
}X;

char *ReadLine(){
 
    char *frase = NULL;
    int counter=0;
 
    do{
        frase = (char *)realloc(frase,(counter+1)*(sizeof(char)));
        frase[counter]=getchar();
        if(frase[counter] != '\r'){
            counter++;
        }else frase[counter++]=getchar();
    }while((frase[counter-1] != '\n') && (frase[counter-1] != ' ') && (frase[counter-1] != EOF));
    frase[counter-1]='\0';
 
    return frase;
}

unsigned char* le_arquivo(FILE* arq, int* tam){

    unsigned char* x;

    fseek(arq, 0, SEEK_END);
    *tam = ftell(arq);
    fseek(arq, 0, SEEK_SET);

    x = (unsigned char*)malloc(*tam);
    if(x == NULL){
        return NULL;
    }
    fread(x, sizeof(unsigned char), *tam/(sizeof(unsigned char)), arq);

    return x;    
}

unsigned char* rand_sign(){
	
	unsigned char* x = (unsigned char*)malloc(100 * sizeof(unsigned char));

	srand(time(NULL));
	for(int i = 0; i < 100; i++){
		x[i] = ((2.0 * rand()) / RAND_MAX) * 255;
	}

	return x;
}

float* DFT(unsigned char* x, X* new_x, int tam){

	float* c = (float*)calloc((tam/2)+1,sizeof(float));
	
	for (int k = 0; k <= tam/2; ++k){
		new_x[k].real=0, new_x[k].imaginaria=0;
		for (int n = 0; n < tam; ++n){
			new_x[k].real += (x[n] *(cos(((2*3.14)/tam)*k*n)));
			new_x[k].imaginaria += -(x[n] *(sin(((2*3.14)/tam)*k*n)));
		}
		c[k] += sqrtf((new_x[k].real)*(new_x[k].real) + (new_x[k].imaginaria)*(new_x[k].imaginaria));	
	}

	return c;
}

int comp(const void * a, const void * b) {
   return (( *(copy_c*)b).value - (*(copy_c*)a).value );
}

int* sort(float* c, int tam){

	copy_c* vet = (copy_c*)malloc(sizeof(copy_c)*tam);
	int* ret = (int*)malloc(sizeof(int)*tam);

	for (int i = 0; i < tam; ++i){
		vet[i].value = c[i];
		vet[i].index = i;
	}

	qsort(vet, tam, sizeof(copy_c), comp);	
	for (int i = 0; i < tam; ++i){
		ret[i] = vet[i].index;
	}

	free(vet);
	return ret;
}

void remove_C(int* index, float* c, int C, int tam, X* new_x){   

	for (int i = tam-1; i >= C; i--){
		c[index[i]] = 0;
		new_x[index[i]].real = 0;
		new_x[index[i]].imaginaria = 0;
	}
}

unsigned char* inverse_DFT(X* new_x, int tam){

	X* x = (X*)malloc(sizeof(X)*tam);
	unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*tam);

	for (int n = 0; n < tam; ++n){
		for (int k = 0; k < tam; ++k){
			x[n].real += ( (new_x[k].real *(cos(((2*3.14)/tam)*k*n))) + (-1*(new_x[k].imaginaria *(sin(((2*3.14)/tam)*k*n)))) );
			x[n].imaginaria += ( (new_x[k].imaginaria *(cos(((2*3.14)/tam)*k*n))) + (new_x[k].real *(sin(((2*3.14)/tam)*k*n))) );
		}
		x[n].real = x[n].real/tam;
		x[n].imaginaria = x[n].imaginaria/tam;
		r[n] = round(sqrt((x[n].real)*(x[n].real) + (x[n].imaginaria)*(x[n].imaginaria)));
	}

	return r;
}

int savePlot(const char* filename, const unsigned char* x, const float* c, unsigned char* audio, int tam) {
  
  FILE* f = fopen(filename, "w");
  fprintf(f, "n = [0:%d];\n", tam-1);
  fprintf(f, "x = [ ");
  for (int n = 0 ; n < tam ; ++n) fprintf(f, "%u ", (unsigned int) x[n]);
  fprintf(f, "];\n");

  fprintf(f, "inversoX = [ ");
  for (int n = 0 ; n < tam ; ++n) fprintf(f, "%u ", (unsigned int) audio[n]);
  fprintf(f, "];\n");

  fprintf(f, "C = [ ");
  for (int k = 0 ; k < (tam/2)+1 ; ++k) fprintf(f, "%f ", c[k]);
  fprintf(f, "];\n");
  fprintf(f, "subplot(3,1,1)\nplot(n,x)\n");
  fprintf(f, "xlim([0 %d])\n", tam-1);

  fprintf(f, "subplot(3,1,2)\nplot(n,inversoX)\n");
  fprintf(f, "xlim([0 %d])\n", tam-1);

  fprintf(f, "subplot(3,1,3)\nstem(n,C)\n");
  fprintf(f, "xlim([0 %d])\n", tam-1);
  fclose(f);

  return 1;
}

int main(int argc, char const *argv[]){

    /*unsigned char* roberto = rand_sign();
    int tam = 100;
    X* new_x = (X*)malloc(sizeof(X)*tam);
    float* c = DFT(roberto,new_x,tam);
    c[0] = 0;
    int* index = sort(c,tam); //copia de c
    remove_C(index,c,10,tam,new_x);
    float* audio = inverse_DFT(c,new_x,tam);
    printf("%d\n", tam);
    int count = 0;
    for (int i = 0; i <= tam/2; ++i){
    	if(c[i] > 0.1) count++;
    }
    printf("%d\n", count);
    for (int i = 0; i < 10; ++i){
    	printf("%f ", c[index[i]]);
    }printf("\n");
    for (int i = 0; i < tam; ++i){
    	printf("%f\n", audio[i]);
    }
    return 0;*/

    char* nome = (char*)malloc(sizeof(char)*200);
    scanf("%s ", nome);
    int C; //inteiro C representa o numero do componentes mais relevantes
    FILE* arq;
    unsigned char* x; 
    unsigned char* audio; 
    float* c;
    int* index, tam = 0;
    X* new_x;

    scanf("%d", &C);
    arq = fopen(nome,"rb");
    if(arq == NULL){
    	return 0;	
    } 

    x = le_arquivo(arq,&tam); //le arquivo
    new_x = (X*)malloc(sizeof(X)*tam);
    c = DFT(x,new_x,tam);
    index = sort(c,(tam/2)+1); //copia de c
    printf("%d\n", tam);
    int count = 0;
    for (int i = 0; i <= tam/2; ++i){
    	if(c[i] > 0.1) count++;
    }
    printf("%d\n", count);
    for (int i = 0; i < C; ++i){
    	printf("%u ", (unsigned int)c[i]);
    }printf("\n");
    remove_C(index,c,C,(tam/2)+1,new_x);
    audio = inverse_DFT(new_x,tam);
    
    for (int i = 0; i < tam; ++i){
    	printf("%u\n", (unsigned int)audio[i]);
    }

    savePlot("batata.m", x, c, audio, tam);

    fclose(arq);
    return 0;
}
