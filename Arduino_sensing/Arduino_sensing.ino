   

   
   //****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;

 
   
       
 void PlottArray(unsigned int Cmd,float Array1[],float  Array2[], float ttm[]){
   
      SendData(Cmd+1, 1,1,ttm);                        // Tell PC an array is about to be sent                      
      delay(1);
      for(int x=0;  x < sizeOfArray;  x++){     // Send the arrays 
        SendData(Cmd, round(Array1[x]),round(Array2[x]), ttm);
        //delay(1);
      }
      
      SendData(Cmd+2, 1,1,ttm);                        // Confirm arrrays have been sent
    }

   

void setup()
{
  
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(115200);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+
}

void loop()
{
  unsigned int d;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[50]=results[d]*0.5+(float)(v)*0.5; //Filter results
   
    freq[d] = d;

 //plot(v,0);              //-Display
 //plot(results[d],1);
 //delayMicroseconds(1);
  }
  float ttm[N];
for (int i=0 ;i<160;i++)
{
ttm[i]=results[i];}
PlottArray(1,freq,results,ttm); 
 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
}
   

   
    
 
