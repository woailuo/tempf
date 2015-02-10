#include<stdio.h>
#include<stdlib.h>

#include<time.h>


#define FEEA 50
#define FEEB 30
#define FEEC 90
#define TIMESECOND 3600

typedef struct node_1
{
	int No;
	time_t T;
	int time;
	struct node_1 *next;
}Park;

typedef struct node_3
{
	int max;
	int current;
	int flag;
	struct node_1 *next;
}HeadNode;

Park *TimeCount(Park *head,Park *p,int num);
Park *FixPark(Park *head,Park *p,int num);
Park *Unparking(Park *head);
void head_output(HeadNode *head);
void output(Park *head);
int check(Park *head);
int Count(time_t T);
int menu_select_1();
int menu_select_2();
int menu_select_3();
int menu_select_4();
int menu_select_5();

int main()
{
	int num=1;
	int flags=0;
	HeadNode *head=NULL;
	Park *head_1=NULL;
	Park *head_0=NULL;
	Park *p=NULL;
	Park *q=NULL;
	head=(HeadNode *)malloc(sizeof(HeadNode));
	head_1=(Park *)malloc(sizeof(Park));
	head_0=(Park *)malloc(sizeof(Park));
	head->max=30;
	head->current=0;
	head->flag=-1;
	head->next=head_0;
	head_0->next=head_1;
	head_0->No=100;
	head_0->time=-2;
	head_0->T=100;
	head_1->next=NULL;
	for(;head->current<=head->max;)
	{
		if(head->current<head->max)
		{
			head_output(head);
			switch(menu_select_1())
			{
			case 1:
				{
					switch(menu_select_2())
					{
					case 1:
						{
							num=check(head_1);
							head_1=TimeCount(head_1,p,num);
							head->current++;
							break;
						}
					case 2:
						{
							num=check(head_1);
							head_1=FixPark(head_1,q,num);
							head->current++;
							break;
						}
					case 3:
						{
							break;
						}
					default:
						{
							printf("Please confirm your choice again!\n");
							break;
						}
					}
					head->flag=0;
					break;
				}
			case 2:
				{
					if(head->flag==(-1))
					{
						printf("No car to unparking!\n");
						break;
					}
					else
					{
						switch(menu_select_3())
						{
						case 1:
							{
pa:								head_1=Unparking(head_0);
								num--;
								head->current--;
								break;
							}
						case 2:
							{
								break;
							}
						default:
							{
								printf("Please confirm your choice again!\n");
								break;
							}
						}
						break;
					}
				default:
					{
						printf("Please confirm your choice again!\n");
						break;
					}
				}
			}
			for(flags=0;flags!=1;)
			{
				printf("For your own safty, please input \"1\" button after remeber your site number\n");
				scanf("%d",&flags);
				if(flags==1)
				{
					system("cls");
				}
				else 
				{
					flags=0;
				}

			}
		}
		else
		{
			printf("This parking spot is full!\nPlease wait for the lastest unparking!\n");
			break;
		}	
	}
	output(head_0);
ca:	switch(menu_select_5())
	{
	case 1:
		{
			goto pa;
		}
	case 2:
		{
			
			goto ca;
		}
	default:
		{
			printf("Wrong input!\n");
			goto ca;
		}
	}
}

void head_output(HeadNode *head)
{
	printf("**********************************************\n");
	printf("         Max:%2d|Current:%2d  ",head->max,head->current);
	if(head->flag==1)
	{
		printf("Full\n");
	}
	else 
	{
		printf("Available\n");
	}
	printf("**********************************************\n");
}

int menu_select_1()
{
	int i=0;
	printf("1.Parking\n2.Unparking\n");
	printf("**********************************************\n");
	printf("Your selection:");
	scanf("%d",&i);
	return i;
}

int menu_select_2()
{
	int i=0;
	printf("***********************************************\n");
	printf("                 Parking menu\n");
	printf("***********************************************\n");
	printf("1.Free Parking\n2.Fixed time Parking\n3.Back to the former list\n");
	printf("***********************************************\n");
	printf("Your selection:");
	scanf("%d",&i);
	return i;
}

int menu_select_3()
{
	int i=0;
	printf("***********************************************\n");
	printf("                 Unparking menu\n");
	printf("***********************************************\n");
	printf("1.Input Your Parking Site\n2.Back to the former list\n");
	printf("***********************************************\n");
	printf("Your selection:");
	scanf("%d",&i);
	return i;
}

int menu_select_4()
{
	int i=0;
	printf("************************************************\n");
	printf("1.One hour\n2.Two hour\n3.Three hour\n");
	printf("************************************************\n");
	printf("Your selection:");
	scanf("%d",&i);
	return i;
}


int menu_select_5()
{
	int i=0;
	printf("************************************************\n");
	printf("1.Unparking\n2.Nothing\n");
	printf("************************************************\n");
	scanf("%d",&i);
	return i;
}

Park *TimeCount(Park *head,Park *p,int num)
{
	Park *x=head;
	if(num!=1)
	{
		p=(Park *)malloc(sizeof(Park));
		p->No=num;
		p->T=time(NULL);
		p->next=NULL;
		p->time=-1;
		while(x->next!=NULL)
		{
			x=x->next;
		}
		x->next=p;
	}
	else 
	{
		head->next=NULL;
		head->No=num;
		head->T=time(NULL);
		head->time=-1;
	}
	printf("Thank you for your use!You site is No.%d\n",num);
	return head;
}

Park *FixPark(Park *head,Park *p,int num)
{
	Park *x=head;
	int	 T;
	switch(menu_select_4())
	{
	case 1:
		{
			T=60*60;
			break;
		}
	case 2:
		{
			T=2*60*60;
			break;
		}
	case 3:
		{
			T=3*60*60;
			break;
		}
	default:
		{
			printf("Please confirm your choice again!\n");
			break;
		}
	}
	if(num!=1)
	{
		p=(Park *)malloc(sizeof(Park));
		p->No=num;
		p->T=time(NULL);
		p->time=T;
		p->next=NULL;
		while(x->next!=NULL)
		{
			x=x->next;
		}
		x->next=p;
	}
	else
	{
		head->next=NULL;
		head->No=num;
		head->T=time(NULL);
		head->time=T;
	}
	printf("Your parking site is No.%d\n",num);
	return head;
}

Park *Unparking(Park *head)      
{                               
	int fee=0;
	int num=0;
	int fixed=0;
	int space=0;
	Park *p=NULL;
	Park *q=head;                  
	printf("Please Input Your Site:\n");
	scanf("%d",&num);
	for(p=head->next;p->next!=NULL;p=p->next)
	{
		if(p->No==num)
		{
			space=Count(p->T); 
			fixed=p->time;
			while(q->next!=p)
			{
				q=q->next;
			}
			q->next=p->next;
			p->next=NULL;
			free(p);
			goto la;
		}
		else
		{
			continue;
		}
	}
la:	if(fixed==(-1))
	{
		fee=space*FEEA;
	}
	else
	{
		if(space>fixed)
		{
			fee=fixed*FEEB+(space-fixed)*FEEC;
		}
		else
		{
			fee=space*FEEB;
		}
	}
	if(fee!=1)
	{
		printf("The expenses of your parking is:%4d dollars\n",fee);
	}
	else
	{
		printf("The expenses of your parking is:%4d dollar\n",fee);
	}
	printf("Please pay your expenses in the service counter!\nThank you!\n");
	return head;
}

int Count(time_t T)
{
	int differ=0;
	time_t T_1=T;
	time_t T_2=time(NULL);
	int hour_1, min_1, sec_1;
	int hour_2, min_2, sec_2;
	struct tm *Current=localtime(&T_1);
	struct tm *Recent=localtime(&T_2);
	hour_1=(Current->tm_hour); 
	min_1=(Current->tm_min); 
	sec_1=(Current->tm_sec);
	hour_2=(Recent->tm_hour); 
	min_2=(Recent->tm_min); 
	sec_2=(Recent->tm_sec);
	differ=(hour_2-hour_1)*3600+(min_2-min_1)*60+(sec_2-sec_1);
	return ((differ/TIMESECOND)+1);
}

void output(Park *head)
{
	Park *p=head;
	printf("\tNO\tT\t\ttime\n");
	while(p->next!=NULL)
	{
		if(p->No!=100)
		{
			printf("\t%d\t%ld%8d s\n",p->No,p->T,p->time);
			p=p->next;
		}
		else 
		{
			printf("------------------------------------------\n");
			p=p->next;
		}
	}
	printf("\t%d\t%ld%8d s\n",p->No,p->T,p->time);
}

int check(Park *head)
{
	Park *p=head;
	int num=1;
	for(;num<=30;num++)
	{
		for(p=head;p->next!=NULL;p=p->next)
		{
			if(p->No!=num)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		if(p->next==NULL)
		{
			if(p->No!=num)
			{
				goto ka;
			}
		}
	}
ka:	return num;
}

