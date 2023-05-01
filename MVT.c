#include<stdio.h>

int mm, frs, p[15], fre1, fre2, fr, i = -1, fre;
int main()
{
	char ch;
	void insert();
	void del();
	void display();
	printf("Enter Maximum size: ");
	scanf("%d", &mm);
	printf("Enter number of partitions: ");
	scanf("%d", &fr);
	fre2 = mm;
	fre1 = fr;
	if (mm % fr != 0)
	{
		printf("No memory\n");
		return 1;
	}
	frs = mm / fr;
	while (1)
	{
		printf("\n1.Insertion\n2.Deletion\n3.Display\n4.Quit");
		printf("\nEnter your choice: ");
		fflush(stdin);
		scanf(" %c", &ch);
		switch (ch)
		{
		case '1': insert(); break;
		case '2': del(); break;
		case '3': display(); break;
		case '4': return 0;
		}
	}
}
void insert()
{
	int j, temp, fre = 0;
	printf("\nEnter the size of process: ");
	scanf("%d", &temp);
	if (temp <= fre2)
	{
		p[++i] = temp;
		for (j = i; j >= 0; j--)
		{
			fre += (p[j] / frs);
			if (p[j] % frs != 0)
				fre++;
		}
		fre1 = fr - fre;
		printf("\nNo. of Pages free: %d", fre1);
		fre2 = fre1*frs;
	}
	else
		printf("\nUnable to insert");
}
void del()
{
	int temp, var;
	printf("\n Enter which process to delete: ");
	scanf("%d", &temp);
	temp--;
	if (temp > i)
		printf("\nNot possible");
	else
	{
		var = p[temp];
		while (temp != i)
		{
			p[temp] = p[temp + 1];
			temp++;
		}
		i--;
		temp = var / frs;
		if (var % frs != 0)
			temp++;
		fre1 += temp;
		fre2 = fre1*frs;
		printf("\nNo. of Partitions free = %d", fre1);
	}
}
void display()
{
	int j, waste = 0;
	for (j = i; j >= 0; j--)
		waste += p[j];
	waste += fre2;
	waste = mm - waste;
	printf("\nNo. of Partitions free = %d", fre1);
	printf("\nWaste memory in Internal partitions = %d", waste);
}
