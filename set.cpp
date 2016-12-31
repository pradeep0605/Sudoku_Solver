#include<iostream>
#include<bitset>
#include<stdlib.h>
#include<time.h>
#define NNull -1
#define NO_BITS 1

using namespace std;

unsigned int getRandomValue()
{
   return (rand()*time(NULL));
}


class node
{
	public:
	unsigned int base;
	bitset <NO_BITS> bits;
	node *next, *prev;
	    
	node()
	{
	   next=prev=NULL;
	   base=0;   	
	}
};

class Set
{
     public:
	node *first, *last;
	bool displayNumbers;
	node *current;
	
     unsigned int no_nodes; 
	Set()
	{
	  	first=last=NULL;
	  	current=(node*)NNull;
	  	no_nodes=0;
	  	displayNumbers=false;
	}
	
	Set(int start,int end)
	{
		first=last=NULL;
		current=(node*)NNull;
	  	no_nodes=0;
	  	displayNumbers=false;
		for(int i=start;i<=end;i++)
			addElement(i);	
	}
	
	Set setDisplayNumbers(bool b)
	{
	    displayNumbers=b;
	    return *this;
	}
	
	unsigned int getSizeofSet()
	{
	    	return (sizeof(node)*no_nodes  + 12);  // +12 is for two pointers and one interger       
	}
	
	unsigned int getNodeCount()// just returns the number of nodes in the set;
	{
	  	return (no_nodes);
	}

        friend ostream& operator <<(ostream &out, Set S)
        {
             S.display(S.displayNumbers);
             return out;
        }

        /* SET ASSIGNMENT */
        void operator = (Set a)  // copy of two sets.
        {
             this->no_nodes=a.no_nodes;
             node *temp=a.first;
             node *prev=NULL;
             while(temp!=NULL)
             {
                	node *n=new node;
                	n->base=temp->base;
                	n->bits=temp->bits;
                        
                        
                        if(prev!=NULL)
                        {
				prev->next=n;
                           	n->prev=prev;
                           	last=n;
                        }
                        else
                        {
                           first=last=n;
			}                           	
                     	prev=n;
                       	temp=temp->next;                	
             }
        }
        /////////////////////////////////////////////////////////////////////////////////// 
        
        /* SET COMPLEMENT OPERATION   */
        Set operator ~()   // Complement of a set.
        {
             Set TP;
             TP.operator=(*this);
             node *temp=TP.first;
             while(temp!=NULL)
             {          
                       temp->bits = (~temp->bits);
                       /*
              		for(int i=0;i<NO_BITS;i++)
              		{
              		   	temp->bits[i]= (temp->bits[i] + 1 ) % 2;
              		}*/ 
              		temp=temp->next;
             }
             return TP;
        }
        ///////////////////////////////////////////////////////////////////////////////////
 	
        bool search(unsigned int key)
        {
        	node *temp=this->first;
        	while(temp!=NULL)
        	{
        		if(  (temp->base+NO_BITS-1) >= key)
        		{
				if(temp->bits[ key - temp->base])
				  return true;	
        		}
        		temp=temp->next;
        	}
		return false;
        }

	unsigned int getElementsCount()
	{
		int count=0;
		node *temp=this->first;
        	while(temp!=NULL)
        	{
			if(temp->bits[0])
				  count++;
        		temp=temp->next;
        	}
		return count;
	}

        /*  SET UNION OPERATION */
        friend Set operator + (Set A, Set B)
        {
               	Set TP;
             	node *tempA=A.first, *tempB=B.first, *prev;
             	prev=NULL;
	     	while(tempA != NULL && tempB!=NULL)
	     	{
	     	    TP.no_nodes++;   
	     	    if(tempA->base== tempB->base)
	     	    {
	     	         node *n=new node ;
	     	         n->base=tempA->base;
	     	         n->bits = (tempA->bits | tempB->bits); // OR OPERATION FOR UNION
	     	         if(prev==NULL)
	     	             TP.first=TP.last=n;
	     	         else
	     	         {
	     	             TP.last->next=n;
	     	             n->prev=TP.last;
	     	             TP.last=n;
	     	         }
	     	         prev=n;
	     	         tempA=tempA->next;
	     	         tempB=tempB->next;
	     	    }
	     	    else
		     	    if(tempA->base < tempB->base)
		     	    {
		     	      	 node *n=new node;
		     	         n->base=tempA->base;
		     	         n->bits=tempA->bits;
		     	         if(prev==NULL)
		     	             TP.first=TP.last=n;
		     	         else
		     	         {
		     	             TP.last->next=n;
		     	             n->prev=TP.last;
		     	             TP.last=n;
		     	         }
		     	         prev=n;
		     	         tempA=tempA->next;
		     	    }
		     	    else
		     	    {
		     	      	 node *n=new node ;
		     	         n->base=tempB->base;
		     	         n->bits=tempB->bits;
		     	          if(prev==NULL)
		     	             TP.first=TP.last=n;
		     	         else
		     	         {
		     	             TP.last->next=n;
		     	             n->prev=TP.last;
		     	             TP.last=n;
		     	         }
		     	         prev=n;
		     	         tempB=tempB->next;
		     	    }
	     	    
	     	}
	     	
	     	while(tempA==NULL && tempB!=NULL)
	     	{
		   	node *n=new node ;
		    	n->base=tempB->base;
		     	n->bits=tempB->bits;
		     	if(prev==NULL)
		      		TP.first=TP.last=n;
		     	else
		     	{
		     		TP.last->next=n;
		     	        n->prev=TP.last;
		     	        TP.last=n;
		     	}
		     	prev=n;
		     	tempB=tempB->next;
		     	TP.no_nodes++;
	     	}	             
	     
	        while(tempA!=NULL && tempB==NULL)
	     	{
	     	  	node *n=new node ;
		    	n->base=tempA->base;
		     	n->bits=tempA->bits;
		     	if(prev==NULL)
		      		TP.first=TP.last=n;
		     	else
		     	{
		     		TP.last->next=n;
		     	        n->prev=TP.last;
		     	        TP.last=n;
		     	}
		     	prev=n;
		     	tempA=tempA->next;
		     	TP.no_nodes++;
	     	}
             	return TP;
        }   
        ///////////////////////////////////////////////////////////////////////////////////
 	
 	
 	/*  SET INTERSECTION OPERATION */
        friend Set operator & (Set A, Set B)
        {
               Set TP=Set();
             	node *tempA=A.first, *tempB=B.first, *prev;
             	prev=NULL;
	     	while(tempA != NULL && tempB!=NULL)
	     	{
	     	    if(tempA->base== tempB->base)
	     	    {
	     	    		if( (tempA->bits.to_ulong() & tempB->bits.to_ulong()) != 0  )
	     	    		{
	     	    			node *n=new node;
	     	    			n->base=tempA->base;
	     	    			n->bits = (tempA->bits & tempB->bits);
	     	    			if(prev==NULL)
	     	    		        {
	     	    			    TP.first=TP.last=n;
	     	    			    prev=n;
	     	    			}    
	     	    			else
	     	    			{
	     	    			    TP.last->next=n;
	     	    			    n->prev=TP.last;
	     	    			    TP.last=n;
	     	    			}
	     	    		}
	     	    		
	     	    		tempA=tempA->next;
	     	    		tempB=tempB->next;
	     	    }
	     	    else
		     	    if(tempA->base < tempB->base)
	     	    		tempA=tempA->next;
		     	    else
	     	    		tempB=tempB->next;
	     	}
             	return TP;
        }
        ///////////////////////////////////////////////////////////////////////////////////
 	

 	
 	/* SET DIFFERENCE OPERATION */
 	friend Set operator - (Set A, Set B)
 	{
          	Set TP=Set(); // Set TP= A; wouuld make just to directly assign the fields of the class but won't copy the DataStructure.
          	TP=A; // copy the set 'A' to the TP
          	
             	node *tempTP=TP.first, *tempB=B.first, *prev;
             	prev=NULL;
	     	while(tempTP != NULL && tempB!=NULL)
	     	{
	     	    if(tempTP->base == tempB->base)
	     	    {
	     	    		tempTP->bits= (tempTP->bits & (~tempB->bits));  // A - b= A INTERSECTION ( B COMPLEMENT)
	     	    		tempTP=tempTP->next;
	     	    		tempB=tempB->next;
	     	    }
	     	    else
		     	    if(tempTP->base < tempB->base)
					tempTP=tempTP->next;
		     	    else
					tempB=tempB->next;
	     	}
             	return TP;       
 	}
 	///////////////////////////////////////////////////////////////////////////////////
 	
	void addElement(unsigned int i)        //adding an element in to the set. (if node already exists then set the bit of the node accordingly
	{				       //else create a new node which will accomodate the 'i')		
            
            if(first==NULL && last==NULL)      // If this is the first value in the set then create a new node and add it.
            {
               node *n=new node;
               unsigned int tempres= i/NO_BITS; // To get to know the base value for the node in which this ('i') value will fall.
               n->base=tempres*NO_BITS;
               n->bits[i-n->base]=1;
               first=last=n;
               no_nodes++;
               return ;
            }
            
            if( i < first->base)    //if the value to be added is less than the first node's base then a new node should be added to the beggining.
            {
   			node *n=new node;
    	      		n->base = (unsigned int)(i/NO_BITS) * NO_BITS;
	    	      	n->bits [ i - n->base ] =1;
	    	      	n->next=first;
	    	      	first->prev=n;
	    	      	first=n;
	    	      	no_nodes++;
	    	      	return;
            }
             
	    node *temp=first;
	    
	    while(temp!=NULL) 	//search for an appropriate node where 'i' will fall.
	    {
	    	if( (temp->base+(NO_BITS - 1)) >= i) // if node which will accomodate the 'i' already exists then.
	    	{
                  	temp->bits[ i- temp->base  ] =1;			    	  	
                  	break;
	    	}
	    	else
	    	{
	    	      if(temp->next==NULL) // if this is the node to be inserted at the end.
	    	      {
	    	      		node *n=new node;
	    	      		n->base = (unsigned int)(i/NO_BITS) * NO_BITS;
	    	      		n->bits [ i - n->base ] =1;
	    	      		temp->next = n;
	    	      		n->prev=temp;
	    	      		last=n;
	    	      		no_nodes++;
	    	      		return;
	    	      }
	    	      else
	    	      if(  (((unsigned int)(i/NO_BITS)* NO_BITS) >  temp->base) &&  (((unsigned int)(i/NO_BITS)* NO_BITS) <  temp->next->base) )
	    	      {                         // if a node to be inserted in between
                               	node *n=new node;
	    	      		n->base = (unsigned int)(i/NO_BITS) * NO_BITS;
	    	      		n->bits [ i - n->base ] =1;
	    	      		n->prev=temp;
	    	      		n->next=temp->next;
	    	      		temp->next=n;
	    	      		n->next->prev=n;  	      		
	    	      		no_nodes++;
	    	      		return;
	    	      }
	    	      else
		    	        temp=temp->next;   // used to go to the next node in the set
	    	}
	    }
	}
	
	void display()
	{
	   display(false);
	}
	
	void display(bool ele)
	{
	    node *temp=first;
	    while(temp!=NULL)
	    {
	        if(ele==false)
		     cout<<temp->base<<"\t:  "<<temp->bits<<endl;
	       	else
	       	{
	       	   cout<<temp->base<<"\t:   { ";
	       	  
	       	   for(int i=0;i<NO_BITS;i++)
	       	   {
	       	       if(temp->bits[i]==1)
	       	         cout<<temp->base + i<<",  ";
	       	   }
	       	   cout<<"}"<<endl;
	       	}
	       	temp=temp->next;
	    }
	    
	    cout<<endl<<endl;
	}
	
	int getFirstElement()
	{
		node *temp=first;
		while(temp!=NULL)
		{
			if(temp->bits[0]==1)
				return temp->base;
			temp=temp->next;
		}
		
		return -1;
	}
	
	int getNextElement() // This implementation is specific to Sudoku solving.. (don't use any where else)
	{
		if(current==(node*)NNull) //If statring to get values from the beggning now then initialize to 
			current=first;
			
		if(current==NULL) //To start From the beggining.
		{
			current=(node*)NNull;
			return 0;
		}
				
			
		while(current!=NULL)
		{	
			if(!current->bits[0]) 
				current=current->next;
			else
				break;
		}
		
		if(current == NULL)
		{
			current=(node*)NNull;
			return 0;
		}
		else
		{
			if(current->bits[0])
			{
				int res=current->base;
				current=current->next;
				return res;
			}
		}
		return 0;
	}
	
	int getCurrentElement()
	{
		return current->base;
	}
};


/*
Set setDisplayNumbers(bool b)
	
	unsigned int getSizeofSet()
	
	unsigned int getNodeCount()// just returns the number of nodes in the set;

        friend ostream& operator <<(ostream &out, Set S)

        /* SET ASSIGNMENT
        void operator = (Set a)  // copy of two sets.
        /////////////////////////////////////////////////////////////////////////////////// 
        
        /* SET COMPLEMENT OPERATION   
        Set operator ~()   // Complement of a set.
        ///////////////////////////////////////////////////////////////////////////////////
 	
        bool search(unsigned int key)

        /*  SET UNION OPERATION 
        friend Set operator + (Set A, Set B)
 	
 	
 	/*  SET INTERSECTION OPERATION 
        friend Set operator & (Set A, Set B)
 	
 	/* SET DIFFERENCE OPERATION 
 	friend Set operator - (Set A, Set B)
 	
	void addElement(unsigned int i)        //adding an element in to the set. (if node already exists then set the bit of the node accordingly
	void display()
	
	void display(bool ele)
*/
/*
int main()
{
	Set A,B, T,S;
     int val;
       
     	A.addElement(1);A.addElement(2);A.addElement(3);A.addElement(4);A.addElement(5);A.addElement(6);A.addElement(7);A.addElement(8);
     	B.addElement(3);B.addElement(4);B.addElement(5);B.addElement(6);
		T= A-B;
		while(val=T.getNextElement())
			cout<<val<<endl;			
		cout<<T.search(3)<<"   "<<T.search(8)<<endl;
        	T.display(true);
  return 0;
}   */
 


