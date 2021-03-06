#include<stdio.h>	//to use the printf function
#include<conio.h>         		//to use the getche function
#include<stdlib.h>         		//to use the rand function
#include<time.h>

typedef struct Chrom            // creating the chromosome structure
	{
  short int bit[126];
  short int bit21[21];
  double fitness; //all nutrient
  int allCost;
  double allCal;
  }chrom_t;

typedef struct Data             		
  {
  	char name[32];
  	double car; //carbohydrate
  	double fat; //fat
  	double pro; //protein
  	int dish_size;  //dize size in gram
	int cost; //cost in bath unit
	double kcal; // callory per dish
  }data_t;

/*define*/
  #define POP_SIZE 100
  #define NUM_ITERATION 2
/*Global variable*/
  int countMenu=64;
  char fileName[]="Output.txt";
  int popNow=0;
  int popNextCount=0;
  

  data_t foods[64]; //64 menu in program
  chrom_t popCurrent[POP_SIZE];
  chrom_t popNext[POP_SIZE];
  chrom_t popTemp[POP_SIZE];
  chrom_t popTest[2];
	
/*Input variable*/
  int inputWeight=60;
  int inputMoney=800;
  int inputCal=10500; 

/*OTHER FUNCTION*/
  void readFile();
  void get_user_input();
  
  struct Chrom decode(chrom_t popcurrent);
  void showData126(chrom_t popData);
  void showData1(chrom_t popData);
  void deleteData(int numDelete, chrom_t * popData);
  int randomNum(int max);

  void showData(chrom_t * popData);
  void swopData(chrom_t * popData);
  void mutationData(chrom_t * popData);
  void crossOver();
  void resetpopTest(chrom_t popData);
  
  double calFitness(chrom_t popcurrent);
  int calCost(chrom_t popcurrent);
  double calCal(chrom_t popcurrent);
  
  void finalShow(chrom_t popData);
/*--------------*/

void main()
{
  int i,j,k;
  int random1=0,random2=0;
  int count=0;
  int row=0;
  
srand(time(NULL));
  	
//  Read File
    readFile();  
//	Get data from user
	get_user_input();

//  M1. Create first generation 
//  1.1 Random data -> by check with cost and cal
for(i=0;i<POP_SIZE;i++)
{   
	while(1){
	//random 21 menu in 126 bit
    for(j=0;j<126;j++)
    	{
    	popCurrent[i].bit[j] = rand()%2;
        //popCurrent[i].bit21[j] = randomNum(0);
    	}
    	
    //decode 126 to 21 meal
    popCurrent[i]=decode(popCurrent[i]);
    
//  1.2 Calculate fitness & allCost & allCal  
    popCurrent[i].fitness=calFitness(popCurrent[i]);
    popCurrent[i].allCost=calCost(popCurrent[i]);
    popCurrent[i].allCal=calCal(popCurrent[i]);
    
    if(popCurrent[i].allCal<inputCal && popCurrent[i].allCost<inputMoney)
    	{
    	break;
		}
	}//while
}//for

	//set pop in Now equal POP_SIZE
	popNow=POP_SIZE; 
	
//  1.3 Sort by fitnesss
	swopData(&popCurrent);

//	1.4	copy data to popNext
	for(i=0;i<POP_SIZE;i++)
	{
		popNext[i]=popCurrent[i];
	}
	
//  1.5 Cut 20 value that least in popCurrent
	deleteData(40,&popCurrent);

// LOOP INTERRATION
for(j=0;j<NUM_ITERATION;j++)
{
	printf("\nCOM : INTERATION %d -------------------------------------------\n",j+1);
//  M2. Create NEW generation
//  2.1 Crossover -> Random to pick 2 different parent -> new child by check Cost and cal
	crossOver();  //keep crossdata in popTemp + Calculate new fitness and cost of Mutation + sort
	//copy popTemp to popNext
	count=0;
	for(i=popNow-1;i<POP_SIZE;i++)
	{
		popNext[i]=popTemp[count];
		count++;
	}
	popNow=POP_SIZE;
	swopData(&popNext);
	//Copy popNext to popCurrent
	for(i=0;i<POP_SIZE;i++)
	{
	popCurrent[i]=popNext[i];
	}
	printf("\nCOM : gerate new gen already.");
	
//  2.2 Mutation -> random popuration to has mutation -> if has, random row and position
	mutationData(&popCurrent);

//  2.3 Sort data by fitness  from fit H-L
	swopData(&popCurrent);

	//copy data to popNext before cut 
	for(i=0;i<POP_SIZE;i++)
	{
		popNext[i]=popCurrent[i];
	}
	
//  2.4 Cut 20 value that least in popCurrent
	if(row!=NUM_ITERATION-1)
	{
	deleteData(40,&popCurrent);
	}
	row++;
}//for

//show last result
	printf("\nPOP current\n");
	showData(&popCurrent);
	
//  SHOW DATA TO USER
	finalShow(popCurrent[0]);
}//main

/*-------------------------------------------------------------------------------------------------------------*/
/*OTHER FUNCTION*/
void readFile()
{
  FILE * fread=NULL;
  int countMenu1=0;
  printf("COM : Open file name [%s]\n", fileName);
  fread = fopen(fileName,"r");
  int i;

  if(fread==NULL)
    {
      printf("COM : Cannot open file <%s>\n",fileName);
      exit(0);
    }
    
  //can run
  fscanf(fread,"%d",&countMenu1);
  printf("COM : Total foods have %d menu for choose in this program\n",countMenu1);

  for(i=0;i<countMenu1;i++)
  {
    fscanf(fread,"%s %lf %lf %lf %d %d %lf",foods[i].name, &foods[i].car, &foods[i].fat, &foods[i].pro, &foods[i].dish_size, &foods[i].cost, &foods[i].kcal);
    //printf("%d. %-40s %.2lf %.2lf %.2lf | %d | %d | %.2lf\n",i+1 ,foods[i].name, foods[i].car, foods[i].fat, foods[i].pro, foods[i].dish_size, foods[i].cost, foods[i].kcal);
  }
  fclose(fread);

  printf("COM : Read file already.\n");
}

void get_user_input()
{
	int calory = 0;
    int expense = 0;
    char input[64];

    printf("Please input calorie in each day : ");
	fgets(input,sizeof(input),stdin);
	sscanf(input,"%d",&calory);

	inputCal = calory*7;

	printf("Please input cost in each day : ");
	fgets(input,sizeof(input),stdin);
	sscanf(input,"%d",&expense);

	inputMoney = expense*7;
}

struct Chrom decode(chrom_t popcurrent)
{
    int i,k;
    int j=0;
    int value = 0;
    chrom_t ex;

    ex = popcurrent;
    //loop to keep value in value[i]
    for(i=0;i<21;i++)
    {
		j = j+6;
        //printf("j = %d\n",j);
        for(k=0;k<6;k++)
        {
            value +=  (popcurrent.bit[j-k-1]*(pow(2,k)));
        }
		value+=1;// chang to menu 1 - 64
        ex.bit21[i] = value;
        //printf("popcurrent.value[%d] =  %d\n",i,ex.value[i]);
        value = 0;
        //printf("\n\n");
        
    }
    return ex;
}

int randomNum(int max)
{
  int num;
  //srand(time(NULL));
  num = (rand()%max)+1;
  //printf("random is %d\n",num);
  return num;
}

void showData(chrom_t * popData)
{ 
  int  i,j,k=0;
  for(i=0;i<POP_SIZE;i++)
  {
  printf("Random Menu %d : ",i+1);
  for(j=0; j<21; j++)
    {
      printf(" %d",popData[i].bit21[j]);
      k++;
      if(k==3)
      {
        printf(" |");
        k=0;
      }
      if(j==20)
      {
        printf("\n");
      }
    }
  printf("Fitness %.2lf Cost %d Cal %.2lf\n",popData[i].fitness, popData[i].allCost, popData[i].allCal);
  }

}

void showData1(chrom_t popData)
{ 
  int  i,j,k=0;
  
  printf("Random Menu %d : ",i+1);
  for(j=0; j<21; j++)
    {
      printf(" %d",popData.bit21[j]);
      k++;
      if(k==3)
      {
        printf(" |");
        k=0;
      }
      if(j==20)
      {
        printf("\n");
      }
    }
  printf("Fitness %.2lf Cost %d Cal %.2lf\n",popData.fitness, popData.allCost, popData.allCal);
  
}

double calFitness(chrom_t popcurrent) //calAllCal
{
    int j=0;
    double fitnessValue = 0;
	double fatG =0,carG=0,proG=0;

	for(j=0;j<21;j++)
    {
        carG += foods[popcurrent.bit21[j]-1].car;
        fatG += foods[popcurrent.bit21[j]-1].fat;
        proG += foods[popcurrent.bit21[j]-1].pro;
    }
    fitnessValue = carG + fatG + proG;
    //printf(" fitValue = %.2lf",fitnessValue);
     //printf(" totalC = %.2lf",fitC);
     //printf("\n");

    return fitnessValue;
}

int calCost(chrom_t popcurrent)
{
	int j=0;
	int allCost=0;
	
	for(j=0;j<21;j++)
	{
		allCost += foods[popcurrent.bit21[j]-1].cost;
		//printf("%d .. %d \n",j+1, foods[popcurrent.bit21[j]-1].cost);
	}
//	printf("allCost = %d\n",allCost);
	return allCost;
}

double calCal(chrom_t popcurrent)
{
	int j=0;
	double allCal=0;
	
	for(j=0;j<21;j++)
	{
		allCal += foods[popcurrent.bit21[j]-1].kcal;
		//printf("%d .. %d \n",j+1, foods[popcurrent.bit21[j]-1].cost);
	}
//	printf("allCal = %.2lf\n",allCal);
	return allCal;
}

void showData126(chrom_t popData)
{
	int i=0;
	int j=0;
	
	for(i=0;i<126;i++)
	{
		printf("%d ",popData.bit[i]);
		
		j++;
		if(j==6)
			{	
			printf(" | ");
			j=0;
			}
	}
	printf("\n");
}

void swopData(chrom_t * popData)
{
	int i,j,k;
	chrom_t temp;
	for(i=0;i<POP_SIZE;i++)
	{
		for(j=0;j<POP_SIZE-1;j++)
		{
		//	printf("before fitness %d %d",popcurrent[j].fitness,popcurrent[j+1].fitness);
			if(popData[j].fitness < popData[j+1].fitness)
			{   //swop temp=popcurrent[j]
				for(k=0;k<126;k++)
				{
				temp.bit[k]=popData[j].bit[k];
				}
				for(k=0;k<21;k++)
				{
				temp.bit21[k]=popData[j].bit21[k];
				}
				temp.fitness=popData[j].fitness;
				temp.allCost=popData[j].allCost;
				temp.allCal=popData[j].allCal;
				for(k=0;k<126;k++) //swop popcurrent[j]=popcurrent[j+1]
				{
				popData[j].bit[k]=popData[j+1].bit[k];
				}
				for(k=0;k<21;k++)
				{
				popData[j].bit21[k]=popData[j+1].bit21[k];
				}
				popData[j].fitness=popData[j+1].fitness;
				popData[j].allCost=popData[j+1].allCost;
				popData[j].allCal=popData[j+1].allCal;
				for(k=0;k<126;k++)//swop popcurrent[j+1]=temp
				{
				popData[j+1].bit[k]=temp.bit[k];
				}
				for(k=0;k<21;k++)
				{
				popData[j+1].bit21[k]=temp.bit21[k];
				}
				popData[j+1].fitness=temp.fitness;
				popData[j+1].allCost=temp.allCost;
				popData[j+1].allCal=temp.allCal;
			}
		//	printf("After fitness %d %d\n\n",popcurrent[j].fitness,popcurrent[j+1].fitness);
		}
	}
	printf("COM : Swop data already.\n");	
}

void deleteData(int numDelete, chrom_t * popData)
{
	int i=0,k=0;
	if(numDelete>POP_SIZE)
	{
		printf("Cannot delete data more than %d/%d\n",numDelete,popNow);
	}
	for(i=POP_SIZE-numDelete;i<POP_SIZE;i++)
	{
		for(k=0;k<126;k++)
		{
			popData[i].bit[k]=0;
		}
		for(k=0;k<21;k++)
		{
			popData[i].bit21[k]=0;
		}
		popData[i].fitness=0;
		popData[i].allCost=0;
		popData[i].allCal=0;
	}
	popNow-=numDelete;
	printf("\nCOM : population after delete  is %d\n", popNow);
}

void mutationData(chrom_t * popData)
{
	int random;
	int i;
	int row=0,col=0;
	int bit21=0;

	random = rand()%100;
	
	if(random >= 1) // popbability 50%
	{
		row = rand()%POP_SIZE;
		col = rand()%126;
		bit21=col/6;
		if(col%6>0)
		{
			bit21++;
		}		
		printf("\nMutation Random menu[%d] bit is %d/126  in Meal %d/21\n", row+1, col+1, bit21);
			
		if(popData[row].bit[col]==1)
		{
			popData[row].bit[col]=0;
		}
		else
		{
			popData[row].bit[col]=1;
		}
	
	//decode 
	popData[row]=decode(popData[row]);	
	
	//new calculate
	popData[row].fitness=calFitness(popData[row]);
    popData[row].allCost=calCost(popData[row]);
    popData[row].allCal=calCal(popData[row]);
	
	//swopdata	
	swopData(&popCurrent); //change it to pop next
	//check if cal and cost if it not chang fitness to 0
    if(popData[row].allCal>inputCal || popData[row].allCost>inputMoney)
    	{
    	popData[row].fitness=0;
    	printf("COM : Data change and the value is not correct in condition.");
		}
	}
	
	else // no mutation
	{
		printf("\nCOM : NO Mutation in this generation.");
	}
	
	printf("COM : Mutation already.\n");
}

void crossOver()
{
	int random1,random2;
	int crosspoint;
	int i,j;
	int countTemp=0;
//popCurrent popNext popTemp
	//C. create data to popNext
	while(1)
	{
		//C.1 random differrent parent
		while(1)
        {
            random1=rand()%popNow; //popNow from popCurrent that still alive
            random2=rand()%POP_SIZE;

            if(random1 != random2)
            {
                break;
            }
        }
		//C.2 random crosspoint
		crosspoint = ((rand()%126)+1);
		//C.3 crossover
		//crossing the bits before the cross point index
		for(i=0;i<crosspoint;i++)                     	    
        {
        	popTest[0].bit[i]=popNext[random1].bit[i];
        	popTest[1].bit[i]=popNext[random2].bit[i];
    	}
    	//crossing the bits after the cross point index
    	for(i=crosspoint;i<126;i++)
		{
			popTest[1].bit[i]=popNext[random1].bit[i];
        	popTest[0].bit[i]=popNext[random2].bit[i];
		}
		//C.4 decode
    	popTest[0]=decode(popTest[0]);
		popTest[1]=decode(popTest[1]);
		
		//C.5 calculate fitness + cal + cost
		popTest[0].fitness=calFitness(popTest[0]);
    	popTest[0].allCost=calCost(popTest[0]);
    	popTest[0].allCal=calCal(popTest[0]);
		
		popTest[1].fitness=calFitness(popTest[1]);
    	popTest[1].allCost=calCost(popTest[1]);
    	popTest[1].allCal=calCal(popTest[1]);
		//C.6 check if it more cal and cost than input go to new selecting
		//check countTemp
		if(popTest[0].allCost<inputMoney && popTest[0].allCal<inputCal)
		{
		popTemp[countTemp]=popTest[0];
		resetpopTest(popTest[0]);
		countTemp++;
		}
		if(countTemp==POP_SIZE)
			{
				break;
			}
		//check countTemp+1
		if(popTest[1].allCost<inputMoney && popTest[1].allCal<inputCal)
		{
		popTemp[countTemp]=popTest[1];
		resetpopTest(popTest[1]);
		countTemp++;
		}
		//C.7 else it ok go out from infinity loop while
		if(countTemp==POP_SIZE)
			{
				break;
			}
	}//while
	//after crossover
	swopData(&popTemp);

	printf("\nCOM : CrossOver in popTemp Already.\n");
}

void resetpopTest(chrom_t popData)
{
	int i=0,k=0;
	
	for(k=0;k<126;k++)
		{
		popData.bit[k]=0;
		}
	for(k=0;k<21;k++)
		{
		popData.bit21[k]=0;
		}
	popData.fitness=0;
	popData.allCost=0;
	popData.allCal=0;

}

void finalShow(chrom_t popData)
{
	int i,k=1;
	int day=1;
	printf("\n* ------------------------The best random for You----------------------------- *\n\n");

	for(i=0;i<21;i++)
	{
		printf("DAY %d: Meal %d - > %-40s ",day, k, foods[popData.bit21[i]-1].name);
		printf(" %-10.2lf kcal\n",foods[popData.bit21[i]].kcal);
		k++;
		if(k==4)
		{
		k=1;
		day++;
		printf("\n");
		}
			
	}
	printf("\nMax fitness is %.2lf points\n",popData.fitness);
	printf("Total callery is %.2f/%d kcal\n",popData.allCal,inputCal);
	printf("Total cost is %d/%d bath.\n\n",popData.allCost,inputMoney);
	printf("COM : Thank you");
}
