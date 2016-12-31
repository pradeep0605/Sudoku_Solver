#include<stdio.h>
#include<iostream>
#include "set.cpp"

#define N 9

using namespace std;	

class Cell
{
	public:
	unsigned int value;
	unsigned int box;
	Set PossibleValues;
	
	Cell()
	{
		value=0;box=-1;
		PossibleValues=Set();
		for(int i=0;i<N;i++)
			PossibleValues.addElement(i+1); // Initially a cell may contain any one of 'N' (9 for eg) values
	}
	
	Cell(int n)
	{
		if(n==-1)
			for(int i=0;i<N;i++)
				PossibleValues.addElement(i+1);
		else
			PossibleValues.addElement(n);
	}
	
	void setValue(unsigned int x)
	{
		value=x;
	}	
	
	unsigned int getValue()
	{
		return value;
	}
};


class SudokuSolver
{
	public:
	Cell cells[N][N];
	Set rows[N];
	Set columns[N];
	Set boxes[N];
	unsigned int remainingCells;
	
	SudokuSolver()
	{
		remainingCells=N*N;
		for(int i=0;i<N;i++)
		{
			rows[i]=Set();
			columns[i]=Set();
			boxes[i]=Set();
			for(int j=0;j<N;j++)
			{
				cells[i][j]=Cell();
				cells[i][j].box = getBoxValue(i,j);
			}
				
		}		
	}
	
	void display()
	{
		for(int i=0;i<N;i++)
		{
			if(i%3==0)
				cout<<endl;
			for(int j=0;j<N;j++)
			{	
				if(j%3==0)
					cout<<"\t";
				cout<<cells[i][j].getValue()<<":"<<cells[i][j].box<<"   ";
			}
			cout<<endl;
		}
		
		for(int i=0;i<N;i++)
		{
			cout<<"BOX "<<i<<endl;
			boxes[i].display(true);
				
		}
		
		/*for(int i=0;i<N;i++)
		{
			cout<<"ROW "<<i<<endl;
			rows[i].display(true);
		}
		
		for(int i=0;i<N;i++)
		{
			cout<<"COLUMN "<<i<<endl;
			columns[i].display(true);
		}*/
	
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<N;j++)
			{
			    if(cells[i][j].value==0)
			    {
				cout<<"cell["<<i<<"]["<<j<<"] 's possible values are\n";
				cells[i][j].PossibleValues.display(true);	
				cout<<cells[i][j].PossibleValues.getElementsCount()<<endl;			
			    }
			}
			cout<<endl;
		}
	}
	
	int getBoxValue(int x,int y)
	{
		return ( ((x/3)*3) + (y/3));
	}
	
	void setValueAt(unsigned int x,unsigned int y,unsigned int value)
	{
		cells[x][y].setValue(value);	
		rows[x].addElement(value);
		columns[y].addElement(value);
		boxes[getBoxValue(x,y)].addElement(value);
	}
	
	void readInitialValues()
	{	
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)
			{
				int temp=0;
				cin>>temp;
				if(temp>0) // if the read value is greater than zero then add the value to the corresponding
				{	   // row and columns
					remainingCells--;
					cells[i][j].setValue(temp);
					boxes[getBoxValue(i,j)].addElement(temp	);
					rows[i].addElement(temp);
					columns[j].addElement(temp);
				}		
			}
		
	}
	
	
	void filter() // this method would make each cells to keep only those possible values that might appear in that cell.
	{	      // That is a cell might hold only those values which are not their in the box, in the row and column if belongs to.
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<N;j++)
			{
				if(cells[i][j].value==0)
				{
					Set temp=Set();
					temp=cells[i][j].PossibleValues;
					cells[i][j].PossibleValues= temp - boxes[getBoxValue(i,j)] - rows[i] - columns[j];
				// Initially PossibleValues will have all the 'N' (eg. 9 here) values.
				}
			}
		}
	}
	
	bool solveSudoku()
	{
		bool basicFilling=false,  elimination=false;
		while(remainingCells > 0)
		{
			int n=0;
			basicFilling=false;

			do
			{
				n=0;
				cout<<"Remaining :"<<remainingCells<<endl;
				filter();
				n=performBasicFilling();
				remainingCells-=n;
				if(n>0)
					basicFilling=true;
				
			}while(n);	


			elimination=false;
			do
			{
				n=0;
				cout<<"Remaining :"<<remainingCells<<endl;
				filter();
				n=fillUsingEliminationTechnique();			
				remainingCells-=n;
				if(n>0)
					elimination=true;
			}while(n);
			
			
			if(!(basicFilling || elimination))
				break;
		}
		cout<<"Terminating with remaining cells to be filled :"<<remainingCells<<endl;
		if(!(basicFilling || elimination))
		{
			cout<<"AM NOT ABLE TO SOLVE THIS COZ EITHER INPUT IS WRONG OR I DON'T HAVE ABILITY TO SOLVE !!!\n";
			return false;
		}
		else
		{
			display();
			cout<<"SUDOKU SOLVED !!!!! \n\n";
			return true;
		}
		
					
	}
	
	
	// This method would fill a cell (i,j) with a value which cannot appear in any other cell..
	int fillUsingEliminationTechnique()
	{
		int count=0;
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)
			{
				if(cells[i][j].value == 0)
				{ 
				        Set temp=Set();
				       // cout<<"Before\n";
				       // cells[i][j].PossibleValues.display(true);
				        temp=cells[i][j].PossibleValues;
				        temp =  temp -  getUnion(i,j);
				        //	cout<<"DAMN COMING HERE !\n";
				        //cells[i][j].PossibleValues.display(true);
				        //getUnion(i,j) will give Union of possibleValues of cells for which values yet to be decided
				        // ,excluding cell[i][j]..
				        if( temp.getElementsCount() == 1)
				        {
				        	int k=temp.getFirstElement();
				        	setValueAt(i,j,k);
						count++;
						cout<<"A value \""<<cells[i][j].value<<"\" was determined at ["<<i<<","<<j<<"]";
						cout<<" using ELIMINATION TECHINIQUE\n";
						filter();
						getchar();
				        }
				}
			}
		return count;
	}

			
	// This method helps in getting the union of all the cells'(only those for which values yet to be determined)
	//  possible values ( except cell (i,j)).
	// Set difference of cell (i,j) with union is done. if results in one "value" then it means that it is the only cell which 
	// could get the "value".  e.g. consider there are four cells in a box where 7 cannot appear in 3 of those cells
	// due to that value is present in rows or columns of those 3 cells. And the only remaining cell can get that value.
	Set getUnion(int I, int J)
	{
		//cout<<"FInding union for the cells ("<<I<<","<<J<<" ) \n";
		//cells[I][J].PossibleValues.display(true);
		Set s=Set();
		int n=((I/3)*3);
		int m=((J/3)*3);
		for(int i=n ; i< n+3; i++)
			for(int j=m; j< m+3; j++)
				if(cells[i][j].value == 0 && (i!=I || j!=J ))
					s=s + cells[i][j].PossibleValues;	
		return s;			
	}
	

	//this method perform basic filling operation. i.e. fill those cells which have number of PossibleValues to be filled as only one.
	//this method returns the number of cells which were filled using this method at once.
	int performBasicFilling()
	{
		int count=0;
		int ch;
		
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)
			{ 
			    if(cells[i][j].value == 0)
				if( cells[i][j].PossibleValues.getElementsCount() == 1)
				{
					int k=cells[i][j].PossibleValues.getFirstElement();
					setValueAt(i,j,k);
					count++;
					cout<<"A value \""<<cells[i][j].value<<"\" was determined at ["<<i<<","<<j<<"]";
					cout<<" using BASIC FILLING METHOD \n";

				} 
			}
			
		return count;
	}
	

	
};


int main()
{
	SudokuSolver S=SudokuSolver();
	S.readInitialValues();
	S.filter();
	S.display();
	S.solveSudoku();
	
	return 0;
}




