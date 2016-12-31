fillUsingRowColumnAnalysis()
{   n=0;
	for(i=0 to 8)
	{	value=0
		Set currentRow=rows[i],temp=Set(1,2,..,9), temp2
		temp= temp-currentRow
		while((value=temp.getNextElement()))
		   for(j=0 to 8)
			 if( cells[i][j] to be filled)
			   if(cells[i][j].PossibleValues.search(value)= found)
				{	temp2=cells[i][j].PossibleValues - getRowUnion(i,j)
					if( |temp2| = 1)
					{	setValueAt(i,j,temp2)
						n++
					}
				}
	}
	for(j=0 to 8)
	{	int value=0
		Set currentColumn=columns[j],temp=Set(1,2,..,9), temp2
		temp= temp-currentColumn
		while((value=temp.getNextElement()))
			for(int i=0 to 8) 
			   if(cells[i][j] to be filled)
				if(cells[i][j].PossibleValues.search(value)= found)
				{	temp2=cells[i][j].PossibleValues - getColumnUnion(i,j)
					if( |temp2| = 1)
					{   setValueAt(i,j,temp2)
						n++
					}
				}
	}
	return n
}
