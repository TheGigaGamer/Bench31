   #include "pi2c.h"
   #include "string.h"
//new

	char encoders[] = {0,0};
	char commands[] = {0,0,0};
	Pi2c arduino(0x68);
	Pi2c drivebot(0x52);

		void drivecommands(char commands[4])
	{
        //turn it into  a six element 1 byte num'
        std::cout << "Values:" << (int)commands[0] <<" "<< (int)commands[1] <<" "<< (int)commands[2] << std::endl;
        drivebot.i2cWrite(commands,4);
		drivebot.i2cRead(encoders,2);
	}

	void messaging(int a, int b, int c,int d)
	{
		char message[] = {a,b,c,d};
		drivecommands(message);
	}

	void speak(int distance)
    {
        char command[1];
        char c1[] = ("espeak --stdout 'The car is parked ");
        char c2[] = (" centimeters away from the edge of the spot' -a 200 -s 120 | aplay");
        char c3[] = "0";

        sprintf(c3, "%d", distance);

        strcat(command,c1);
        strcat(command,c3);
        strcat(command,c2);

        system(command);
    }

	void turnround()
	{
	    messaging(1,33,33,0);
        sleep(1);
		messaging(0,0,0,0);
	}


    int main ()
    {
        char command[] = "espeak --stdout 'Meth gator alert' -a 200 -s 120 | aplay";
        system(command);
		float distance = 3; //how far to drive
		float rev = (2*distance*10/(.8*3.14159)); //how many revolutions needed for that distance
		int average_encoder = 1;
		uint8_t seperation = 100;
        std::cout << "To go "<< distance << " meters, "<< rev*10 << " rotations are needed" << std::endl;
		messaging(0,30,30,1);
		while(average_encoder < rev) //while theres still distance to go
		{
			messaging(0,30,30,0);
			average_encoder = (encoders[0]+encoders[1]); //read the encoders
			std::cout << average_encoder <<std::endl;
			sleep(1);
		}

		turnround();


		while(seperation > 10 && seperation != 0)
		{
			seperation = arduino.i2cReadArduinoInt();
            while (seperation == 0)
                seperation = arduino.i2cReadArduinoInt();
			std::cout << seperation <<std::endl;
			messaging(3,10,10,0);
			sleep(1);
        }
        messaging(0,0,0,0);
        speak(seperation);
        return 0;
    }
//new

