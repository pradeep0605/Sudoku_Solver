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
	{ 	value=x; 	}	
	
	unsigned int getValue()
	{ 		return value; 	}
};

class Box
{
	public:
	Set rows[3],columns[3];
	Box()
	{	
		for(int i=0;i<3;i++)
		{
			rows[i]=Set();
			columns[i]=Set();
		}
	}
};

class SudokuSolver
{
	public:
	Cell cells[N][N];
	Set rows[N];
	Set columns[N];
	Set boxes[N];
	Box EnsuredBoxValues[N];
	unsigned int remainingCells;
	
	SudokuSolver()
	{
		remainingCells=N*N;
		for(int i=0;i<N;i++)
		{
			rows[i]=Set();
			columns[i]=Set();
			boxes[i]=Set();
			EnsuredBoxValues[i]=Box();
			for(int j=0;j<N;j++)
			{
				cells[i][j]=Cell();
				cells[i][j].box = getBoxValue(i,j);
			}
				
		}		
	}
	
	
	
	int getBoxValue(int x,int y)
	{ 	return ( ((x/3)*3) + (y/3)); 	}
	
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
  
  
 
	
	void filter() // This method would make each cells to keep only those possible values that might appear in that cell.
	{	         // That is a cell might hold only those values which are not their in the box, in the row and column if belongs to.
	
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
		bool basicFilling=false,  elimination=false, RowColumnAnalysis=false;
		int remainingnow=81;
		while(remainingCells > 0)
		{
			int n=0;
			basicFilling=false;

			do
			{
				n=0;
				//cout<<"Remaining :"<<remainingCells<<endl;
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
				//cout<<"Remaining :"<<remainingCells<<endl;
				filter();
				n=fillUsingEliminationTechnique();			
				remainingCells-=n;
				if(n>0)
					elimination=true;
			}while(n);
			
			RowColumnAnalysis=false;
			do
			{
				n=0;
				//cout<<"Remaining :"<<remainingCells<<endl;
				filter();
				n=fillUsingRowColumnAnalysis();			
				remainingCells-=n;
				if(n>0)
					RowColumnAnalysis=true;
			}while(n);
			
			filter();
			
			
			if(!(basicFilling || elimination || RowColumnAnalysis))
				break;
		}
		//cout<<"Terminating with remaining cells to be filled :"<<remainingCells<<endl;
		if(!(basicFilling || elimination || RowColumnAnalysis))
			return false;
		else
			return true;
	}
	
	
	int fillUsingRowColumnAnalysis()
	{
		int n=0;
		
		//row analysis
		for(int i=0;i<N;i++)
		{
			int value=0;
			Set currentRow=rows[i],temp=Set(1,9), temp2=Set();
			temp= temp-currentRow; // will give you the values to be filled in this row.
			while((value=temp.getNextElement()))
				for(int j=0;j<N;j++)
				   if(cells[i][j].value==0)
					if(cells[i][j].PossibleValues.search(value)==true)
					{
						temp2=cells[i][j].PossibleValues - getRowUnion(i,j);
						//cout<<"Here\n";
						if( temp2.getElementsCount() == 1)
						{
							setValueAt(i,j,temp2.getFirstElement());
							n++;
							cout<<cells[i][j].value<<" "<<i<<" "<<j<<endl;
						     //cout<<" using Row Analysis\n";
						}
					}
		}
		
		filter();		// To make sure that column analysis does not do the same job as row analysis
						// So update the PossibleValues of all the cell affected by RowAnalysis
		//column analysis
		for(int j=0;j<N;j++)// for each column
		{
			int value=0;
			Set currentColumn=columns[j],temp=Set(1,9), temp2=Set();
			temp= temp-currentColumn; // will give you the values to be filled in this column. 
			while((value=temp.getNextElement()))
				for(int i=0;i<N;i++) // every row in each column
				   if(cells[i][j].value==0)
					if(cells[i][j].PossibleValues.search(value)==true)
					{
						temp2=cells[i][j].PossibleValues - getColumnUnion(i,j);
						//cout<<"Here\n";
						if( temp2.getElementsCount() == 1)
						{
							setValueAt(i,j,temp2.getFirstElement());
							n++;
							cout<<cells[i][j].value<<" "<<i<<" "<<j<<endl;
						     //cout<<" using column Analysis\n";
						}
					}
		}
		return n;
	}
	
	//This method would return a set which would give the union of all the cell in row 'x' except the cell[x][y]
	Set getRowUnion(int x,int y)
	{
		Set result=Set();
		for(int i=0; i<N  ; i++)
			if(cells[x][i].value==0 && (i!=y))
				result= result + cells[x][i].PossibleValues;
		return result;
	}
	
	Set getColumnUnion(int x,int y)
	{
		Set result=Set();
		for(int i=0; i<N  ; i++)
			if(cells[i][y].value==0 && (i!=x))
				result= result + cells[i][y].PossibleValues;
		return result;
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
				        // getUnion(i,j) will give Union of possibleValues of cells for which values yet to be decided
				        // ,excluding cell[i][j]..ss
				        if( temp.getElementsCount() == 1)
				        {
				        	int k=temp.getFirstElement();
				        	setValueAt(i,j,k);
						count++;
						cout<<cells[i][j].value<<" "<<i<<" "<<j<<endl;
						//cout<<" using ELIMINATION TECHINIQUE\n";
						//filter(0);
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
					cout<<cells[i][j].value<<" "<<i<<" "<<j<<endl;
					//cout<<" using BASIC FILLING METHOD \n";
					//filter(0);
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
	cout<<S.solveSudoku()<<endl;
	cout.flush();
	
	return 0;
}



/*

*/
				
		
