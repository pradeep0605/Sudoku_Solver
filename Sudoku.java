import java.io.*;
import java.lang.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;




class PKButton extends JButton
{
	boolean fixed;
	
	PKButton()
	{
		fixed=false;
	}
	PKButton(String s)
	{
		super(s);
		fixed=false;
	}
	
	void setFixed(boolean x)
	{ fixed=x; }
	
	boolean isFixed()
	{  return fixed; }
	
	void setColor(Color c)
	{  setForeground(c); }
	
}

class Counter extends Thread
{
	int hours=0,minutes=0,seconds=0, miliseconds=0;
	JLabel TimeTakenToSolve;
	
	Counter(JLabel l)
	{
		TimeTakenToSolve=l;
		hours=minutes=seconds=0;
	}
	
	public void run()
	{
		try
		{
			while(true)
			{	
				TimeTakenToSolve.setText("    " + hours + ":" + minutes +":" + seconds+ ":" + miliseconds);
				sleep(100);
				miliseconds+=1;
				if(miliseconds==10)
				{
					miliseconds=0;
					seconds++;
					if(seconds==60)
					{
						seconds=0;
						minutes++;
						if(minutes==60)
						{
							hours++;
							minutes=0;
						}
					}
				}
			}
		}catch(Exception e){}
	}
}


class SFrame 
{	
		
	String title="Sudoku Solver !! - Pradeep R. Kashyap, 8th Sem, CS, SJCE";
	JFrame MainFrame=new JFrame(title);
	JPanel MainPanel=new JPanel();
	JPanel LeftPanel=new JPanel();
	JPanel RightPanel=new JPanel();
	JPanel []NineBoxes=new JPanel[9];
	PKButton [][]cells=new PKButton[9][9];
	JComboBox SudokuType=new JComboBox();
	JLabel TimeTakenToSolve=new JLabel("");
	JButton Generator=new JButton("Generate !");
	JButton AutoCompleteSolver=new JButton("Solve Automatically");
	JButton Clear=new JButton("Clear");
	JButton PauseResume=new JButton("Pause");
	JTextField speed=new JTextField("400",7);
	String input=new String();
	String Solution=new String();
	Counter timer=new Counter(TimeTakenToSolve);
	Vector<String> IndividualSolutions=new Vector<String>();
	int InitialX=0, InitialY=0, ButtonWidth=0, ButtonHeight=0;
	SolveInGUI sgui=null;
	int cellValues[][]=new int[9][9];
	boolean solved=false;
	SFrame()
	{
		MainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		MainFrame.setSize(500,500);
		MainFrame.setExtendedState(MainFrame.MAXIMIZED_BOTH);
		//MainFrame.setUndecorated(true);
		//MainFrame.setAlwaysOnTop(true);
		MainPanel.setLayout(new BorderLayout());
		
		
		LeftPanel.setPreferredSize(new Dimension(1000,1000));
		RightPanel.setPreferredSize(new Dimension(350,1000));
		
		LeftPanel.setLayout(new GridLayout(3,3,5,10));
		LeftPanel.setBorder(BorderFactory.createEmptyBorder(30,100,30,100)); //top,left,bottom,right
		LeftPanel.setBackground(Color.black);

		RightPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
		
		TimeTakenToSolve.setFont(new Font("", Font.BOLD, 40 ));
		
		addCells(); // adds cells, sets their action, alignment, fontsize etc
		
		arrangeRightPanel();
		
		MainPanel.add(LeftPanel, BorderLayout.WEST);
		MainPanel.add(RightPanel,BorderLayout.EAST);
		
		MainFrame.setContentPane(MainPanel);
		MainFrame.pack();
		MainFrame.setVisible(true);
	}
	
	void setForeground(int x, int y,Color c)
	{
		cells[x][y].setForeground(c);
	}
	public void arrangeRightPanel()
	{	
		SudokuType.addItem("Easy");
		SudokuType.addItem("Medium");
		SudokuType.addItem("HARD");
		SudokuType.addItem("Very Hard");
		
		JLabel heading= (new JLabel("SUDOKU SOLVER" ));
		heading.setFont(new Font("",Font.BOLD, 35)); 
		
		Generator.setFont(new Font("",Font.BOLD, 15));
		Generator.setMnemonic('G');
		Generator.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent ae)
			{	
			   try
			   {
				Process p=null;
				File f1=new File("output.sudoku");
				File f2=new File("input.sudoku");
				f1.delete();
				f2.delete();
				IndividualSolutions.removeAllElements();
				solved=false;
				PauseResume.setEnabled(false);
				if(sgui!=null)
					sgui.stop();
					
				if(timer.isAlive())
					timer.stop();
				TimeTakenToSolve.setText("");					
				timer=new Counter(TimeTakenToSolve);
			     timer.start();
					
				
			     
				try
				{	
					if(SudokuType.getSelectedItem().equals("Easy"))
					{ 	
					   		MainFrame.setTitle(title + "                    PLAYING IN \"EASY\" MODE");
					   	     p=Runtime.getRuntime().exec("sudoku -ceasy -g 1"); 
					}

					if(SudokuType.getSelectedItem().equals("Medium"))
					{  
					   		MainFrame.setTitle(title + "                    PLAYING IN \"MEDIUM\" MODE");
					   		p=Runtime.getRuntime().exec("sudoku -cmedium -g 1"); 
					}
					   
					if(SudokuType.getSelectedItem().equals("HARD"))
	  				{  
	  				   		MainFrame.setTitle(title + "                    PLAYING IN \"HARD!\" MODE");
	  				   		p=Runtime.getRuntime().exec("sudoku -chard -g 1"); 
	  				}
			
					if(SudokuType.getSelectedItem().equals("Very Hard"))
	  				{  
	  				   		MainFrame.setTitle(title + "                    PLAYING IN \"VERY HARD!!\" MODE");
	  				   		p=Runtime.getRuntime().exec("sudoku -cfiendish -g 1"); 
	  				}
				}
				catch(Exception e)
				      { JOptionPane.showMessageDialog(null,"Error " + e.toString(),"d",1);   }


				       InputStream ins=p.getInputStream();
			               while(ins.available() ==0);
				       int size=ins.available();
				       byte []b=new byte[size];
				       ins.read(b);
				       String s=new String(b);
				       s= s.substring(s.indexOf("\n") +1 );
				       s=s.replace('-',' ');
				       s=s.replace('+',' ');
				       s=s.replace('|',' ');
				       s=s.replace('.','0');
				       s=s.replace("   "," ");
				       s=s.replace("\n ","\n");
				       s=s.replaceAll("\n[' ']+\n","\n");
				       s=s.trim();
				       input=s;
				       
				       
				       initializeCells();
				       
				       FileOutputStream out=new FileOutputStream("input.sudoku");
				       out.write(s.getBytes());
				       out.close();
					  
					  
				       Process pif=Runtime.getRuntime().exec("./interface.out");	
				       File check=new File("output.sudoku");
				       while( !check.exists());

				       FileInputStream fin=new FileInputStream("output.sudoku");
				       Thread.currentThread().sleep(100);
				       byte []ip=new byte[fin.available()];
				       fin.read(ip);
				       Solution=new String(ip);
				       //System.out.println(Solution + "\n\n\n");
				       tokenize();
				       

				       for(int i=0;i<IndividualSolutions.size();i++)
				       	   System.out.println( IndividualSolutions.elementAt(i));
				       	   
				       AutoCompleteSolver.setEnabled(true);
					  Clear.setEnabled(true);
				       cells[0][0].requestFocus();
			     }catch(Exception e)
			     { JOptionPane.showMessageDialog(null,"Error " + e.toString(),"info!!",1); }
			}	
		});
		
		AutoCompleteSolver.setEnabled(false);
		AutoCompleteSolver.setFont(new Font("",Font.BOLD, 15));
		AutoCompleteSolver.setMnemonic('a');	
		AutoCompleteSolver.addActionListener(new ActionListener() {
		
			public void actionPerformed(ActionEvent ae)
			{
			   try
			   {
			   	PauseResume.setEnabled(true);
				ButtonWidth=cells[0][0].getWidth();
				ButtonHeight=cells[0][0].getHeight();
				
			  	Point pt=new Point(cells[0][0].getLocation());
				SwingUtilities.convertPointToScreen(pt,cells[0][0]);
				InitialX= (int) pt.getX();
				InitialY= (int) pt.getY();
				sgui= new SolveInGUI(IndividualSolutions);
				
			   }catch(Exception e){}
			}	
		});
		
		
		Clear.setEnabled(false);
		Clear.setMnemonic('c');
		Clear.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent ae)
			{
				timer.stop();
				sgui.stop();
				tokenize();
				solved=false;
				initializeCells();
				AutoCompleteSolver.setEnabled(true);	
				PauseResume.setEnabled(false);
				timer=new Counter(TimeTakenToSolve);
				timer.start();
				cells[0][0].requestFocus();
							
			}
		});
		
		PauseResume.setEnabled(false);
		PauseResume.setMnemonic('p');
		PauseResume.addActionListener(new ActionListener(){
			
			public void actionPerformed(ActionEvent ae)
			{
				if(PauseResume.getText().equals("Pause"))
				{
					sgui.suspend();
					PauseResume.setText("Resume");
					PauseResume.setMnemonic('r');
				}
				else
				{
					sgui.resume();
					PauseResume.setText("Pause");
					PauseResume.setMnemonic('p');
				}
			}
		});
		
		RightPanel.add( heading );
		RightPanel.add( new JLabel("                                                                          "));
		RightPanel.add( new JLabel("                 By \n Pradeep R. Kashyap"));
		RightPanel.add( new JLabel("                                                                          "));
		RightPanel.add(new JLabel("SUDOKU TYPE:  "));
		RightPanel.add(SudokuType);
		RightPanel.add(Generator);
		RightPanel.add( new JLabel("                                                                          "));
		speed.setFont(new Font("",Font.BOLD,15));
		RightPanel.add(AutoCompleteSolver);
		RightPanel.add(speed);
		RightPanel.add( new JLabel("                                                                          "));
		RightPanel.add( new JLabel("                                                                          "));
		RightPanel.add(PauseResume);
		RightPanel.add(Clear);
		RightPanel.add( new JLabel("                                                                          "));
		RightPanel.add( new JLabel("                                                                          "));
		JLabel time=new JLabel("       TIME ");
		time.setFont(new Font("",Font.BOLD,40));
		RightPanel.add(time);
		RightPanel.add( new JLabel("                                                                          "));
		RightPanel.add(TimeTakenToSolve);
	}
	
	void initializeCells()
	{
		int I=0,J=0;
	  	for(int i=0;i<input.length() ;i++)
		  {
		  	      char ch=input.charAt(i);
		  	      if(ch== ' '){ J++; continue; }
			 if(ch=='\n') { I++; J=0; continue;}
			 if(ch >='0' && ch <= '9')
			 {
			    if(ch=='0')
			    {
			       cells[I][J].setText(" ");
			       cells[I][J].setFixed(false);
		 	            cells[I][J].setFont(new Font("" ,Font.PLAIN,35));
			    }
			    else
			    {
			 	    cells[I][J].setFont(new Font("" ,Font.BOLD,35));
			 	    cells[I][J].setText("" + ch); 
			 	    cells[I][J].setFixed(true);
			    }
			    cells[I][J].setEnabled(true);
			    cells[I][J].setForeground(Color.black);
			    cellValues[I][J]= ch-'0';
			 }
		  }
	}
	
	public void tokenize()
	{
		IndividualSolutions.removeAllElements();
		StringTokenizer tokenizer=new StringTokenizer(Solution,"\n");
	     while(tokenizer.hasMoreElements())
				IndividualSolutions.addElement(tokenizer.nextToken());
	}
	
	public void addCells()
	{
		for(int i=0;i<9;i++){
			NineBoxes[i]=new JPanel(new GridLayout(3,3,3,3)); // GridLayout(rows, cols, hgap, vgap)
			NineBoxes[i].setBackground(Color.black);
			LeftPanel.add(NineBoxes[i]);			
		}			
		
		for( int i=0;i<9;i++)
		    for(int j=0;j<9;j++)
		    {
		    	final int I=i, J=j;
			cells[i][j]=new PKButton(new String(" "));	
			NineBoxes[getBoxValue(i,j)].add(cells[i][j]);
		    	cells[i][j].setFixed(true);
				   
			cells[I][J].addKeyListener( new KeyListener(){
			    	public void keyPressed(KeyEvent ke) 
			    	{
			    	    try
			    	    {		
			    	    	if(!cells[I][J].isFixed())
			    	    	{ 
				    	    	if(ke.getKeyChar() >='1' && ke.getKeyChar() <= '9')
				    	    	{//if key pressed is between 1-9 then set button text.
									    	    	
							if( !isValidEntry( (ke.getKeyChar() - '0'),I,J ))		    	    
							{
							   	cells[I][J].setText("" + ke.getKeyChar() );
							   	cells[I][J].setForeground(Color.red);
							 	JOptionPane.showMessageDialog(null,"INVALID ENTRY!","Error!",1);
							 	cells[I][J].requestFocus();
							}
							else
							{
								cells[I][J].setText( "" + ke.getKeyChar() );
							  	cells[I][J].setForeground(Color.black);
							  	String s=new String( ke.getKeyChar() + " " + I + " " + J);
							  	IndividualSolutions.remove(s);
							  	IndividualSolutions.trimToSize();
							  	if(sgui!=null)
								  	sgui.remove(s);
							}
							cellValues[I][J]= ke.getKeyChar() - '0';
						}
						if(ke.getKeyCode() == 127) //Delete Button
						{
							    
							cells[I][J].setText(" ");
							cellValues[I][J]= 0;
							
						}
					}
					if( ke.getKeyCode() >=37 && ke.getKeyCode() <= 40)
					{	
					        switch(ke.getKeyCode())
						{
							case 37: if(J!=0) 				//left
								        cells[I][J-1].requestFocus();  
								 else 
								  	cells[I][8].requestFocus();
								  break;
								  
							case 38: if(I!=0) 				//up
									cells[I-1][J].requestFocus(); 
								else
									cells[8][J].requestFocus();
								  break;
								  
							case 39: if(J!=8) 				//right
									cells[I][J+1].requestFocus();
								else
									cells[I][0].requestFocus();
								  break;
							case 40: if(I!=8) //down
									cells[I+1][J].requestFocus();
								else
								 	cells[0][J].requestFocus();
								  break;
						}

					}
					
					if(isSudokuDone())
					{
					    if(!solved)
					    {
						    timer.stop();
						    JOptionPane.showMessageDialog
						    (null,"     CONGRATS !!! \n SUDOKU SOLVED\nSolved in:" + TimeTakenToSolve.getText(), "info!!" ,1);
					    }
				            solved=true;
					    
					    for(int i=0;i<9;i++)
					    	for(int j=0;j<9;j++){
					    		cells[i][j].setFixed(true); cells[i][j].setForeground(Color.blue); }
					    Generator.requestFocus();
					    AutoCompleteSolver.setEnabled(false);
					    PauseResume.setEnabled(false);	
					}
					
				      	
				    } catch( Exception e){}
			    	}
			    	public void keyReleased(KeyEvent Ke) {}
			    	public void keyTyped(KeyEvent ke) {}
			    });
		    } 
	}
	
	
	boolean isSudokuDone()
	{
		for(int i=0;i<9;i++)
		  for(int j=0;j<9;j++)
		  	if(cellValues[i][j]== 0 || cells[i][j].getForeground()==Color.red  )
		  	 	return false;
		return true;
	}
	
	boolean isValidEntry(int value, int x, int y)
	{
		for(int i=0;i<9 ;i++) // row checking i.e to check whether the value is present in the same row or not.
   		  if(i!=y)
		  	if( cellValues[x][i] == value)
		  		return false;
		for(int i=0;i<9 ;i++)// column checking
   		  if(i!=x)
		  	if( cellValues[i][y] == value)
		  		return false;
		int n=((x/3)*3);  
		int m=((y/3)*3);
		for(int i=n ; i< n+3; i++) // box checking
			for(int j=m; j< m+3; j++)  
				if((i!=x || j!=y ))
				   if(cellValues[i][j] == value)
				  	return false;
		return true;
	}
	
	int getBoxValue(int x,int y)
	{ return ( ((x/3)*3) + (y/3)); }

			
	class SolveInGUI extends Thread
	{
		Vector <String> IndividualSolutions=new Vector<String>();
		public SolveInGUI (Vector <String> v)
		{
			for(int i=0;i<v.size();i++)
				IndividualSolutions.addElement(v.elementAt(i));
			start();
		}

	     void remove(String s)
	     {
	     	IndividualSolutions.remove(s);
			IndividualSolutions.trimToSize();
//			JOptionPane.showMessageDialog(null,"here","df",1);
	     }
	     
		public void run()
		{
			try
			{
				Robot r=new Robot();
				for(int i=0;i<IndividualSolutions.size()-1;i++)
					{
						String s=IndividualSolutions.elementAt(i);
						int value= s.charAt(0) -'0', x= s.charAt(2) - '0', y=s.charAt(4) - '0';

						/*
						int X= InitialX + ButtonWidth*y + ButtonWidth/2 + (y/3)*12;
						int Y= InitialY + ButtonHeight*x + ButtonHeight/2 + (x/3)*17;
						r.mouseMove(X, Y);*/

				
						cells[x][y].requestFocus();
						cells[x][y].setText("<html> <font color=green> <b>" +  value + "</b></font></html>");
						if( !isValidEntry( value,x,y ))		    	    
						{
							   	cells[x][y].setText("" + s.charAt(0) );
							   	cells[x][y].setForeground(Color.red);
							 	JOptionPane.showMessageDialog(null,"INVALID ENTRY!","Error!",1);
							 	cells[x][y].requestFocus();
						}
						cellValues[x][y]= value;
						/*r.keyPress(value + 0x30);
						r.keyRelease(value + 0x30); */
						sleep(Integer.parseInt(speed.getText()));					
						cells[x][y].setText("" + value);
						if( !isValidEntry( value,x,y) )		    	    
						{
								   	cells[x][y].setText("" + value);
								   	cells[x][y].setForeground(Color.red);
								 	JOptionPane.showMessageDialog(null,"INVALID ENTRY!","Error!",1);
								 	cells[x][y].requestFocus();
						}
					}
					
					if( IndividualSolutions.elementAt(IndividualSolutions.size()-1).charAt(0) == '0')
						JOptionPane.showMessageDialog(null,"This SuDoKu CANNOT be solved WITHOUT GUESS!!!","info!",2);
				
					if(isSudokuDone())
					{
					    if(!solved)
					    {
						    timer.stop();
						    JOptionPane.showMessageDialog
						    (null,"     CONGRATS !!! \n  SUDOKU SOLVED\nSolved in:" + TimeTakenToSolve.getText(), "info!!" ,1);
					    }	
				            solved=true;
					    
					    for(int i=0;i<9;i++)
					    	for(int j=0;j<9;j++){
					    		cells[i][j].setFixed(true); cells[i][j].setForeground(Color.blue); }
					    Generator.requestFocus();
					    AutoCompleteSolver.setEnabled(false);
					    PauseResume.setEnabled(false);	
					    
					}
			}
			catch(Exception e){ JOptionPane.showMessageDialog(null,e.toString() + "\n Inside Thread","info!",1);}
			//System.out.println("COMING DAMN HERE!\n" + IndividualSolutions.size() );
		}
	}
		
}

public class Sudoku
{
	public static void main(String args[]) throws Exception
	{
		new SFrame();
	}	
}

 /*
				for(int i=0;i<9;i++)
				  for(int j=0;j<9;j++)
				  {
					int x,y;
					x=  InitialX + ButtonWidth*j + ButtonWidth/2 + (j/3)*12;  
					y=  InitialY + ButtonHeight*i + ButtonHeight/2 + (i/3)*17 ; 
					r.mouseMove( x , y );
					Thread.currentThread().sleep(300);
				  }
				*/


						/*switch(ke.getKeyCode())
						{
							case 37: if(J!=0) cells[I][J-1].requestFocus();
								  break;
							case 38: if(I!=0) cells[I-1][J].requestFocus();
								  break;
							case 39: if(J!=8) cells[I][J+1].requestFocus();
								  break;
							case 40: if(I!=8) cells[I+1][J].requestFocus();
								  break;
						}*/
