package game;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

public class queryMark 
{
	private ArrayList<data> da=new ArrayList<data>();
	
	public queryMark()
	{
		
	}
	
	class data
	{
		public String id;
		public String score;
		public data(String[] curdata)
		{
			this.id=curdata[0];
			this.score=curdata[1];
		}
	}
	
	public String AddAndQueryMark(String id,String score)
	{
		File file=new File("score");
		String obj="";

		if(!file.exists())
		{
			try 
			{
				file.createNewFile();
			} 
			catch (IOException e) 
			{
				e.printStackTrace();
			}
		}
		
		try 
		{
			InputStreamReader fr=new InputStreamReader(new FileInputStream("score"),"UTF-8");
			BufferedReader br=new BufferedReader(fr);
			String str="";
			boolean canfind=false;
			while((str=br.readLine()) != null)
			{
				data curda=new data(str.split(" "));
				if(curda.id == id && Integer.parseInt(curda.score) < Integer.parseInt(score))
				{
					curda.score=score;
					canfind=true;
				}
				da.add(curda);
				System.out.println("id="+curda.id+" score="+curda.score+"\n");
			}
			br.close();
			fr.close();
			
			if(!canfind)
			{
				da.add(new data(new String[]{id,score}));
				System.out.println("id="+id+" score="+score+"\n");
			}
			
			OutputStreamWriter fw=new OutputStreamWriter(new FileOutputStream("score"),"UTF-8");
			
			BufferedWriter bw=new BufferedWriter(fw);
			
			for(data dd:da)
			{
				bw.write(id+" "+score+"\n");
				obj += "\t<item>\n";
				obj += "\t\t<id>"+dd.id+"</id>\n";
				obj += "\t\t<score>"+dd.score+"</score>\n";
				obj += "\t</item>\n";	
			}
			
			bw.close();
			fw.close();
		}
		catch (IOException e) 
		{
			e.printStackTrace();
		}
		
		return obj;
	}
	
	public static void main(String[] args)
	{
		int SIZE=20;
		for(int i=0;i<100;i++)
		{
			int result=(int) (Math.random()*SIZE);
			System.out.println(result);
		}
	}
}
