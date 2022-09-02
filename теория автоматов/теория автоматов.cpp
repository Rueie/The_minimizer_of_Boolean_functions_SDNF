#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<math.h>
#include <malloc.h>
#include<time.h>

//функция печати данной функции
void FunctionPrintf(int* MassiveOfMinterms, int NumberArguments, int NumberOfMinterms);
//сортировка введенных минтермов
int* shakersort(int* MassiveOfMinterms, int NumberofMinterms);
//метод Квайна
void Kuine(int* MassiveOfMinterms, int NumberArguments, int NumberOfMinterms);
//метод Мак-Ласки
void Mak_Klasky(int* MassiveOfMinterms, int NumberArguments, int NumberOfMinterms);

void main()
{
	int NumberArguments;
	long long int MaxNumberOfMinterms;
	int NumberOfMinterms;
	int* MassiveOfMinterms;
	int CountOfMinterms = 0;
	int mintermvalue;


	printf("How much arguments your function has: ");
	scanf("%d",&NumberArguments);

	//по заданию поставлено 5, но функция работает для любого числа аргументов, больших 1
	if (NumberArguments < 5)//проверка на число аргументов
	{
		printf("Wrang number of arguments!!!\n");
		return;
	}

	MaxNumberOfMinterms = pow(2, NumberArguments);
	printf("MaxNumberOfMinterms=%d\n", MaxNumberOfMinterms);

	printf("How much minterms do you want to use: ");
	scanf("%d", &NumberOfMinterms);

	//можно ставить любое допустимое число минтермов, большее 1
	if ((NumberOfMinterms > MaxNumberOfMinterms)||(NumberOfMinterms<15))//проверка на число минтермов
	{
		printf("Wrang number of minterms!!!\n");
		return;
	}

	MassiveOfMinterms = (int*)malloc(NumberOfMinterms*sizeof(int));

	printf("Enter minterms('-1' complete entering):\n");
	mintermvalue = 0;
	while (mintermvalue != -1)//считываем номера минтермов (-1=условие выхода из печати)
	{		
		scanf("%d", &mintermvalue);
		if ((mintermvalue > (MaxNumberOfMinterms-1))||(mintermvalue<-1))//проверка на номер минтерма (N>=0 && N<MaxNumberOfMinterms)
		{
			printf("Wrang size of minterm!!!\n");
			return;
		}
		if (mintermvalue == (-1))
		{
			CountOfMinterms--;
			break;
		}
		MassiveOfMinterms[CountOfMinterms] = mintermvalue;
		CountOfMinterms++;

		if (CountOfMinterms == NumberOfMinterms)//остановка ввода, если шаловливые ручки захотят ввести больше минтермов 
		{
			break;
		}

	}

	if (CountOfMinterms != NumberOfMinterms)//проверка на число ВВЕДЕНЫХ минтермов (если их меньше, то ошибка)
	{
		printf("Wrang number of minterms was enter!!!\n");
		return;
	}

	for (int i = 0;i < CountOfMinterms;i++)//проверка на повторяющиеся минтермы
	{
		for (int j = i+1;j < CountOfMinterms;j++)
		{
			if (MassiveOfMinterms[i] == MassiveOfMinterms[j])
			{
				printf("There are duplicate minterms!!!\n");
				return;
			}
		}
	}

	printf("Your function:\n");
	MassiveOfMinterms =shakersort(MassiveOfMinterms, NumberOfMinterms);//сортировка минтермов по их номерам
	FunctionPrintf(MassiveOfMinterms, NumberArguments, NumberOfMinterms);//печать функции, которая была введена
	Kuine(MassiveOfMinterms, NumberArguments, NumberOfMinterms);//склейка по Квайну
	printf("\n");
	Mak_Klasky(MassiveOfMinterms, NumberArguments, NumberOfMinterms);//склейка по Мак-Класки
}

void FunctionPrintf(int* MassiveOfMinterms, int NumberArguments, int NumberOfMinterms)
{	
	int* MassiveOfArguments;
	int minterm;
	MassiveOfArguments = (int*)malloc(NumberArguments * sizeof(int));
	//печать function(x0,x1,...)= 
	printf("f(");
	for (int j = 0;j < NumberArguments;j++)
	{
		printf("x%d", j);
		if (j != (NumberArguments - 1))
			printf(",");
	}
	printf(")=");
	//и суммы минтермов заданной функции
	for (int i = 0; i < NumberOfMinterms;i++)
	{
		minterm = MassiveOfMinterms[i];
		for (int j = NumberArguments - 1;j >= 0;j--)
		{
			MassiveOfArguments[j] = minterm % 2;
			minterm = minterm / 2;
		}
		for (int j = 0;j < NumberArguments;j++)
		{
			printf("%d", MassiveOfArguments[j]);
		}
		if(i!=(NumberOfMinterms-1))
			printf(" + ");
	}
	printf("\n");
}

int* shakersort(int* MassiveOfMinterms, int NumberofMinterms)
{
	//Шейкер сортировка заданных номеров минтермов по возрастанию
	int switches = -1;
	int value,i;
	while (switches != 0)
	{
		switches = 0;
		for (i = 0;i < NumberofMinterms-1;i++)
		{
			if (MassiveOfMinterms[i + 1] < MassiveOfMinterms[i])
			{
				value = MassiveOfMinterms[i];
				MassiveOfMinterms[i] = MassiveOfMinterms[i + 1];
				MassiveOfMinterms[i + 1] = value;
				switches++;
			}
		}
		for (i = NumberofMinterms - 1;i > 0;i--)
		{
			if (MassiveOfMinterms[i] < MassiveOfMinterms[i - 1])
			{
				value = MassiveOfMinterms[i];
				MassiveOfMinterms[i] = MassiveOfMinterms[i - 1];
				MassiveOfMinterms[i - 1] = value;
				switches++;
			}
		}
	}
	return MassiveOfMinterms;//вернули отсортированные номера минтермов
}

void Kuine(int* MassiveOfMinterms, int NumberArguments, int NumberOfMinterms)
{
	int** buf1;//буффер 1
	int** buf2;//буффер 2
	int i,//счетчик
		j,//счетчик
		k,//счетчик
		begin,//число минтермов после шага склейки без повторных склеек
		end,//число минтермов после склейки без затирания повторных склеек
		numberofglues=-1;//число склеек
	int r=0,//число несовпадений у двух минтермов(для склейки нужно 1-о несовпадение)
		sk=0;//число склеек у соотв-го минтерма
	int value;//переменная, учавствующая в преобразовании номера минтерма в сам минтерма
	clock_t time_begin, time_end;

	//проверка на число минтермов, если их макс кол-во, то склейка выдаст 1, незачем вызывать процесс склейки
	if (NumberOfMinterms == pow(2, NumberArguments))
	{
		printf("Quine minimization:\nresult of minimization: 1");
		printf("\nTime Quine:1 nanosecond\n");
		return;
	}

	//задание двумерных массивов для дальнейшего жанглирования между ними минтермов
	buf1 = (int**)calloc((NumberOfMinterms*2) , sizeof(int));
	for(i=0;i<NumberOfMinterms*2;i++)
		buf1[i]= (int*)calloc((NumberArguments+1) , sizeof(int));
	buf2 = (int**)calloc((NumberOfMinterms * 2) , sizeof(int));
	for (i = 0;i < NumberOfMinterms*2;i++)
		buf2[i] = (int*)calloc((NumberArguments + 1) , sizeof(int));

	//создали двоичном минтермы с числом аргументов и указателем работы с ними
	for (i = 0;i < NumberOfMinterms;i++)
	{
		value = MassiveOfMinterms[i];
		for (j = NumberArguments-1;j >=0;j--)
		{
			buf1[i][j] = value % 2;
			value = value / 2;
		}
		buf1[i][NumberArguments] = 0;
	}
	//засекли время начала склейки
	time_begin = ((double)clock())/((double)CLOCKS_PER_SEC);
	//запомнили число минтервом для первой прогонки
	begin = NumberOfMinterms;
	//начало цикла работы с буферами минтермов
	while (1)
	{
		numberofglues = 0;//обнуляем число склеек
		end = 0;//обнуление числа склеенных минтермов
		//склеивание минтермов
		for (i = 0;i < begin;i++)
		{
			sk = 0;
			for (j = i + 1;j < begin;j++)
			{
				for (k = 0;k < NumberArguments;k++)//сравнение двух минтермов
				{
					if (buf1[i][k] != buf1[j][k])//сравнение элементов минтермов
						r++;
				}
				if (r == 1)//минтермы могут быть склеены и заносится результат склейки во второй буффер
				{
					for (k = 0;k < NumberArguments;k++)//переписывание склейки во второй буффер с заменой склееннго элемента
					{
						if (buf1[i][k] != buf1[j][k])
							buf2[end][k] = -1;
						else
							buf2[end][k] = buf1[i][k];
					}
					buf2[end][k] = 0;//число склеек у склекйки =0
					//увеличение числа склеек у склеенных минтермов
					buf1[i][k] = buf1[i][k] + 1; 
					buf1[j][k] = buf1[j][k] + 1;
					sk++;
					//увеличение числа склеек
					end++;
					numberofglues++;
				}
				r = 0;//обнуление числа различий у двух анализируеммыз минтермов
			}
			if ((sk == 0)&&(buf1[i][NumberArguments]==0))//у данного минтерма есть склейки если нет, то помечаем его
				buf1[i][NumberArguments] = -1;
		}
		
		//проверка на число склеек, если 0, то останавливаем процесс склейки и выносим из цикла результата склейки
		if (numberofglues == 0)
			break;
		
		//перенос в буффер 2 несклеенные минтермы
		for (i = 0;i < begin;i++)
		{
			if (buf1[i][NumberArguments] == -1)
			{
				for (j = 0;j < NumberArguments + 1;j++)
					buf2[end][j] = buf1[i][j];
				end++;
			}
		}

		//освободили память, выделенную для буфера 1, чтобы после проверки на одинаковые минтермы в буффере 2,
		//выделить меньшее или большее число памяти, чтобы не выйти за границу памяти при дальнейшей работе
		for (i = 0;i < begin*2;i++)
			free(buf1[i]);
		free(buf1);
		//выделение памяти для буффера 1, чтобы потом поместить в него результат соотв-его цикла склейки
		buf1 = (int**)calloc((end * 2), sizeof(int));
		for (i = 0;i < end * 2;i++)
			buf1[i] = (int*)calloc((NumberArguments + 1), sizeof(int));

		//проверка на одинаковые минтермы после склейки
		sk = 0;
		for (i = 0;i < end;i++)
		{
			for (j = i + 1;j < end;j++)
			{
				for (k = 0;k < NumberArguments;k++)//сравнение двух минтермов
				{
					if (buf2[i][k] != buf2[j][k])
						r++;
				}
				if (r == 0)//минтермы могут быть склеены
					sk++;
				r = 0;
			}
			if (sk != 0)//если есть идентичный минтерм, то у данного минтерма указатель обработки =0
				buf2[i][NumberArguments] = 0;
			else//если же нет, то -2
				buf2[i][NumberArguments] = -2;
			sk = 0;
		}

		//перенеос в буффер 1 неповторяющихся минтермов из буффера 2
		sk = 0;
		for (i = 0;i < end;i++)
		{
			if (buf2[i][NumberArguments] < 0)
			{
				for (j = 0;j < NumberArguments;j++)
					buf1[sk][j] = buf2[i][j];
				buf1[sk][j] = 0;
				sk++;
			}
		}

		//очистка памяти, выделенной для буффера 2, для уменьшения или увеличения его размерности для дальнейшей работы
		for (i = 0;i < begin*2;i++)
			free(buf2[i]);
		free(buf2);
		
		//задание памяти для буффера 2, для дальнейших помещений в него склеек
		buf2 = (int**)calloc((sk * 2), sizeof(int));
		for (i = 0;i < sk * 2;i++)
			buf2[i] = (int*)calloc((NumberArguments + 1), sizeof(int));

		begin = sk;
	}
	//засекли время конца склейки
	time_end = ((double)clock()) / ((double)CLOCKS_PER_SEC);
	//печать результата минимизации
	printf("Quine minimization:\n");
	printf("\nresult of minimization: ");

	for (i = 0;i < begin;i++)
	{
		for (j = 0;j < NumberArguments;j++)
		{
			if (buf1[i][j] != (-1))
				printf("%d", buf1[i][j]);
			else
				printf("_");
		}
		if(i!=(begin-1))
			printf("+");
	}
	//печать времени минимизации
	if (time_end != time_begin)
		printf("\nTime Quine:%d nanoseconds\n", time_end - time_begin);
	else
		printf("\nTime Quine:1 nanosecond\n");
}

void Mak_Klasky(int* MassiveOfMinterms, int NumberArguments, int NumberOfMinterms)
{
	int** buf1;//буффер 1
	int** buf2;//буффер 2
	int* buf3 = { 0 };//буффер, нужный для начальной сортировки минтермов по числу единиц
	int i,j,k,//просто счетчики для циклов
		s,//счетчик числа минтермов, прошедших цикл склейки, т.е. buf1->buf2->buf1
		sk,//счетчик числа склеенных минтермов за этап
		r;//счетчик числа отличий у сравниваемых минтермов
	int value,//переменная, нужная для сортировки минтермов по числу единиц
		swithces = -1,//число склеек у соотв-го минтерма
		numberofglues = -1;//число склеек на соотв-ем этапе, нужна для остановки цикла склейки
	int begin, end;//число минтермов в начале и конце "круга склейки" соотв-но
	clock_t time_begin, time_end;//время начала и конца склейки

	//проверка на число минтермов, если их макс кол-во, то склейка выдаст 1, незачем вызывать процесс склейки
	if (NumberOfMinterms == pow(2, NumberArguments))
	{
		printf("Mak-Laski minimization:\nresult of minimization: 1");
		printf("\nTime Quine:1 nanosecond\n");
		return;
	}
	//задание двумерных массивов для дальнейшего жанглирования между ними минтермов
	buf1 = (int**)calloc((NumberOfMinterms * 2), sizeof(int));
	for (i = 0;i < NumberOfMinterms * 2;i++)
		buf1[i] = (int*)calloc((NumberArguments + 2), sizeof(int));
	buf2 = (int**)calloc((NumberOfMinterms * 2), sizeof(int));
	for (i = 0;i < NumberOfMinterms * 2;i++)
		buf2[i] = (int*)calloc((NumberArguments + 2), sizeof(int));

	//создали двоичном минтермы с числом аргументов, указателем работы с ними и числом единиц
	for (i = 0;i < NumberOfMinterms;i++)
	{
		k = 0;
		value = MassiveOfMinterms[i];
		for (j = NumberArguments - 1;j >= 0;j--)
		{
			buf1[i][j] = value % 2;
			if (buf1[i][j] == 1)
				k++;
			value = value / 2;
		}
		buf1[i][NumberArguments] = 0;
		buf1[i][NumberArguments + 1] = k;
	}
	//отсортировали минтермы по числу единиц в них
	while (swithces != 0)
	{
		swithces = 0;
		for (i = 0;i < NumberOfMinterms - 1;i++)
		{
			for (j = i + 1;j < NumberOfMinterms;j++)
			{
				if (buf1[i][NumberArguments + 1] > buf1[j][NumberArguments + 1])
				{
					buf3 = buf1[i];
					buf1[i] = buf1[j];
					buf1[j] = buf3;
					swithces++;
				}
			}
		}
	}
	//засекли время начала склейки
	time_begin = ((double)clock()) / ((double)CLOCKS_PER_SEC);
	//запомнили число минтервом для первой прогонки
	begin = NumberOfMinterms;
	//начало цикла склеиваний
	while(1)
	{
		numberofglues = 0;//обнуляем число склеек
		sk = 0;//обнуление числа склеенных минтермов
		//склеивание минтермов
		for (i = 0;i < begin;i++)
		{
			s = 0;
			for (j = i + 1;j < begin;j++)
			{
				if ((buf1[i][NumberArguments + 1] + 1) == buf1[j][NumberArguments + 1])//проверка на соседние группы минтермов, т.е. данный минтерм будет склеиваться с тем, у которого число единиц ровно на 1 больше чем у него
				{
					r = 0;
					for (k = 0;k < NumberArguments;k++)//сравнение двух минтермов
					{
						if (buf1[i][k] != buf1[j][k])
							r++;
					}
					if (r == 1)//если отличаются только в одном аргументе
					{
						for (k = 0;k < NumberArguments;k++)//запись склейки в буффер 2
						{
							if (buf1[i][k] != buf1[j][k])
								buf2[sk][k] = -1;
							else
								buf2[sk][k] = buf1[i][k];

						}
						buf2[sk][NumberArguments + 1] = buf1[i][NumberArguments + 1];//запись числа единиц у склейки, равное числу единиц у первого минтерма, с которым производится склейка
						//увеличение числа склеек у первого и второго склеенного минтерма
						buf1[i][NumberArguments]++;
						buf1[j][NumberArguments]++;
						numberofglues++;//увеличение числа склееку данного минтерма
						sk++;//переход на следующую позицию в буффере 2
						s++;//увеличение числа склеек у данного минтерма, нужна для дальнейшей проверки
					}
				}
			}
			if ((s == 0) && (buf1[i][NumberArguments] == 0))//если данный минтерма ни с чем не склеелся, то помечаем его флагом "-1"
				buf1[i][NumberArguments] = -1;
		}

		if (numberofglues == 0)//проверка на наличие склеек
			break;
		//перенос несклееных минтеров в буффер 2
		for (i = 0;i < begin;i++)
		{
			if (buf1[i][NumberArguments] == (-1))
			{
				for (j = 0;j < NumberArguments + 2;j++)
					buf2[sk][j] = buf1[i][j];
				sk++;
			}
		}
		end = sk;
		//освободили память, выделенную для буфера 1, чтобы после проверки на одинаковые минтермы в буффере 2,
		//выделить меньшее или большее число памяти, чтобы не выйти за границу памяти при дальнейшей работе
		for (i = 0;i < begin*2 ;i++)
			free(buf1[i]);
		free(buf1);
		//выделение памяти для буффера 1, чтобы потом поместить в него результат соотв-его цикла склейки
		buf1 = (int**)calloc((end * 2), sizeof(int));
		for (i = 0;i < end * 2;i++)
			buf1[i] = (int*)calloc((NumberArguments + 2), sizeof(int));

		//проверка на одинаковые минтермы после склейки
		for (i = 0;i < end;i++)
		{
			sk = 0;
			for (j = i + 1;j < end;j++)
			{
				s = 0;
				for (k = 0;k < NumberArguments;k++)
				{
					if (buf2[i][k] == buf2[j][k])
						s++;
				}
				if (s == NumberArguments)
					sk++;
			}
			if(sk==0)
				buf2[i][NumberArguments] = -2;
			else
				buf2[i][NumberArguments] = 0;
		}

		//перенеос в буффер 1 неповторяющихся минтермов из буффера 2
		s = 0;
		for (i = 0;i < end;i++)
		{
			if (buf2[i][NumberArguments] < 0)
			{
				for (j = 0;j < NumberArguments + 2;j++)
					buf1[s][j] = buf2[i][j];
				buf1[s][NumberArguments] = 0;
				s++;
			}
		}

		//очистка памяти, выделенной для буффера 2, для уменьшения или увеличения его размерности для дальнейшей работы
		for (i = 0;i < begin*2;i++)
			free(buf2[i]);
		free(buf2);

		//задание памяти для буффера 2, для дальнейших помещений в него склеек
		buf2 = (int**)calloc((s * 2), sizeof(int));
		for (i = 0;i < s * 2;i++)
			buf2[i] = (int*)calloc((NumberArguments + 2), sizeof(int));
		begin = s;
	}
	//засекли время конца склейки
	time_end = ((double)clock()) / ((double)CLOCKS_PER_SEC);

	//печать результата минимизации
	printf("Mak-Klaski minimization:\n ");
	printf("\nresult of minimization: ");

	for (i = 0;i < begin;i++)
	{
		for (j = 0;j < NumberArguments;j++)
		{
			if (buf1[i][j] != (-1))
				printf("%d", buf1[i][j]);
			else
				printf("_");
		}
		if (i != (begin - 1))
			printf("+");
	}
	//печать времени минимизации
	if (time_end != time_begin)
		printf("\nTime Mak-Klaski:%d nanosekonds\n", time_end - time_begin);
	else
		printf("\nTime Mak-Klaski:1 nanosekond\n");
}