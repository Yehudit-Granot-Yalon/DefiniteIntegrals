//Name:Yehudit Granot
//I.D.:207346925
# include <pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>
#include<signal.h>
# include <sys/stat.h>
# include <fcntl.h>
#include <math.h>
char** fixPsik(char* string);
char** fixPlus(char* string);
void print(char** str) ;
double func(double d);
void* fthread(void* s);
double numericalIntegration(double a, double b,double (*func)(double), int k ) ;
void fixMat(char** str);
int m[]={0,0,0};
void  my_free(char** str);
typedef struct s{// define sstruct to the integral thread plan
double a;//up border
double b;//down border
double d;// the difference
int location;//wich thread work all time
}s;
int main(int argc, char *argvi[]){
 int argv=atoi(argvi[1]);
  if(argv<0){
    printf("The input uncorrect");
    exit(1);
  }
  char** arr;
  char str[100];
  printf("Enter integral and limits");
   while(fgets(str, 510, stdin)==NULL){
   printf("Enter integral and limits"); 
   }
  str[strlen(str)-1]=' ';//removes an unnecessary character from the string
   arr = fixPsik(str);//call to the function that separates between integral and limits
   double from=atoi(arr[1]);
   double to=atoi(arr[2]);
   char** arrFunc=fixPlus(arr[0]);//A call to a function that separates the components of the vowel by plus
  fixMat(arrFunc);//Arranges the array of coefficients
pthread_t a[argv];//Defines a process set and arranges the struct that accepted in each process
s st[argv];
int status;
int i;
double* d;
double sum=0;
for (i=0;i<argv;i++){
 st[i].a=from;
 st[i].b=to;
 st[i].d=(to-from)/argv;
 st[i].location=i;
 status=  pthread_create(&a[i], NULL, fthread, (void*) &st[i]);//call to the thread
if(status!=0){
   fputs("erro",stderr);
  }
}
  int j;
  for(j=0;j<argv;j++){//Waiting for processes and then summarizing the value returned from each process
    pthread_join(a[j],(void**)&d);
sum+=*d;
  free(d);
   }
    printf("%lf",sum);
my_free(arr);
my_free(arrFunc);
return 0;
}
void fixMat(char** str){
  int i;
  int count=0;
for(i=0;str[i]!=NULL;i++)
count++;
if(count==3){//In case the function consisted of 3 arguments
m[0]=1;
m[1]=1;
m[2]=atoi(str[2]);
}
if(count==2){//In case the function consisted of 2 arguments
if(strcmp(str[0],"x^2")!=0){
m[1]=1;
m[2]=atoi(str[1]);
}
else if(strcmp(str[1],"x")!=0){
m[0]=1;
m[2]=atoi(str[1]);
}
else{
m[0]=1;
m[1]=1;
}
}
if(count==1){//In case the function consisted of 1 arguments
if(strcmp(str[0],"x^2")==0)
m[0]=1;
else if(strcmp(str[0],"x")==0){
m[1]=1;
}
else{
m[2]=atoi(str[0]);
}  
}
}
void* fthread(void* st){//thread function
double from;
double  to;
 s s1=*((s*)st);
 from=s1.a+s1.d*s1.location;//Calculates how borders will be sent each time
  to=s1.a+s1.d*(s1.location+1);
 int k=8;//The number of divisions of the area
 double (*pfunc)(double);//Pointer to a function that calculates the value at a point
 pfunc=func;
 double* d=(double*)malloc(sizeof(double));
 *d=numericalIntegration(from,to,pfunc, k ) ;//Integral calculation
  return (void*)d;//Returns a pointer containing the amount
}

double func(double x){//A function that calculates a value at a point
double sum=0;
sum+=m[0]*(x*x);
sum+=m[1]*x;
sum+=m[2]; 
return sum;
}

void print(char** str) {//The function prints the array
int i = 0;
for (; str[i] != NULL; i++)
printf("arr[%d]=%s\n",i, str[i]);
}

double numericalIntegration(double a, double b,double (*func)(double), int k )  {//The function given     
  double delta = (b - a)/k;    
   double sum = 0;     
   for(double x = a+0.5*delta; x < b ;x+=delta){      
       sum += (*func)(x);     }    
       return sum*delta;  } 
 
char** fixPsik(char* string) {//The function separates words received in a sentence and puts them in an array
int count = 0;
int i;
count = 1;
  for (i = 0; string[i+2] != '\0'; i++) {
//if (string[i] == ' ' && string[i + 1] != ' ')
if (string[i] == ',' && string[i + 1] == ' ' )
count++;//count how many words exit in a sentance
}
char** arr = (char**)malloc((count + 1) * sizeof(char*));//alocate 2 dimantion  array
if (arr == NULL) { //Checks if allocation error
printf("EROR");
exit(0);
}
char* token = strtok(string, ", "); //strtok-this function separate the words
for (  i = 0; i < count; i++) {
arr[i] = (char*)malloc(sizeof(char) * (strlen(token)+1));
if (arr[i] == NULL) {//Checks if allocation error
printf("EROR");
return 0;
}
strcpy(arr[i], token);
token = strtok(NULL, ", ");
}
arr[i] = NULL;
return arr;
}
char** fixPlus(char* string) {//The function separates words received in a sentence and puts them in an array
int count = 0;
int i;
count = 1;
for (i = 0; string[i+1] != '\0'; i++) {
if (string[i] == '+' )
count++;//count how many words exit in a sentance
}
char** arr = (char**)malloc((count + 1) * sizeof(char*));//alocate 2 dimantion  array
if (arr == NULL) { //Checks if allocation error
printf("EROR");
exit(0);
}
char* token = strtok(string, "+"); //strtok-this function separate the words
for (  i = 0; i < count; i++) {
arr[i] = (char*)malloc(sizeof(char) * (strlen(token)+1));
if (arr[i] == NULL) {//Checks if allocation error
printf("EROR");
return 0;
}
strcpy(arr[i], token);
token = strtok(NULL, "+");
}
arr[i] = NULL;
return arr;
}
void  my_free(char** str) {//The function releases the array
	int i = 0;
	for (i=0; str[i] != NULL; i++) {
		free(str[i]);
	}
  free(str);	//relreas  the array
}