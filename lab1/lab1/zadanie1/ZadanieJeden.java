/*
  @author Paweł Otrębski 188383
 */

/*
  Implement a statistical test for the LCG 
  X_n+1 = (aX_n + c) mod m 

  X_0, 0<=X_0<m 
 */

import java.util.HashMap;
import java.util.ArrayList;
import java.io.FileWriter;

public class ZadanieJeden {

    private long a;
    private long m;
    private long c;
    private long x;
    private FileWriter fw;
    private String fileName;
    public ZadanieJeden(){}

    public ZadanieJeden(long a,long m, long c, long x,String fileName){
	this.a = a;
	this.m = m;
	this.c = c;
	this.x = x;
	this.fileName = fileName;
	try{
	    fw = new FileWriter(this.fileName);
	}catch(Exception e){
	    fw = null;
	    System.exit(0);
	}
	
    }

    public long genRandom(){
	x = (a*x + c)%m;
	return x;
    }

    public void writeToFile(Long rand, int num){
	String str = String.format("%s", Long.toBinaryString(rand));
	int len = 32 - str.length();
	String zeroes = "";
	for(int i=0;i<len;i++){
	    zeroes+="0";
	}
	str = zeroes+str;
	try{
	    fw.write(str+"\t"+rand+"\n");
	}catch(Exception e){
	    System.out.println(e.getMessage());
	}
    }

    public void closeFile(FileWriter fw){
	try{
	    fw.close();
	}catch(Exception e){

	}
    }

    public void writeToFile(String str){
	try{
	    fw.write(str);
	}catch(Exception e){
	    
	}
    }
    
    public void runTest(int number){
	Long rand = null;
	writeToFile(String.format("a:%d,m:%d,c:%d,x:%d\n\n",a,m,c,x));
	for(int i=1;i<number+1;i++){
	    rand = new Long(genRandom());
	    writeToFile(rand,i);
	}
	closeFile(fw);
    }

    public static void main(String args[]){
        if(args.length<1){
	    System.out.println("No numbers to generate, pass values");
	    System.exit(0);
	}

	try{
	    long a = Long.parseLong(args[0]);
	    long m = Long.parseLong(args[1]);
	    long c = Long.parseLong(args[2]);
	    long x = Long.parseLong(args[3]);
	    int tests = Integer.parseInt(args[4]);
	    String fileName = args[5];
	    ZadanieJeden zj = new ZadanieJeden(a,m,c,x,fileName);
	    zj.runTest(tests);
	    		
	}catch(Exception e){
	    e.printStackTrace();
	}

	
    }
}
