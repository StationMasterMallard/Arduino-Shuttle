

// Shuttle with Hall Effect Linear Pole Detection - 49E OH49E SS49E

// Variables

    const String VERSION              = "1.1.5.2";

    const int NORTH                   = 1;
    const int SOUTH                   = 2;
  
    const int NORTH_HALL_SENSOR       = A0;
    const int SOUTH_HALL_SENSOR       = A5;

    const int NORTH_SPEED_STEP_DOWN   = 15;
    const int SOUTH_SPEED_STEP_DOWN   = 22;
    const int START_SPEED             = 140;
    const int TOP_SPEED               = 200;
    
    const int EnA                     = 6;

    const int In1                     = 2;
    const int In2                     = 4;

    int Train_Direction               = NORTH;

    bool Above_Range                  = false;
    bool Below_Range                  = false;

    bool Northbound_Train_Detected    = false;
    bool Southbound_Train_Detected    = false;
    
    int North_Highest_Range_Reading   = 0;
    int North_Lowest_Range_Reading    = 1000;

    int North_Current_Reading         = 0;
    
    int South_Highest_Range_Reading   = 0;
    int South_Lowest_Range_Reading    = 1000;
    
    int South_Current_Reading         = 0;
    
void setup()
{         
     pinMode(NORTH_HALL_SENSOR, INPUT);  
     pinMode(SOUTH_HALL_SENSOR, INPUT);

     pinMode (EnA, OUTPUT);

     pinMode (In1, OUTPUT);

     pinMode (In2, OUTPUT);

     Determine_Base_Line_Range(NORTH);
   
     Determine_Base_Line_Range(SOUTH);  
 
     analogWrite (EnA, START_SPEED); 

     // Head North - Note magnet on wagon has South pole pointing downwards towards track

     digitalWrite (In1, HIGH);
     digitalWrite (In2, LOW);
}

void loop()
{     
      North_Current_Reading = analogRead(NORTH_HALL_SENSOR);
     
      if (North_Current_Reading > North_Highest_Range_Reading) // South Magnet
         {                        
            if (Train_Direction == NORTH & Northbound_Train_Detected == false)
            {                
                // Speed up train
             
                analogWrite (EnA, TOP_SPEED);

                Northbound_Train_Detected = true;
            }  
            else  // Train heading South
            {
                // Slow down train and bring it to a halt

                Bring_Train_To_A_Halt(NORTH_SPEED_STEP_DOWN);
                
                delay(5000);  // Wait 5 seconds

                // Switch direction so train returns back up North

                digitalWrite (In1, HIGH);
                digitalWrite (In2, LOW);

                Train_Direction = NORTH;

                analogWrite (EnA, START_SPEED); 
                    
                Southbound_Train_Detected = false;
            }                      
         }
 
      South_Current_Reading = analogRead(SOUTH_HALL_SENSOR);
   
      if (South_Current_Reading > South_Highest_Range_Reading) // South Magnet
         {  
            if (Train_Direction == SOUTH & Southbound_Train_Detected == false)
            {                             
                // Speed up train
             
                analogWrite (EnA, TOP_SPEED);

                Southbound_Train_Detected = true;
            }            
            else // Train heading North
            {
                // Slow down train and bring it to a halt

                Bring_Train_To_A_Halt(SOUTH_SPEED_STEP_DOWN);
                
                delay(3000);  // Wait 3 seconds

                // Switch direction so train returns back down South

                digitalWrite (In1, LOW);
                digitalWrite (In2, HIGH);

                Train_Direction = SOUTH;

                analogWrite (EnA, START_SPEED); 
                       
                Northbound_Train_Detected = false;                              
            }            
         }
                           
     delay(200);  // Wait 2/10ths of a second     
}

void Bring_Train_To_A_Halt (int Speed_Steps)
{       
      for (int Speed = TOP_SPEED - Speed_Steps; Speed > START_SPEED; Speed = Speed - Speed_Steps)
          {
            analogWrite (EnA, Speed);

            delay(500); // Wait 1/2 second
          }
                
      // Stop Train

      analogWrite (EnA, 0);
}

void Determine_Base_Line_Range(int Pole)
{        
    int Current_Reading       = 0;
    int Highest_Range_Reading = 0;
    int Lowest_Range_Reading  = 2000;
  
    for (int count = 0; count < 100; count ++)
        {   

        if (Pole == NORTH)
           {
              Current_Reading = analogRead(NORTH_HALL_SENSOR);               
           }
        else
           {             
              Current_Reading = analogRead(SOUTH_HALL_SENSOR);               
           }

        if (Current_Reading > Highest_Range_Reading)
           {
              Highest_Range_Reading = Current_Reading; 
           }

        if (Current_Reading < Lowest_Range_Reading)
           {
              Lowest_Range_Reading = Current_Reading; 
           } 

        delay (100);                     
        }

      if (Pole == NORTH)
         {
            North_Highest_Range_Reading = Highest_Range_Reading;
            North_Lowest_Range_Reading  = Lowest_Range_Reading;
         }
      else
         {
            South_Highest_Range_Reading = Highest_Range_Reading;
            South_Lowest_Range_Reading  = Lowest_Range_Reading;
         }    
}
